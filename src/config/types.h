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
#ifndef MODULES_CONFIG_TYPES_H_
#define MODULES_CONFIG_TYPES_H_


typedef enum {
  MODULE_UNPLUGGED         = -1, // -1 HEAD_UNPLUGGED
  MODULE_PRINT             = 0,  // 0
  MODULE_CNC               = 1,  // 1
  MODULE_LASER             = 2,  // 2
  MODULE_LINEAR            = 3,  // 3 LINEAR_MODULE_TBS_2019
  MODULE_LIGHT             = 4,  // 4 ADDON_LEDS
  MODULE_ENCLOSURE         = 5,  // 5
  MODULE_ROTATE            = 6,  // 6 ROTARY_MODULE
  MODULE_PURIFIER          = 7,  // 7
  MODULE_EMERGENCY_STOP    = 8,  // 8
  MODULE_CNC_TOOL_SETTING  = 9,  // 9 ADDON_CNC_CALIBRATION
  MODULE_PRINT_V_SM1       = 10, // 10 ADDON_TOOL_HEAD_ORIGINAL_TO_2_0_CONVERTER
  MODULE_FAN               = 11, // 11
  MODULE_LINEAR_TMC        = 12, // 12 LINEAR_MODULE_TMC_2021
  MODULE_DUAL_EXTRUDER     = 13, // 13
  MODULE_LASER_10W         = 14, // 14
  MODULE_CNC_200W          = 15, // 15
  MODULE_ENCLOSURE_A400    = 16, // 16
  MODULE_DRYBOX            = 17, // 17
  MODULE_CALIBRATOR        = 18, // 18
  MODULE_LASER_20W         = 19, // 19
  MODULE_LASER_40W         = 20, // 20
  MODULE_ROTARY_2023       = 21, // 21
  MODULE_LASER_2W_INFRARED = 23, // 23
  MODULE_HEATED_BED_S20    = 512, // 512
  MODULE_HEATED_BED_J1     = 513, // 513
  MODULE_HEATED_BED_A400   = 515, // 515
  MODULE_LINEAR_A400       = 516, // 516
  MODULE_EMERGENCY_STOP_A400 = 517, // 517
  MODULE_SNAPMAKER2_CONTROL = 2048, // 2048
  MODULE_J1_CONTROL         = 2049, // 2049
  MODULE_A400_CONTROL       = 2050  // 2050

} MODULE_TYPE;


#endif //MODULES_CONFIG_TYPES_H_
