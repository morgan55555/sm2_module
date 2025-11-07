#include <sm2registry.h>
#include <sm2module.h>

#include <PID_v1.h>
#include <DHT.h>

// Debug
// #define UART_ENABLED 1
#define UART_SPEED 115200

#if defined UART_ENABLED
   #define debug_init()   Serial.begin(UART_SPEED)
   #define debug(...)     Serial.print(__VA_ARGS__)
   #define debugln(...)   Serial.println(__VA_ARGS__)
#else
   #define debug_init()
   #define debug(...)
   #define debugln(...)
#endif

// Heater pins
#define TEMP_SENSOR A5
#define RELAY_FAN 4
#define RELAY_400W 5
#define RELAY_600W 6

DHT sens(TEMP_SENSOR, DHT21);

// PID params
#define PID_KP 2.0
#define PID_KI 0.05
#define PID_KD 0.1

// PID values
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, PID_KP, PID_KI, PID_KD, P_ON_E, DIRECT);

// System values
bool systemActive = false;
bool fanRunning = false;
unsigned long systemStopTime = 0;

// Heating modes
enum HeatingMode {
  MODE_OFF = 0,
  MODE_400W = 1,
  MODE_600W = 2,
  MODE_800W = 3
};

HeatingMode currentMode = MODE_OFF;

// DHT Sensor
float humidity = 0;

// Timers
const unsigned long READING_DELAY = 2000;
const unsigned long MIN_FAN_RUN_TIME = 30000;
const unsigned long MODE_CHANGE_DELAY = 5000;
unsigned long lastReadingTime = 0;
unsigned long lastModeChangeTime = 0;
unsigned long lastHeaterLoopTime = 0;

// Snapmaker module
SM2Registry registry(10);
SM2Module vmodule(MODULE_PURIFIER, 123);

// Module functions
enum : uint8_t {
    PURIFIER_SET_FAN_STA,
    PURIFIER_SET_FAN_GEARS,
    PURIFIER_SET_FAN_POWER,
    PURIFIER_SET_LIGHT,
};

// Module information
enum : uint8_t {
    PURIFIER_REPORT_LIFETIME,
    PURIFIER_REPORT_ERR,
    PURIFIER_REPORT_FAN_STA,
    PURIFIER_REPORT_ELEC,
    PURIFIER_REPORT_POWER,
    PURIFIER_REPORT_STATUS,
    PURIFIER_INFO_ALL,
};

// SM DataPack structure
uint16_t funcId;
uint8_t dataLen = 0;
uint8_t data[8];

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    debug_init();

    // Init sensor pin
    pinMode(TEMP_SENSOR, INPUT);
    digitalWrite(TEMP_SENSOR, HIGH);

    // Init sensor
    sens.begin();

    // Init relays
    pinMode(RELAY_400W, OUTPUT);
    pinMode(RELAY_600W, OUTPUT);
    pinMode(RELAY_FAN, OUTPUT);
    digitalWrite(RELAY_400W, HIGH);
    digitalWrite(RELAY_600W, HIGH);
    digitalWrite(RELAY_FAN, HIGH);

    // Init PID
    myPID.SetMode(MANUAL);
    myPID.SetOutputLimits(0, 255);
    Output = 0;
    
    // Register modules
    registry.RegisterModule(&vmodule);

    // Init registry and modules
    registry.Init();

    digitalWrite(LED_BUILTIN, LOW);
    debugln(F("All modules configured successfully!"));

    // Initial report
    report_lifetime();
    report_err();
}

void set_fan_status(uint8_t is_open, uint8_t is_forced) {
    // Ignore turning on, we are not actually a purifier
    // But if user wan't to turn off, then turning off
    // (For emergency situations or cancelled prints)

    if (!is_open && systemActive) {
        heater_stop();
    }
}

void set_fan_gears(uint8_t gears) {
    // Do not do anything
}

void set_fan_power(uint8_t power) {
    // I havent found yet this gcode :(
}

void set_light(uint8_t r, uint8_t g, uint8_t b) {
    // Set light is broken, every color is the same
    // But now it will be a temperature!
    if (r > 0) {
        Setpoint = r;
        debug(F("New temperature: "));
        debugln(Setpoint);
        heater_start();
    } else {
        heater_stop();
    }
}

void report_lifetime() {
    // Always normal
    uint8_t data[2] = {PURIFIER_REPORT_LIFETIME, 2};
    registry.SendMessage(FUNC_REPORT_PURIFIER, data, sizeof(data));
}

void report_err() {
    // No errors
    uint8_t data[2] = {PURIFIER_REPORT_ERR, 0};
    registry.SendMessage(FUNC_REPORT_PURIFIER, data, sizeof(data));
}

void report_fan_speed() {
    uint16_t fan_spd = (uint16_t) Setpoint;  // rpm, target temperature
    uint8_t pwm_value = (uint8_t) Output;
    uint8_t mode = (uint8_t) currentMode;
    uint8_t data[8] = {
        PURIFIER_REPORT_FAN_STA,
        systemActive,   // State 0 = idle, 1 = working // TODO: working state
        fan_spd >> 8,   // uint16_t value >> 8
        fan_spd,        // uint16_t value
        pwm_value,      // fan out in %
        mode            // fan gears
    };
    registry.SendMessage(FUNC_REPORT_PURIFIER, data, sizeof(data));
}

void report_fan_elect() {
    uint16_t elec = 75; // mA, just a dummy output
    uint8_t data[3] = {
        PURIFIER_REPORT_ELEC,
        elec >> 8, // uint16_t value >> 8
        elec
    };
    registry.SendMessage(FUNC_REPORT_PURIFIER, data, sizeof(data));
}

