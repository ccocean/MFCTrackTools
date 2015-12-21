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

#define CAM_USER "admin"
#define CAM_PSWD "admin"

typedef struct
{
	char ip[CAMENUM][IP_LEN];
	int  nPort[CAMENUM];
	int  nControPort[CAMENUM];
	char streamName[CAMENUM][LIVE_STREAM_NAME_MAX];
}Panoramic_Camera_Info;
//��2·��ʦ��д����3·ѧ����д��0·��1·Ϊȫ�� ������չ
typedef struct
{
	int nTurnTrack;
	int param1;
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
	void *stream_data;//����
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
	Stream_Call_Back call_back_fun;//�����ݻص���video����
	void *outParm;//��չ(�����ڲ��Լ����)
	void* param1;//��չ����,��ʹ���ߴ���ȥ��ͨ���ص�����call_back������
	void *streamConnectHanlde;
}RecvStream_Handle_t;
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

int ctrlClient_set_stream_display(void* pStream_StuclientHandle, void* pStream_TeaclientHandle, int chanenel);
#endif