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
#include "sm2module.h"

#define FUNC_LIST_INIT(list)  func_ids = list;\
                              func_count = sizeof(list) / sizeof(list[0]);

#define MAC_SEND_POSTFIX 0x1
#define APP_VERSIONS_SIZE 32
#define MODULE_INDEX_NONE 0xFF


SM2Module::SM2Module(MODULE_TYPE moduleType, uint32_t random,
                     uint8_t indexPin, uint16_t axisLen, uint8_t axisLimitSite) {
    
    module_type = moduleType;
    module_mac = (((long(moduleType) & 0x1ff) << 20) | ((long(random) & 0x7ffff) << 1));

    index_pin = indexPin;
    axis_len = axisLen;
    axis_limit = axisLimitSite;

    switch (moduleType) {
        case MODULE_PRINT:
        case MODULE_PRINT_V_SM1:
            FUNC_LIST_INIT(print_func_list_);
            break;
        case MODULE_DUAL_EXTRUDER:
            FUNC_LIST_INIT(dual_extruder_func_list_);
            break;
        case MODULE_LASER:
            FUNC_LIST_INIT(laser_func_list_);
            break;
        case MODULE_LASER_10W:
            FUNC_LIST_INIT(laser_10w_func_list_);
            break;
        case MODULE_CNC:
            FUNC_LIST_INIT(cnc_func_list_);
            break;
        case MODULE_LINEAR:
        case MODULE_LINEAR_TMC:
            FUNC_LIST_INIT(linear_func_list_);
            break;
        case MODULE_LIGHT:
            FUNC_LIST_INIT(light_func_list_);
            break;
        case MODULE_CNC_TOOL_SETTING:
            FUNC_LIST_INIT(tool_setting_func_list_);
            break;
        case MODULE_ENCLOSURE:
            FUNC_LIST_INIT(enclosure_func_list_);
            break;
        case MODULE_FAN:
            FUNC_LIST_INIT(fan_func_list_);
            break;
        case MODULE_PURIFIER:
            FUNC_LIST_INIT(purifier_func_list_);
            break;
        case MODULE_EMERGENCY_STOP:
            FUNC_LIST_INIT(stop_func_list_);
            break;
        case MODULE_CNC_200W:
            FUNC_LIST_INIT(cnc_200w_func_list_);
            break;
        case MODULE_ENCLOSURE_A400:
            FUNC_LIST_INIT(enclosure_a400_func_list_);
            break;
        case MODULE_DRYBOX:
            FUNC_LIST_INIT(drybox_func_list_);
            break;
        case MODULE_CALIBRATOR:
            FUNC_LIST_INIT(calibrator_func_list_);
            break;
        case MODULE_LASER_20W:
        case MODULE_LASER_40W:
            FUNC_LIST_INIT(laser_20w40w_func_list_);
            break;
        default:
            FUNC_LIST_INIT(default_func_list_);
            break;
    }
}


void SM2Module::Init(MCP_CAN *canBus) {
    can = canBus;

    longpack.Init(can, module_mac | CAN_EXT_FLAG | MAC_SEND_POSTFIX);

    if(index_pin)
        pinMode(index_pin, INPUT);
}


void SM2Module::ProcessGenCmd(GEN_CMD cmd) {
    switch (cmd) {
        case CMD_REPORT_MODULE_ID:
            ReportModuleMac();
            break;
    }
}


void SM2Module::ProcessLongpack(uint32_t rxId, uint8_t rxBuf[8], uint8_t rxLen) {
    if (rxId != module_mac)
        return;

    if (longpack.readLongpack(rxBuf, rxLen) != E_TRUE)
        return;

    SYSTEM_CMD cmd = SYSTEM_CMD(longpack.cmd[0]);
    uint8_t * cmdData = longpack.cmd + 1;

    switch (cmd) {
        case CMD_M_CONFIG:
            ReportModuleIndex(cmdData);
            break;
        case CMD_M_REQUEST_FUNCID:
            ReportFunctionIds();
            break;
        case CMD_M_CONFIG_FUNCID:
            RegisterMsgId(cmdData);
            break;
        case CMD_M_UPDATE_REQUEST:
            IsUpdate(cmdData);
            break;
        case CMD_M_VERSIONS_REQUEST:
            ReportVersions(cmdData);
            break;
        case CMD_M_DEBUG_INFO:
            ReportFuncidAndMsgid();
            break;

        // Others CMDs not supported.
        // It's only virtual module for getting msg_ids.
        //
        // If you need fully functional module,
        // you can rewrite original SNAPMAKER2-MODULES
        // to mcp2515 library.

    }

    longpack.cmd_clean();
}


