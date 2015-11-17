// DlgCam.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgCam.h"
#include "afxdialogex.h"


// DlgCam 对话框

IMPLEMENT_DYNAMIC(DlgCam, CDialog)

DlgCam::DlgCam(CWnd* pParent /*=NULL*/)
	: CDialog(DlgCam::IDD, pParent)
{

}

DlgCam::~DlgCam()
{
}

void DlgCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgCam, CDialog)
END_MESSAGE_MAP()


// DlgCam 消息处理程序
