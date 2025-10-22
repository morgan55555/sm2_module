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
#ifndef MODULES_CONFIG_MAIN_H_
#define MODULES_CONFIG_MAIN_H_

#include "cmds.h"
#include "funcs.h"
#include "types.h"

#define FIRMWARE_VERSION "v9.9.9"
#define MODULE_MAX_COUNT 4
#define MAX_SYS_CMD_LEN 64

#define CAN_RTR_FLAG 0x40000000
#define CAN_EXT_FLAG 0x80000000
#define CAN_EXT_MASK 0x1FFFFFFF

#endif //MODULES_CONFIG_MAIN_H_
