/**************************************************************************************************
*													注意事项
*
*					1. Nonblock 设置超时无效  Block 设置超时有效
*					2. Select  与阻塞，非阻塞没有任何关系。
*																																					write by zhengyb
****************************************************************************************************/

#include"stdafx.h"
#include "share_socket.h"
#include "winsock.h"
static int RH_GetPrivateError()
{
	int RetError = WSAGetLastError();

	if (RetError != 0) 
	{
		RetError = (RetError ^ -1) + 1;
	}
	else 
	{
		RetError = -11;
	}
	return RetError;
}
int RH_TcpSndBlockFd(int Fd, char *SndBuf, int *SndLen)
{
	if (Fd < 0 || NULL == SndBuf || SndLen == NULL) {
		//nslog(NS_ERROR, "<RH_TcpSndBlockFd IS ERROR> <FD : %d> <SndBuf :%p> <SndLen :%p>\n",
			//Fd, SndBuf, SndLen);
		return RHRETFAIL;
	}

	if (*SndLen < 0 || *SndLen == 0) {
		//nslog(NS_ERROR, "<RH_TcpSndBlockFd IS ERROR> <FD : %d> <SndBuf :%p> <SndLen :%d>\n",
		//	Fd, SndBuf, *SndLen);
	}

	int SndTotalLen = 0;
	int SndBytes	= 0;
	int SndTempLen  = *SndLen;
	*SndLen = 0;

	while(SndTotalLen < SndTempLen) {

		SndBytes = send(Fd , SndBuf + SndTotalLen, SndTempLen - SndTotalLen, 0);

		if(SndBytes < 0) {
			//nslog(NS_ERROR, "<RH_TcpSndBlockFd IS ERROR> <Snd> <ERROR_S :%s> <ERROR_D :%d> <FD : %d>\n",
			//	strerror(errno), errno, Fd);
			return RH_GetPrivateError();
		} else {
			SndTotalLen += SndBytes;
			*SndLen = SndTotalLen;
		}
	}

	return RHRETSUCCESS;

}
int RH_TcpRcvBlockFd(int Fd, char *RcvBuf, int RcvLen, int *readlen)
{
	if (Fd < 0 || NULL == RcvBuf || readlen == NULL) {
		//nslog(NS_ERROR, "<RH_TcpRcvBlockFd IS ERROR> <FD : %d> <RcvBuf :%p> <RcvLen :%p>\n", Fd, RcvBuf, readlen);
		return RHRETFAIL;
	}

	if (RcvLen < 0 || RcvLen == 0) {
		//nslog(NS_ERROR, "<RH_TcpRcvBlockFd IS ERROR> <FD : %d> <RcvBuf :%p> <RcvLen :%d>\n", Fd, RcvBuf, RcvLen);
		return RHRETFAIL;
	}

	int RcvTotalLen = 0;
	int RcvBytes = 0;
	int timeout_cnt = 0;
	char *TempBuf = NULL;
	int RcvTempLen = 0;
	*readlen = 0;

	while (RcvTotalLen < RcvLen) {

		TempBuf = RcvBuf + RcvTotalLen;
		RcvTempLen = RcvLen - RcvTotalLen;
		RcvBytes = recv(Fd, TempBuf, RcvTempLen, 0);

		//printf( "<RH_TcpSndBlockFd IS Begin> <RcvAdress : %p> <RcvTempLen :%d><RcvLen :%d><readlen :%d><Rcv> <FD : %d>\n",
		//	 TempBuf,RcvTempLen, RcvLen, *readlen, Fd);

		if (RcvBytes < 0) {
			if (11 == errno) {
				timeout_cnt++;

				if (timeout_cnt < 5) {
					//nslog(NS_ERROR, "recv failed timeout_cnt=%d errno=%d-<%s>\n", timeout_cnt, errno, strerror(errno));
					continue;
				}
			}
			//nslog(NS_ERROR, "<RH_TcpRcvBlockFd IS ERROR> <RcvAdress : %p> <RcvTempLen :%d> <RcvLen :%d><readlen :%d><Rcv> <ERROR_S :%s> <ERROR_D :%d> <FD : %d>\n",
				//TempBuf, RcvTempLen, RcvLen, *readlen, strerror(errno), errno, Fd);
			return RH_GetPrivateError();
		}
		else if (RcvBytes == 0) {
			//nslog(NS_ERROR, "ret =%d ---\n", RcvBytes);
			return -1;
		}
		else {
			RcvTotalLen += RcvBytes;
			*readlen = RcvTotalLen;
		}
	}

	return RHRETSUCCESS;
}
static int  i = 0;
int32_t RH_Socket(char *file, char *func, int32_t domain, int32_t type, int32_t protocol)
{
	int fd = -1;
	fd = socket(domain, type, protocol);

	if (1) {
		if (i == 0xffffffff) {
			i = 0;
		}

		i++;
		//nslog(NS_DEBUG, "[%s:%s] create socke[%d],close num=[%d]open num=[%d]\n", file, func, fd, m, i);
	}

	return fd;
}
static int m = 0;
int32_t RH_Close(char *file, char *func, int32_t fd)
{
	if (fd < 0) {
		//nslog(NS_ERROR, "[%s:%s] ,fd [%d] is invaild\n", file, func, fd);
		return 0;
	}

	if (1) {
		if (m == 0xffffffff) {
			m = 0;
		}

		m++;
		//nslog(NS_DEBUG, "[%s:%s] close socke[%d],close num=[%d]open num=[%d]\n", file, func, fd, m, i);
	}

	closesocket(fd);
	fd = -1;
	return errno;
}
int RH_CreateTcpNoBindFd(void)
{
	int Fd = -1;
	Fd = RH_Socket(__FILE__, (char *)__FUNCTIONW__, AF_INET, SOCK_STREAM, 0);

	if (Fd < 0) {
		//nslog(NS_ERROR, "<RH_CreateTcpNoBindFd IS ERROR> <FD : %d> <ERROR_S :%s> <ERROR_D: %d>\n", Fd, strerror(errno), errno);
		return RH_GetPrivateError();
	}

	return Fd;
}
int RH_ConnetBlockFd(int Fd, int ServPort, char *ServIp) //,int Timeout)
{
	if (Fd < 0 || ServPort < 1 || ServIp == NULL) {
		//nslog(NS_ERROR, "<RH_ConnetNonblock IS ERROR>  <FD : %d> <ServPort: %d> <ServIp :%s>\n", Fd, ServPort, ServIp);
		return RHRETFAIL;
	}

#if 0

	if (RH_SetSndTimeoutFd(Fd, Timeout, 0) < 0) {
		printf("<RH_ConnetNonblock IS ERROR> <RH_SetSndTimeoutFd> <FD : %d> <ServPort: %d> <ServIp :%s> <Timeout :%d>\n",
			Fd, ServPort, ServIp, Timeout);
		return RHRETFAIL;
	}

#endif
	struct sockaddr_in serv_addr = { 0 };
	//bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(ServPort);
	//inet_aton((const char *)ServIp, (struct in_addr *)&serv_addr.sin_addr);
	serv_addr.sin_addr.s_addr = inet_addr(ServIp);
	if (connect(Fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) {
		//nslog(NS_ERROR, "<RH_ConnetNonblock IS ERROR> <connect>  <ERROR_S :%s> <ERROR_D :%d> <FD : %d> <ServPort: %d> <ServIp :%s>\n",
		//	strerror(errno), errno, Fd, ServPort, ServIp);
		return RH_GetPrivateError();
	}

	return RHRETSUCCESS;
}
int RH_SetRcvTimeoutFd(int Fd, int TimeoutSec, int TimeoutUsec)
{
	if (Fd < 0 || TimeoutSec < 0 || TimeoutUsec < 0) {
		//nslog(NS_ERROR, "<RH_SetRcvTimeoutFd IS ERROR>  <FD : %d>   <TimeoutSec :%d> <TimeoutUsec :%d>\n", Fd, TimeoutSec, TimeoutUsec);
		return RHRETFAIL;
	}

	struct timeval Time;

	Time.tv_sec = TimeoutSec;

	Time.tv_usec = TimeoutUsec;

	if (setsockopt(Fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&Time, sizeof(struct timeval)) < 0) {
		//nslog(NS_ERROR, "<RH_SetRcvTimeoutFd IS ERROR> <SO_RCVTIMEO> <FD : %d> <ERROR_S :%s> <ERROR_D :%d> <TimeoutSec :%d> <TimeoutUsec :%d>\n",
		//	Fd, strerror(errno), errno, TimeoutSec, TimeoutUsec);
		return RH_GetPrivateError();
	}

	return RHRETSUCCESS;
}