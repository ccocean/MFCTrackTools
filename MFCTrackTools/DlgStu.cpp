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

void DlgStu::OnBnClickedBtnstuapply()
{
	// TODO:  在此添加控件通知处理程序代码
	stu_params = { 0 };
}
