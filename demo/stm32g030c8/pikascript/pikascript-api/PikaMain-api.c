/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaMain.h"
#include "PikaStdLib_SysObj.h"
#include "STM32_PWM.h"
#include "STM32_Time.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

PikaObj *New_PikaMain(Args *args){
    PikaObj *self = New_PikaStdLib_SysObj(args);
    obj_import(self, "STM32_PWM", New_STM32_PWM);
    obj_newObj(self, "pwm", "STM32_PWM");
    obj_import(self, "STM32_Time", New_STM32_Time);
    obj_newObj(self, "time", "STM32_Time");
    return self;
}
