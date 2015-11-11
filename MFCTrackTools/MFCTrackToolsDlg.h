
// MFCTrackToolsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
//#include "CvvImage.h"
#include "tch_params.h"
#include<string>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "avcodec.h"
#include "track_client_commintication.h"
#define WIDTH 480
#define HEIGHT 264
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
	// 跟踪区域设置框体	// 跟踪区域设置框体
	CStatic m_grpBoxTrack;
	CStatic m_txtX;
	CEdit m_editX;
	CStatic m_txtY;
	CEdit m_editY;
	CStatic m_txtTch;
	// Width
	CStatic m_txtW;
	// Edit Width
	CEdit m_editW;
	// Height
	CStatic m_txtH;
	// Edit Height
	CEdit m_editH;
	// 教师保存参数按钮
	CButton m_btnSaveTch;
	// 板书保存参数按钮
	CButton m_btnSaveBlk;
	CStatic m_txtT;
	CStatic m_txtB;
	// 用来显示教师跟踪区域的参数
	CStatic m_txtTchArg;
	// 用来显示板书跟踪区域的参数
	CStatic m_txtBlkArg;
	CStatic m_grpBoxPos;
	CStatic m_txtPos;
	CStatic m_txtSlide;
	// 用来填写预置位的总个数
	CEdit m_editPos;
	// 用来填写预置位滑框的宽度
	CEdit m_editSlide;
	// 用来设置参数观察效果的按钮
	CButton m_btnSet;
	// 保存当前已经设置好的预置位参数
	CButton m_btnSavePos;
	// 用来播放视频的控件
	CStatic m_picSrc;
	//CButton m_btnPlay;
	//afx_msg void OnBnClickedbtnplay();

	//CvCapture *g_video;
	//string g_videoPath = "video/teacher.mp4";
	int g_flag = 0;
	int g_drawPS = 0;
	int g_drawTch = 0;
	int g_drawBlk = 0;
	int int_pos, int_slide;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

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
	int mouseStatus=0;
	Track_CamPosSlide_t camPosSlide;
	TeaITRACK_Params params;

	CPoint p1, p2, pt;
	Tch_Rect_t tch, blk;

	CDC *pDC ;
	HDC hdc ;
	double fps;
	int vfps;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedbtnsavetch();
	afx_msg void OnBnClickedbtnsaveblk();
	afx_msg void OnBnClickedbtnset();
	afx_msg void OnBnClickedbtnsavepos();
	CListBox m_listErr;
	CButton m_btnSaveTrack;
	afx_msg void OnBnClickedbtnsavetrack();
	CStatic m_txtStand;
	CStatic m_txtTargetArea;
	CStatic m_txtOutSide;
	CEdit m_editStand;
	CEdit m_editTargetArea;
	CEdit m_editOutSide;
	CStatic m_grpBoxThreshold;
	CButton m_btnSaveThreshold;
	afx_msg void OnBnClickedbtnsavethreshold();
	CButton m_btnApply;
	afx_msg void OnBnClickedbtnapply();
	CButton m_btnDefault;
	afx_msg void OnBnClickedbtndefault();
	CStatic m_picCam;
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
	BOOL initProgramControl();
	BOOL initNetCommuntication();
public:
	int video_display(Decode_Info_t *pInfo);
	afx_msg void OnBnClickedbtnup();
};
