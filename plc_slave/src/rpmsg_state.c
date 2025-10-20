
/***************************** Include Files *********************************/
#include <string.h>
#include "rpmsg_state.h"
#include "platform_info.h"
#include "rpmsg_service.h"
#include "rsc_table.h"
//For Debug
#include "fsleep.h"


/************************** Constant Definitions *****************************/
/***************** Macros (Inline Functions) Definitions *********************/

#define     SHUTDOWN_MSG                    0xEF56A55A
#define     PLC_MSG_HEAD                    0xFE
#define     RPMSG_GATE_LENGTH               16
#define     MSG_HEAD_MAX_LENGTH             10
#define     MSG_DATA_MAX_LENGTH             RPMSG_BUFFER_SIZE - RPMSG_GATE_LENGTH - MSG_HEAD_MAX_LENGTH

// [[command]:[data]] cmd:unit_start
// [[command]:[data]] unit:indix_unitName_unitType_connectIO
// [[command]:[data]] inPort:unitName1_unitName2_...
// [[command]:[data]] outPort:unitName1_unitName2_...
// [[command]:[data]] cmd:unit_end
#define     SUB_MSG_TYPE_CMD        "cmd"
#define     SUB_MSG_TYPE_UNIT       "unit"
#define     SUB_MSG_TYPE_INPORT     "inPort"
#define     SUB_MSG_TYPE_OUTPORT    "outPort"

#define     SUB_DATA_UNIT_START       "unit_start"
#define     SUB_DATA_UNIT_END         "unit_end"

/***************** RPMSG Definitions *********************/
//haeder:10 byte; data:(0-486 char)
//head format [HEAD(0xFE)][TYPE(1byte)][data length(2byte)][channel(1byte)][msg id(1byte)][total id(1byte)]
//data format [DATA(0-486 char)] 
typedef struct
{
    u8     head; 
    u8     type;
    u16    data_len;
    u8     channel;
    u8     msg_id;
    u8     total_id;
} MsgHeader;   

typedef enum {
    TYPE_CMD = 0,
    TYPE_MSG,
}plc_msg_type;

typedef enum {
    CLIENT_TRY_CONNECT = 0,
    SERVER_BUILD_CONNECT = 1,
    CLIENT_GET_CONNECT = 2,
}plc_cmd;

/************************** Variable Definitions *****************************/
//all state unit instance

static void rpmsg_run_task(void);
static MsgHeader decodeMsgHeader(char* pHeaderData);
static void plcMsgHandler(char* pHead, char* pData);
static void execTypeMsg(char* pHead, char* pData);
static void encodeHeaderToMsg(MsgHeader* phead, char* pData, char* ret);


static struct rpmsg_device *rpdev;
static struct rpmsg_endpoint lept;
static int shutdown_req = 0;
static bool isCommunicationMode = false;
static bool isReceivingUnit = false;
//message array need change to malloc
static char test_data[RPMSG_BUFFER_SIZE] = {PLC_MSG_HEAD, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01};

extern void *platform;

static const FStateUnit rpmsg_state = {
    .id       = ST_RPMSG_STATE,
    .priority = 0,
    .pre_task = NULL,
    .run_task = rpmsg_run_task,
    .exit_task = NULL,
};

/************************** Function Prototypes ******************************/

/*-----------------------------------------------------------------------------*
 *  RPMSG endpoint callbacks
 *-----------------------------------------------------------------------------*/

static void execMsgType(char* pData, u16 len){

    char* subdata = NULL;

    if (isReceivingUnit == true){
        


    }

    subdata = strstr(pData, SUB_MSG_TYPE_CMD);
    if (NULL != subdata){
        if(strcmp(subdata + sizeof(SUB_MSG_TYPE_CMD) + 1, SUB_DATA_UNIT_START)){
            isReceivingUnit = true;
            return;
        } 

        if(strcmp(subdata + sizeof(SUB_MSG_TYPE_CMD) + 1, SUB_DATA_UNIT_END)){
            isReceivingUnit = false;
            return;
        } 
    }

}

static void execCmdType(char* pData, u16 len){
    plc_cmd cmd = (plc_cmd)*pData;

    switch(cmd){
    case CLIENT_TRY_CONNECT:{
        char back_data[RPMSG_BUFFER_SIZE];
        char back_msg = (char)SERVER_BUILD_CONNECT;
        MsgHeader back_head = {
            .head = ST_RPMSG_STATE,
            .type = TYPE_CMD,
            .data_len = sizeof(back_msg),
        };
        encodeHeaderToMsg(&back_head, &back_msg, back_data);
        int len = MSG_HEAD_MAX_LENGTH + back_head.data_len;
        if (rpmsg_send(&lept, back_data, len) < 0){
            OPENAMP_MAIN_DEBUG_E("rpmsg_send failed.\r\n");
        }
        isCommunicationMode = true;
    }
        break;
    case CLIENT_GET_CONNECT:{
        OPENAMP_MAIN_DEBUG_W("Communication Mode start: \r\n");
    }
        break;
    default:
        break;
    }
}

