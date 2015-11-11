#include <stdio.h>
#include<string.h>
#include<tchar.h>
#include<stdlib.h>
#include "track_client_commintication.h"

#define C_CONTROL_TRACK 15200
Commutication_Handle_t g_track_clientHandle = NULL;


static Track_cmd_info_t g_track_cmd[] =
{
	{ STU_SETTRACK_CMD, "设置学生参数" },
	{ TEA_SETTRACK_CMD, "设置老师参数" },
	{ STU_GETTRACK_CMD, "获取学生参数" },
	{ TEA_GETTRACK_CMD, "获取老师参数" },
};

static  inline char *get_track_cmd_name(int cmd)
{
	int i = 0;
	while (g_track_cmd[i].cmd != -1)
	{
		if (cmd == g_track_cmd[i].cmd)
			return g_track_cmd[i].cmd_name;
		i++;
	}
	return "unknown name";
}
static int ctrlClient_process_trackHeart(char *buff, void * param)
{
	OutputDebugString(_T("ctrlClient_process_trackHeart=====================\n"));
	return 0;
}

static int ctrl_init_track(void * param)
{
	return  0;
}
//回调函数处理
static int ctrlClient_process_trackMsg(Communtication_Head_t *head, void *msg, Commutication_Handle_t handle, void * param)
{
	char errMsg[128] = { 0 };
	if (NULL == head || NULL == msg || NULL == handle) {
		MessageBox(NULL, _T("接收信息失败"), _T("标题"), MB_OK);
		return -1;
	}
	int return_code = 0;
	return_code = head->return_code;

	if (return_code != 0) 
	{
		sprintf_s(errMsg, sizeof(errMsg), "%s失败", get_track_cmd_name(head->cmd));
		MessageBox(NULL, _T(errMsg), _T("标题"), MB_OK);
		return -1;
	}
	switch (head->cmd)
	{
		case STU_SETTRACK_CMD:
		{
								break;
		}
		case TEA_SETTRACK_CMD:
		{
								 break;
		}
		case STU_GETTRACK_CMD:
		{
								 break;
		}
		case TEA_GETTRACK_CMD:
		{
								 break;
		}
	}
	sprintf_s(errMsg, sizeof(errMsg), "%s成功", get_track_cmd_name(head->cmd));
	MessageBox(NULL, _T(errMsg), _T("标题"), MB_OK);
	return 0;
}
int ctrlClient_init_trackCommuntication()
{

	if (g_track_clientHandle != NULL) 
	{
		OutputDebugString(_T("ctrlClient_init_trackCommuntication is init"));
		return -1;
	}

	g_track_clientHandle = communtication_create_clientHandle("192.168.11.140", C_CONTROL_TRACK, 
		ctrlClient_process_trackMsg, ctrlClient_process_trackHeart, ctrl_init_track, NULL);
	if (g_track_clientHandle == NULL) {
		MessageBox(NULL, TEXT("创建客户端失败"), TEXT("标题"), MB_OK);
		return -1;
	}

	return  0;
}
int ctrlClient_set_teach_params(TeaITRACK_Params * tec_param)
{
	if (g_track_clientHandle == NULL) {
		MessageBox(NULL, TEXT("客户端连接失败"), TEXT("标题"), MB_OK);
		return -1;
	}
	Communtication_Head_t head;
	memset(&head, 0, sizeof(Communtication_Head_t));
	head.cmd = STU_SETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(tec_param), sizeof(TeaITRACK_Params), g_track_clientHandle);
	return 0;
}
//netstream==================================================================================================================

static int free_stream_message(Stream_Message_t *pStream_messgage)
{
	if (pStream_messgage == NULL)
	{

		share_outputlog(NS_ERROR, "free_stream_message is fail\n");
		return -1;
	}

	if (pStream_messgage->stream_data)
	{
		free(pStream_messgage->stream_data);
		pStream_messgage->stream_data = NULL;
	}
	free(pStream_messgage);
	pStream_messgage = NULL;
	return 0;

}
static int track_process_streamMsg(int msg_code, void *arg)
{
	Client_Handle_t* pClient_handle = (Client_Handle_t*)arg;

	if (NULL == pClient_handle)
	{
		OutputDebugString(_T("net_info ==NUL \n"));
		return -1;
	}
	if (msg_code < 0)
	{
		//upEnc_endConnect_set(pClient_handle);
	}

	return 1;
}
static void *track_get_dataEmptyBuf(void *arg, void **pEmptyBufInfo, int data_len)
{
	//	nslog(NS_ERROR, "upEnc_get_dataEmptyBuf\n");
	if (NULL == arg || data_len <= 0)
	{
		OutputDebugString(_T("argpEmptyBufInfo=,data_len=\n"));
		return NULL;
	}
	char *data_buf = NULL;
	data_buf = (char *)malloc(data_len);

	if (NULL == data_buf)
	{
		return NULL;
	}

	memset(data_buf, 0, data_len);
	return data_buf;
}

