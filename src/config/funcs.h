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
#ifndef MODULES_CONFIG_FUNCS_H_
#define MODULES_CONFIG_FUNCS_H_

#include <stdint.h>

#define INVALID_VALUE 9999


typedef enum {
    FUNC_REPORT_LIMIT                       ,   // 0
    FUNC_REPORT_PROBE                       ,   // 1
    FUNC_REPORT_CUT                         ,   // 2
    FUNC_SET_STEP_CTRL                      ,   // 3
    FUNC_SET_MOTOR_SPEED                    ,   // 4
    FUNC_REPORT_MOTOR_SPEED                 ,   // 5
    FUNC_REPORT_TEMPEARTURE                 ,   // 6
    FUNC_SET_TEMPEARTURE                    ,   // 7
    FUNC_SET_FAN                            ,   // 8
    FUNC_SET_FAN2                           ,   // 9
    FUNC_SET_PID                            ,   // 10
    FUNC_SET_CAMERA_POWER                   ,   // 11
    FUNC_SET_LASER_FOCUS                    ,   // 12
    FUNC_REPORT_LASER_FOCUS                 ,   // 13
    FUNC_SET_LIGHT_COLOR                    ,   // 14
    FUNC_REPORT_ENCLOSURE                   ,   // 15
    FUNC_REPORT_TEMP_PID                    ,   // 16
    FUNC_REPORT_TOOL_SETTING                ,   // 17
    FUNC_SET_ENCLOSURE_LIGHT                ,   // 18
    FUNC_SET_FAN_MODULE                     ,   // 19
    FUNC_REPORT_STOP_SWITCH                 ,   // 20
    FUNC_TMC_IOCTRL                         ,   // 21
    FUNC_TMC_PUBLISH                        ,   // 22
    FUNC_SET_PURIFIER                       ,   // 23
    FUNC_REPORT_PURIFIER                    ,   // 24
    FUNC_SET_AUTOFOCUS_LIGHT                ,   // 25
    FUNC_REPORT_SECURITY_STATUS             ,   // 26
    FUNC_MODULE_ONLINE_SYNC                 ,   // 27
    FUNC_MODULE_SET_TEMP                    ,   // 28
    FUNC_MODULE_LASER_CTRL                  ,   // 29
    FUNC_MODULE_GET_HW_VERSION              ,   // 30
    FUNC_REPORT_PIN_STATUS                  ,   // 31
    FUNC_CONFIRM_PIN_STATUS                 ,   // 32
    FUNC_SWITCH_EXTRUDER                    ,   // 33
    FUNC_REPORT_NOZZLE_TYPE                 ,   // 34
    FUNC_SET_FAN_NOZZLE                     ,   // 35
    FUNC_REPORT_EXTRUDER_INFO               ,   // 36
    FUNC_SET_EXTRUDER_CHECK                 ,   // 37
    FUNC_SET_MOTOR_SPEED_RPM                ,   // 38
    FUNC_SET_MOTOR_CTR_MODE                 ,   // 39
    FUNC_SET_MOTOR_RUN_DIRECTION            ,   // 40
    FUNC_REPORT_MOTOR_STATUS_INFO           ,   // 41
    FUNC_REPORT_MOTOR_SENSOR_INFO           ,   // 42
    FUNC_REPORT_TEMP_HUMIDITY               ,   // 43
    FUNC_SET_HOTEND_OFFSET                  ,   // 44
    FUNC_REPORT_HOTEND_OFFSET               ,   // 45
    FUNC_SET_PROBE_SENSOR_COMPENSATION      ,   // 46
    FUNC_REPORT_PROBE_SENSOR_COMPENSATION   ,   // 47
    FUNC_SET_HEAT_TIME                      ,   // 48
    FUNC_REPORT_HEATING_TIME_INFO           ,   // 49
    FUNC_SET_MAINCTRL_TYPE                  ,   // 50
    FUNC_MODULE_START                       ,   // 51
    FUNC_REPORT_HEATER_POWER_STATE          ,   // 52
    FUNC_REPORT_MOTOR_SELF_TEST_INFO        ,   // 53
    FUNC_REPORT_COVER_STATE                 ,   // 54
    FUNC_REPORT_DRYBOX_STATE                ,   // 55
    FUNC_MOVE_TO_DEST                       ,   // 56
    FUNC_SET_RIGHT_EXTRUDER_POS             ,   // 57
    FUNC_REPORT_RIGHT_EXTRUDER_POS          ,   // 58
    FUNC_PROXIMITY_SWITCH_POWER_CTRL        ,   // 59
    FUNC_SET_CROSSLIGHT                     ,   // 60
    FUNC_GET_CROSSLIGHT_STATE               ,   // 61
    FUNC_SET_FIRE_SENSOR_SENSITIVITY        ,   // 62
    FUNC_GET_FIRE_SENSOR_SENSITIVITY        ,   // 63
    FUNC_SET_FIRE_SENSOR_REPORT_TIME        ,   // 64
    FUNC_REPORT_FIRE_SENSOR_RAW_DATA        ,   // 65
    FUNC_SET_CROSSLIGHT_OFFSET              ,   // 66
    FUNC_GET_CROSSLIGHT_OFFSET              ,   // 67
    FUNC_MODULE_LASER_BRANCH_CTRL           ,   // 68
    FUNC_SET_RIGHT_LEVEL_MODE               ,   // 69
    FUNC_REPORT_RIGHT_LEVEL_MODE_INFO       ,   // 70

    FUNC_EMERGENCY_STOP = 0xFE,
    FUNC_HEARTBEAT = 0xFF,
} FUNC_ID;