static void plcMsgHandler(char* pHead, char* pData){

    MsgHeader header = decodeMsgHeader(pHead);

    OPENAMP_MAIN_DEBUG_I("get raw head is : \r\n");
    OPENAMP_MAIN_DEBUG_I("head %02X: \r\n", header.head);
    OPENAMP_MAIN_DEBUG_I("data_len %04X: \r\n", header.data_len);
    OPENAMP_MAIN_DEBUG_I("channel %02X: \r\n", header.channel);
    OPENAMP_MAIN_DEBUG_I("msg_id %02X: \r\n", header.msg_id);
    OPENAMP_MAIN_DEBUG_I("total_id %02X: \r\n", header.total_id);

    plc_msg_type type = (plc_msg_type)header.type;

    switch(type){
    case TYPE_CMD:
        execCmdType(pData, header.data_len);
        break;
    case TYPE_MSG:
        execMsgType(pData, header.data_len);
        break;
    default:
        break;

    }

}


static MsgHeader decodeMsgHeader(char* pHeaderData){
    MsgHeader ret = {};
    if (*pHeaderData != PLC_MSG_HEAD){
        return ret;
    }
    ret.head = (u8)*pHeaderData;
    ret.type = (u8)*(pHeaderData + 1);
    ret.data_len = *(u16*)(pHeaderData + 1);
    ret.channel = (u8)*(pHeaderData + 4);
    ret.msg_id = (u8)*(pHeaderData + 5);
    ret.total_id = (u8)*(pHeaderData + 5);
    
    return ret;
}

static void encodeHeaderToMsg(MsgHeader* phead, char* pData, char* ret){

    memcpy(ret, phead, sizeof(MsgHeader));

    int dataLen = (int)phead->data_len;
    if (dataLen < MSG_DATA_MAX_LENGTH){
        memcpy(ret + MSG_HEAD_MAX_LENGTH, pData, dataLen);
    }
}

static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv)
{
    (void)priv;
    (void)src;
    char msg_header[MSG_HEAD_MAX_LENGTH];
    char msg_data[MSG_DATA_MAX_LENGTH];

    // /* 请勿直接对data指针对应的内存进行写操作，操作vring中remoteproc发送通道分配的内存，引发错误的问题*/
    // memset(temp_data,0,len) ;
    memcpy(msg_header, data, MSG_HEAD_MAX_LENGTH);
    memcpy(msg_data, data + MSG_HEAD_MAX_LENGTH, MSG_DATA_MAX_LENGTH);

    plcMsgHandler(msg_header, msg_data);
    


    return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
    (void)ept;
    OPENAMP_MAIN_DEBUG_I("Unexpected remote endpoint destroy.\r\n");
    shutdown_req = 1;
}

const FStateUnit* getRpmsgStateUnit(void){
    return &rpmsg_state;
}


int createRpmsgVdev(void){
    rpdev = platform_create_rpmsg_vdev(platform, 0, VIRTIO_DEV_SLAVE, NULL, NULL);
    if (!rpdev){
        return -1;
    }
    return 0;
}


int createRpmsgEndPoints(void){
    int ret = 0;
    shutdown_req = 0;
    /* Initialize RPMSG framework */
    OPENAMP_MAIN_DEBUG_I("Try to create rpmsg endpoint.\r\n");

    ret = rpmsg_create_ept(&lept, rpdev, RPMSG_SERVICE_NAME, 0, RPMSG_ADDR_ANY, rpmsg_endpoint_cb, rpmsg_service_unbind);
    if (ret)
    {
        OPENAMP_MAIN_DEBUG_E("Failed to create endpoint. %d \r\n", ret);
        return -1;
    }

    OPENAMP_MAIN_DEBUG_I("Successfully created rpmsg endpoint.\r\n");

    return ret;
}

static void rpmsg_run_task(void){
    OPENAMP_MAIN_DEBUG_I("%s :start\r\n", __FUNCTION__);
    do{
        platform_poll(platform);
    }while(isCommunicationMode);
    
end_flow:
    setFsmState(ST_PLC_MAIN_STATE);
}
