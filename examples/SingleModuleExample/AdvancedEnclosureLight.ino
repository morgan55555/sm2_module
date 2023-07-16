#include <sm2registry.h>
#include <sm2module.h>

SM2Registry registry(10);
SM2Module vmodule(&registry.can, MODULE_ENCLOSURE, 123);

uint16_t funcId;
uint8_t dataLen = 0;
uint8_t data[8];

void setup() {
    Serial.begin(115200);

    // Init enclosure light pin
    pinMode(LED_BUILTIN, OUTPUT);

    // Register module
    registry.RegisterModule(&vmodule);

    // Init registry and modules
    registry.Init();

    Serial.println("VModule configured successfully!");
}


void loop() {
    if (registry.ReadMessage(&funcId, data, &dataLen)) {
        switch (funcId) {
            case FUNC_SET_ENCLOSURE_LIGHT:
                Serial.print("Set enclosure light: ");
                Serial.println(data[1]);
                digitalWrite(LED_BUILTIN, data[1]);
                break;
        }
    }
}
