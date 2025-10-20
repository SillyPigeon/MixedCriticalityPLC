#ifndef PLC_DRIVERS_H
#define PLC_DRIVERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************** Include Files *********************************/

#include "fio_mux.h"
#include "fgpio_hw.h"
#include "pin_common.h"
#include "pin_gpio_low_level_example.h"
#include "pwm_dead_band_example.h"
#include "serial_mio_example.h"


/************************** Constant Definitions *****************************/

//PWM_1 [电机1]

//IO_IN_1 IO_IN_2 [关电开关1 关电开关2]

//OUT_1 OUT_2 OUT_3 [推杆1 推杆2]

//TRIG_1 [触发器1-用于称重]


#ifdef __cplusplus
}
#endif

#endif