const uint16_t print_func_list_[] = {
  FUNC_SET_FAN,
  FUNC_SET_FAN2,
  FUNC_REPORT_TEMPEARTURE,
  FUNC_SET_TEMPEARTURE,
  FUNC_REPORT_PROBE,
  FUNC_SET_PID,
  FUNC_REPORT_CUT,
  FUNC_REPORT_TEMP_PID,
};

const uint16_t dual_extruder_func_list_[] = {
  FUNC_SET_FAN,
  FUNC_SET_FAN2,
  FUNC_REPORT_TEMPEARTURE,
  FUNC_SET_TEMPEARTURE,
  FUNC_REPORT_PROBE,
  FUNC_SET_PID,
  FUNC_REPORT_CUT,
  FUNC_REPORT_TEMP_PID,
  FUNC_SWITCH_EXTRUDER,
  FUNC_REPORT_NOZZLE_TYPE,
  FUNC_SET_FAN_NOZZLE,
  FUNC_REPORT_EXTRUDER_INFO,
  FUNC_SET_EXTRUDER_CHECK,
  FUNC_SET_HOTEND_OFFSET,
  FUNC_REPORT_HOTEND_OFFSET,
  FUNC_SET_PROBE_SENSOR_COMPENSATION,
  FUNC_REPORT_PROBE_SENSOR_COMPENSATION,
  FUNC_MOVE_TO_DEST,
  FUNC_SET_RIGHT_EXTRUDER_POS,
  FUNC_REPORT_RIGHT_EXTRUDER_POS,
  FUNC_PROXIMITY_SWITCH_POWER_CTRL,
  FUNC_MODULE_GET_HW_VERSION,
  FUNC_SET_RIGHT_LEVEL_MODE,
  FUNC_REPORT_RIGHT_LEVEL_MODE_INFO,
};

const uint16_t laser_func_list_[] = {
  FUNC_SET_FAN,
  FUNC_SET_CAMERA_POWER,
  FUNC_SET_LASER_FOCUS,
  FUNC_REPORT_LASER_FOCUS,
};

const uint16_t laser_10w_func_list_[] = {
  FUNC_SET_FAN,
  FUNC_SET_CAMERA_POWER,
  FUNC_SET_LASER_FOCUS,
  FUNC_REPORT_LASER_FOCUS,
  FUNC_SET_AUTOFOCUS_LIGHT,
  FUNC_REPORT_SECURITY_STATUS,
  FUNC_MODULE_ONLINE_SYNC,
  FUNC_MODULE_SET_TEMP,
  FUNC_MODULE_LASER_CTRL,
  FUNC_MODULE_GET_HW_VERSION,
  FUNC_REPORT_PIN_STATUS,
  FUNC_CONFIRM_PIN_STATUS,
};

