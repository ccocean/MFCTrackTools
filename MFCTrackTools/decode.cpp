#include "decode.h"


extern CListBox m_listErr;

static int cur_size;
static int ret;
static int got_picture;
static int y_size;

int H264_init(Tools_decoder_t* decoder)
{
	//avcodec_init();
	avcodec_register_all();

	/* find the h264 video decoder */
	decoder->pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!decoder->pCodec) {
		m_listErr.AddString(_T("codec not found!\r\n"));
		return -1;
	}

	decoder->pCodecCtx = avcodec_alloc_context3(decoder->pCodec);
	if (!decoder->pCodecCtx)
	{
		m_listErr.AddString(_T("Could not allocate video codec context\r\n"));
		return -1;
	}

	decoder->pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);
	if (!decoder->pCodecParserCtx){
		m_listErr.AddString(_T("Could not allocate video parser context\n"));
		return -1;
	}

	if (avcodec_open2(decoder->pCodecCtx,decoder->pCodec,NULL)<0)
	{
		m_listErr.AddString(_T("Could not open codec\r\n"));
		return -1;
	}

	decoder->pFrame = av_frame_alloc();
	if (decoder->pFrame==NULL)
	{
		return -1;
	}

	decoder->pFrameRGB = av_frame_alloc();
	if (decoder->pFrameRGB==NULL)
	{
		return -1;
	}
	av_init_packet(&decoder->packet);

	return 0;
}

int H264_To_RGB(unsigned char *inputbuffer, int frame_size, unsigned char *outputbuffer, int *outsize, Tools_decoder_t *decoder)
{
	int             decode_size;
	int             numBytes;
	int             av_result;
	uint8_t         *buffer = NULL;
	decoder->packet.data = inputbuffer;
	decoder->packet.size = frame_size;

	decoder->cur_ptr = inputbuffer;

	av_result = avcodec_decode_video2(decoder->pCodecCtx, decoder->pFrame, &decode_size, &decoder->packet);

	if (av_result < 0)
	{
		m_listErr.AddString(_T("Decode err.\r\n"));
		return -1;
	}

	
	numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, decoder->pCodecCtx->width, decoder->pCodecCtx->height, 1);
	buffer = (uint8_t *)malloc(numBytes*sizeof(uint8_t));

	//avpicture_fill((AVPicture*)decoder->pFrameRGB, buffer, AV_PIX_FMT_BGR24, decoder->pCodecCtx->width, decoder->pCodecCtx->height);
	av_image_fill_arrays(decoder->pFrameRGB->data, decoder->pFrameRGB->linesize, buffer, AV_PIX_FMT_BGR24, decoder->pCodecCtx->width, decoder->pCodecCtx->height, 1);

	decoder->img_convert_ctx = sws_getCachedContext(decoder->img_convert_ctx, decoder->pCodecCtx->width, decoder->pCodecCtx->height,
		decoder->pCodecCtx->pix_fmt, decoder->pCodecCtx->width, decoder->pCodecCtx->height, AV_PIX_FMT_BGR24,SWS_X,NULL,NULL,NULL);

	if (decoder->img_convert_ctx == NULL)
	{

		m_listErr.AddString(_T("can't init convert context!\n"));
		return -1;
	}

	decoder->pFrame->data[0] += decoder->pFrame->linesize[0] * (decoder->pCodecCtx->height - 1);
	decoder->pFrame->linesize[0] *= -1;
	decoder->pFrame->data[1] += decoder->pFrame->linesize[1] * (decoder->pCodecCtx->height / 2 - 1);;
	decoder->pFrame->linesize[1] *= -1;
	decoder->pFrame->data[2] += decoder->pFrame->linesize[2] * (decoder->pCodecCtx->height / 2 - 1);;
	decoder->pFrame->linesize[2] *= -1;

	sws_scale(decoder->img_convert_ctx, decoder->pFrame->data, decoder->pFrame->linesize,
		0, 0 - decoder->pCodecCtx->width, decoder->pFrameRGB->data, decoder->pFrameRGB->linesize);
	
	if (decode_size)
	{
		*outsize = decoder->pCodecCtx->width * decoder->pCodecCtx->height * 3;
		memcpy(outputbuffer, decoder->pFrameRGB->data[0], *outsize);
	}

	free(buffer);
	return 0;
}