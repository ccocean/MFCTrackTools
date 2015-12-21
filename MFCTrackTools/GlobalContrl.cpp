// GlobalContrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "GlobalContrl.h"
#include "afxdialogex.h"


// GlobalContrl �Ի���

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


// GlobalContrl ��Ϣ�������


void GlobalContrl::OnBnClickedChkMultiple()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_chk_multiple.GetCheck())
	{
		m_chk_multiple.SetWindowText(_T("�໭�濪��"));
		ctrl_params.mut_pic_flag = TRUE;
	}
	else
	{
		m_chk_multiple.SetWindowText(_T("�໭��ر�"));
		ctrl_params.mut_pic_flag = FALSE;
	}
}


void GlobalContrl::OnBnClickedBtnCtrlApply()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_edt_timeBlk<=0)
	{
		MessageBox(_T("����ʱ�����"));
		return;
	}
	if (m_edt_timeStu <= 0)
	{
		MessageBox(_T("ѧ��ʱ�����"));
		return;
	}
	if (m_edt_timeTch <= 0)
	{
		MessageBox(_T("��ʦʱ�����"));
		return;
	}
	if (m_edt_timeVGA <= 0)
	{
		MessageBox(_T("����ʱ�����"));
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
		pMsg->wParam = VK_RETURN;   //��ESC������Ϣ�滻Ϊ�س�������Ϣ����������ESC��ʱ��  
		//Ҳ��ȥ����OnOK��������OnOKʲôҲ����������ESCҲ������   
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}
