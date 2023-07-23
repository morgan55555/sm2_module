#include <mcp_can.h>
#include <sm2module.h>

uint32_t rxId;
uint8_t len = 0;
uint8_t rxBuf[8];

MCP_CAN CAN0(10);
SM2Module vmodule(MODULE_ENCLOSURE, 123);

void setup() {
    Serial.begin(115200);

    // Init enclosure light pin
    pinMode(LED_BUILTIN, OUTPUT);

    // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s
    while (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
        delay(10);

    CAN0.setMode(MCP_NORMAL);
    Serial.println("MCP2515 Initialized Successfully!");

    vmodule.Init(&CAN0);

    while (!vmodule.IsConfigured()) {
        
        if (CAN0.readMsgBuf(&rxId, &len, rxBuf) == CAN_OK) {

            bool rxExt = (rxId & CAN_EXT_FLAG) == CAN_EXT_FLAG;
            bool rxRtr = (rxId & CAN_RTR_FLAG) == CAN_RTR_FLAG;
            uint32_t rxIdMasked = rxId & CAN_EXT_MASK;

            if (rxExt && rxRtr)
                vmodule.ProcessGenCmd(rxIdMasked);
            else if (rxExt && !rxRtr)
                vmodule.ProcessLongpack(rxIdMasked, rxBuf, len);

        }
    }

    Serial.println("VModule configured successfully!");
}


void loop() {

    if (CAN0.readMsgBuf(&rxId, &len, rxBuf) == CAN_OK) {

        if (((rxId & CAN_EXT_FLAG) == CAN_EXT_FLAG) ||
            ((rxId & CAN_RTR_FLAG) == CAN_RTR_FLAG))
            return;

        uint32_t rxIdMasked = rxId & CAN_EXT_MASK;
        uint16_t cmdId = vmodule.MsgId2FuncId(rxIdMasked);

        if (cmdId == FUNC_SET_ENCLOSURE_LIGHT) {
            Serial.print("Set enclosure light: ");
            Serial.println(rxBuf[1]);
            digitalWrite(LED_BUILTIN, rxBuf[1]);
        }
    }

}
