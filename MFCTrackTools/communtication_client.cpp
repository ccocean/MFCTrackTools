
#include "stdafx.h"
#include "communtication.h"

#include<windows.h>
#include <winbase.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int communtication_get_handleStatus(Commutication_Handle_t handle)
{
	if (handle == NULL) {
		share_outputlog(NS_ERROR, "handle is NULL\n");
		return NO_INIT_STATUS;
	}

	return handle->status;
}
int communtication_check_head(Communtication_Head_t *head)
{
	if ((head == NULL) || (head->check_start[0] != DEFAULT_CHECK_START_CODE)
		|| (head->check_start[1] != DEFAULT_CHECK_START_CODE)
		|| (head->check_start[2] != DEFAULT_CHECK_START_CODE)
		|| (head->check_start[3] != DEFAULT_CHECK_START_CODE)
		|| (head->check_end[0] != DEFAULT_CHECK_END_CODE)
		|| (head->check_end[1] != DEFAULT_CHECK_END_CODE)
		|| (head->check_end[2] != DEFAULT_CHECK_END_CODE)
		|| (head->check_end[3] != DEFAULT_CHECK_END_CODE)) {
		share_outputlog(NS_ERROR, "the head is not head\n");
		share_outputlog(NS_DEBUG, "[%c][%c][%c][%c][%c][%c][%c][%c]\n", head->check_start[0], head->check_start[1], head->check_start[2], head->check_start[3],
			head->check_end[0], head->check_end[1], head->check_end[2], head->check_end[3]);
		return -1;
	}

	return 0;
}
void communtication_free_head(Commutication_Handle_t *handle)
{
	Commutication_Handle_t temp = *handle;

	if (temp != NULL) {
		pthread_mutex_destroy(&(temp->lock));
		free(temp);
		*handle = NULL;
	}

	return;
}
int communtication_set_handleStatus(Commutication_Handle_t handle, int status)
{
	if (handle == NULL) {
		share_outputlog(NS_ERROR, "handle is NULL\n");
		return -1;
	}

	handle->status = status;
	return 0;
}
#if 1
static int communtication_process_clientHeartMsg(Communtication_Head_t *head, char *buf, int buflen, Communtication_Handle_t *handle)
{
	if (handle == NULL || handle->client_socket < 0) {
		return -1;
	}

	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg\n");
	int socket = handle->client_socket;
	int sendlen = 0;
	Communtication_Head_t head_s;
	memcpy(&head_s, head, sizeof(Communtication_Head_t));
	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg1\n");
	pthread_mutex_lock(&(handle->lock));
	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg4\n");
	sendlen = sizeof(head_s);
	RH_TcpSndBlockFd(socket, (char *)&head_s, &sendlen);
	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg5,buflen=%d\n",buflen);
	sendlen = head_s.total_len;
	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg6,buflen=%d\n",buflen);
	RH_TcpSndBlockFd(socket, buf, &sendlen);
	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg6\n");
	pthread_mutex_unlock(&(handle->lock));

	if (handle->DealHeartbitFuncPtr != NULL) {
		handle->DealHeartbitFuncPtr(buf, handle->param);
	}

	////nslog(NS_DEBUG,"communtication_process_clientHeartMsg2\n");
	return 0;
}
#endif

