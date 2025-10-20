//SMART PLC
#include "plc_fsm.h"
#include "init_state.h"
#include "rpmsg_state.h"
#include "plc_main_state.h"



/***************************** Include Files *********************************/


/************************** Constant Definitions *****************************/
/***************** Macros (Inline Functions) Definitions *********************/
#define MAX_FSM_STATE_NUM    10

/************************** Variable Definitions *****************************/
//all state unit instance
static const FStateUnit* fsmUnits[MAX_FSM_STATE_NUM] = {};
static u32 validUnitNum = 0;
static state_id currentStateId;


/************************** Function Prototypes ******************************/

void registerFsmState(const FStateUnit* unit){
	if(unit->id > MAX_FSM_STATE_NUM){
		return;
	}
	fsmUnits[unit->id] = unit;
	validUnitNum++;
}

state_id fsmSchedule(void){
	return currentStateId;
}

void setFsmState(state_id id){
	currentStateId = id;
}

void runFsmState(state_id id){
	const FStateUnit* curUnit = fsmUnits[id];
	if (NULL == curUnit){
		return;
	}

	if (NULL != curUnit->pre_task){
		curUnit->pre_task();
	}

	if (NULL != curUnit->run_task){
		curUnit->run_task();
	}
}

int plcFsmInitialize(void){
	//init Variable
	int ret = 0;
	const FStateUnit* tmpUnit;
	currentStateId = 0;


	//Init state
	tmpUnit = getInitStateUnit();
	if (tmpUnit != NULL){
		registerFsmState(tmpUnit);
	}

	//rpmsg state
	tmpUnit = getRpmsgStateUnit();
	if (tmpUnit != NULL){
		registerFsmState(tmpUnit);
	}

	//plc main state
	tmpUnit = getPlcMainStateUnit();
	if (tmpUnit != NULL){
		registerFsmState(tmpUnit);
	}

	return 0;
}

