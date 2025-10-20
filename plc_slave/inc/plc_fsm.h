#ifndef MYFSM_H
#define MYFSM_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************** Include Files *********************************/

#include "ftypes.h"
#include "ferror_code.h"
#include "fassert.h"
#include "fdebug.h"


/************************** Constant Definitions *****************************/
//for DEBUG
#define PLC_DEBUG

#define OPENAMP_MAIN_DEBUG_TAG "OPENAMP_SLAVE_MAIN"
#ifdef PLC_DEBUG
#define OPENAMP_MAIN_DEBUG_I(format, ...) FT_DEBUG_PRINT_I(OPENAMP_MAIN_DEBUG_TAG, format, ##__VA_ARGS__)
#define OPENAMP_MAIN_DEBUG_W(format, ...) FT_DEBUG_PRINT_W(OPENAMP_MAIN_DEBUG_TAG, format, ##__VA_ARGS__)
#define OPENAMP_MAIN_DEBUG_E(format, ...) FT_DEBUG_PRINT_E(OPENAMP_MAIN_DEBUG_TAG, format, ##__VA_ARGS__)
#else
#define OPENAMP_MAIN_DEBUG_I(format, ...) 
#define OPENAMP_MAIN_DEBUG_W(format, ...) 
#define OPENAMP_MAIN_DEBUG_E(format, ...) 
#endif

/**************************** Type Definitions *******************************/


typedef enum {
    ST_INIT_STATE = 0,
    ST_RPMSG_STATE,
    ST_PLC_MAIN_STATE,
}state_id;


typedef void (*fsm_task)(void);


//priority: 0~3
typedef struct
{
    state_id     id;    	/*the index of each state Unit*/
    u32     	 priority;   /* Device interrupt id */
    fsm_task     pre_task;
    fsm_task     run_task;
    fsm_task     exit_task;
} FStateUnit;               /*mio configs*/


/************************** Function Prototypes ******************************/
/*init fsm*/
int plcFsmInitialize(void);

void registerFsmState(const FStateUnit* unit);

state_id fsmSchedule(void);

void setFsmState(state_id id);

void runFsmState(state_id id);

#ifdef __cplusplus
}
#endif

#endif