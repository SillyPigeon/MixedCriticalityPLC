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
 * FilePath: pin_gpio_low_level_example.c
 * Date: 2022-03-01 14:56:42
 * LastEditTime: 2022-03-05 18:36:06
 * Description:  This file is for pin gpio register operation example function implmentation.
 *
 * Modify History:
 *  Ver      Who        Date         Changes
 * -----   -------    --------     --------------------------------------
 *  1.0  liqiaozhong  2023/03/05   first commit
 *  1.1  liqiaozhong  2023/8/11    adapt to new iomux
 */

/***************************** Include Files *********************************/
#include "sdkconfig.h"
#ifndef SDK_CONFIG_H__
    #warning "Please include sdkconfig.h"
#endif

#include <string.h>
#include <stdio.h>
#include "strto.h"

#include "ftypes.h"
#include "fdebug.h"
#include "fassert.h"
#include "fsleep.h"
#include "fio_mux.h"

#include "fgpio_hw.h"

#include "pin_common.h"
#include "pin_gpio_low_level_example.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions ****************************/
int red_flag_1 = 0;  //两个红外定义两个全局变量，当检测到变化时改变标志位
int red_flag_2 = 0;


/***************** Macros (Inline Functions) Definitions *********************/
#if defined(CONFIG_FIREFLY_DEMO_BOARD)
static uintptr gpio_base_0 = FGPIO0_BASE_ADDR;
static uintptr gpio_base_1 = FGPIO1_BASE_ADDR;
static uintptr gpio_base_2 = FGPIO2_BASE_ADDR;
static uintptr gpio_base_3 = FGPIO3_BASE_ADDR;
static uintptr gpio_base_4 = FGPIO4_BASE_ADDR;

static const u32 ctrl_id_0 = FGPIO0_ID;
static const u32 ctrl_id_1 = FGPIO1_ID;
static const u32 ctrl_id_2 = FGPIO2_ID;
static const u32 ctrl_id_3 = FGPIO3_ID;
static const u32 ctrl_id_4 = FGPIO4_ID;

static u32 input_pin_1 = (u32)FGPIO_PIN_1;  //3_1
static u32 input_pin_10 = (u32)FGPIO_PIN_10; //2_10

static u32 output_pin_2 = (u32)FGPIO_PIN_2; //3_2
static u32 output_pin_5 = (u32)FGPIO_PIN_5; //1_5
static u32 output_pin_11 = (u32)FGPIO_PIN_11; //1_11,4_11
static u32 output_pin_12 = (u32)FGPIO_PIN_12; //1_12
static u32 output_pin_13 = (u32)FGPIO_PIN_13; //4_14

#elif defined(CONFIG_E2000Q_DEMO_BOARD) || defined(CONFIG_E2000D_DEMO_BOARD)
static uintptr gpio_base = FGPIO4_BASE_ADDR;
static const u32 ctrl_id = FGPIO4_ID;
static u32 input_pin = (u32)FGPIO_PIN_11;
static u32 output_pin = (u32)FGPIO_PIN_12;
#elif defined(CONFIG_PD2308_DEMO_BOARD)
static uintptr gpio_base = FGPIO0_BASE_ADDR;
static const u32 ctrl_id = FGPIO0_ID;
static u32 input_pin = (u32)FGPIO_PIN_8;
static u32 output_pin = (u32)FGPIO_PIN_10;
#endif

