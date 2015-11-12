#ifndef	__RH_STREAM_H__
#define	__RH_STREAM_H__
#include "share_socket.h"
#include "pthread.h"
#define IP_LEN 16
#define	RH_FH_LEN	sizeof(RH_FRAMEHEAD_t)
#define STREAM_CONNECT_TIMEOUT 2

typedef enum
{

	START_STREAM = 0,
	STOP_STREAM,
};
typedef unsigned short int	uint16_t;
typedef signed char		int8_t;
typedef struct RH_FRAMEHEAD_ {
	unsigned char  check_start[4]; /*,标示符*/
	unsigned int ID;							//=mmioFOURCC('4','D','S','P');
	unsigned int nChannel;
	unsigned int nPTimeTick;    					//time
	unsigned int nDTimeTick;    					//time
	unsigned int nFrameLength; 					//length of frame
	unsigned int nDataCodec;   					//encode type
	unsigned int s_blue;						// 1 blue， 0 no
	//video:mmioFOURCC('H','2','6','4');
	//audio:mmioFOURCC('A','D','T','S');
	union {
		unsigned int nFrameRate;   				//video:framerate
		unsigned int nSamplerate;   				//aduio:samplerate (default:44100)
	};
	union {
		unsigned int nWidth;       					//video:width
		unsigned int nAudioChannel;					//audio:channel (default:2)
	};
	union {
		unsigned int nHight;       					//video:height
		unsigned int nSamplebit;					//audio:samplebit (default:16)
	};
	union {
		unsigned int nColors;      					//video:colors
		unsigned int nBandwidth;				//audio:bandwidth (default:64000)
	};
	union {
		unsigned int nIframe;					//video: I frame
		unsigned int nReserve;					//audio:  reserve
	};

	unsigned int nPacketNumber; 					//packages serial number
	unsigned int nOthers;      						//reserve
	unsigned char  check_end[4]; /*,标示符*/
} RH_FRAMEHEAD_t;
typedef struct RH_tcp_stream_recv_cond_ {
	int8_t	ip[IP_LEN]; //接收端ip地址
	uint16_t	port;        //接收端端口
	int          timeout;// timeout -1 不需要设置
	void		*arg;
	void * (*getEmptyBuf)(void *arg, void **ppEmptyBufInfo, int data_len);
	int(*putStreaminfo)(RH_FRAMEHEAD_t *fh, void *pEmptyBufInfo, void *arg, void *data);
	int(*exception_msg)(int msg, void* prm);
	int runstatus;
} RH_tcp_stream_recv_cond_t;



void *Stream_init_client(RH_tcp_stream_recv_cond_t *src);
#endif	//__RH_STREAM_H__