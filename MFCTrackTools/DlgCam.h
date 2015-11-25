#pragma once
#include "afxwin.h"
#include "client_cameraControl.h"

// DlgCam �Ի���

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
	CString str;

	CComboBox m_comboSpeed;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btnUp;
	CButton m_btnLeft;
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButton2();
	CEdit m_edit1;
};
