// DlgTch.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "MFCTrackToolsDlg.h"
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
	DDX_Control(pDX, IDC_editTargetArea, m_editTargetArea);
	DDX_Control(pDX, IDC_editOutSide, m_editOutSide);
	DDX_Control(pDX, IDC_txtTchArg, m_txtTchArg);
	DDX_Control(pDX, IDC_txtBlkArg, m_txtBlkArg);
	DDX_Control(pDX, IDC_comboSlide, m_comboSlide);
	DDX_Control(pDX, IDC_comboStand, m_comboStand);
}


BEGIN_MESSAGE_MAP(DlgTch, CDialog)
	ON_BN_CLICKED(IDC_btnApply, &DlgTch::OnBnClickedbtnapply)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDefault, &DlgTch::OnBnClickedbtndefault)
END_MESSAGE_MAP()


// DlgTch 消息处理程序

void DlgTch::getParameters()
{
	int ret = 0;
	ret = ctrlClient_get_teach_params(m_Connect_clientHandle);
	if (ret != 0)
	{
		OutputDebugString("获取老师参数失败！");
	}
	return;
}

void DlgTch::setParams(TeaITRACK_Params* params)
{
	tch_params.blk = params->blk;
	tch_params.frame = params->frame;
	tch_params.isSetParams = params->isSetParams;
	tch_params.numOfPos = params->numOfPos;
	tch_params.numOfSlide = params->numOfSlide;
	tch_params.tch = params->tch;
	tch_params.threshold = params->threshold;
}

void DlgTch::setTrackRects(Tch_Rect_t rc,int flag)
{
	if (flag==TCH_RECT)
	{
		tch_params.tch = rc;
	}
	else
	{
		tch_params.blk = rc;
	}
}

int DlgTch::checkParameters()
{
	CString position, slide;
	int int_pos, int_slide;
	CString stand, targetArea, outside;
	int _stand, _targetArea, _outside;
	if (tch_params.tch.x<0||tch_params.tch.y<0)
	{
		MessageBox("教师跟踪区域未设置！");
		return -1;
	}
	if (tch_params.blk.x < 0 || tch_params.blk.y < 0)
	{
		MessageBox("板书跟踪区域未设置！");
		return -1;
	}
	m_editPos.GetWindowText(position);
	m_comboSlide.GetWindowText(slide);
	//int_slide = m_comboSlide.GetCurSel();
	//m_comboSlide.GetLBText(int_slide, slide);
	if (position.IsEmpty() || slide.IsEmpty())
	{
		MessageBox("预置位或滑框宽度为空！");
		return -1;
	}
	else
	{
		int_pos = _ttoi(position);
		int_slide = _ttoi(slide);
		if (int_slide > int_pos)
		{
			AfxMessageBox(_T("数据不正确，预置位数目必须大于滑框宽度！"));
			return -1;
		}
		else if (int_slide <= 0||int_pos<=0)
		{
			AfxMessageBox(_T("预置位或滑框数据不正确。"));
			return -1;
		}
		else
		{
			tch_params.numOfPos = int_pos;
			tch_params.numOfSlide = int_slide;
		}
	}
	//m_editStand.GetWindowText(stand);
	m_comboStand.GetWindowText(stand);
	m_editTargetArea.GetWindowText(targetArea);
	m_editOutSide.GetWindowText(outside);
	if (stand.IsEmpty() || targetArea.IsEmpty() || outside.IsEmpty())
	{
		AfxMessageBox(_T("没有获得阈值数据！"));
		return -1;
	}
	else
	{
		_stand = _ttoi(stand)*1000;
		_targetArea = _ttoi(targetArea);
		_outside = _ttoi(outside);
		if (_stand <= 0 || _targetArea <= 0 || _outside <= 0)
		{
			AfxMessageBox(_T("阈值数据错误！"));
			return -1;
		}
		else
		{
			tch_params.threshold.stand = _stand;
			tch_params.threshold.targetArea = _targetArea;
			tch_params.threshold.outside = _outside;
		}
	}
	tch_params.frame.width = WIDTH;
	tch_params.frame.height = HEIGHT;
	tch_params.isSetParams = FALSE;
	return 0;
}

void DlgTch::OnBnClickedbtnapply()
{
	// TODO:  在此添加控件通知处理程序代码
	if (checkParameters()==0)
	{
		if (m_Connect_clientHandle)
		{
			HWND hWnd = ::FindWindow(NULL, _T("ITC TrackTools"));
			CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(hWnd);
			pWnd->dlgCam.setNumOfPreset(tch_params.numOfPos);
			pWnd->g_drawPS = 1;
			pWnd->camPosSlide.center = tch_params.numOfPos / 2;
			pWnd->camPosSlide.width = tch_params.numOfSlide / 2;
			pWnd->camPosSlide.left = pWnd->camPosSlide.center - pWnd->camPosSlide.width;
			pWnd->camPosSlide.right = pWnd->camPosSlide.center + pWnd->camPosSlide.width;
			pWnd->int_pos = tch_params.numOfPos;

			pWnd->centre_pt2.x = (pWnd->camPosSlide.left + tch_params.numOfSlide / 2 + 0.5)*(WIDTH / tch_params.numOfPos);
			pWnd->centre_pt2.y = tch_params.tch.y + tch_params.tch.height / 2;

			pWnd->pl = { 0, tch_params.threshold.outside + tch_params.tch.y };
			pWnd->pr = { WIDTH, tch_params.threshold.outside + tch_params.tch.y };

			ctrlClient_set_teach_params(&tch_params, m_Connect_clientHandle);
		}
		else
		{
			MessageBox("未连接服务器！");
		}
	}
}
void DlgTch::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}

BOOL DlgTch::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽   
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void DlgTch::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


void DlgTch::OnBnClickedbtndefault()
{
	// TODO:  在此添加控件通知处理程序代码
}
