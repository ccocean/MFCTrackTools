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
