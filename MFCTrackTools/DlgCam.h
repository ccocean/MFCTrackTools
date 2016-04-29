#pragma once
#include "afxwin.h"
#include "client_cameraControl.h"
#include "MFCTrackToolsDlg.h"

// DlgCam 对话框

#define LeftToRight 0
#define RightToLeft 1

#define PreSet_OK 1
#define PreSet_NO 0

#define WM_USER_THREADEND WM_USER + 1

class DlgCam : public CDialog
{
	DECLARE_DYNAMIC(DlgCam)

public:
	DlgCam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgCam();

// 对话框数据
	enum { IDD = IDD_CAMCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
	bool m_process_flag;
	HANDLE m_hThread;
	DWORD m_threadID;

public:
	UINT m_uiHandle;//云台相机句柄
	CString		m_strHost;
	UINT		m_uiPort;
	CString		m_strPword;
	CString		m_strUname;
	CRect rc;
	int whichBtn = -1;

	CPoint m_calibPt[4];
	int m_zoom[4];

	
	//云台相机控制
	PanAndTiltCameraControl m_CameraControl_tch;
	PanAndTiltCameraControl m_CameraControl_stu;
	int m_get_panPosit;
	int m_get_tiltPosit;
	int m_get_zoomValue;
	int m_leftPreset, m_rightPreset;
	int left = 0, right = 0;
	int numPos;//预置位个数
	int preset_status=-1;
	CString str;
	int speed;
	CComboBox m_comboSpeed;

	void setConnectHandle(Commutication_Handle_t pConnect_clientHandle);
	Commutication_Handle_t m_Connect_clientHandle;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btnUp;
	CButton m_btnLeft;
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonLeftPreset();
	afx_msg void OnBnClickedButtonRightPreset();
	afx_msg LRESULT OnUserThreadend(WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI automaticPreset(LPVOID pParam);
	void setNumOfPreset(int num);
	void autoPreSet(int a, int b ,int direct);
	void enableButton();
	void disableButton();
	CStatic m_grpBoxCam;
	CStatic m_txtPreset;
	afx_msg void OnBnClickedButCalibration();
	afx_msg void OnBnClickedButAgaincalib();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CButton m_checkCam;
};
