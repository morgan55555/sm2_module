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
#ifndef MODULES_CONFIG_CMDS_H_
#define MODULES_CONFIG_CMDS_H_


typedef enum {
  CMD_REPORT_MODULE_ID     = 1,  // 1
} GEN_CMD;

typedef enum {
  CMD_HEARTBEAT            = 1,  // 1
  CMD_E_STOP               = 2,  // 2
} MGMT_CMD;

typedef enum {
  CMD_M_CONFIG = 0,            // 0
  CMD_S_CONFIG_REACK,          // 1
  CMD_M_REQUEST_FUNCID,        // 2
  CMD_S_REPORT_FUNCID,         // 3
  CMD_M_CONFIG_FUNCID,         // 4
  CMD_S_CONFIG_FUNCID_REACK,   // 5
  CMD_M_UPDATE_REQUEST,        // 6
  CMD_S_UPDATE_REQUEST_REACK,  // 7
  CMD_M_UPDATE_PACKDATA,       // 8
  CMD_S_UPDATE_PACK_REQUEST,   // 9
  CMD_M_UPDATE_END,            // 10 (a)
  CMD_M_VERSIONS_REQUEST,      // 11 (b)
  CMD_S_VERSIONS_REACK,        // 12 (c)
  CMD_M_SET_RANDOM,            // 13 (d)
  CMD_S_SET_RANDOM_REACK,       // 14 (e)
  CMD_M_SET_LINEAR_LEN,         // 15 (f)
  CMD_S_SET_LINEAR_LEN_REACK,   // 16 (10)
  CMD_M_SET_LINEAR_LEAD,        // 17 (11)
  CMD_S_SET_LINEAR_LEAD_REACK,  // 18 (12)
  CMD_M_SET_LINEAR_LIMIT,       // 19 (13)
  CMD_S_SET_LINEAR_LIMIT_REACK, // 20 (14)
  CMD_M_UPDATE_STATUS_REQUEST,  // 21 (15)
  CMD_S_UPDATE_STATUS_REACK,    // 22 (16)
  CMD_M_UPDATE_START,           // 23 (17)
  CMD_M_DEBUG_INFO = 0xFE,
  CMD_S_DEBUG_INFO = 0xFF,
} SYSTEM_CMD;


#endif //MODULES_CONFIG_CMDS_H_
