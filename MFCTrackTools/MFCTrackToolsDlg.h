
// MFCTrackToolsDlg.h : ͷ�ļ�
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

//Ԥ��λ����
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

// CMFCTrackToolsDlg �Ի���
class CMFCTrackToolsDlg : public CDialogEx
{
// ����
public:
	CMFCTrackToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCTRACKTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	// �����������ÿ���	// �����������ÿ���
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
	// ��ʦ���������ť
	CButton m_btnSaveTch;
	// ���鱣�������ť
	CButton m_btnSaveBlk;
	CStatic m_txtT;
	CStatic m_txtB;
	// ������ʾ��ʦ��������Ĳ���
	CStatic m_txtTchArg;
	// ������ʾ�����������Ĳ���
	CStatic m_txtBlkArg;
	CStatic m_grpBoxPos;
	CStatic m_txtPos;
	CStatic m_txtSlide;
	// ������дԤ��λ���ܸ���
	CEdit m_editPos;
	// ������дԤ��λ����Ŀ��
	CEdit m_editSlide;
	// �������ò����۲�Ч���İ�ť
	CButton m_btnSet;
	// ���浱ǰ�Ѿ����úõ�Ԥ��λ����
	CButton m_btnSavePos;
	// ����������Ƶ�Ŀؼ�
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
