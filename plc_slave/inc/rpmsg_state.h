#ifndef PLC_RPMSG_STATE_H
#define PLC_RPMSG_STATE_H

#ifdef __cplusplus
extern "C"
{
#endif
/***************************** Include Files *********************************/

#include "plc_fsm.h"

/**************************** Type Definitions *******************************/


/************************** Function Prototypes ******************************/

const FStateUnit* getRpmsgStateUnit(void);

int createRpmsgVdev(void);

int createRpmsgEndPoints(void);

#ifdef __cplusplus
}
#endif

#endif