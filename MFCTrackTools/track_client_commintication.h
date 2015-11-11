#pragma onece
#include "tch_params.h"
#include "communtication.h"
#include "share_stream.h"
#include"decode.h"
#include <list>

using namespace std;
#define STU_SETTRACK_CMD  0x100
#define TEA_SETTRACK_CMD  0x101
#define STU_GETTRACK_CMD  0x102
#define TEA_GETTRACK_CMD  0x103
#define TIMEOUT 500

#define TEACH_STREAM_PORT 21303
#define STUDENT_STREAM_PORT  21302
#define STU_CHANNL 0
#define TEACH_CHANNL 1

#define MAX_STEAM_WIDTH 1280
#define MAX_STEAM_HEIGHT 720
typedef enum
{
	STREAMCLINT_START = 0,
	STREAMCLINT_STOP,
	STREAM_STATUS_ERR,

}StreamClient_Status_t;

typedef struct _DecodeInfo
{
	unsigned char * data;
	int height;
	int width;

}Decode_Info_t;

typedef int(*Stream_Call_Back)(Decode_Info_t *pInfo, void*param);
typedef struct Stream_Message
{
	RH_FRAMEHEAD_t fh;
	void *stream_data;//数据
}Stream_Message_t;
typedef list<Stream_Message_t *> LISTSTREAM;

typedef struct _TRACK_CMD_INFO_T{
	int cmd;
	char cmd_name[64];
}Track_cmd_info_t;
typedef struct _Client_Handle
{
	LISTSTREAM list_Handle;
	Decode_Info_t  streamDecode;
	StreamClient_Status_t nRun_Status;
	Tools_decoder_t decoder;
	int first_frame;
	int nChannel;
	pthread_mutex_t lock;

}Client_Handle_t;
typedef struct Stream_Handle
{
	int channel;
	int port;
	char iP[16];
	Stream_Call_Back call_back_fun;//将数据回调由video处理
	void *outParm;//扩展(可由内部自己填充)
	void* param1;//扩展参数,由使用者带进去，通过回调函数call_back带出来
}RecvStream_Handle_t;
int init_stream_recv(RecvStream_Handle_t* pRecv_stream_handle);
int ctrlClient_init_trackCommuntication();
int ctrlClient_set_teach_params(TeaITRACK_Params * tec_param);
