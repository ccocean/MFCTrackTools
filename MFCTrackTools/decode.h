#ifndef _DECODE_H_
#define _DECODE_H_

#include "stdafx.h"

extern "C"
{
#include "../include/libavcodec/avcodec.h"  
#include "../include/libavformat/avformat.h"  
#include "../include/libavutil/avutil.h"  
#include "../include/libswscale/swscale.h" 
#include "../include/libavutil/imgutils.h"
#include "../include/libavutil/pixfmt.h"
}

typedef struct _decoder 
{
	AVCodec         *pCodec = NULL;
	AVCodecContext  *pCodecCtx = NULL;
	AVCodecParserContext *pCodecParserCtx = NULL;

	uint8_t			*cur_ptr;
	SwsContext      *img_convert_ctx = NULL;
	AVFrame         *pFrame = NULL;
	AVFrame         *pFrameRGB = NULL;
	AVPacket        packet;
}Tools_decoder_t;

int H264_init(Tools_decoder_t* decoder);

#endif