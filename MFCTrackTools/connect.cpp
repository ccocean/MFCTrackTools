// connect.cpp : implementation file
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "connect.h"
#include "afxdialogex.h"


// connect dialog

IMPLEMENT_DYNAMIC(trackconnect, CDialogEx)

trackconnect::trackconnect(CWnd* pParent /*=NULL*/)
: CDialogEx(trackconnect::IDD, pParent)
{
	m_initNetFun = NULL;
	m_DialogParam = NULL;

}

trackconnect::~trackconnect()
{
}

void trackconnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_TRACK, m_ip);
}


BEGIN_MESSAGE_MAP(trackconnect, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &trackconnect::OnBnClickedButtonConnect)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// connect message handlers
void trackconnect::getConnectip(char * ip)
{
	if (ip)
	{
		strcpy(ip, m_strIp.GetString());
	}
	
}
int trackconnect::setConectfunCall(initConnectNet fun, void * param)
{
	if (fun)
	{
		m_initNetFun = fun;
		m_DialogParam = param;
	}
	return 0;
}
void trackconnect::OnBnClickedButtonConnect()
{
	BYTE nf1, nf2, nf3, nf4;
	
	m_ip.GetAddress(nf1, nf2, nf3, nf4);
	m_strIp.Format("%d.%d.%d.%d", nf1, nf2, nf3, nf4);//这里的nf得到的值是IP值了.
	strcpy(m_connectInfo.Ip, m_strIp.GetString());
	if (strcmp(m_strIp.GetString(), "0.0.0.0") == 0)
	{
		MessageBox("IP为空");
		return;
	}
	if (m_initNetFun)
	{
		m_initNetFun(m_DialogParam, &m_connectInfo);
	}
	// TODO: Add your control notification handler code here
}


BOOL trackconnect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽    
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void trackconnect::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}
void trackconnect::autoclose()
{
	OnClose();
}


void trackconnect::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}
