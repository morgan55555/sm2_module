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
#ifndef MODULES_MODULE_H_
#define MODULES_MODULE_H_

#include <stdint.h>
#include <mcp_can.h>
#include "config/main.h"
#include "longpack.h"

#define FUNC_MAX_LEN 30

class SM2Module {
  public:
    SM2Module(MODULE_TYPE moduleType, uint32_t random,
              uint8_t indexPin = 0, uint16_t axisLen = 0, uint8_t axisLimitSite = 0);

    void Init(MCP_CAN *canBus);

    void ProcessGenCmd(GEN_CMD cmd);
    void ProcessLongpack(uint32_t rxId, uint8_t rxBuf[8], uint8_t rxLen);

    bool IsConfigured();

    uint16_t FuncId2MsgId(uint16_t funcId);
    uint16_t MsgId2FuncId(uint16_t msgId);

    MODULE_TYPE module_type;
    uint32_t module_mac;
    uint8_t module_index;

    uint8_t func_count = 0;
    uint16_t const *func_ids = nullptr;
    uint16_t msg_ids[FUNC_MAX_LEN];
  
  private:
    void ReportModuleMac();

    bool SetModuleIndex(uint8_t axis_index);

    void ReportModuleIndex(uint8_t * data);
    void ReportFunctionIds();
    void RegisterMsgId(uint8_t * data);
    void IsUpdate(uint8_t * data);
    void ReportVersions(uint8_t * data);
    void ReportFuncidAndMsgid();

    uint8_t index_pin;
    uint16_t axis_len;
    uint8_t axis_limit;

    bool is_configured;

    Longpack longpack;
    MCP_CAN *can;
};


#endif //MODULES_MODULE_H_
