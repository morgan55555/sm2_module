/*
 * Snapmaker2-Modules library
 *
 * Based on Snapmaker2-Modules
 * (see https://github.com/Snapmaker/Snapmaker2-Modules)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "sm2registry.h"

SM2Registry::SM2Registry(const uint8_t csPin, const uint8_t intPin) : can(csPin) {
    int_pin = intPin;
}

void SM2Registry::RegisterModule(SM2Module *sm2module) {
    modules[module_count++] = sm2module;
}

void SM2Registry::Init(const uint32_t spiClock) {
    while (can.begin(MCP_ANY, CAN_500KBPS, spiClock) != CAN_OK)
        delay(10);

    can.setMode(MCP_NORMAL);

    if(int_pin)
        pinMode(int_pin, INPUT);

    // Init all modules
    for (uint8_t i = 0; i < module_count; i++)
        modules[i]->Init();

    ConfigureModules();
    FillFuncMsgIds();
}

uint16_t SM2Registry::FuncId2MsgId(uint16_t funcId) {
    for (int i = 0; i < func_count; ++i) {
        if (func_ids[i] == funcId) {
            return msg_ids[i] | 0x600;
        }
    }
    return INVALID_VALUE;
}

uint16_t SM2Registry::MsgId2FuncId(uint16_t msgId) {
    for (int i = 0; i < func_count; ++i) {
        if (msg_ids[i] == msgId) {
            return func_ids[i];
        }
    }
    return INVALID_VALUE;
}

bool SM2Registry::ReadMessage(uint16_t *funcId, uint8_t data[8], uint8_t *dataLen) {
    if (!ReadNext() || rx_ext_flag)
        return false;

    if (rx_rtr_flag)
        *funcId = MgmtCmd2FuncId(rx_id);
    else
        *funcId = MsgId2FuncId(rx_id);

    for (uint8_t i = 0; i < rx_len; i++)
        data[i] = rx_buf[i];

    *dataLen = rx_len;

    return true;
}

bool SM2Registry::SendMessage(uint16_t funcId, uint8_t data[8], uint8_t dataLen) {
    uint32_t txId = FuncId2MsgId(funcId);
    return (can.sendMsgBuf(txId, dataLen, data) == CAN_OK);
}

void SM2Registry::DeathLoop() {
    while(true)
        delay(10);
}


bool SM2Registry::ReadNext() {
    // No messages available (if configured with interrupt pin)
    if(int_pin && digitalRead(int_pin))
        return false;

    // No messages readed
    if(can.readMsgBuf(&rx_id, &rx_len, rx_buf) != CAN_OK)
        return false;

    // Message catched, set flags
    rx_ext_flag = (rx_id & CAN_EXT_FLAG) == CAN_EXT_FLAG;
    rx_rtr_flag = (rx_id & CAN_RTR_FLAG) == CAN_RTR_FLAG;

    // Mask id
    rx_id = rx_id & CAN_EXT_MASK;

    // Catched CAN message
    return true;
}

void SM2Registry::ConfigureModules() {
    if (!module_count)
        return;

    SM2Module *curr_module = nullptr;
    bool modules_configured = false;

    while (!modules_configured) {

        // Get new CAN msg
        if (!ReadNext())
            continue;

        // Process mgmt cmds
        if (!rx_ext_flag && rx_rtr_flag) {
            ProcessMgmtCmd(MGMT_CMD(rx_id));
            continue;
        }

        modules_configured = true;

        // Pass msg to all unconfigured modules
        for (uint8_t i = 0; i < module_count; i++) {
            curr_module = modules[i];

            if (curr_module->IsConfigured())
                continue;

            if (rx_ext_flag && rx_rtr_flag)
                curr_module->ProcessGenCmd(GEN_CMD(rx_id));
            else if (rx_ext_flag && !rx_rtr_flag)
                curr_module->ProcessLongpack(rx_id, rx_buf, rx_len);

            if (!curr_module->IsConfigured())
                modules_configured = false;
        }
    }
}

void SM2Registry::FillFuncMsgIds() {
    // Copy FuncIds and MsgIds from all modules
    for (uint8_t i = 0; i < module_count; i++) {
        for (uint8_t j = 0; j < modules[i]->func_count; j++) {
            if (FuncId2MsgId(modules[i]->func_ids[j]) == INVALID_VALUE) {
                func_ids[func_count] = modules[i]->func_ids[j];
                msg_ids[func_count] = modules[i]->msg_ids[j];
                func_count++;
            }
        }
    }
}

void SM2Registry::ProcessMgmtCmd(MGMT_CMD cmd) {
    switch (cmd) {
        case CMD_E_STOP:
            // If we catched E_STOP at init stage
            // We need to freeze all before power cycle
            return DeathLoop();
    }
}

uint16_t SM2Registry::MgmtCmd2FuncId(MGMT_CMD cmd) {
    switch(cmd) {
        case CMD_E_STOP:
            return FUNC_EMERGENCY_STOP;
        case CMD_HEARTBEAT:
            return FUNC_HEARTBEAT;
        default:
            return INVALID_VALUE;
    }
}