#pragma once
#include "afxwin.h"
#include "client_cameraControl.h"

// DlgCam �Ի���

#define LeftToRight 0
#define RightToLeft 1

#define PreSet_OK 1
#define PreSet_NO 0

class DlgCam : public CDialog
{
	DECLARE_DYNAMIC(DlgCam)

public:
	DlgCam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgCam();

// �Ի�������
	enum { IDD = IDD_CAMCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	UINT m_uiHandle;//��̨������
	CString		m_strHost;
	UINT		m_uiPort;
	CString		m_strPword;
	CString		m_strUname;
	CRect rc;
	int whichBtn = -1;



	//��̨�������
	PanAndTiltCameraControl m_CameraControl;
	int m_get_panPosit;
	int m_get_tiltPosit;
	int m_leftPreset, m_rightPreset;
	int left = 0, right = 0;
	int numPos;//Ԥ��λ����
	int preset_status=-1;
	CString str;
	int speed;
	CComboBox m_comboSpeed;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btnUp;
	CButton m_btnLeft;
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonLeftPreset();
	afx_msg void OnBnClickedButtonRightPreset();

	void setNumOfPreset(int num);
	void autoPreSet(int a, int b ,int direct);
	CStatic m_grpBoxCam;
	CStatic m_txtPreset;
};
