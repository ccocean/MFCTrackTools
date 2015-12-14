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

}

GlobalContrl::~GlobalContrl()
{
}

void GlobalContrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GlobalContrl, CDialog)
END_MESSAGE_MAP()


// GlobalContrl 消息处理程序
