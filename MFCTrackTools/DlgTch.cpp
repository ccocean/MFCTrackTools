// DlgTch.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgTch.h"
#include "afxdialogex.h"


// DlgTch 对话框

IMPLEMENT_DYNAMIC(DlgTch, CDialog)

DlgTch::DlgTch(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTch::IDD, pParent)
{

}

DlgTch::~DlgTch()
{
}

void DlgTch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_editX, m_editX);
	DDX_Control(pDX, IDC_editY, m_editY);
	DDX_Control(pDX, IDC_editW, m_editW);
	DDX_Control(pDX, IDC_editH, m_editH);
	DDX_Control(pDX, IDC_editPos, m_editPos);
	DDX_Control(pDX, IDC_editSlide, m_editSlide);
	DDX_Control(pDX, IDC_editStand, m_editStand);
	DDX_Control(pDX, IDC_editTargetArea, m_editTargetArea);
	DDX_Control(pDX, IDC_editOutSide, m_editOutSide);
}


BEGIN_MESSAGE_MAP(DlgTch, CDialog)
	ON_BN_CLICKED(IDC_btnApply, &DlgTch::OnBnClickedbtnapply)
END_MESSAGE_MAP()


// DlgTch 消息处理程序


void DlgTch::OnBnClickedbtnapply()
{
	// TODO:  在此添加控件通知处理程序代码
	ctrlClient_set_teach_params(&tch_params, m_Connect_clientHandle);
}
void DlgTch::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}