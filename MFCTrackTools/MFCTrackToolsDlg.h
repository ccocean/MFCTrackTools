
// MFCTrackToolsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
//#include "CvvImage.h"

#include<string>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "avcodec.h"
#include "track_client_commintication.h"
#include "afxcmn.h"
#include "DlgTch.h"
#include "DlgStu.h"
#include "DlgCam.h"
#include "GlobalContrl.h"
#include"connect.h"
#include <cmath>
#include "decode.h"


#define SKINNAME "\\skin\\Longhorn.ssk"

#define Mouse_LBUP 0
#define Mouse_LBDOWN 1
#define Mouse_RBDOWN 2
#define Mouse_RBUP 3
#define Mouse_LBDRAG 4
#define Mouse_RBDRAG 5
#define Mouse_ADJUST_A 6
#define Mouse_ADJUST_B 7
#define Mouse_ADJUST_C 8
#define Mouse_ADJUST_D 9

#define CTRL_KEY_DOWN 1
#define CTRL_KEY_UP 0

#define Frame_Width WIDTH	    //960
#define Frame_Height HEIGHT	//540

#define TCH_TAB 0
#define STU_TAB 1
#define CTRL_TAB 2

#define DRAW_ANGLE 0
#define DRAW_WIDTH 1

#define PARAM_POSITION 0
#define PARAM_WIDTH 1

#define ITC_RADIAN_TO_ANGLE	57.29577951308 //弧度转角度参数
#define ITC_ANGLE_TO_RADIAN	0.017453292520

//using namespace cv;

//预置位滑块
typedef struct CamPositionSlide
{
	//index
	int center;
	int left;
	int right;
	int width;
}Track_CamPosSlide_t;



// CMFCTrackToolsDlg 对话框
class CMFCTrackToolsDlg : public CDialogEx
{
// 构造
public:
	CMFCTrackToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCTRACKTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnBnClickedbtnsavetch();
	//afx_msg void OnBnClickedbtnsaveblk();
	//afx_msg void OnBnClickedbtnset();
	//afx_msg void OnBnClickedbtnsavepos();
	//afx_msg void OnBnClickedbtnsavetrack();
	//afx_msg void OnBnClickedbtnsavethreshold();
	afx_msg void OnBnClickedbtnapply();
	afx_msg void OnBnClickedbtndefault();

	

	DlgTch dlgTch;
	DlgStu dlgStu;
	DlgCam dlgCam;
	GlobalContrl dlgCtrl;
	
	int CurSel;//标签的标志
	int pic_top = 0;
	// 用来播放视频的控件
	CStatic m_picOverall;//全景控件
	CStatic m_picFeature;//特写控件

	int g_flag = 0;
	int g_drawPS = 0;
	int g_drawTch = 0;
	int g_drawBlk = 0;
	int int_pos, int_slide;

	AVCodec *pCodec = NULL;

	CRect picRect;
	CString str;
	CString tmp;
	CString s;
	//CvvImage cimg;
	
	Track_CamPosSlide_t camPosSlide;
	TeaITRACK_Params params;
	Panoramic_Camera_Info m_cameraInfo;
	void* m_streamTeaHandle;
	void* m_streamStuHandle;
	CPoint centre_pt;
	CPoint p1 = { 0 }, p2 = { 0 }, pt = { 0 };//教师区域跟踪框的点
	CPoint p3 = { 0 }, p4 = { 0 };
	CPoint pl = { 0 }, pr = { 0 };
	CPoint pa = { 0 }, pb = { 0 }, pc = { 0 }, pd = { 0 };//学生跟踪参数四顶点的坐标
	CPoint pA = { 0 }, pB = { 0 }, pC = { 0 }, pD = { 0 };//学生跟踪参数四顶点的角度，delta值为pA-pa。
	CPoint ln1[2],ln2[2],ln3[2],ln4[2];//表示宽度的线段

	//鼠标控制变量
	Tch_Rect_t tch, blk;
	int mouseStatus = 0;
	int mouseCnt = 0;
	int whichRect = 0;
	int isKeyDown = CTRL_KEY_UP;
	//int isRightButton = 0;
	int whichVertex = -1;

	int rst = -1;
	int angle = 0;//角度
	int dist = 0;//宽度

	CDC *pDC ;
	CDC *pDC2;
	HDC hdc ;
	CPen penY;
	CPen penG;
	CPen penR;
	CPen penB;
	CPen penPink;
	CPen penPurple;
	CPen *pOldPen;//画笔
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)
		GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush;//画刷
	BITMAPINFO m_bmphdr;//bmp文件头
private:
	char m_trackIp[16];
	BOOL initProgramControl();
	BOOL initNetCommuntication();
	void trackdraw();
	void drawRectangle(CPoint a, CPoint b);
	void drawRectangle(CPoint a, CPoint b, CPoint c, CPoint d);
	void drawLine(CPoint a, CPoint b);
	void drawEndRect(CPoint center,int size);
	void drawLines(int flag);
	int minimalDistance(CPoint in);
	int getDistance(CPoint a, CPoint b);
	void updateLines();
	void updateParams(int flag);
	void loadParamsFromTch(TeaITRACK_Params* params);
	void loadParamsFromStu(StuITRACK_ClientParams_t* params);
	void initCamDlg(int cx, int cy, CRect rct);
public:
	int video_display(Decode_Info_t *pInfo);
	CTabCtrl m_tabTrack;
public:
	CIPAddressCtrl m_ipAddr;
	int ctrlClient_init_trackCommuntication();
	
	int ctrlClient_process_trackMsg(Communtication_Head_t *head, void *msg, Commutication_Handle_t handle);
	int ctrlClient_process_trackHeart(char *buff);
	BOOL ctrlClient_init_Stream();

	Commutication_Handle_t m_track_clientHandle;
	void* m_streamHandle;

	afx_msg void OnTcnSelchangetabtrack(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static int ctrlClient_init_trackCommunticationEx(void*param, Net_Info * pnetInfo);
	trackconnect m_connectDialog;
	char m_pExeDir[MAX_PATH];
	CString m_strSkin;
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};
