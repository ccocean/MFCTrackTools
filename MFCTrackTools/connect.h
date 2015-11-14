#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// connect dialog
typedef struct
{
	char Ip[16];

}Net_Info;
typedef int(*initConnectNet)(void*, Net_Info *);
class trackconnect : public CDialogEx
{
	DECLARE_DYNAMIC(trackconnect)

public:
	trackconnect(CWnd* pParent = NULL);   // standard constructor
	virtual ~trackconnect();

// Dialog Data
	enum { IDD = IDD_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonConnect();
	CIPAddressCtrl m_ip;
	CString m_strIp;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void getConnectip(char * ip);
	int setConectfunCall(initConnectNet fun, void * param);
	initConnectNet m_initNetFun;
	void * m_DialogParam;
	Net_Info m_connectInfo;
	afx_msg void OnClose();
	virtual void OnOK();
	void autoclose();
};