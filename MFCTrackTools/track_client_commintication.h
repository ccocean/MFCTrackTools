#ifndef TRACK_CLIENT_COMMUNTICATION_H
#define TRACK_CLIENT_COMMUNTICATION_H
#include "tch_params.h"
#include"stuTrack_settings_parameter.h"
#include "track_ctrl_params.h"
#include "communtication.h"
#include "share_stream.h"
#include"decode.h"
#include <list>

using namespace std;
#define STU_SETTRACK_CMD  0x100
#define TEA_SETTRACK_CMD  0x101
#define STU_GETTRACK_CMD  0x102
#define TEA_GETTRACK_CMD  0x103
#define GET_CAMERA_INFO   0x104
#define SET_TRACK_STATUS_CMD   0x105
#define GET_TRACK_STATUS_CMD   0x106
#define PLC_GETTRACK_CMD 0x107
#define PLC_SETTRACK_CMD 0x108
#define GET_TRACK_DEBUG_CMD   0x109
#define SET_TRACK_DEBUG_CMD   0x10a
#define TRACK_LOGIN   0x10b
#define CAM_SET_CMD 0x10c
#define CAM_GET_CMD 0x10d
#define TIMEOUT 500

#define TEACH_STREAM_PORT 21301
#define STUDENT_STREAM_PORT  21302
#define STU_CHANNL 0
#define TEACH_CHANNL 1

#define MAX_STEAM_WIDTH 1280
#define MAX_STEAM_HEIGHT 720

#define C_CONTROL_TRACK 15200
#define LIVE_STREAM_NAME_MAX 64
#define CAMENUM 4
#define TCH_FEATURE_CAM 2
#define STU_FEATURE_CAM 3
#define FEATURE_CAM_PORT 5556

#define CAM_USER "admin"
#define CAM_PSWD "admin"

typedef enum
{
	CAM_MOVE = 0,
	CAM_SITE = 1
}Track_type_t;

typedef enum
{
	GOHOME = 0,
	GONEAR = 1,
	GOFAR = 2,
	STOPZ = 3,
	GOUP = 4,
	GODOWN = 5,
	GOLEFT = 6,
	GORIGHT = 7,
	STOPTURN = 8
}Move_Type_t;

typedef enum
{
	STU_PORT = 0,
	TEA_PORT = 1,
}Comport_Type_t;

typedef enum
{
	SET = 0,
	DEL = 1,
	CALL = 2
}Presion_Type_t;

typedef struct Param_t
{
	Move_Type_t move_type;
	Comport_Type_t port_type;
	Presion_Type_t site_type;
	int speed;
	int index;
	Track_type_t  type;
} Serial_Param_t;

typedef struct Pos_t
{
	int posit_pan;
	int posit_tilt;
	int coefficient;
	Comport_Type_t port;
} Serial_Position_t;

typedef struct
{
	char ip[CAMENUM][IP_LEN];
	int  nPort[CAMENUM];
	int  nControPort[CAMENUM];
	char streamName[CAMENUM][LIVE_STREAM_NAME_MAX];
}Panoramic_Camera_Info;
//第2路老师特写，第3路学生特写，0路和1路为全景 留着扩展
typedef struct
{
	int isTchTrack;
	int isStuTrack;
}Track_Status_t;
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
	int callstatus;
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
	void *streamConnectHanlde;
}RecvStream_Handle_t;
typedef struct Track_Login_Info_t
{
	char username[32];
	char passwd[32];
}Login_t;
void * init_stream_recv(RecvStream_Handle_t* pRecv_stream_handle);
int stop_stream_stream(void* pRecv_stream_handle);

int ctrlClient_set_teach_params(TeaITRACK_Params * tec_param, Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_set_stu_params(StuITRACK_ClientParams_t * stu_param, Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_get_teach_params(Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_get_stu_params(Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_get_camera_params(Commutication_Handle_t ptrack_clientHandle);

int ctrlClient_set_policy_params(Policy_Set_t * policy_param, Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_get_policy_params(Commutication_Handle_t ptrack_clientHandle);

int ctrlClient_set_track_status(Track_Status_t * track_status, Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_get_track_status(Commutication_Handle_t ptrack_clientHandle);

int ctrlClient_set_track_debug(int debug, Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_set_stream_display(void* pStream_StuclientHandle, void* pStream_TeaclientHandle, int chanenel);
int ctrlClient_set_Camera_params(Serial_Param_t * cam_param, Commutication_Handle_t ptrack_clientHandle);

int ctrlClient_login(Login_t* logininfo, Commutication_Handle_t ptrack_clientHandle);
int ctrlClient_get_Camera_position(Serial_Position_t * cam_pos, Commutication_Handle_t ptrack_clientHandle);
#endif