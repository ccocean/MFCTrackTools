// DlgStu.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgStu.h"
#include "afxdialogex.h"
#include "stuTrack_settings_parameter.h"


// DlgStu 对话框

IMPLEMENT_DYNAMIC(DlgStu, CDialog)

DlgStu::DlgStu(CWnd* pParent /*=NULL*/)
	: CDialog(DlgStu::IDD, pParent)
{

}

DlgStu::~DlgStu()
{
}

void DlgStu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_LEFTUP_POS, m_edtLfetUpPos);
	DDX_Control(pDX, IDC_EDT_LEFTDN_POS, m_edtLeftDnPos);
	DDX_Control(pDX, IDC_EDT_RIGHTUP_POS, m_edtRightUpPos);
	DDX_Control(pDX, IDC_EDT_RIGHTDN_POS, m_edtRightDnPos);
	DDX_Control(pDX, IDC_EDT_LEFTUP_AGL, m_edtLeftUpAgl);
	DDX_Control(pDX, IDC_EDT_LEFTDN_AGL, m_edtLeftDnAgl);
	DDX_Control(pDX, IDC_EDT_RIGHTUP_AGL, m_edtRightUpAgl);
	DDX_Control(pDX, IDC_EDT_RIGHTDN_AGL, m_edtRightDnAgl);
	DDX_Control(pDX, IDC_EDT_LEFTUP_WID, m_edtLeftUpWid);
	DDX_Control(pDX, IDC_EDT_LEFTDN_WID, m_edtLeftDnWid);
	DDX_Control(pDX, IDC_EDT_RIGHTUP_WID, m_edtRightUpWid);
	DDX_Control(pDX, IDC_EDT_RIGHTDN_WID, m_edtRightDnWid);
	DDX_Control(pDX, IDC_EDT_STAND_AGL, m_edtStandAgl);
	DDX_Control(pDX, IDC_EDT_STAND_FRM, m_edtStandFrm);
	DDX_Control(pDX, IDC_EDT_SIT_FRM, m_edtSitFrm);
	DDX_Control(pDX, IDC_EDT_MOVE_DEV, m_edtMoveDev);
	DDX_Control(pDX, IDC_EDT_MOVE_DLY, m_edtMoveDly);
}


BEGIN_MESSAGE_MAP(DlgStu, CDialog)
	ON_BN_CLICKED(IDC_BTNSTUAPPLY, &DlgStu::OnBnClickedBtnstuapply)
END_MESSAGE_MAP()


// DlgStu 消息处理程序

void DlgStu::getParameters()
{
	int ret = 0;
	ret = ctrlClient_get_stu_params(m_Connect_clientHandle);
	if (ret != 0)
	{
		OutputDebugString("获取学生参数失败！");
	}
	return;
}

int DlgStu::checkParameters()
{
	CString standAgl, standFrm, sitFrm, moveDev, moveDly;
	int _standAgl, _standFrm, _sitFrm, _moveDev, _moveDly;
	if (stu_params.stuTrack_vertex[0].x <= 0 || stu_params.stuTrack_vertex[0].y <= 0)
	{
		MessageBox("顶点位置错误！");
		return -1;
	}
	if (stu_params.stuTrack_direct_standard[0] <= 0 || stu_params.stuTrack_direct_standard[1] <= 0
		|| stu_params.stuTrack_direct_standard[2] <= 0 || stu_params.stuTrack_direct_standard[3] <= 0)
	{
		MessageBox("顶点角度错误！");
		return -1;
	}
	if (stu_params.stuTrack_stuWidth_standard[0] <= 0 || stu_params.stuTrack_stuWidth_standard[1] <= 0
		|| stu_params.stuTrack_stuWidth_standard[2] <= 0 || stu_params.stuTrack_stuWidth_standard[3] <= 0)
	{
		MessageBox("顶点宽度错误！");
		return -1;
	}
	m_edtStandAgl.GetWindowText(standAgl);
	m_edtStandFrm.GetWindowText(standFrm);
	m_edtSitFrm.GetWindowText(sitFrm);
	m_edtMoveDev.GetWindowText(moveDev);
	m_edtMoveDly.GetWindowText(moveDly);
	if (standAgl.IsEmpty()||standFrm.IsEmpty()||sitFrm.IsEmpty()||moveDev.IsEmpty()||moveDly.IsEmpty())
	{
		MessageBox("数据不能为空！");
		return -1;
	}
	else
	{
		_standAgl = _ttoi(standAgl);
		_standFrm = _ttoi(standFrm);
		_sitFrm = _ttoi(sitFrm);
		_moveDev = _ttoi(moveDev);
		_moveDly = _ttoi(moveDly);
		if (_standAgl<0)
		{
			MessageBox("起立角度偏移数据错误！");
			return -1;
		}
		if (_standFrm<=0)
		{
			MessageBox("起立阈值数据错误！");
			return -1;
		}
		if (_sitFrm <= 0)
		{
			MessageBox("坐下阈值数据错误！");
			return -1;
		}
		if (_moveDev <= 0)
		{
			MessageBox("移动偏离数据错误！");
			return -1;
		}
		if (_moveDly <= 0)
		{
			MessageBox("移动延时数据错误！");
			return -1;
		}
		stu_params.stuTrack_direct_range = _standAgl;
		stu_params.stuTrack_standCount_threshold = _standFrm;
		stu_params.stuTrack_sitdownCount_threshold = _sitFrm;
		stu_params.stuTrack_move_threshold = _moveDev;
		stu_params.stuTrack_moveDelayed_threshold = _moveDly;
	}
	return 0;
}
void DlgStu::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}
void DlgStu::OnBnClickedBtnstuapply()
{
	// TODO:  在此添加控件通知处理程序代码
	if (checkParameters()==0)
	{
		ctrlClient_set_stu_params(&stu_params, m_Connect_clientHandle);
	}
}


BOOL DlgStu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽    
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}
