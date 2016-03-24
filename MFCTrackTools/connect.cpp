// connect.cpp : implementation file
//

#include "stdafx.h"
#include "TrackTools.h"
#include "connect.h"
#include "afxdialogex.h"


// connect dialog

IMPLEMENT_DYNAMIC(trackconnect, CDialogEx)

trackconnect::trackconnect(CWnd* pParent /*=NULL*/)
: CDialogEx(trackconnect::IDD, pParent)
{
	m_initNetFun = NULL;
	m_DialogParam = NULL;
	
	//m_ip.SetWindowText(_T("192.168.0.0"));
}

trackconnect::~trackconnect()
{
}

void trackconnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_TRACK, m_ip);
	DDX_Control(pDX, IDC_EDIT_USER, m_username);
	DDX_Control(pDX, IDC_EDIT_PASSWD, m_passwd);

	
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
	char *fileName = "loginInfo.txt";
	//CFile myFile;
	CFileException fileException;
	CStdioFile myFile;

	CString usrename, passwd;
	BYTE nf1, nf2, nf3, nf4;
	m_ip.GetAddress(nf1, nf2, nf3, nf4);
	m_strIp.Format("%d.%d.%d.%d", nf1, nf2, nf3, nf4);//这里的nf得到的值是IP值了.
	strcpy(m_connectInfo.Ip, m_strIp.GetString());
	m_username.GetWindowText(usrename);
	m_passwd.GetWindowText(passwd);
	if (strcmp(m_strIp.GetString(), "0.0.0.0") == 0)
	{
		MessageBox("IP为空");
		return;
	}
	if (strcmp(usrename.GetString(), "") == 0)
	{
		MessageBox("用户名为空");
		return;
	}
	memcpy(m_connectInfo.passwd, passwd.GetString(), sizeof(m_connectInfo.passwd));
	memcpy(m_connectInfo.username, usrename.GetString(), sizeof(m_connectInfo.username));
	if (m_initNetFun)
	{
		m_initNetFun(m_DialogParam, &m_connectInfo);
	}

	BOOL openResult = myFile.Open(fileName, CFile::modeCreate | CFile::modeReadWrite);
	if (!openResult)
	{
		MessageBox(_T("打开文件错误！"));
	}
	else
	{
		myFile.SeekToBegin();
		myFile.WriteString(usrename);
		myFile.Write(("\r\n"), 2);
		myFile.WriteString(m_strIp);
		myFile.Write(("\r\n"), 2);
		myFile.Flush();
		myFile.Close();
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


BOOL trackconnect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	char *fileName = "loginInfo.txt";
	CStdioFile myFile;
	BOOL openResult = myFile.Open(fileName, CFile::modeRead);
	int cnt = 0;
	int len;
	if (!openResult)
	{
		openResult = myFile.Open(fileName, CFile::modeCreate);
		if (!openResult)
		{
			MessageBox(_T("创建文件错误！"));
		}
	}
	else
	{
		while (myFile.ReadString(str))
		{
			len = str.GetLength();
			str.Delete(len - 1, 1);
			if (cnt == 0)
			{
				m_username.SetWindowText(_T(str));
			}
			if (cnt == 1)
			{
				m_ip.SetWindowText(_T(str));
			}
			cnt++;
		}
	}
	myFile.Close();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
