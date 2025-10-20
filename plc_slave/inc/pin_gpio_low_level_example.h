/*
 * Copyright : (C) 2023 Phytium Information Technology, Inc.
 * All Rights Reserved.
 *
 * This program is OPEN SOURCE software: you can redistribute it and/or modify it
 * under the terms of the Phytium Public License as published by the Phytium Technology Co.,Ltd,
 * either version 1.0 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the Phytium Public License for more details.
 *
 *
 * FilePath: pin_gpio_low_level_example.h
 * Date: 2022-03-01 17:55:49
 * LastEditTime: 2022-03-05 13:36:11
 * Description:  This file is for gpio register operation function definition.
 * Modify History:
 *  Ver      Who        Date         Changes
 * -----   -------    --------     --------------------------------------
 *  1.0  liqiaozhong  2023/03/05   first commit
 */

#ifndef  PIN_GPIO_LOW_LEVEL_EXAMPLE_H
#define  PIN_GPIO_LOW_LEVEL_EXAMPLE_H

#ifdef __cplusplus
extern "C"
{
#endif
/***************************** Include Files *********************************/
#include "ftypes.h"
#include "fkernel.h"

#include "pwm_dead_band_example.h"
/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/
int FPinGpioLowLevelExample(void);
int RedInit(void);
int RedDetect(void);
int PullPashInit(void);
int Push_1(void);
int Pull_1(void);
int Push_2(void);
int Pull_2(void);
int Push_3(void);
int Pull_3(void);

#ifdef __cplusplus
}
#endif

#endif