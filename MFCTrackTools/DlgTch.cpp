// DlgTch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgTch.h"
#include "afxdialogex.h"


// DlgTch �Ի���

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


// DlgTch ��Ϣ��������

void DlgTch::getParameters()
{

}

int DlgTch::checkParameters()
{
	CString position, slide;
	int int_pos, int_slide;
	CString stand, targetArea, outside;
	int _stand, _targetArea, _outside;
	if (tch_params.tch.x<0||tch_params.tch.y<0)
	{
		MessageBox("��ʦ��������δ���ã�");
		return -1;
	}
	if (tch_params.blk.x < 0 || tch_params.blk.y < 0)
	{
		MessageBox("�����������δ���ã�");
		return -1;
	}
	m_editPos.GetWindowText(position);
	m_editSlide.GetWindowText(slide);
	if (position.IsEmpty() || slide.IsEmpty())
	{
		MessageBox("Ԥ��λ�򻬿����Ϊ�գ�");
		return -1;
	}
	else
	{
		int_pos = _ttoi(position);
		int_slide = _ttoi(slide);
		if (int_slide > int_pos)
		{
			AfxMessageBox(_T("���ݲ���ȷ��Ԥ��λ��Ŀ������ڻ�����ȣ�"));
			return -1;
		}
		else if (int_slide <= 0||int_pos<=0)
		{
			AfxMessageBox(_T("Ԥ��λ�򻬿����ݲ���ȷ��"));
			return -1;
		}
		else
		{
			tch_params.numOfPos = int_pos;
			tch_params.numOfSlide = int_slide;
		}
	}
	m_editStand.GetWindowText(stand);
	m_editTargetArea.GetWindowText(targetArea);
	m_editOutSide.GetWindowText(outside);
	if (stand.IsEmpty() || targetArea.IsEmpty() || outside.IsEmpty())
	{
		AfxMessageBox(_T("û�л����ֵ���ݣ�"));
		return -1;
	}
	else
	{
		_stand = _ttoi(stand);
		_targetArea = _ttoi(targetArea);
		_outside = _ttoi(outside);
		if (_stand <= 0 || _targetArea <= 0 || _outside <= 0)
		{
			AfxMessageBox(_T("��ֵ���ݴ���"));
			return -1;
		}
		else
		{
			tch_params.threshold.stand = _stand;
			tch_params.threshold.targetArea = _targetArea;
			tch_params.threshold.outside = _outside;
		}
	}
	return 0;
}

void DlgTch::OnBnClickedbtnapply()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	if (checkParameters()==0)
	{
		ctrlClient_set_teach_params(&tch_params, m_Connect_clientHandle);
	}
}
void DlgTch::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}