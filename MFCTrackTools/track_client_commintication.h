#pragma onece
#include "tch_params.h"
#include "communtication.h"
#include "share_stream.h"
#include <list>
using namespace std;
#define STU_SETTRACK_CMD  0x100
#define TEA_SETTRACK_CMD  0x101
#define STU_GETTRACK_CMD  0x102
#define TEA_GETTRACK_CMD  0x103
#define TIMEOUT 500

#define TEACH_STREAM_PORT 10086
#define STUDENT_STREAM_PORT  10087
typedef enum
{
	STREAMCLINT_START = 0,
	STREAMCLINT_STOP,
	STREAM_STATUS_ERR,

}StreamClient_Status_t;
typedef struct Stream_Message
{
	RH_FRAMEHEAD_t fh;
	void *stream_data;
}Stream_Message_t;
typedef list<Stream_Message_t *> LISTSTREAM;

typedef struct _TRACK_CMD_INFO_T{
	int cmd;
	char cmd_name[64];
}Track_cmd_info_t;
typedef struct _Client_Handle
{
	LISTSTREAM list_Handle;
	StreamClient_Status_t nRun_Status;
	int first_frame;
	int nChannel;
	pthread_mutex_t lock;

}Client_Handle_t;
typedef struct Stream_Handle
{
	int channel;
	int port;
	char iP[16];
	void* call_back_fun;//将数据回调由video处理
	void *outParm;//扩展(可由内部自己填充)
	void* param1;//扩展参数
}RecvStream_Handle_t;
