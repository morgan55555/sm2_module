#include <sm2registry.h>
#include <sm2module.h>

#define DEBUG_MODE 0

#define LIGHT_PIN 3
#define FAN_PIN 5
#define E_STOP_R_PIN 9
#define E_STOP_G_PIN 6
#define E_STOP_B_PIN 8

#define DOOR_PIN A2
#define E_STOP_PIN 2

#define DOOR_DEBOUNCE_DELAY 50

SM2Registry registry(10);
SM2Module vmodule(MODULE_ENCLOSURE, 123);
SM2Module vmodule2(MODULE_EMERGENCY_STOP, 123);

uint16_t funcId;
uint8_t dataLen = 0;
uint8_t data[8];

uint64_t door_debounce_time;
bool door_last_state;
bool door_state;

bool e_stop_state;

void setup() {
    Serial.begin(115200);

    // Init pins
    pinMode(LIGHT_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(E_STOP_R_PIN, OUTPUT);
    pinMode(E_STOP_G_PIN, OUTPUT);
    pinMode(E_STOP_B_PIN, OUTPUT);

    pinMode(DOOR_PIN, INPUT_PULLUP);
    pinMode(E_STOP_PIN, INPUT_PULLUP);

    // Register modules
    registry.RegisterModule(&vmodule);
    registry.RegisterModule(&vmodule2);

    // Init registry and modules
    registry.Init();

    Serial.println("All modules configured successfully!");

    // Read current state
    door_state = digitalRead(DOOR_PIN);
    e_stop_state = digitalRead(E_STOP_PIN);

    door_last_state = door_state;

    // Initial report
    report_door();
    report_e_stop();
    
}

void report_door() {
    Serial.print("Report enclosure: ");
    Serial.println(!door_state);
    uint8_t report[1] = {door_state};
    registry.SendMessage(FUNC_REPORT_ENCLOSURE, report, 1);
}

void report_e_stop() {
    Serial.print("Report emergency stop: ");
    Serial.println(!e_stop_state);
    uint8_t report[1] = {!e_stop_state};
    registry.SendMessage(FUNC_REPORT_STOP_SWITCH, report, 1);
}

void set_enclosure_light(uint8_t value) {
    Serial.print("Set enclosure light: ");
    Serial.println(value);
    digitalWrite(LIGHT_PIN, value);
}

void set_fan_power(uint8_t value) {
    Serial.print("Set fan_power: ");
    Serial.println(value);
    digitalWrite(FAN_PIN, value);
}

void emergency_stop() {
    Serial.println("Emergency stop!");
    digitalWrite(LIGHT_PIN, 0);
    digitalWrite(FAN_PIN, 0);
    digitalWrite(E_STOP_R_PIN, 1);
    return registry.DeathLoop();
}


void door_loop() {
    bool reading = digitalRead(DOOR_PIN);
  
    if (reading != door_last_state)
        door_debounce_time = millis();
    
    if ((millis() - door_debounce_time) > DOOR_DEBOUNCE_DELAY) {
        bool prev_state = door_state;
        
        if (door_state != reading) {
            door_state = reading;
            report_door();
        }
    }

    door_last_state = reading;
}

void e_stop_loop() {
    bool reading = digitalRead(E_STOP_PIN);
  
    if (e_stop_state != reading)
        e_stop_state = reading;
        report_e_stop();
}

void loop() {
    if (registry.ReadMessage(&funcId, data, &dataLen)) {
        switch (funcId) {
            case FUNC_REPORT_STOP_SWITCH:
                report_e_stop();
                break;
            case FUNC_REPORT_ENCLOSURE:
                report_door();
                break;
            case FUNC_SET_ENCLOSURE_LIGHT:
                set_enclosure_light(data[1]);
                break;
            case FUNC_SET_FAN_MODULE:
                set_fan_power(data[1]);
                break;
            case FUNC_EMERGENCY_STOP:
                emergency_stop();
                break;
        }
    }

    door_loop();
    e_stop_loop();
}
