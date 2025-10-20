
/***************************** Include Files *********************************/
#include <openamp/version.h>
#include <metal/version.h>
#include <stdio.h>
#include <string.h>
#include "fio_mux.h"
#include "ftypes.h"
#include "fprintk.h"
#include "stdio.h"
// #include "finterrupt.h"
#include "platform_info.h"
#include "init_state.h"
#include "rpmsg_state.h"
//For Debug
#include "fsleep.h"

/************************** Constant Definitions *****************************/
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
//all state unit instance

void *platform;

static void init_run_task(void);

static const FStateUnit init_state = {
    .id       = ST_INIT_STATE,
    .priority = 1,
    .pre_task = NULL,
    .run_task = init_run_task,
    .exit_task = NULL,
};


/************************** Function Prototypes ******************************/

static void init_run_task(void){
    OPENAMP_MAIN_DEBUG_I("%s :start\r\n", __FUNCTION__);
    int ret = 0;

    /* Initialize platform */
    ret = platform_init(0, NULL, &platform);
    if (ret)
    {
        OPENAMP_MAIN_DEBUG_E("Failed to initialize platform.\r\n");
        platform_cleanup(platform);
        goto end_flow;
    }
    //init rpmsg vdev
    ret = createRpmsgVdev();
    if (ret)
    {
        OPENAMP_MAIN_DEBUG_E("Failed to create rpmsg virtio device.\r\n");
        ret = platform_cleanup(platform);
        goto end_flow;
    }

    ret = createRpmsgEndPoints();
    if (ret)
    {
        OPENAMP_MAIN_DEBUG_E("Failed to createRpmsgEndPoints.\r\n");
        ret = platform_cleanup(platform);
        goto end_flow;
    }

end_flow:
    setFsmState(ST_PLC_MAIN_STATE);
}


const FStateUnit* getInitStateUnit(void){
    return &init_state;
}