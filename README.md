# Simple Arduino Snapmaker 2 Module library
### by [morgan55555](https://github.com/morgan55555)

With this library you can make own simple SM2 modules.


### Requirements
 - [MCP_CAN_lib](https://github.com/coryjfowler/MCP_CAN_lib)


### Usage

First of all you have to include the module and the registry.
~~~
#include <sm2module.h>
#include <sm2registry.h>
~~~

Then, you need to create a Registry.
It contains CAN bus and all of the modules.
(yes, you can make one physical module, that contains up to 4 (for default) SM2 modules)
~~~
SM2Registry registry(CAN_CS_PIN, CAN_INTERRUPT_PIN_OPTIONAL);
~~~

Then create an virtual module. It's not a real module, just a fake module for gathering CAN ids.
~~~
// MAC_RANDOM_NUM - random salt for MAC. It's extremely useful, when you creating modules of
// different types, for example: MODULE_LINEAR
// (It's need to be different, even if it's not in the same physical module).
// Optional parameters for axis module: indexPin, axisLen, axisLimitSite
SM2Module vmodule(MODULE_TYPE, MAC_RANDOM_NUM);
~~~

<details>
    <summary>Available module types</summary>
    - MODULE_PRINT
    - MODULE_CNC
    - MODULE_LASER
    - MODULE_LINEAR
    - MODULE_ENCLOSURE
    - MODULE_ROTATE
    - MODULE_PURIFIER
    - MODULE_EMERGENCY_STOP
    - MODULE_PRINT_V_SM1
    - MODULE_LINEAR_TMC
    - MODULE_LASER_10W
</details>


Next step - add module to registry.
Registry can hande up to 4 virtual modules.
You can change MODULE_MAX_COUNT in ./config/main.h
~~~
registry.RegisterModule(&vmodule);
~~~


Then call Init function. It's need to be started when powering Arduino, in Init() section.
If Emergency Stop is activated, module will freeze at this function for safety purposes.
Do not try to enable any dangerous devices before Init!
~~~
registry.Init();
~~~


We are ready now! If you seeing your module at SM2 Touchscreen (or Luban Software),
then you done it's right!

You now can read/write messages from/to SM2 Controller:
~~~
registry.ReadMessage(&funcId, data, &dataLen);
registry.SendMessage(funcId, data, dataLen);
~~~

<details>
    <summary>Available function IDs</summary>
    - FUNC_REPORT_LIMIT
    - FUNC_REPORT_PROBE
    - FUNC_REPORT_CUT
    - FUNC_SET_STEP_CTRL
    - FUNC_SET_MOTOR_SPEED
    - FUNC_REPORT_MOTOR_SPEED
    - FUNC_REPORT_TEMPEARTURE
    - FUNC_SET_TEMPEARTURE
    - FUNC_SET_FAN
    - FUNC_SET_FAN2
    - FUNC_SET_PID
    - FUNC_SET_CAMERA_POWER
    - FUNC_SET_LASER_FOCUS
    - FUNC_REPORT_LASER_FOCUS
    - FUNC_SET_LIGHT_COLOR
    - FUNC_REPORT_ENCLOSURE
    - FUNC_REPORT_TEMP_PID
    - FUNC_REPORT_TOOL_SETTING
    - FUNC_SET_ENCLOSURE_LIGHT
    - FUNC_SET_FAN_MODULE
    - FUNC_REPORT_STOP_SWITCH
    - FUNC_TMC_IOCTRL
    - FUNC_TMC_PUBLISH
    - FUNC_SET_PURIFIER
    - FUNC_REPORT_PURIFIER
    - FUNC_SET_AUTOFOCUS_LIGHT
    - FUNC_REPORT_SECURITY_STATUS
    - FUNC_MODULE_ONLINE_SYNC
    - FUNC_MODULE_SET_TEMP
    - FUNC_MODULE_LASER_CTRL
    - FUNC_MODULE_GET_HW_VERSION
    - FUNC_REPORT_PIN_STATUS
    - FUNC_CONFIRM_PIN_STATUS
    - FUNC_EMERGENCY_STOP
    - FUNC_HEARTBEAT
</details>


### Examples
 - [Raw module (without registry)](https://github.com/morgan55555/sm2_module/examples/RawModuleExample/RawModuleExample.ino)
 - [Enclosure Light single module](https://github.com/morgan55555/sm2_module/examples/SingleModuleExample/SingleModuleExample.ino)
 - [Enclosure+EmergencyStop multiple module](https://github.com/morgan55555/sm2_module/examples/MultipleModulesExample/MultipleModulesExample.ino)