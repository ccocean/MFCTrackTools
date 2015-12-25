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
	DDX_Control(pDX, IDC_EDIT_TIME_TCH, m_edt_timeTch);
	DDX_Control(pDX, IDC_EDIT_TIME_STU, m_edt_timeStu);
	DDX_Control(pDX, IDC_EDIT_TIME_BLK, m_edt_timeBlk);
	DDX_Control(pDX, IDC_EDIT_TIME_VGA, m_edt_timeVGA);
	DDX_Control(pDX, IDC_EDIT_TIME_TCH, m_edt_timeTch);
	DDX_Control(pDX, IDC_EDIT_TIME_STU, m_edt_timeStu);
	DDX_Control(pDX, IDC_EDIT_TIME_BLK, m_edt_timeBlk);
	DDX_Control(pDX, IDC_EDIT_TIME_VGA, m_edt_timeVGA);
	DDX_Control(pDX, IDC_CHK_TRACKSTU, m_chk_trackStu);
	DDX_Control(pDX, IDC_CHK_STUOVERVIEW, m_chk_stuOverview);
}


BEGIN_MESSAGE_MAP(GlobalContrl, CDialog)
	ON_BN_CLICKED(IDC_CHK_MULTIPLE, &GlobalContrl::OnBnClickedChkMultiple)
	ON_BN_CLICKED(IDC_BTN_CTRL_APPLY, &GlobalContrl::OnBnClickedBtnCtrlApply)
	ON_BN_CLICKED(IDC_CHK_TRACKSTU, &GlobalContrl::OnBnClickedChkTrackstu)
	ON_BN_CLICKED(IDC_CHK_STUOVERVIEW, &GlobalContrl::OnBnClickedChkStuoverview)
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


int GlobalContrl::checkParameters()
{
	CString tch, stu, blk, vga;
	int _tch, _stu, _blk, _vga;
	m_edt_timeTch.GetWindowText(tch);
	m_edt_timeStu.GetWindowText(stu);
	m_edt_timeBlk.GetWindowText(blk);
	m_edt_timeVGA.GetWindowText(vga);
	if (tch.IsEmpty()||stu.IsEmpty()||blk.IsEmpty()||vga.IsEmpty())
	{
		MessageBox(_T("参数不能为空！"));
		return -1;
	}
	_tch = _ttoi(tch);
	_stu = _ttoi(stu);
	_blk = _ttoi(blk);
	_vga = _ttoi(vga);
	if (_blk <= 0)
	{
		MessageBox(_T("板书时间错误！"));
		return -1;
	}
	if (_stu <= 0)
	{
		MessageBox(_T("学生时间错误！"));
		return -1;
	}
	if (_tch <= 0)
	{
		MessageBox(_T("教师时间错误！"));
		return -1;
	}
	if (_vga <= 0)
	{
		MessageBox(_T("讲义时间错误！"));
		return -1;
	}

	ctrl_params.time.blb_time_min = _blk;
	ctrl_params.time.tea_time_min = _tch;
	ctrl_params.time.stu_time_min = _stu;
	ctrl_params.time.ppt_time_min = _vga;
	return 0;
}

void GlobalContrl::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}

void GlobalContrl::OnBnClickedBtnCtrlApply()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (checkParameters()==0)
	{
		if (m_Connect_clientHandle)
		{
			ctrlClient_set_policy_params(&ctrl_params, m_Connect_clientHandle);
		}
		else
		{
			MessageBox("未连接服务器！");
		}
	}
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


void GlobalContrl::OnBnClickedChkTrackstu()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_chk_trackStu.GetCheck())
	{
		m_chk_trackStu.SetWindowText(_T("学生跟踪开启"));
		ctrl_params.stu_track_flag = TRUE;
	}
	else
	{
		m_chk_trackStu.SetWindowText(_T("学生跟踪关闭"));
		ctrl_params.stu_track_flag = FALSE;
	}
}


void GlobalContrl::OnBnClickedChkStuoverview()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_chk_stuOverview.GetCheck())
	{
		m_chk_stuOverview.SetWindowText(_T("学生全景开启"));
		ctrl_params.stu_overview_flag = TRUE;
	}
	else
	{
		m_chk_stuOverview.SetWindowText(_T("学生全景关闭"));
		ctrl_params.stu_overview_flag = FALSE;
	}
}
