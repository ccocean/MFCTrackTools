// DlgStu.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgStu.h"
#include "afxdialogex.h"
#include "stuTrack_settings_parameter.h"


// DlgStu �Ի���

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


// DlgStu ��Ϣ�������

void DlgStu::OnBnClickedBtnstuapply()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	stu_params = { 0 };
}
