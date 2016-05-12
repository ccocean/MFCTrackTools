#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <afx.h>

#define WM_USER_LOGIN WM_USER + 2
// connect dialog
typedef struct
{
	char Ip[16];
	char username[32];
	char passwd[32];
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
	CEvent loginSuccess;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
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
	CEdit m_username;
	CString str;
	CEdit m_passwd;
	virtual BOOL OnInitDialog();
};
