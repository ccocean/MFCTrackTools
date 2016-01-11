#include <stdio.h>
#include<string.h>
#include<tchar.h>
#include<stdlib.h>
#include "track_client_commintication.h"
static int stream_getcall_status(Client_Handle_t* pStream_ClientHandle);
static int stream_setcall_status(Client_Handle_t* pStream_ClientHandle, int status);
int ctrlClient_set_teach_params(TeaITRACK_Params * tec_param, Commutication_Handle_t ptrack_clientHandle)
{
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = TEA_SETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(tec_param), sizeof(TeaITRACK_Params), ptrack_clientHandle);
	return 0;
}

int ctrlClient_set_policy_params(Policy_Set_t * policy_param, Commutication_Handle_t ptrack_clientHandle)
{
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = PLC_SETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(policy_param), sizeof(Policy_Set_t), ptrack_clientHandle);
	return 0;
}

int ctrlClient_get_policy_params(Commutication_Handle_t ptrack_clientHandle)
{
	Policy_Set_t  policy_param = { 0 };
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = PLC_GETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(&policy_param), sizeof(Policy_Set_t), ptrack_clientHandle);
	return 0;
}

int ctrlClient_get_teach_params(Commutication_Handle_t ptrack_clientHandle)
{
	TeaITRACK_Params  tec_param = { 0 };
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = TEA_GETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(&tec_param), sizeof(TeaITRACK_Params), ptrack_clientHandle);
	return 0;
}
int ctrlClient_set_stu_params(StuITRACK_ClientParams_t * stu_param, Commutication_Handle_t ptrack_clientHandle)
{
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = STU_SETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(stu_param), sizeof(StuITRACK_ClientParams_t), ptrack_clientHandle);
	return 0;

}
int ctrlClient_get_stu_params(Commutication_Handle_t ptrack_clientHandle)
{
	StuITRACK_ClientParams_t stu_param = { 0 };
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = STU_GETTRACK_CMD;
	communtication_send_clientMsg(&head, (char *)(&stu_param), sizeof(StuITRACK_ClientParams_t), ptrack_clientHandle);
	return 0;
}

int ctrlClient_set_track_status(Track_Status_t * track_status, Commutication_Handle_t ptrack_clientHandle)
{
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = SET_TRACK_STATUS_CMD;
	communtication_send_clientMsg(&head, (char *)(track_status), sizeof(Track_Status_t), ptrack_clientHandle);
	return 0;
}
int ctrlClient_get_track_status(Commutication_Handle_t ptrack_clientHandle)
{
	Track_Status_t track_param = { 0 };
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = GET_TRACK_STATUS_CMD;
	communtication_send_clientMsg(&head, (char *)(&track_param), sizeof(Track_Status_t), ptrack_clientHandle);
	return 0;
}
int ctrlClient_get_camera_params(Commutication_Handle_t ptrack_clientHandle)
{
	Panoramic_Camera_Info camera_param = { 0 };
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = GET_CAMERA_INFO;
	communtication_send_clientMsg(&head, (char *)(&camera_param), sizeof(Panoramic_Camera_Info), ptrack_clientHandle);
	return 0;
}
int ctrlClient_set_track_debug(int debug, Commutication_Handle_t ptrack_clientHandle)
{
	if (ptrack_clientHandle == NULL) {
		AfxMessageBox(TEXT("客户端连接失败"));
		return -1;
	}
	Communtication_Head_t head;
	commutication_init_head(&head, C_CONTROL_TRACK);
	head.cmd = SET_TRACK_DEBUG_CMD;
	communtication_send_clientMsg(&head, (char *)(&debug), sizeof(int), ptrack_clientHandle);
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
	if (stream_getcall_status(pClient_handle) != 1)
	{
		if (data)
		{
			free(data);
		}
		OutputDebugString("helllo \n");
		return 0;
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

			OutputDebugString("stream============\n");
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
	//clean_alloc(pStream_recv_handle);

	pthread_detach(pthread_self());
	pthread_exit(0);
	return NULL;
	
}
int stop_stream_stream(void* param)
{
	RecvStream_Handle_t* pRecv_stream_handle = (RecvStream_Handle_t*)param;
	if (pRecv_stream_handle)
	{
		Stream_Stop(pRecv_stream_handle->streamConnectHanlde);

		Client_Handle_t* pClient_handle = (Client_Handle_t*)pRecv_stream_handle->outParm;
		set_streamClient_status(pClient_handle, STREAMCLINT_STOP);
	}
	return 0;
}
static int stream_setcall_status(Client_Handle_t* pClient_handle, int status)
{

	if (pClient_handle == NULL)
	{
		OutputDebugString("stream_setcall_status fail\n");
		return -1;
	}
	pthread_mutex_lock(&(pClient_handle->lock));
	pClient_handle->callstatus = status;
	pthread_mutex_unlock(&(pClient_handle->lock));
	return 0;
}
static int stream_getcall_status(Client_Handle_t* pClient_handle)
{
	int status = 0;
	if (pClient_handle == NULL)
	{
		OutputDebugString("stream_getcall_status is fail\n");
		return -1;
	}
	pthread_mutex_lock(&(pClient_handle->lock));
	status = pClient_handle->callstatus;
	pthread_mutex_unlock(&(pClient_handle->lock));
	return status;
}
int ctrlClient_set_stream_display(void* pStream_StuclientHandle, void* pStream_TeaclientHandle, int chanenel)
{
	RecvStream_Handle_t* pRecv_streamStu_handle = (RecvStream_Handle_t*)pStream_StuclientHandle;
	RecvStream_Handle_t* pRecv_streamTea_handle = (RecvStream_Handle_t*)pStream_TeaclientHandle;
	Client_Handle_t* pClient_Stuhandle = NULL;
	Client_Handle_t* pClient_Teahandle = NULL;
	if (pRecv_streamStu_handle == NULL || pRecv_streamTea_handle == NULL)
	{
		OutputDebugString("stream_setcall_status is fial\n");
		return -1;
	}
	pClient_Stuhandle = (Client_Handle_t*)pRecv_streamStu_handle->outParm;
	pClient_Teahandle = (Client_Handle_t*)pRecv_streamTea_handle->outParm;
	if (chanenel == STU_CHANNL)
	{
		stream_setcall_status(pClient_Teahandle, 0);
		stream_setcall_status(pClient_Stuhandle, 1);
	}
	else
	{
		stream_setcall_status(pClient_Stuhandle, 0);
		stream_setcall_status(pClient_Teahandle, 1);
	}
	return 0;
}
void* init_stream_recv(RecvStream_Handle_t* pRecv_stream_handle)
{
	int ret = 0;
	pthread_t tid;
	Client_Handle_t* pClient_handle = NULL;
	if (pRecv_stream_handle == NULL)
	{
		MessageBox(NULL, _T("接收流失败"), _T("标题"), MB_OK);
		return NULL;
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
	pClient_handle->callstatus = 0;
	if (pStream_handle->streamConnectHanlde = Stream_init_client(&(data_prm)))
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
		pStream_handle = NULL;
	}
	return pStream_handle;
}


