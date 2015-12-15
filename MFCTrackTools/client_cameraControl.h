#include "stdafx.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "winsock.h"
// #pragma comment(lib, "ws2_32.lib")

#include "pthread.h"
///UDP���ƶ���
#define PANandTILT_CTRL_PTZ_STOP            0x1000
#define PANandTILT_CTRL_PTZ_UP              0x1001
#define PANandTILT_CTRL_PTZ_DOWN            0x1002
#define PANandTILT_CTRL_PTZ_LEFT            0x1003
#define PANandTILT_CTRL_PTZ_RIGHT           0x1004

#define PANandTILT_CTRL_PTZ_ZOOMIN          0x2001
#define PANandTILT_CTRL_PTZ_ZOOMOUT         0x2002
#define PANandTILT_CTRL_PTZ_ZOOMSTOP        0x2003
#define PANandTILT_CTRL_PTZ_FOCUSIN         0x3001
#define PANandTILT_CTRL_PTZ_FOCUSOUT        0x3002
#define PANandTILT_CTRL_PTZ_FOCUSSTOP       0x3003
#define PANandTILT_CTRL_PTZ_FOCUSAUTO		0x3044
#define PANandTILT_CTRL_PTZ_FOCUSMANUAL		0x3045

#define PANandTILT_CTRL_PTZ_GOTO_PRESET     0x4001
#define PANandTILT_CTRL_PTZ_SET_PRESET      0x4002
#define PANandTILT_CTRL_PTZ_CLE_PRESET      0x4003

#define PANandTILT_CTRL_PTZ_HOME            0x5001

#define PANandTILT_CTRL_PTZ_MASK			0XF000

#define ACCEPT_BUFFER_SIZE 256

#define INFO_ZOOM_FOCUS 7
#define INFO_PAN_TILT 11

class PanAndTiltCameraControl
{
public:
	PanAndTiltCameraControl();
	~PanAndTiltCameraControl();
	/*==============================================================================
	����: <startControl>
	����: ��ʼ���������
	����: addr :���ip��ַ��port������˿�
	����ֵ:��0-�ɹ�   
	==============================================================================*/
	int startControl(const char addr[], const int port);

	/*==============================================================================
	����: <stopControl>
	����: ֹͣ�������
	����: ��
	����ֵ:��
	==============================================================================*/
	void stopControl();

	/*==============================================================================
	����: <setMoveSpeed>
	����: �����ƶ��ٶ�
	����: speed_pan��ˮƽ�ٶ� 0-24�� speed_tilt����ֱ�ٶ� 0-20
	����ֵ:��
	==============================================================================*/
	void setMoveSpeed(int speed_pan, int speed_tilt);

	/*==============================================================================
	����: <move>
	����: �ƶ���
	����: posittion_pan��ˮƽ���ꣻ posittion_tilt����ֱ���ꣻ flag�����λ��/����λ��
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL move(int posittion_pan, int posittion_tilt, int flag = TRUE);

	/*==============================================================================
	����: <setZoom>
	����: ���ñ佹
	����: zoomPosition���佹����
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL setZoom(int zoomPosition);

	/*==============================================================================
	����: <home>
	����: Go back to home!
	����: ��
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL home();

	/*==============================================================================
	����: <keepInstruct>
	����: �����ָ������ϡ��¡����ң��佹���۽�
	����: type��ָ������
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL keepInstruct(int type, void* otherData = NULL);

	/*==============================================================================
	����: <preset>
	����: ���Ԥ��λ��ָ��������ã��ƶ������
	����: type��ָ�����ͣ�id��Ԥ��λ���
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL preset(int type, int id);

	/*==============================================================================
	����: <getPosit>
	����: ��ȡ�����ǰ������
	����: posit_pan��ˮƽ���꣬posit_tilt����ֱ���꣬waitMillisecond���ȴ�ʱ��
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL getPosit(int *posit_pan, int *posit_tilt, int waitMillisecond);

	/*==============================================================================
	����: <getZoom>
	����: ��ȡ�����ǰ�佹
	����: zoomValue���佹ֵ
	����ֵ: trueΪ��������ɹ�
	==============================================================================*/
	BOOL getZoom(int *zoomValue, int waitMillisecond);

	BOOL getRun_Status();
	int recv_CameraInfo(char* buffer);
	HANDLE m_hHandle1;
	HANDLE m_hHandle2;
	void set_CameraInfo_panTilt(int posit_pan, int posit_tilt);
	void set_CameraInfo_zoom(int zoomValue);
	BOOL getStart_Status();

protected:

private:
	BOOL m_flag_start;

	struct sockaddr_in m_addr;
	char m_buffer[ACCEPT_BUFFER_SIZE];
	int m_addr_len;
	int m_send_socket;

	int move_speed_pan;		//ˮƽ��Χ0-18
	int move_speed_tilt;	//��б��Χ0-14
	int send_net_cmd(char *cmd, int len);

	BOOL keepMove(int type);
	BOOL keepZoom(int type);
	BOOL keepFocus(int type);

	BOOL m_thread_run_flag;
	pthread_t heart_tid;
	int m_posit_pan;
	int m_posit_tilt;
	int m_zoomValue;

	
};