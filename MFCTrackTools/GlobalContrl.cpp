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
		MessageBox(_T("��������Ϊ�գ�"));
		return -1;
	}
	_tch = _ttoi(tch);
	_stu = _ttoi(stu);
	_blk = _ttoi(blk);
	_vga = _ttoi(vga);
	if (_blk <= 0)
	{
		MessageBox(_T("����ʱ�����"));
		return -1;
	}
	if (_stu <= 0)
	{
		MessageBox(_T("ѧ��ʱ�����"));
		return -1;
	}
	if (_tch <= 0)
	{
		MessageBox(_T("��ʦʱ�����"));
		return -1;
	}
	if (_vga <= 0)
	{
		MessageBox(_T("����ʱ�����"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (checkParameters()==0)
	{
		if (m_Connect_clientHandle)
		{
			ctrlClient_set_policy_params(&ctrl_params, m_Connect_clientHandle);
		}
		else
		{
			MessageBox("δ���ӷ�������");
		}
	}
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


void GlobalContrl::OnBnClickedChkTrackstu()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_chk_trackStu.GetCheck())
	{
		m_chk_trackStu.SetWindowText(_T("ѧ�����ٿ���"));
		ctrl_params.stu_track_flag = TRUE;
	}
	else
	{
		m_chk_trackStu.SetWindowText(_T("ѧ�����ٹر�"));
		ctrl_params.stu_track_flag = FALSE;
	}
}


void GlobalContrl::OnBnClickedChkStuoverview()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_chk_stuOverview.GetCheck())
	{
		m_chk_stuOverview.SetWindowText(_T("ѧ��ȫ������"));
		ctrl_params.stu_overview_flag = TRUE;
	}
	else
	{
		m_chk_stuOverview.SetWindowText(_T("ѧ��ȫ���ر�"));
		ctrl_params.stu_overview_flag = FALSE;
	}
}
