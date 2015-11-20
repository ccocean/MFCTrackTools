#pragma once
#include "afxwin.h"
#include "./hi_type.h"
#include "./hi_net_dev_errors.h"
#include "./hi_net_dev_msg.h"
#include "./hi_net_dev_sdk.h"

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

	CComboBox m_comboSpeed;
	afx_msg void OnBnClickedButtonUp();
};
