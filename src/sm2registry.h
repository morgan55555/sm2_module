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
#ifndef MODULES_SM2REGISTRY_H_
#define MODULES_SM2REGISTRY_H_

#include <stdint.h>
#include <mcp_can.h>
#include "config/main.h"
#include "sm2module.h"


class SM2Registry {
  public:
    SM2Registry(const uint8_t csPin, const uint8_t intPin = 0);

    // SM2-Controller will discover our device as different modules
    void RegisterModule(SM2Module *sm2module);

    // Init cycle, we need to respond for each module on next messages:
    // CMD_REPORT_MODULE_ID (Extended RTR)
    // CMD_M_CONFIG, CMD_M_REQUEST_FUNCID, CMD_M_CONFIG_FUNCID, CMD_M_VERSIONS_REQUEST (Longpack)
    // Also we need poll CMD_E_STOP (Std RTR)
    void Init(const uint32_t spiClock = MCP_8MHZ);

    uint16_t MsgId2FuncId(uint16_t msgId);
    uint16_t FuncId2MsgId(uint16_t funcId);

    // Use next funcs in user cycle.
    // If Init successful, CMD_E_STOP no longer work automatically.
    // You need to manage it by FUNC_EMERGENCY_STOP

    // Read data.
    bool ReadMessage(uint16_t *funcId, uint8_t data[8], uint8_t *dataLen);
    
    // Send data.
    bool SendMessage(uint16_t funcId, uint8_t data[8], uint8_t dataLen);

    // Call this after CMD_E_STOP
    void DeathLoop();

    // Can bus
    MCP_CAN can;
  
  private:
    bool ReadNext();

    void ConfigureModules();
    void FillFuncMsgIds();

    void ProcessMgmtCmd(MGMT_CMD cmd);

    uint16_t MgmtCmd2FuncId(MGMT_CMD cmd);

    uint32_t rx_id;
    uint8_t rx_buf[8];
    uint8_t rx_len;
    bool rx_rtr_flag;
    bool rx_ext_flag;

    uint8_t int_pin;

    SM2Module * modules[MODULE_MAX_COUNT];
    uint8_t module_count = 0;

    uint16_t func_ids[FUNC_MAX_LEN];
    uint16_t msg_ids[FUNC_MAX_LEN];
    uint8_t func_count;
};


#endif //MODULES_SM2REGISTRY_H_
