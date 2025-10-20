/*
 * @ : Copyright (c) 2021 Phytium Information Technology, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0.
 *
 * @Date: 2021-12-09 17:12:52
 * LastEditTime: 2023-01-11 15:36:09
 * @Description:  This file is for openamp test
 *
 * @Modify History:
 *  Ver   Who           Date         Changes
 * ----- ------         --------    --------------------------------------
 * 1.0   huanghe        2022/06/20      first release
 * 1.1  liushengming    2023/05/16      for openamp test
 */
/***************************** Include Files *********************************/
#include <openamp/version.h>
#include <metal/version.h>
// #include "pwm_dead_band_example.h"
// #include "pin_gpio_low_level_example.h"
#include "plc_fsm.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/
// extern int FOpenampExample(void);

static int metal_os_int(void)
{
    int  ret = 0;
    /* Initialize state machine */
    ret = plcFsmInitialize();
    if (ret < 0){
        return ret;
    }

    return ret;
}



static void metal_os(void){
    while(1){
        //check state
        state_id id = fsmSchedule();

        //run the state
        runFsmState(id);
    }
}


int main(void)
{   
    int ret = 0;
    OPENAMP_MAIN_DEBUG_I("complier %s ,%s \r\n", __DATE__, __TIME__);
    OPENAMP_MAIN_DEBUG_I("openamp lib version: %s (", openamp_version());
    OPENAMP_MAIN_DEBUG_I("Major: %d, ", openamp_version_major());
    OPENAMP_MAIN_DEBUG_I("Minor: %d, ", openamp_version_minor());
    OPENAMP_MAIN_DEBUG_I("Patch: %d)\r\n", openamp_version_patch());

    OPENAMP_MAIN_DEBUG_I("libmetal lib version: %s (", metal_ver());
    OPENAMP_MAIN_DEBUG_I("Major: %d, ", metal_ver_major());
    OPENAMP_MAIN_DEBUG_I("Minor: %d, ", metal_ver_minor());
    OPENAMP_MAIN_DEBUG_I("Patch: %d)\r\n", metal_ver_patch());

    //pre metal os init
    ret = metal_os_int();
    if(ret < 0){
        return ret;
    }

    //run the metal state machine os
    metal_os();

    return ret;
    /*run the atomic example*/
    //return FOpenampExample();
}



//************************** Driver Demo ************************************/
//demo
    // int t = 400;
    // FIOMuxInit();
    // printf("debug_0");
    // RedInit();
    // PullPashInit();
    // printf("debug_1");
    // FPwmDeadBandExample();
    // while (t)
    // {
    //     RedDetect();
    //     fsleep_millisec(200);
    //     t--;
    // }
    // fsleep_millisec(200);
    // printf("debug_2");
    // Pull_1();
    // printf("debug_3");
    // Pull_2();
    // printf("debug_4");
    // Pull_2();
    // printf("debug_5");
