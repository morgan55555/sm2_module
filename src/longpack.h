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
#ifndef MODULES_LONGPACK_H_
#define MODULES_LONGPACK_H_

#include <stdint.h>
#include <mcp_can.h>
#include "config/main.h"


#define MAGIC_PART_1 0xAA
#define MAGIC_PART_2 0x55


struct PackHead {
  uint8_t magic1;
  uint8_t magic2;
  uint8_t lenHigh;
  uint8_t lenLow;
  uint8_t version;
  uint8_t lenCheck;
  uint8_t dataCheckHigh;
  uint8_t dataCheckLow;
};


typedef enum ERR_E {
    E_TRUE = 0,
    E_FALSE,
    E_DOING,
} ERR_E;


uint16_t CalcChecksum(uint8_t *data, uint16_t len);


class Longpack {
  public:
    void Init(MCP_CAN *canBus, uint32_t txId);

    ERR_E readLongpack(uint8_t value);
    ERR_E readLongpack(uint8_t *data, uint8_t len);
    void sendLongpack(uint8_t *data, uint16_t len);
    void sendLongpack(uint16_t *data, uint16_t len);
    void cmd_clean();

    uint8_t * cmd = packData_ + sizeof(PackHead);
  
  private:
    void sendPack(uint8_t *data, uint16_t len);

    uint16_t recv_index_ = 0;

    uint8_t packData_[MAX_SYS_CMD_LEN];
    uint16_t len_ = 0;

    MCP_CAN *can;
    uint32_t tx_id;
};


#endif //MODULES_LONGPACK_H_