bool SM2Module::IsConfigured() {
    return is_configured;
}


uint16_t SM2Module::FuncId2MsgId(uint16_t funcId) {
    for (int i = 0; i < func_count; ++i) {
        if (func_ids[i] == funcId) {
            return msg_ids[i] | 0x600;
        }
    }
    return INVALID_VALUE;
}


uint16_t SM2Module::MsgId2FuncId(uint16_t msgId) {
    for (int i = 0; i < func_count; ++i) {
        if (msg_ids[i] == msgId) {
            return func_ids[i];
        }
    }
    return INVALID_VALUE;
}


void SM2Module::ReportModuleMac() {
    can->sendMsgBuf(module_mac | CAN_RTR_FLAG | CAN_EXT_FLAG | MAC_SEND_POSTFIX, 0, nullptr);
}


bool SM2Module::SetModuleIndex(uint8_t axis_index) {
    module_index = MODULE_INDEX_NONE;
    if (index_pin && digitalRead(index_pin)) {
      module_index = axis_index;
    }
    return module_index != MODULE_INDEX_NONE;
}


void SM2Module::ReportModuleIndex(uint8_t * data) {
    uint8_t   cache[8];
    uint8_t   index  = 0;
    cache[index++] = CMD_S_CONFIG_REACK;
    cache[index++] = SetModuleIndex(data[0]);
    if ((module_type == MODULE_LINEAR) || (module_type == MODULE_LINEAR_TMC)) {
        cache[index++] = axis_len >> 8;
        cache[index++] = (axis_len & 0xff);
        cache[index++] = axis_limit;
    }
    longpack.sendLongpack(cache, index);
}


void SM2Module::ReportFunctionIds() {
    uint16_t index = 0;
    uint8_t cache[30];
    cache[index++] = CMD_S_REPORT_FUNCID;
    cache[index++] = func_count;
    for (int i = 0; i < func_count; i++) {
        cache[index++] = func_ids[i] >> 8;
        cache[index++] = func_ids[i];
    }
    longpack.sendLongpack(cache, index);
}


void SM2Module::RegisterMsgId(uint8_t * data) {
    uint8_t count = data[0];
    uint16_t msgid, funcid, index = 1;
    for (int i = 0; i < count; i++) {
        msgid = data[index++] << 8;
        msgid |= (data[index++] & 0xff);

        funcid = data[index++] << 8;
        funcid |= (data[index++] & 0xff);
        for (int j = 0; j < count; j++)
            if (funcid == func_ids[j]) {
                msg_ids[j] = msgid;
            }
    }
    is_configured = true;
}


void SM2Module::IsUpdate(uint8_t * data) {
    // We doesn't want to update with stock firmware
    uint8_t  send_data[2] = {CMD_S_UPDATE_REQUEST_REACK, 0};
    longpack.sendLongpack(send_data, 2);
}


void SM2Module::ReportVersions(uint8_t * data) {
    uint8_t versions[32];
    uint8_t index = 0;
    versions[index++] = CMD_S_VERSIONS_REACK;
    const char *version;
    if (data[0] == 0) 
        versions[index++] = 0;
    else
        versions[index++] = 1;
    version = FIRMWARE_VERSION;
    size_t length = strlen(version);
    memcpy(versions + index, version, length);
    index += length;
    longpack.sendLongpack(versions, index);
}


void SM2Module::ReportFuncidAndMsgid() {
    uint16_t index = 0, i;
    uint8_t cache[50];

    cache[index++] = CMD_S_DEBUG_INFO;
    cache[index++] = 0;
    cache[index++] = func_count;
    for (i = 0; i < func_count; i++) {
        cache[index++] = func_ids[i] >> 8;
        cache[index++] = func_ids[i];
        cache[index++] = msg_ids[i] >> 8;
        cache[index++] = msg_ids[i];
    }
    longpack.sendLongpack(cache, index);
}
