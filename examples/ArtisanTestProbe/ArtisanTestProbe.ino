#include <sm2registry.h>
#include <sm2module.h>

#define DEBUG_MODE 1
#define BTN_PIN 5
#define DEBOUNCE_DELAY 50

SM2Registry registry(10);
SM2Module vmodule(MODULE_CALIBRATOR, 123);

uint16_t funcId;
uint8_t dataLen = 0;
uint8_t data[8];

uint64_t debounce_time;
bool last_state;
bool state;

void setup() {
   Serial.begin(115200);

   // Init pins
   pinMode(BTN_PIN, INPUT_PULLUP);

   // Register modules
   registry.RegisterModule(&vmodule);

   // Init registry and modules
   registry.Init();

   Serial.println("All modules configured successfully!");

   // Read current state
   state = digitalRead(BTN_PIN);
   last_state = state;

   // Initial report
   report_state();
}

void report_state() {
   Serial.print("Report state: ");
   Serial.println(!state);
   uint8_t report[1] = {state};
   registry.SendMessage(FUNC_REPORT_PROBE, report, 1);
}

void emergency_stop() {
   Serial.println("Emergency stop!");
   return registry.DeathLoop();
}

void btn_loop() {
   bool reading = digitalRead(BTN_PIN);
 
   if (reading != last_state)
       debounce_time = millis();
   
   if ((millis() - debounce_time) > DEBOUNCE_DELAY) {
       bool prev_state = state;
       
       if (state != reading) {
           state = reading;
           report_state();
       }
   }

   last_state = reading;
}

void loop() {
   if (registry.ReadMessage(&funcId, data, &dataLen)) {
       switch (funcId) {
           case FUNC_REPORT_PROBE:
               report_state();
               break;
           case FUNC_EMERGENCY_STOP:
               emergency_stop();
               break;
       }
   }

   btn_loop();
}