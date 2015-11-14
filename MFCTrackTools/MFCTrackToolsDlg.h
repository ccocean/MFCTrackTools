
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
#include"connect.h"
#define WIDTH 480
#define HEIGHT 264
#define SKINNAME "\\skin\\xp_corona.ssk"
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

#define Mouse_LBDOWN 1
#define Mouse_LBUP 0
#define Mouse_DRAG 3

#define Frame_Width 480	    //960
#define Frame_Height 264	//540

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
	afx_msg void OnBnClickedbtnsavetch();
	afx_msg void OnBnClickedbtnsaveblk();
	afx_msg void OnBnClickedbtnset();
	afx_msg void OnBnClickedbtnsavepos();
	afx_msg void OnBnClickedbtnsavetrack();
	afx_msg void OnBnClickedbtnsavethreshold();
	afx_msg void OnBnClickedbtnapply();
	afx_msg void OnBnClickedbtndefault();

	DlgTch dlgTch;
	DlgStu dlgStu;
	int CurSel;//标签的标志
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
	//IplImage *frame;
	//IplImage *srcImg = cvCreateImage(cvSize(Frame_Width, Frame_Height), 8, 3);
	//CvPoint p1, p2;
	//CvPoint pt;
	//CvRect tch, blk;
	CString str;
	CString tmp;
	//CvvImage cimg;
	
	Track_CamPosSlide_t camPosSlide;
	TeaITRACK_Params params;

	CPoint p1, p2, pt;
	Tch_Rect_t tch, blk;
	int mouseStatus = 0;
	int mouseCnt = 0;
	int whichRect = 0;

	CDC *pDC ;
	HDC hdc ;
	CPen penY;
	CPen penG;
	CPen *pOldPen;

	double fps;
	int vfps;
	
	CListBox m_listErr;
	CButton m_btnSaveTrack;
	
	CStatic m_txtStand;
	CStatic m_txtTargetArea;
	CStatic m_txtOutSide;
	CEdit m_editStand;
	CEdit m_editTargetArea;
	CEdit m_editOutSide;
	CStatic m_grpBoxThreshold;
	CButton m_btnSaveThreshold;
	
	CButton m_btnApply;
	
	CButton m_btnDefault;
	
	
	CButton m_btnUp;
	CButton m_btnLeft;
	CButton m_btnDown;
	CButton m_btnRight;
	CStatic m_grpBoxCam;
	CStatic m_txtTune;
	CStatic m_txtFocus;
	CButton m_btnOrigin;
	CButton m_btnTuneAsd;
	CButton m_btnTuneDsd;
	CButton m_btnTuneStop;
	CButton m_btnFocusAsd;
	CButton m_btnFocusStop;
	CButton m_btnFocusDsd;
	BITMAPINFO m_bmphdr;
private:
	char m_trackIp[16];
	BOOL initProgramControl();
	BOOL initNetCommuntication();
	void trackdraw();
	void drawRectangle(CPoint a, CPoint b);
public:
	int video_display(Decode_Info_t *pInfo);
	afx_msg void OnBnClickedbtnup();
	CTabCtrl m_tabTrack;
public:
	int ctrlClient_init_trackCommuntication();

	int ctrlClient_process_trackMsg(Communtication_Head_t *head, void *msg, Commutication_Handle_t handle);
	int ctrlClient_process_trackHeart(char *buff);
	BOOL ctrlClient_init_Stream();

	Commutication_Handle_t m_track_clientHandle;
	afx_msg void OnTcnSelchangetabtrack(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static int ctrlClient_init_trackCommunticationEx(void*param, Net_Info * pnetInfo);
	trackconnect m_connectDialog;
	char m_pExeDir[MAX_PATH];
	CString m_strSkin;
};