void report_fan_power() {
    uint16_t power = (uint16_t) Input; // mV, we can use it as temperature monitor
    uint16_t power2 = (uint16_t) humidity; // mV, we can use it as humidity monitor
    uint8_t data[5] = {
        PURIFIER_REPORT_POWER,
        power >> 8, // uint16_t value >> 8
        power,
        power2 >> 8, // uint16_t value >> 8
        power2
    };
    registry.SendMessage(FUNC_REPORT_PURIFIER, data, sizeof(data));
}

void report_status() {
    uint8_t data[2] = {PURIFIER_REPORT_STATUS, 1};
    registry.SendMessage(FUNC_REPORT_PURIFIER, data, sizeof(data));
}

void emergency_stop() {
    debugln("Emergency stop!");
    return registry.DeathLoop();
}

void loop() {
    if (registry.ReadMessage(&funcId, data, &dataLen)) {
        switch (funcId) {
            case FUNC_SET_PURIFIER:
                switch (data[0]) {
                    case PURIFIER_SET_FAN_STA:
                        debugln(F("SET STATUS"));
                        set_fan_status(data[1], data[2]);
                        break;
                    case PURIFIER_SET_FAN_GEARS:
                        debugln(F("SET GEARS"));
                        set_fan_gears(data[1]);
                        break;
                    case PURIFIER_SET_FAN_POWER:
                        debugln(F("SET POWER"));
                        set_fan_power(data[1]);
                        break;
                    case PURIFIER_SET_LIGHT:
                        debugln(F("SET LIGHT"));
                        set_light(data[1], data[2], data[3]);
                        break;
                }
                break;
            case FUNC_REPORT_PURIFIER:
                switch (data[0]) {
                    case PURIFIER_REPORT_LIFETIME:
                        debugln(F("REPORT LIFETIME"));
                        report_lifetime();
                        break;
                    case PURIFIER_REPORT_ERR:
                        debugln(F("REPORT ERR"));
                        report_err();
                        break;
                    case PURIFIER_REPORT_FAN_STA:
                        debugln(F("REPORT FAN"));
                        report_fan_speed();
                        break;
                    case PURIFIER_REPORT_ELEC:
                        debugln(F("REPORT ELEC"));
                        report_fan_elect();
                        break;
                    case PURIFIER_REPORT_POWER:
                        debugln(F("REPORT POWER"));
                        report_fan_power();
                        break;
                    case PURIFIER_REPORT_STATUS:
                        debugln(F("REPORT STATUS"));
                        report_status();
                        break;
                    case PURIFIER_INFO_ALL:
                        debugln(F("REPORT ALL"));
                        report_lifetime();
                        report_err();
                        report_fan_speed();
                        report_fan_elect();
                        report_fan_power();
                        report_status();
                        break;
                }
                break;
            case FUNC_EMERGENCY_STOP:
                debugln(F("EMERGENCY"));
                emergency_stop();
                break;
        }
    }

    heater_loop();
}

void heater_start() {
    myPID.SetMode(AUTOMATIC);
    systemActive = true;
    debugln(F("STARTED"));
}

void heater_stop() {
    myPID.SetMode(MANUAL);
    systemActive = false;
    systemStopTime = millis();
    debugln(F("STOPPED"));
}

void heater_loop() {
    // Update every 100ms
    if (millis() - lastHeaterLoopTime < 100) {
        return;
    }
  
    // Read DHT sensor
    readSensorData();
  
    if (systemActive) {
        // Compute PID
        myPID.Compute();
        // Select current power (less or equal "gears")
        selectHeatingMode();
        // Enable fan
        if (!fanRunning) {
            digitalWrite(RELAY_FAN, LOW);
            fanRunning = true;
        }
    } else {
        // Stop all
        setHeatingMode(MODE_OFF);
        // Except fan, it need to be working 30 sec after for cooldown
        if (fanRunning && millis() - systemStopTime >= MIN_FAN_RUN_TIME) {
            digitalWrite(RELAY_FAN, HIGH);
            fanRunning = false;
        }
    }

    lastHeaterLoopTime = millis();
}

void readSensorData() {
    if (millis() - lastReadingTime < READING_DELAY) {
        return;
    }
    
    // Read temp
    float currentTemp = sens.readTemperature();
    if (currentTemp != -1) {
        Input = currentTemp;
    }
    
    // Read humid
    float currentHumidity = sens.readHumidity();
    if (currentHumidity != -1) {
        humidity = currentHumidity;
    }
  
    lastReadingTime = millis();
}

void selectHeatingMode() {
    if (!systemActive) return;
    
    if (millis() - lastModeChangeTime < MODE_CHANGE_DELAY) {
        return;
    }
    
    HeatingMode newMode = MODE_OFF;
  
    if (Output >= 1) {
        if (Output <= 102) {
            newMode = MODE_400W;
        } else if (Output <= 153) {
            newMode = MODE_600W;
        } else {
            newMode = MODE_800W;
        }
    } else {
        newMode = MODE_OFF;
    }
    
    if (newMode != currentMode) {
        setHeatingMode(newMode);
        lastModeChangeTime = millis();
    }
}

void setHeatingMode(HeatingMode mode) {
    digitalWrite(RELAY_400W, HIGH);
    digitalWrite(RELAY_600W, HIGH);
    
    switch (mode) {
        case MODE_400W:
            digitalWrite(RELAY_400W, LOW);
            break;
        case MODE_600W:
            digitalWrite(RELAY_600W, LOW);
            break;
        case MODE_800W:
            digitalWrite(RELAY_400W, LOW);
            digitalWrite(RELAY_600W, LOW);
            break;
        case MODE_OFF:
        default:
            break;
    }
    
    currentMode = mode;
}
