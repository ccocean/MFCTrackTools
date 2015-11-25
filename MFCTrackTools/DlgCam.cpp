// DlgCam.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgCam.h"
#include "afxdialogex.h"
#include <vector>


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
	DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
	DDX_Control(pDX, IDC_BUTTON_UP, m_btnUp);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_btnLeft);
}


BEGIN_MESSAGE_MAP(DlgCam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &DlgCam::OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PRESET, &DlgCam::OnBnClickedButtonLeftPreset)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PRESET, &DlgCam::OnBnClickedButtonRightPreset)
END_MESSAGE_MAP()


// DlgCam 消息处理程序

BOOL DlgCam::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽   
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		m_CameraControl.setMoveSpeed(m_comboSpeed.GetCurSel(), m_comboSpeed.GetCurSel());
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_UP);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->m_hWnd)
		{
			m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_LEFT);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->m_hWnd)
		{
			m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_RIGHT);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_DOWN);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOMIN)->m_hWnd)
		{
			m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMIN);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOMOUT)->m_hWnd)
		{
			m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMOUT);
		}
		m_btnUp.SetState(TRUE);
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		//HI_NET_DEV_PTZ_Ctrl_Standard(m_uiHandle, HI_NET_DEV_CTRL_PTZ_STOP, m_comboSpeed.GetCurSel());
		m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_STOP);
		m_CameraControl.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMSTOP);
		m_btnUp.SetState(FALSE);
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

//void DlgCam::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	m_btnUp.GetWindowRect(rc);
//	if (PtInRect(rc,point))
//	{
//		SetDlgItemText(IDC_grpBoxCam, "Button Up DOWN");
//		whichBtn = 0;
//	}
//
//	CDialog::OnLButtonDown(nFlags, point);
//}
//
//
//void DlgCam::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	//m_btnUp.GetWindowRect(rc);
//	if (0==whichBtn)
//	{
//		SetDlgItemText(IDC_grpBoxCam, "Button Up Up");
//	}
//
//	CDialog::OnLButtonUp(nFlags, point);
//}

//void DlgCam::OnBnClickedButtonUp()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	//MessageBox("fuck");
//	whichBtn = 0;
//	//SetTimer(1, 1, NULL);
//}
//
//
//void DlgCam::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	if (whichBtn==0)
//	{
//		HI_S32 s32Ret = HI_FAILURE;
//
//
//		if (m_uiHandle == 100)
//		{
//			MessageBox(ConvertString("No connection"), ConvertString("msg"), MB_ICONEXCLAMATION);
//			return;
//		}
//
//		HI_NET_DEV_PTZ_Ctrl_Standard(m_uiHandle, HI_NET_DEV_CTRL_PTZ_UP, m_comboSpeed.GetCurSel());
//	}
//	CDialog::OnTimer(nIDEvent);
//}


void DlgCam::OnBnClickedButtonHome()
{
	// TODO:  在此添加控件通知处理程序代码
	//HI_NET_DEV_PTZ_Ctrl_Standard(m_uiHandle, HI_NET_DEV_CTRL_PTZ_HOME, m_comboSpeed.GetCurSel());
	m_CameraControl.home();
}


void DlgCam::OnBnClickedButtonLeftPreset()
{
	// TODO:  在此添加控件通知处理程序代码
	if (numPos <= 0)
	{
		MessageBox("没有设置预置位个数！");
		return;
	}
	if (1==right)
	{
		m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit>0)
		{
			MessageBox("相机位置不在左端。");
			return;
		}
		m_leftPreset = m_get_panPosit;

		autoPreSet(m_leftPreset, m_rightPreset,LeftToRight);
	}
	else
	{
		m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit > 0)
		{
			MessageBox("相机位置不在左端。");
			return;
		}
		m_leftPreset = m_get_panPosit;
		left = 1;
	}
}


void DlgCam::OnBnClickedButtonRightPreset()
{
	// TODO:  在此添加控件通知处理程序代码
	if (numPos<=0)
	{
		MessageBox("没有设置预置位个数！");
		return;
	}
	if (1==left)
	{
		m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit < 0)
		{
			MessageBox("相机位置不在右端。");
			return;
		}
		m_rightPreset = m_get_panPosit;

		autoPreSet(m_leftPreset, m_rightPreset,RightToLeft);
	}
	else
	{
		m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit < 0)
		{
			MessageBox("相机位置不在右端。");
			return;
		}
		m_rightPreset = m_get_panPosit;
		right = 1;
	}
}

void DlgCam::setNumOfPreset(int num)
{
	numPos = num;
}

void DlgCam::autoPreSet(int a, int b, int direct)
{
	int width = (b - a) / (numPos - 1);
	int fix = (b - a) % (numPos - 1);
	int num = 0;
	int nums[10] = { 0 };
	for (int i = a; i <= b; i += width)
	{
		if (num==numPos-1)
		{
			m_CameraControl.move(i+fix, m_get_tiltPosit,FALSE);
			Sleep(3000);
		}
		else
		{
			if (num==0)
			{
				if (direct==LeftToRight)
				{
					m_CameraControl.move(i, m_get_tiltPosit, FALSE);
					Sleep(3000);
				}
				else
				{
					m_CameraControl.move(i, m_get_tiltPosit, FALSE);
					Sleep(7000);
				}
			}
			else
			{
				m_CameraControl.move(i, m_get_tiltPosit, FALSE);
				Sleep(3000);
			}
		}
		m_CameraControl.preset(PANandTILT_CTRL_PTZ_SET_PRESET, num);
		nums[num] = i;
		num++;
	}
	left = 0;
	right = 0;
	MessageBox("云台摄像机预置位设置成功！");
}