//数据接收模块回调函数
static int  track_process_data(RH_FRAMEHEAD_t *fh, void *pEmptyBufInfo, void *arg, void *data)
{
	
	
	Client_Handle_t* pClient_handle = (Client_Handle_t*)arg;
	if (data == NULL || pClient_handle == NULL )
	{
		if (data)
		{
			free(data);
		}
		return -1;
	}
	Stream_Message_t* pStream_message = (Stream_Message_t*)malloc(sizeof(*pStream_message));
	if (pStream_message == NULL)
	{
		if (data)
		{
			free(data);
		}
		return -1;
	}

	//将数据放进链表中
	memcpy(&(pStream_message->fh), fh, sizeof(RH_FRAMEHEAD_t));
	pStream_message->fh.nChannel = pClient_handle->nChannel;
	pStream_message->stream_data = data;


//	if (fh->nIframe == 1 && pClient_handle->first_frame == 0)
	{
		pClient_handle->first_frame = 1;
		//upEnc_firstData_set(pClient_handle);
	}
	pthread_mutex_lock(&(pClient_handle->lock));
	if ((pClient_handle->first_frame == 1) && (pClient_handle->list_Handle).size() < 15)
	{
		(pClient_handle->list_Handle).push_back(pStream_message);
	}
	else
	{
		printf("\033[32m""pClient_handle->first_frame: %d ID:%d\n""\033[0m", pClient_handle->first_frame, pStream_message->fh.nChannel);
		free_stream_message(pStream_message);
	}
	pthread_mutex_unlock(&(pClient_handle->lock));


	return 0;
}

static StreamClient_Status_t get_streamClient_status(Client_Handle_t* pStrem_recv_handle)
{
	StreamClient_Status_t ret = STREAMCLINT_START;
	if (pStrem_recv_handle == NULL)
	{
		return STREAM_STATUS_ERR;
	}
	pthread_mutex_lock(&(pStrem_recv_handle->lock));
	ret = pStrem_recv_handle->nRun_Status;
	pthread_mutex_unlock(&(pStrem_recv_handle->lock));
	return ret;
}
static int set_streamClient_status(Client_Handle_t* pStrem_recv_handle, StreamClient_Status_t client_status)
{

	StreamClient_Status_t ret = STREAMCLINT_START;
	if (pStrem_recv_handle == NULL)
	{
		return -1;
	}
	pthread_mutex_lock(&(pStrem_recv_handle->lock));
	pStrem_recv_handle->nRun_Status = client_status;
	pthread_mutex_unlock(&(pStrem_recv_handle->lock));
	return ret;
}
static int clean_alloc(RecvStream_Handle_t* pStream_handle)
{
	if (pStream_handle == NULL)
	{
		share_outputlog(NS_ERROR, "clean_alloc is fail\n");
		return -1;
	}

	Client_Handle_t* pClient_handle = (Client_Handle_t*)pStream_handle->outParm;
	if (pStream_handle)
	{
		free(pStream_handle);
		pStream_handle = NULL;

	}
	if (pClient_handle)
	{
		if (pClient_handle->streamDecode.data)
		{
			free(pClient_handle->streamDecode.data);
			pClient_handle->streamDecode.data = NULL;
		}
		delete pClient_handle;
		pClient_handle = NULL;
	}
	return 0;
}