static int communtication_clientHeartThread(void *argv)
{
	char ip[16] = "127.0.0.1";
	unsigned short port = 3100;
	Commutication_Handle_t handle = (Commutication_Handle_t)argv;
	int ret = 0;
	char RecvBuf[1500] = { 0 };
	int RecvLen = 0;
	//	int NoRecvTime = 0;
	int client_socket = -1;
	int needlen = 0;
	Communtication_Head_t headbuf;

	if (handle == NULL) {
		share_outputlog(NS_ERROR, "handle is NULL\n");
		//printf_pthread_delete(__FILE__, (char *)__FUNCTION__);
		pthread_detach(pthread_self());
		pthread_exit(0);
		return -1;
	}

	sprintf_s(ip, sizeof(ip), "%s", handle->ip);
	port = handle->port;

	Communtication_Head_t *head = NULL;
	//char headbuf[256] = {0};
	Sleep(2000);
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
REPEAT_CONNECT:
	share_outputlog(NS_WARN, "client will connet to server [%s:%d]\n", ip, port);
	handle->client_socket = -1;

	if (client_socket > 0) {
		RH_Close(__FILE__, (char *)__FUNCTION__, client_socket);
		client_socket = -1;
	}

	client_socket = RH_CreateTcpNoBindFd();
	//	RH_SetNonBlockFd(client_socket);

	ret = RH_ConnetBlockFd(client_socket, port, ip, COMMUN_CONNECT_TIMEOUT);

	if (ret != RHRETSUCCESS) {
		share_outputlog(NS_ERROR, "connet server fd is failed,the pot is [%d] ip:%s\n", port, ip);
		Sleep(1000);
		goto REPEAT_CONNECT;
	}

	share_outputlog(NS_INFO, "connet [%s:%d] is ok\n", ip, port);
	handle->client_socket = client_socket;
	communtication_set_handleStatus(handle, START_STATUS);

	//init ;
	if (handle->ConnectServerInitPtr != NULL) {
		handle->ConnectServerInitPtr(handle->param);
	}

	RH_SetRcvTimeoutFd(client_socket, 3, 0);
	//设置发送连接超时
	RH_SetSndTimeoutFd(client_socket, 1, 0);
	//重连的话，需要触发一些初始化逻辑，
	//比如room重启或者HD重启后，需要重新下发 control的参数给其他模块，考虑在此加入一个回调来处理
	
	while (communtication_get_handleStatus(handle) == START_STATUS) {
		needlen = sizeof(Communtication_Head_t);
		memset(&headbuf, 0, sizeof(headbuf));
		ret = RH_TcpRcvBlockFd(client_socket, (char *)(&headbuf), needlen, &RecvLen);

		if (ret != RHRETSUCCESS || RecvLen != needlen) {
			OutputDebugString(_T("tcp recv failed\n"));
			share_outputlog(NS_ERROR, "tcp recv failed,the port is [%u].\n", port);
			Sleep(500);
			goto REPEAT_CONNECT;
		}

		//check head
		if (communtication_check_head(&headbuf) != 0) {
			share_outputlog(NS_ERROR, "%s,communtication_check_head is failed,the port = [%u].\n", __FUNCTION__, port);
			Sleep(500);
			goto REPEAT_CONNECT;
		}

		head = &headbuf;
		needlen = head->total_len;
		memset(&RecvBuf, 0, sizeof(RecvBuf));
		ret = RH_TcpRcvBlockFd(client_socket, RecvBuf, needlen, &RecvLen);


		if (ret != RHRETSUCCESS || RecvLen != needlen) {
			share_outputlog(NS_ERROR, "Commutication tcp recv is failed,the cmd is [%u],the port = [%u]\n", head->cmd, port);
			Sleep(500);
			goto REPEAT_CONNECT;
		}

		//心跳也需要上报
		if (head->cmd == HEARTBIT_CMD) {
			communtication_process_clientHeartMsg(head, RecvBuf, RecvLen, handle);
		}
		else {
			if (handle->DealCmdFuncPtr != NULL) {
				handle->DealCmdFuncPtr(head, RecvBuf, handle, handle->param);
			}
		}
	}


	handle->client_socket = -1;

	if (client_socket > 0) {
		RH_Close(__FILE__, (char *)__FUNCTION__, client_socket);
		client_socket = -1;
	}
	WSACleanup();
	//printf_pthread_delete(__FILE__, (char *)__FUNCTION__);
	pthread_detach(pthread_self());
	pthread_exit(0);
	return 0;
}

static void* communtication_conv_clientHeartThread(void *argv)
{
	communtication_clientHeartThread(argv);
	return NULL;
}


int communtication_send_clientMsg(Communtication_Head_t *head, char *date, int buflen, Communtication_Handle_t *handle)
{
	if (handle == NULL || handle->status != START_STATUS || handle->client_socket < 0) {
		return -1;
	}

	char buff[DEFATULT_COMMUTICATION_TOTAL_LEN] = { 0 };
	int socket = handle->client_socket;
	int sendlen = 0;

	memcpy(buff, date, buflen);
	//Communtication_Head_t head_s ;
	//	memcpy(&head_s, head, sizeof(Communtication_Head_t));

	head->total_len = buflen;
	pthread_mutex_lock(&(handle->lock));
	sendlen = sizeof(Communtication_Head_t);
	RH_TcpSndBlockFd(socket, (char *)(head), &sendlen);
	sendlen = head->total_len;
	RH_TcpSndBlockFd(socket, buff, &sendlen);
	pthread_mutex_unlock(&(handle->lock));
	return 0;

}


/*创建handle，同时产生后台的线程*/
Commutication_Handle_t communtication_create_clientHandle(char *dst_ip, unsigned short dst_port, DealCmdFunc func1, 
	DealheartbitFunc func2, ConnectServerInitFunc func3, void * param)
{
	if (dst_ip == NULL || dst_port == 0) {
		 share_outputlog(NS_ERROR, "local_ip or local_port is error\n");
		return NULL;
	}

	share_outputlog(NS_DEBUG, "-------------dst_ip[%s]dst_port[%d]\n", dst_ip, dst_port);

	Commutication_Handle_t handle = NULL;
	char task_name[128] = { 0 };
	int ret = 0;
	handle = (Commutication_Handle_t)malloc(sizeof(Communtication_Handle_t));

	if (handle == NULL) {
		share_outputlog(NS_ERROR, "Malloc handle is failed\n");
		return NULL;
	}

	pthread_mutex_t mutex;
	pthread_t tid;
	pthread_mutex_init(&mutex, NULL);

	//init handle
	memset(handle, 0, sizeof(Communtication_Handle_t));
	handle->lock = mutex;

	communtication_set_handleStatus(handle, NO_INIT_STATUS);
	handle->seq_num = 0;
	sprintf_s(handle->ip, sizeof(handle->ip), "%s", dst_ip);
	handle->port = dst_port;
	handle->DealCmdFuncPtr = func1;
	handle->DealHeartbitFuncPtr = func2;
	handle->ConnectServerInitPtr = func3;
	handle->param = param;
	sprintf_s(task_name, sizeof(task_name), "clientcommutication_%d", dst_port);
	//printf_pthread_create(__FILE__, ("communtication_clientHeartThread"));

	//mid_task重新用8168平台封装
	ret = pthread_create(&tid, NULL, communtication_conv_clientHeartThread, (void *)(handle));
	if (ret != 0) {
		share_outputlog(NS_ERROR, "crate communtication client thread failed\n");

		communtication_free_head(&handle);
		return NULL;
	}

	return handle;
}