/************************** Function Prototypes ******************************/
int RedInit(void)   //初始化两个红外传感器，设置引脚为输入（2_10,3_1）
{
    u32 reg_val;
    /* init pin */
    
    FIOPadSetGpioMux(ctrl_id_2, input_pin_10);
    reg_val = FGpioReadReg32(gpio_base_2, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val &= ~BIT(input_pin_10); /*0-input ，设置引脚方向为输入*/
    FGpioWriteReg32(gpio_base_2, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    

    FIOPadSetGpioMux(ctrl_id_3, input_pin_1);
    reg_val = FGpioReadReg32(gpio_base_3, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val &= ~BIT(input_pin_1); /* 0-input, */
    FGpioWriteReg32(gpio_base_3, FGPIO_SWPORTA_DDR_OFFSET, reg_val);

    printf("red init is succeed \n");

    return 0;

}

int RedDetect(void)  //红外引脚电平获取，当高电平时说明检测到物体
{
    u32 reg_val;
    reg_val = FGpioReadReg32(gpio_base_2, FGPIO_EXT_PORTA_OFFSET); /* get input pin level */
    if (((BIT(input_pin_10) & reg_val) ? FGPIO_PIN_HIGH : FGPIO_PIN_LOW) == FGPIO_PIN_HIGH) //检测为高电平
    {
      Push_1();
      Push_3();
      stoppwm();
      fsleep_seconds(3);
      Pull_1();
      Push_3();
      fsleep_seconds(3);
      startpwm();
      red_flag_1 = 1; //标志位置一
    //   printf("red_1 detect");
    }

    else
    {
        red_flag_1 = 0; //标志位复位
        // printf("red_1 no detect");
    }

    reg_val = FGpioReadReg32(gpio_base_3, FGPIO_EXT_PORTA_OFFSET); /* get input pin level */
    if (((BIT(input_pin_1) & reg_val) ? FGPIO_PIN_HIGH : FGPIO_PIN_LOW) == FGPIO_PIN_HIGH) //检测为高电平
    {
      Push_2();
      fsleep_seconds(3);
      Pull_2();
      red_flag_2 = 1; //标志位置一
    //   printf("red_2 detect");
    }

    else
    {
        red_flag_2 = 0; //标志位复位
        // printf("no detect");
    }

    return 0;
    
}

/************************** pull and push *****************************************/
int PullPashInit(void)
{  
    u32 reg_val;
    /* init pin */
    FIOPadSetGpioMux(ctrl_id_1, output_pin_12);
    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_12); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    /* operations */
    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
    reg_val |= BIT(output_pin_12);
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    
    FIOPadSetGpioMux(ctrl_id_1, output_pin_11);
    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_11); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
    reg_val |= BIT(output_pin_11); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    FIOPadSetGpioMux(ctrl_id_3, output_pin_2);
    reg_val = FGpioReadReg32(gpio_base_3, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_2); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_3, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    reg_val = FGpioReadReg32(gpio_base_3, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
    reg_val |= BIT(output_pin_2); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_3, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    FIOPadSetGpioMux(ctrl_id_4, output_pin_12);
    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_12); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
    reg_val |= BIT(output_pin_12); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    FIOPadSetGpioMux(ctrl_id_4, output_pin_13);
    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_13); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /*set output pin to low-level */
    reg_val |= BIT(output_pin_13); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    FIOPadSetGpioMux(ctrl_id_4, output_pin_11);
    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_11); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DDR_OFFSET, reg_val);
    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
    reg_val |= BIT(output_pin_11); /* 1-output,引脚初始化为低电平 */
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    printf("pull and push Init is succeed \n");

    return 0;

}


int Push_3(void)//第一个推杆推
{
    u32 reg_val;

    printf("debug2_1");
    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val &= ~BIT(output_pin_12); /* 引脚电平拉低*/
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val &= ~BIT(output_pin_11); 
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    return 0;
}

int Pull_3(void)
{
    u32 reg_val;

    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_12); /* 引脚电平拉高*/
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    reg_val = FGpioReadReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_11); 
    FGpioWriteReg32(gpio_base_1, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    return 0;
}