const uint16_t cnc_func_list_[] = {
  FUNC_REPORT_MOTOR_SPEED,
  FUNC_SET_MOTOR_SPEED,
};

const uint16_t enclosure_func_list_[] = {
  FUNC_REPORT_ENCLOSURE,
  FUNC_SET_ENCLOSURE_LIGHT,
  FUNC_SET_FAN_MODULE,
};

const uint16_t tool_setting_func_list_[] = {
    FUNC_REPORT_TOOL_SETTING,
};

const uint16_t light_func_list_[] = {
  FUNC_SET_LIGHT_COLOR,
};

const uint16_t linear_func_list_[] = {
  FUNC_REPORT_LIMIT,
};

const uint16_t stop_func_list_[] = {
  FUNC_REPORT_STOP_SWITCH,
};

const uint16_t purifier_func_list_[] = {
  FUNC_SET_PURIFIER,
  FUNC_REPORT_PURIFIER,
};

const uint16_t fan_func_list_[] = {
  FUNC_SET_FAN_MODULE,
};

const uint16_t cnc_200w_func_list_[] = {
  FUNC_SET_MOTOR_SPEED,
  FUNC_SET_FAN,
  FUNC_SET_PID,
  FUNC_MODULE_GET_HW_VERSION,
  FUNC_SET_MOTOR_SPEED_RPM,
  FUNC_SET_MOTOR_CTR_MODE,
  FUNC_SET_MOTOR_RUN_DIRECTION,
  FUNC_REPORT_MOTOR_STATUS_INFO,
  FUNC_REPORT_MOTOR_SENSOR_INFO,
  FUNC_REPORT_MOTOR_SELF_TEST_INFO,
};

const uint16_t enclosure_a400_func_list_[] = {
  FUNC_REPORT_ENCLOSURE,
  FUNC_SET_ENCLOSURE_LIGHT,
  FUNC_SET_FAN_MODULE,
  FUNC_MODULE_GET_HW_VERSION,
};

const uint16_t drybox_func_list_[] = {
  FUNC_SET_FAN,
  FUNC_SET_TEMPEARTURE,
  FUNC_REPORT_TEMP_HUMIDITY,
  FUNC_REPORT_TEMP_PID,
  FUNC_SET_PID,
  FUNC_SET_HEAT_TIME,
  FUNC_REPORT_HEATING_TIME_INFO,
  FUNC_SET_MAINCTRL_TYPE,
  FUNC_MODULE_START,
  FUNC_REPORT_HEATER_POWER_STATE,
  FUNC_REPORT_COVER_STATE,
  FUNC_REPORT_DRYBOX_STATE,
};

const uint16_t calibrator_func_list_[] = {
  FUNC_REPORT_PROBE,
};

const uint16_t laser_20w40w_func_list_[] = {
  FUNC_SET_FAN,
  FUNC_SET_CAMERA_POWER,
  FUNC_SET_LASER_FOCUS,
  FUNC_REPORT_LASER_FOCUS,
  FUNC_SET_AUTOFOCUS_LIGHT,
  FUNC_REPORT_SECURITY_STATUS,
  FUNC_MODULE_ONLINE_SYNC,
  FUNC_MODULE_SET_TEMP,
  FUNC_MODULE_LASER_CTRL,
  FUNC_MODULE_GET_HW_VERSION,
  FUNC_REPORT_PIN_STATUS,
  FUNC_CONFIRM_PIN_STATUS,
  FUNC_SET_CROSSLIGHT,
  FUNC_GET_CROSSLIGHT_STATE,
  FUNC_SET_FIRE_SENSOR_SENSITIVITY,
  FUNC_GET_FIRE_SENSOR_SENSITIVITY,
  FUNC_SET_FIRE_SENSOR_REPORT_TIME,
  FUNC_REPORT_FIRE_SENSOR_RAW_DATA,
  FUNC_SET_CROSSLIGHT_OFFSET,
  FUNC_GET_CROSSLIGHT_OFFSET,
  FUNC_MODULE_LASER_BRANCH_CTRL,
};

const uint16_t default_func_list_[] = {};


#endif //MODULES_CONFIG_FUNCS_H_
