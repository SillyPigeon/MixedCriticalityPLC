
/***************************** Include Files *********************************/
#include "plc_main_state.h"
#include "platform_info.h"
#include "plc_drivers.h"
//For Debug
#include "fsleep.h"

/************************** Constant Definitions *****************************/
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
//all state unit instance

static void plc_main_run_task(void);

static bool plc_main_schedule(void);

extern void *platform;

static const FStateUnit plc_main_state = {
    .id       = ST_PLC_MAIN_STATE,
    .priority = 0,
    .pre_task = NULL,
    .run_task = plc_main_run_task,
    .exit_task = NULL,
};


/************************** Function Prototypes ******************************/

static void plc_main_run_task(void){
    OPENAMP_MAIN_DEBUG_I("%s :start\r\n", __FUNCTION__);
    //fsleep_millisec(500);
    FIOMuxInit();
    OPENAMP_MAIN_DEBUG_I("debug_0");
    RedInit();
    PullPashInit();
    OPENAMP_MAIN_DEBUG_I("debug_1");
    FPwmDeadBandExample();

    // fsleep_seconds(20);
    RedDetect();

    OPENAMP_MAIN_DEBUG_I("debug_2");
    FSerialMioExample();

    while(1){
        RedDetect();
        fsleep_millisec(200);
    }
    
    if(FALSE == plc_main_schedule())
        return;
    
end_flow:
    setFsmState(ST_PLC_MAIN_STATE);
}

const FStateUnit* getPlcMainStateUnit(void){
    return &plc_main_state;
}

static bool plc_main_schedule(void){
    return ST_PLC_MAIN_STATE == fsmSchedule();
}
