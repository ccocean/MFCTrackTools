#include "share_stream.h"
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<timeapi.h>
#pragma comment(lib,"winmm.lib")
#define DEFAULT_CHECK_START_CODE '$'
#define DEFAULT_CHECK_END_CODE   '#'

static int priv_check_head(RH_FRAMEHEAD_t *head)
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
static int privClient_connect_Srv(char * ip, int port) {
	int sockfd = INVALID_SOCKET;

	sockfd = RH_Socket(__FILE__, (char *)__FUNCTIONW__, AF_INET, SOCK_STREAM, 0);

	if (RH_ConnetBlockFd(sockfd, port, ip, STREAM_CONNECT_TIMEOUT) < 0) {
		RH_Close(__FILE__, (char *)__FUNCTIONW__, sockfd);
		sockfd = INVALID_SOCKET;
	};

	return sockfd;
}
static void *privCient_process_thread(void * arg) {
	RH_tcp_stream_recv_cond_t *src = (RH_tcp_stream_recv_cond_t *)arg;
	int sockfd = 0, ret = 0;
	//	int32_t fret = 0;
	//	int return_code = 0;
	char *data = NULL;
	int data_len = 0;
	void *pEmptyBufInfo = NULL;
	RH_FRAMEHEAD_t fh;
	const int fh_len = RH_FH_LEN;
	int readlen = fh_len;
	int msg_num = 0;

	int timeout = -1;
	unsigned int time = 0, temp;

	timeout = src->timeout;

	int timeoutsec = 0;
	int timeoutusec = 0;

	if (timeout > 200) {
		timeoutsec = timeout / 1000;
		timeoutusec = 1000 * (timeout % 1000);
	}
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	for (;;) {
		//nslog(NS_INFO, "connect server, ip = %s-port=%d,timeout=[%d]\n", src->ip, src->port, timeout);
		sockfd = privClient_connect_Srv((char *)(src->ip), src->port);
		if (sockfd < 0) {
			share_outputlog(NS_ERROR, "sock privClient_connect_Srv failed!port =%d-<%s>-WSAGetLastError()=%d-<%s>\n", src->port, src->ip, WSAGetLastError(), strerror(WSAGetLastError()));

			if (src->exception_msg) {
				src->exception_msg(-1, src->arg);
			}

			Sleep(3000);
			continue;
		}

		share_outputlog(NS_INFO, "connect success, sockfd = %d-port=[%s:%d]\n", sockfd, src->ip, src->port);

		if (timeout > 200) {
			share_outputlog(NS_DEBUG, "tiemoutsec.[%d.%d]\n", timeoutsec, timeoutusec);
			RH_SetRcvTimeoutFd(sockfd, timeoutsec, timeoutusec);
		}


		for (;;) {
			msg_num = 0;
			memset(&fh, 0, RH_FH_LEN);
			ret = RH_TcpRcvBlockFd(sockfd, (char *)&fh, fh_len, &readlen);

			if (ret < 0 || fh_len != readlen || (priv_check_head(&fh) != 0)) {
				share_outputlog(NS_ERROR, "port=%d -sockfd = %d,fh_len=%d,recv len=%d\n", src->port, sockfd, fh_len, readlen);
				msg_num = -1;
				Sleep(3000);
				break;
			}

			readlen = fh.nFrameLength;
			data = (char*)src->getEmptyBuf(src->arg, &pEmptyBufInfo, fh.nFrameLength);
			if (data == NULL) {
				share_outputlog(NS_ERROR, "get [%s:%d]empty buf is NULL,len =%d\n", src->ip, src->port, fh.nFrameLength);
				msg_num = -2;
				Sleep(3000);
				break;
			}

			time = timeGetTime();

			ret = RH_TcpRcvBlockFd(sockfd, data, readlen, &data_len);
			temp = timeGetTime() - time;

			if (temp > 1000) {
				share_outputlog(NS_WARN, "Error curr_time - send_before time =%d,fh_len=%d \n", temp, fh_len);

			}

			if (RHRETSUCCESS == ret) {
				ret = src->putStreaminfo(&fh, pEmptyBufInfo, src->arg, data);

				if (ret < 0) {
					share_outputlog(NS_ERROR, "putStreaminfo failed ...\n");
					msg_num = -3;
					Sleep(2000);
					break;
				}
			}
			else {
				msg_num = -1;
				share_outputlog(NS_ERROR, "recv_ error:[%s] len:<%d>recv len<%d>\n", strerror(WSAGetLastError()), readlen, data_len);
				Sleep(2000);
				break;
			}
		}

		//nslog(NS_ERROR, "--msg_num=%d-\n", msg_num);

		if (src->exception_msg) {
			src->exception_msg(msg_num, src->arg);
		}

		RH_Close(__FILE__, (char *)__FUNCTIONW__, sockfd);
		sockfd = -1;

	}
	if (src)
	{
		free(src);
		src = NULL;
	}
	WSACleanup();
	pthread_detach(pthread_self());
	pthread_exit(0);
	return NULL;
}

void *Stream_init_client(RH_tcp_stream_recv_cond_t * InParm) {

	pthread_attr_t attr;
	struct sched_param param;
	if (InParm == NULL)
	{
		return NULL;
	}
	RH_tcp_stream_recv_cond_t * src = (RH_tcp_stream_recv_cond_t *)malloc(sizeof(RH_tcp_stream_recv_cond_t));
	if (src == NULL)
	{
		return NULL;
	}
	memcpy(src, InParm, sizeof(RH_tcp_stream_recv_cond_t));
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	param.sched_priority = 80;
	pthread_attr_setschedparam(&attr, &param);
	pthread_t    p;
	printf("pthread_create-------fuck80-----------[%s:%s:%d] \n", __FILE__, __FUNCTION__, __LINE__);
	//printf_pthread_create(__FILE__, ("privCient_process_thread"));
	if (pthread_create(&p, &attr, privCient_process_thread, (void *)src)) {
		share_outputlog(NS_ERROR, "pthread_create error");
		pthread_attr_destroy(&attr);
		return NULL;
	}

	pthread_attr_destroy(&attr);
	return src;
}