static void* stream_pop_thread(void* arg)
{

	RecvStream_Handle_t* pStream_recv_handle = (RecvStream_Handle_t*)arg;
	Stream_Message_t *pStream_messgage = NULL;
	Stream_Call_Back call_back_fun = NULL;
	int list_size = 0;
	if (pStream_recv_handle == NULL || pStream_recv_handle->outParm == NULL)
	{
		goto EXIT;
	}
	call_back_fun = (Stream_Call_Back)pStream_recv_handle->call_back_fun;

	Client_Handle_t* pClient_handle = (Client_Handle_t*)pStream_recv_handle->outParm;
	while (get_streamClient_status(pClient_handle) == STREAMCLINT_START)
	{
		pthread_mutex_lock(&(pClient_handle->lock));
		if (list_size = pClient_handle->list_Handle.size() > 0)
		{
			pStream_messgage = pClient_handle->list_Handle.front();
			pClient_handle->list_Handle.pop_front();

			H264_To_RGB((unsigned char*)(pStream_messgage->stream_data), pStream_messgage->fh.nFrameLength,
				pClient_handle->streamDecode.data, &(pClient_handle->decoder));
			pClient_handle->streamDecode.height = pStream_messgage->fh.nHight;
			pClient_handle->streamDecode.width = pStream_messgage->fh.nWidth;
			call_back_fun(&(pClient_handle->streamDecode), pStream_recv_handle->param1);
		
			free_stream_message(pStream_messgage);

		}
		pthread_mutex_unlock(&(pClient_handle->lock));

		Sleep(10);
	}
EXIT:
	share_outputlog(NS_ERROR, "%s : stream_pop_thread is finsh\n", __FILE__);
	clean_alloc(pStream_recv_handle);

	pthread_detach(pthread_self());
	pthread_exit(0);
	return NULL;
	
}
int init_stream_recv(RecvStream_Handle_t* pRecv_stream_handle)
{
	int ret = 0;
	pthread_t tid;
	Client_Handle_t* pClient_handle = NULL;
	if (pRecv_stream_handle == NULL)
	{
		MessageBox(NULL, _T("接收流失败"), _T("标题"), MB_OK);
		return -1;
	}
	RecvStream_Handle_t* pStream_handle = (RecvStream_Handle_t*)malloc(sizeof(RecvStream_Handle_t));
	if (pStream_handle == NULL)
	{
		ret = -1;
		goto EXIT;
	}
	memcpy(pStream_handle, pRecv_stream_handle, sizeof(RecvStream_Handle_t));


	pClient_handle = new Client_Handle_t;
	if (pClient_handle == NULL)
	{
		OutputDebugString(_T("pClient_handle is NULL"));
		ret = -1;
		goto EXIT;
	}
	pClient_handle->streamDecode.data =(unsigned char *)malloc(MAX_STEAM_WIDTH * MAX_STEAM_HEIGHT * 3);
	if (pClient_handle->streamDecode.data == NULL)
	{
		OutputDebugString(_T("pClient_handle->streamDecode.data malloc is NULL"));
		ret = -1;
		goto EXIT;
	}

	H264_init(&(pClient_handle->decoder));
	pClient_handle->list_Handle.clear();
	//指向客户端信息
	pStream_handle->outParm = pClient_handle;
	pClient_handle->nChannel = pRecv_stream_handle->channel;

	RH_tcp_stream_recv_cond_t data_prm;
	memset(&(data_prm), 0, sizeof(RH_tcp_stream_recv_cond_t));
	data_prm.port = pStream_handle->port;
	strcpy_s((char *)data_prm.ip,sizeof(data_prm.ip), pStream_handle->iP);
	data_prm.arg = pClient_handle;
	data_prm.getEmptyBuf = track_get_dataEmptyBuf;
	data_prm.putStreaminfo = track_process_data;
	data_prm.exception_msg = track_process_streamMsg;
	data_prm.timeout = TIMEOUT;


	pthread_mutex_init(&(pClient_handle->lock), NULL);
	if (Stream_init_client(&(data_prm)))
	{
		set_streamClient_status(pClient_handle, STREAMCLINT_START);
	}

	ret = pthread_create(&tid, NULL, stream_pop_thread, (void *)(pStream_handle));
	if (ret != 0)
	{
		ret = -1;
		OutputDebugString(_T("pthread_create pop_thrrad is NULL"));
		goto EXIT;
	}

EXIT:
	if (ret == -1)
	{
		clean_alloc(pStream_handle);
	}
	return ret;
}


