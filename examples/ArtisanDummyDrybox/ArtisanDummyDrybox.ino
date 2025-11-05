#include <sm2registry.h>
#include <sm2module.h>

#define DEBUG_MODE 1

SM2Registry registry(10);
SM2Module vmodule(MODULE_DRYBOX, 123);

uint16_t funcId;
uint8_t dataLen = 0;
uint8_t data[8];

uint16_t timer = 0;

void setup() {
   Serial.begin(115200);

   // Register modules
   registry.RegisterModule(&vmodule);

   // Init registry and modules
   registry.Init();

   Serial.println("All modules configured successfully!");
}

void report_humidity() {
   float heater_temp = 24.0;
   float chamber_temp = 23.0;
   float chamber_humidity = 22.0;

   uint16_t h_temp = heater_temp;
   uint16_t c_temp = chamber_temp;
   uint16_t c_hum = chamber_humidity;

   uint8_t data[6] = {
       h_temp >> 8,
       h_temp & 0xff,
       c_temp >> 8,
       c_temp & 0xff,
       c_hum >> 8,
       c_hum & 0xff
   };
   registry.SendMessage(FUNC_REPORT_TEMP_HUMIDITY, data, sizeof(data));
}

void report_time() {
   int32_t target_time = 60*60;
   int32_t acc_dry_time = 80*60;
   int32_t remain_time = 30*60;

   uint8_t buf[8];
   uint8_t index = 0;
  
   index = 0;
   buf[index++] = 0;
   buf[index++] = (target_time >> 24) & 0xff;
   buf[index++] = (target_time >> 16) & 0xff;
   buf[index++] = (target_time >> 8) & 0xff;
   buf[index++] = target_time & 0xff;
   registry.SendMessage(FUNC_REPORT_HEATING_TIME_INFO, buf, index);

   index = 0;
   buf[index++] = 1;
   buf[index++] = (acc_dry_time >> 24) & 0xff;
   buf[index++] = (acc_dry_time >> 16) & 0xff;
   buf[index++] = (acc_dry_time >> 8) & 0xff;
   buf[index++] = acc_dry_time & 0xff;
   registry.SendMessage(FUNC_REPORT_HEATING_TIME_INFO, buf, index);

   index = 0;
   buf[index++] = 2;
   buf[index++] = (remain_time >> 24) & 0xff;
   buf[index++] = (remain_time >> 16) & 0xff;
   buf[index++] = (remain_time >> 8) & 0xff;
   buf[index++] = remain_time & 0xff;
   registry.SendMessage(FUNC_REPORT_HEATING_TIME_INFO, buf, index);
}

void report_power_state() {
   uint8_t report[1] = {1};
   registry.SendMessage(FUNC_REPORT_HEATER_POWER_STATE, report, 1);
}

void report_cover_state() {
   uint8_t report[1] = {0};
   registry.SendMessage(FUNC_REPORT_COVER_STATE, report, 1);
}

void report_drybox_state() {
   uint8_t report[1] = {0};
   registry.SendMessage(FUNC_REPORT_DRYBOX_STATE, report, 1);
}

void emergency_stop() {
   Serial.println("Emergency stop!");
   return registry.DeathLoop();
}

void loop() {
   if (registry.ReadMessage(&funcId, data, &dataLen)) {
       switch (funcId) {
           case FUNC_SET_TEMPEARTURE:
               Serial.println("Set temp!");
               break;
           case FUNC_SET_HEAT_TIME:
               Serial.println("Set time!");
               break;
           case FUNC_REPORT_TEMP_HUMIDITY:
               report_humidity();
               break;
           case FUNC_REPORT_TEMP_PID:
               Serial.println("Report pid!");
               break;
           case FUNC_SET_PID:
               Serial.println("Set pid!");
               break;
           case FUNC_MODULE_START:
               Serial.println("Start!");
               break;
           case FUNC_SET_MAINCTRL_TYPE:
               Serial.println("Set type!");
               break;
           case FUNC_REPORT_HEATER_POWER_STATE:
               report_power_state();
               break;
           case FUNC_REPORT_COVER_STATE:
               report_cover_state();
               break;
           case FUNC_REPORT_DRYBOX_STATE:
               report_drybox_state();
               break;
           case FUNC_EMERGENCY_STOP:
               emergency_stop();
               break;
       }
   }

   if ((timer + 2000) < millis()) {
       timer = millis();
   
       report_humidity();
       report_time();
   }
}