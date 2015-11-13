#ifndef	__RH_SOCKET_H__
#define	__RH_SOCKET_H__

//#include <windows.h>
//#include <WinBase.h>
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#include <string.h>

#include <time.h>
typedef int			int32_t;
#define RHLISTENQUENUM			100
#define RHADDRMAXLEN			16

#define RHRETSUCCESS			0
#define RHRETFAIL				-1
#define COMMUN_CONNECT_TIMEOUT 1
// *****************************************************
// function	: ������ģʽTCP SOCKET ��������
// author 	: zhengyb		2014.9.1
// return   : Succes 0  fail -1
// parameter: Fd > 0
//            SndBuf ��ΪNULL
//			  SndLen ��ΪNULL ��*SndLen����Ҫ�������ݳ��� ���������غ�*SndLenΪ�ѷ��ͳ���
// note		: 1.����SOCKET
//			  2.�˺������ RH_SetSndTimeoutFd �����趨���ͳ�ʱ ����
//******************************************************
int RH_TcpSndBlockFd(int Fd, char *SndBuf, int *SndLen);


// *****************************************************
// function	: ������ģʽTCP SOCKET ��������
// author 	: zhengyb		2014.9.1
// return   : Succes 0  fail -1
// parameter: Fd > 0
//            RcvBuf ��ΪNULL
//			  RcvLen ��ΪNULL ��*RcvLen����Ҫ�������ݳ��� ���������غ�*RcvLenΪ�ѽ��ճ���
// note		: 1.����SOCKET
//			  2.�˺������ RH_SetRcvTimeoutFd �����趨���ͳ�ʱ ����
//******************************************************
int RH_TcpRcvBlockFd(int Fd, char *RcvBuf, int needlen, int *RcvLen);

// *****************************************************
// function	: ����Tcp SOCKET NoBind ģʽ
// author 	: zhengyb		2014.9.1
// return   : Succes fd / fail -1
// parameter: NULL
//******************************************************
int RH_CreateTcpNoBindFd(void); //�Ƽ�

// *****************************************************
// function	: ������ģʽSOCKET ���ӷ�����
// author 	: zhengyb		2014.9.1
// return   : Succes 0 / fail -1
// parameter: Fd > 0 / ServPort ���ڵ��� 0 /ServIp ��Ϊ�� /Timeout ���ڵ��� 0
// note		: ����SOCKET
//******************************************************
int RH_ConnetBlockFd(int Fd, int ServPort, char *ServIp, int Timeout);

int32_t RH_Close(char *file, char *func, int32_t fd);

int RH_SetRcvTimeoutFd(int Fd, int TimeoutSec, int TimeoutUsec);

int32_t RH_Socket(char *file, char *func, int32_t domain, int32_t type, int32_t protocol);

int RH_SetSndTimeoutFd(int Fd, int TimeoutSec, int TimeoutUsec);
typedef enum {
	NS_DEBUG = 20,
	NS_INFO = 40,
	NS_NOTICE = 60,
	NS_WARN = 80,
	NS_ERROR = 100,
	NS_FATAL = 120
} share_log_level;
int share_outputlog(share_log_level levelog, const char *szFormat, ...);
#endif





