// GlobalContrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "GlobalContrl.h"
#include "afxdialogex.h"


// GlobalContrl 对话框

IMPLEMENT_DYNAMIC(GlobalContrl, CDialog)

GlobalContrl::GlobalContrl(CWnd* pParent /*=NULL*/)
	: CDialog(GlobalContrl::IDD, pParent)
	, m_edt_timeTch(0)
	, m_edt_timeStu(0)
	, m_edt_timeBlk(0)
	, m_edt_timeVGA(0)
{
	ctrl_params.mut_pic_flag = TRUE;
}

GlobalContrl::~GlobalContrl()
{
}

void GlobalContrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_MULTIPLE, m_chk_multiple);
	DDX_Text(pDX, IDC_EDIT_TIME_TCH, m_edt_timeTch);
	DDX_Text(pDX, IDC_EDIT_TIME_STU, m_edt_timeStu);
	DDX_Text(pDX, IDC_EDIT_TIME_BLK, m_edt_timeBlk);
	DDX_Text(pDX, IDC_EDIT_TIME_VGA, m_edt_timeVGA);
}


BEGIN_MESSAGE_MAP(GlobalContrl, CDialog)
	ON_BN_CLICKED(IDC_CHK_MULTIPLE, &GlobalContrl::OnBnClickedChkMultiple)
	ON_BN_CLICKED(IDC_BTN_CTRL_APPLY, &GlobalContrl::OnBnClickedBtnCtrlApply)
END_MESSAGE_MAP()


// GlobalContrl 消息处理程序


void GlobalContrl::OnBnClickedChkMultiple()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_chk_multiple.GetCheck())
	{
		m_chk_multiple.SetWindowText(_T("多画面开启"));
		ctrl_params.mut_pic_flag = TRUE;
	}
	else
	{
		m_chk_multiple.SetWindowText(_T("多画面关闭"));
		ctrl_params.mut_pic_flag = FALSE;
	}
}


void GlobalContrl::OnBnClickedBtnCtrlApply()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_edt_timeBlk<=0)
	{
		MessageBox(_T("板书时间错误！"));
		return;
	}
	if (m_edt_timeStu <= 0)
	{
		MessageBox(_T("学生时间错误！"));
		return;
	}
	if (m_edt_timeTch <= 0)
	{
		MessageBox(_T("教师时间错误！"));
		return;
	}
	if (m_edt_timeVGA <= 0)
	{
		MessageBox(_T("讲义时间错误！"));
		return;
	}

	ctrl_params.time.blb_time_min = m_edt_timeBlk;
	ctrl_params.time.tea_time_min = m_edt_timeTch;
	ctrl_params.time.stu_time_min = m_edt_timeStu;
	ctrl_params.time.ppt_time_min = m_edt_timeVGA;
}

BOOL GlobalContrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽   
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}
