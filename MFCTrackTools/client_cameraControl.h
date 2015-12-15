#include "stdafx.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "winsock.h"
// #pragma comment(lib, "ws2_32.lib")

#include "pthread.h"
///UDP控制定义
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
	函数: <startControl>
	功能: 开始控制摄像机
	参数: addr :相机ip地址；port：相机端口
	返回值:非0-成功   
	==============================================================================*/
	int startControl(const char addr[], const int port);

	/*==============================================================================
	函数: <stopControl>
	功能: 停止控制相机
	参数: 无
	返回值:无
	==============================================================================*/
	void stopControl();

	/*==============================================================================
	函数: <setMoveSpeed>
	功能: 设置移动速度
	参数: speed_pan：水平速度 0-24； speed_tilt：垂直速度 0-20
	返回值:无
	==============================================================================*/
	void setMoveSpeed(int speed_pan, int speed_tilt);

	/*==============================================================================
	函数: <move>
	功能: 移动到
	参数: posittion_pan：水平坐标； posittion_tilt：垂直坐标； flag：相对位置/绝对位置
	返回值: true为发送命令成功
	==============================================================================*/
	BOOL move(int posittion_pan, int posittion_tilt, int flag = TRUE);

	/*==============================================================================
	函数: <setZoom>
	功能: 设置变焦
	参数: zoomPosition：变焦距离
	返回值: true为发送命令成功
	==============================================================================*/
	BOOL setZoom(int zoomPosition);

	/*==============================================================================
	函数: <home>
	功能: Go back to home!
	参数: 无
	返回值: true为发送命令成功
	==============================================================================*/
	BOOL home();

	/*==============================================================================
	函数: <keepInstruct>
	功能: 相机的指令，包括上、下、左、右，变焦，聚焦
	参数: type：指令类型
	返回值: true为发送命令成功
	==============================================================================*/
	BOOL keepInstruct(int type, void* otherData = NULL);

	/*==============================================================================
	函数: <preset>
	功能: 相机预置位的指令，包括设置，移动，清空
	参数: type：指令类型，id：预置位编号
	返回值: true为发送命令成功
	==============================================================================*/
	BOOL preset(int type, int id);

	/*==============================================================================
	函数: <getPosit>
	功能: 获取相机当前的坐标
	参数: posit_pan：水平坐标，posit_tilt：竖直坐标，waitMillisecond：等待时间
	返回值: true为发送命令成功
	==============================================================================*/
	BOOL getPosit(int *posit_pan, int *posit_tilt, int waitMillisecond);

	/*==============================================================================
	函数: <getZoom>
	功能: 获取相机当前变焦
	参数: zoomValue：变焦值
	返回值: true为发送命令成功
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

	int move_speed_pan;		//水平范围0-18
	int move_speed_tilt;	//倾斜范围0-14
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