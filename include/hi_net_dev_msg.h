#ifndef _HI_NET_DEV_MSG_H_
#define _HI_NET_DEV_MSG_H_

#include "hi_net_dev_sdk.h"

#ifndef WIN32
#define WIN32
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_SDK_MSG_REQ_STARTSTREAM  		0x1001
#define HI_SDK_MSG_RES_STARTSTREAM			0x1002

#define HI_SDK_MSG_REQ_STOPSTREAM			0x1011
#define HI_SDK_MSG_RES_STOPSTREAM			0x1012

#define HI_SDK_MSG_REQ_HEARTBEAT    		0x1021
#define HI_SDK_MSG_RES_HEARTBEAT			0x1022

#define HI_SDK_MSG_REQ_PTZ					0x1031
#define HI_SDK_MSG_RES_PTZ					0x1032

#define HI_SDK_MSG_REQ_PRESET               0x1041
#define HI_SDK_MSG_RES_PRESET               0x1042

#define HI_SDK_MSG_REQ_MAKEKEYFRAME 		0x1051
#define HI_SDK_MSG_RES_MAKEKEYFRAME			0x1052

#define HI_SDK_MSG_REQ_SETCONFIG    		0x1061
#define HI_SDK_MSG_RES_SETCONFIG			0x1062

#define HI_SDK_MSG_REQ_GETCONFIG    		0x1071
#define HI_SDK_MSG_RES_GETCONFIG			0x1072

#define HI_SDK_MSG_REQ_DISCONNECTED 		0x1098
#define HI_SDK_MSG_RES_DISCONNECTED			0x1099

#define HI_SDK_MSG_DATA      				0x1100


/* 消息头 */
struct hi_sdk_msg_head{
	int type;
	int len;
	int wait_port;
	char data[0];
};

/* response struct for all msg */
struct hi_sdk_msg_response{
	int retcode; /* 0: sucess, others: error code for fail */
	char data[0];
};

/* 请求 启动码流 */
struct hi_sdk_msg_startstream{
	int stream; // main stream , sub stream
	int type;   // audio,video,both
};

/* 请求 停止码流 */
struct hi_sdk_msg_stopstream{
	int stream; // main stream , sub stream
	int type;   // audio,video,both
};

///包含 sysheader 的数据消息
struct hi_sdk_msg_data_sys
{
	HI_S_SysHeader syshead;
};

/* 包含 avframe 的数据消息 */
struct hi_sdk_msg_data_av{
	HI_S_AVFrame avframe;
};

/* 请求 PTZ */
struct hi_sdk_msg_ptz{
	int action; 
	int speed; 
};

/* 请求 PRESET */
struct hi_sdk_msg_preset{
	int action; 
	int num; 
};

/* 请求 MAKE KEY FRAME */
struct hi_sdk_msg_makekeyframe{
	int stream; 
};

/* 请求 Set/Get Config */
struct hi_sdk_msg_config{
	int cmd; 
	union {
		HI_S_Display display;
		HI_S_Display_Ext display_ext;
		HI_S_OSD osd;
		HI_S_SERVERTIME servertime;
		HI_S_Video_Ext video_ext;
		HI_S_Audio_Ext audio_ext;
		HI_S_AudioVolume audiovolume;
		HI_S_Resolution resolution; 
	}u;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_HI_NET_DEV_MSG_H_*/