int Push_2(void)//第二个推杆推
{
    u32 reg_val;

    reg_val = FGpioReadReg32(gpio_base_3, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val &= ~BIT(output_pin_2); /* 引脚电平拉高*/
    FGpioWriteReg32(gpio_base_3, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val &= ~BIT(output_pin_12); 
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    return 0;
}

int Pull_2(void)
{
    u32 reg_val;

    reg_val = FGpioReadReg32(gpio_base_3, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_2); /* 引脚电平拉高*/
    FGpioWriteReg32(gpio_base_3, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_12); 
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    return 0;
}

int Push_1(void)  //第三个推杆推
{
    u32 reg_val;

    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val &= ~BIT(output_pin_13); /* 引脚电平拉高*/
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val &= ~BIT(output_pin_11); 
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    return 0;
}

int Pull_1(void)
{
    u32 reg_val;
    

    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_13); /* 引脚电平拉高*/
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    reg_val = FGpioReadReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET); /* set direction */
    reg_val |= BIT(output_pin_11); 
    FGpioWriteReg32(gpio_base_4, FGPIO_SWPORTA_DR_OFFSET, reg_val);

    return 0;
}



/************************** Function *****************************************/
// /* function of gpio low level example */
// int FPinGpioLowLevelExample(void)
// {
//     int ret = 0;
//     int flag = 1;
//     u32 reg_val;
//     u32 set_level = FGPIO_PIN_HIGH;

//     /* init pin */
//     FIOMuxInit();
//     FIOPadSetGpioMux(ctrl_id, input_pin);
//     FIOPadSetGpioMux(ctrl_id, output_pin);

//     reg_val = FGpioReadReg32(gpio_base, FGPIO_SWPORTA_DDR_OFFSET); /* set direction */
//     reg_val &= ~BIT(input_pin); /* 0-input */
//     reg_val |= BIT(output_pin); /* 1-output */
//     FGpioWriteReg32(gpio_base, FGPIO_SWPORTA_DDR_OFFSET, reg_val);

//     /* operations */
//     reg_val = FGpioReadReg32(gpio_base, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
//     reg_val &= ~BIT(output_pin);
//     FGpioWriteReg32(gpio_base, FGPIO_SWPORTA_DR_OFFSET, reg_val);

//     reg_val = FGpioReadReg32(gpio_base, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to high-level */
//     reg_val |= BIT(output_pin); 
//     FGpioWriteReg32(gpio_base, FGPIO_SWPORTA_DR_OFFSET, reg_val);
//     reg_val = FGpioReadReg32(gpio_base, FGPIO_EXT_PORTA_OFFSET); /* get input pin level */
//     if (((BIT(input_pin) & reg_val) ? FGPIO_PIN_HIGH : FGPIO_PIN_LOW) == FGPIO_PIN_HIGH)
//     {
//         printf("Low level operation works for the first time.\n");
//     }
//     else
//     {
//         printf("Low level operation does not work for the first time.\n");
//         flag = 0;
//     }

//     fsleep_millisec(10); /* delay 10ms */

//     reg_val = FGpioReadReg32(gpio_base, FGPIO_SWPORTA_DR_OFFSET); /* set output pin to low-level */
//     reg_val &= ~BIT(output_pin);
//     FGpioWriteReg32(gpio_base, FGPIO_SWPORTA_DR_OFFSET, reg_val);
//     reg_val = FGpioReadReg32(gpio_base, FGPIO_EXT_PORTA_OFFSET); /* get input pin level */
//     if (((BIT(input_pin) & reg_val) ? FGPIO_PIN_HIGH : FGPIO_PIN_LOW) == FGPIO_PIN_LOW)
//     {
//         printf("Low level operation works for the second time.\n");
//     }
//     else
//     {
//         printf("Low level operation does not work for the second time.\n");
//         flag = 0;
//     }

//     /* print message on example run result */
//     if (1 == flag)
//     {
//         printf("%s@%d: pin GPIO low level example [success].\r\n", __func__, __LINE__);
//         return 0;
//     }
//     else
//     {
//         printf("%s@%d: pin GPIO low level example [failure].\r\n", __func__, __LINE__);
//         return 1;
//     }
// }
