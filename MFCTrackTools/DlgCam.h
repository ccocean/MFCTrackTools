#pragma once


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
};
