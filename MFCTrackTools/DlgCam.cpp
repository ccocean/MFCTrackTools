// DlgCam.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "MFCTrackToolsDlg.h"
#include "DlgCam.h"
#include "afxdialogex.h"
#include "DlgStu.h"


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
	DDX_Control(pDX, IDC_grpBoxCam, m_grpBoxCam);
	DDX_Control(pDX, IDC_txtPreset, m_txtPreset);
	DDX_Control(pDX, IDC_CHECK_CAM, m_checkCam);
}


BEGIN_MESSAGE_MAP(DlgCam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &DlgCam::OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PRESET, &DlgCam::OnBnClickedButtonLeftPreset)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PRESET, &DlgCam::OnBnClickedButtonRightPreset)
	ON_BN_CLICKED(IDC_BUT_CALIBRATION, &DlgCam::OnBnClickedButCalibration)
	ON_BN_CLICKED(IDC_BUT_AGAINCALIB, &DlgCam::OnBnClickedButAgaincalib)
	ON_MESSAGE(WM_USER_THREADEND, OnUserThreadend)
	ON_BN_CLICKED(IDC_BUTTON2, &DlgCam::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &DlgCam::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &DlgCam::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &DlgCam::OnBnClickedButton5)
END_MESSAGE_MAP()


// DlgCam 消息处理程序

void DlgCam::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}

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
		m_comboSpeed.GetWindowText(str);
		speed = _ttoi(str);
		m_CameraControl_tch.setMoveSpeed(speed, speed);
		HWND p_hWnd = ::FindWindow(NULL, _T("ITC TrackTools"));
		CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);

		Serial_Param_t cam_param;
		memset(&cam_param, 0, sizeof(Serial_Param_t));//相机串口控制协议
		int isComControl = m_checkCam.GetCheck();

		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			if (pWnd->CurSel==TCH_TAB)
			{
				if (isComControl)
				{
					cam_param.move_type = GOUP;
					cam_param.port_type = TEA_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_UP);
				}
			}
			else
			{
				if (isComControl)
				{
					cam_param.move_type = GOUP;
					cam_param.port_type = STU_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_UP);
				}
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				if (isComControl)
				{
					cam_param.move_type = GOLEFT;
					cam_param.port_type = TEA_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_LEFT);
				}
			}
			else
			{
				if (isComControl)
				{
					cam_param.move_type = GOLEFT;
					cam_param.port_type = STU_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_LEFT);
				}
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				if (isComControl)
				{
					cam_param.move_type = GORIGHT;
					cam_param.port_type = TEA_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_RIGHT);
				}
			}
			else
			{
				if (isComControl)
				{
					cam_param.move_type = GORIGHT;
					cam_param.port_type = STU_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_RIGHT);
				}
				
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				if (isComControl)
				{
					cam_param.move_type = GODOWN;
					cam_param.port_type = TEA_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_DOWN);
				}
				
			}
			else
			{
				if (isComControl)
				{
					cam_param.move_type = GODOWN;
					cam_param.port_type = STU_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_DOWN);
				}
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOMIN)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				if (isComControl)
				{
					cam_param.move_type = GONEAR;
					cam_param.port_type = TEA_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMIN);
				}
			}
			else
			{
				if (isComControl)
				{
					cam_param.move_type = GONEAR;
					cam_param.port_type = STU_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMIN);
				}
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOMOUT)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				if (isComControl)
				{
					cam_param.move_type = GOFAR;
					cam_param.port_type = TEA_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMOUT);
				}
			}
			else
			{
				if (isComControl)
				{
					cam_param.move_type = GOFAR;
					cam_param.port_type = STU_PORT;
					cam_param.speed = speed;
					cam_param.type = CAM_MOVE;
					ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				}
				else
				{
					m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMOUT);
				}
			}
		}
		//m_btnUp.SetState(TRUE);
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		HWND p_hWnd = ::FindWindow(NULL, _T("ITC TrackTools"));
		CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);

		Serial_Param_t cam_param;
		memset(&cam_param, 0, sizeof(Serial_Param_t));//相机串口控制协议
		int isComControl = m_checkCam.GetCheck();

		if (pWnd->CurSel == TCH_TAB)
		{
			if (isComControl)
			{
				cam_param.move_type = STOPTURN;
				cam_param.port_type = TEA_PORT;
				//cam_param.speed = speed;
				cam_param.type = CAM_MOVE;
				ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				cam_param.move_type = STOPZ;
				ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
			}
			else
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_STOP);
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMSTOP);
			}
			
		}
		else
		{
			if (isComControl)
			{
				cam_param.move_type = STOPTURN;
				cam_param.port_type = STU_PORT;
				//cam_param.speed = speed;
				cam_param.type = CAM_MOVE;
				ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
				cam_param.move_type = STOPZ;
				ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_STOP);
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMSTOP);
			}
		}
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
	HWND p_hWnd = ::FindWindow(NULL, _T("ITC TrackTools"));
	CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);
	m_comboSpeed.GetWindowText(str);
	speed = _ttoi(str);
	Serial_Param_t cam_param;
	memset(&cam_param, 0, sizeof(Serial_Param_t));//相机串口控制协议
	switch (pWnd->CurSel)
	{
	case TCH_TAB:
		if (m_checkCam.GetCheck())
		{
			cam_param.move_type = GOHOME;
			cam_param.port_type = TEA_PORT;
			cam_param.speed = speed;
			cam_param.type = CAM_MOVE;
			ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
		}
		else
		{
			m_CameraControl_tch.setMoveSpeed(speed, speed);
			m_CameraControl_tch.home();
		}
		break;
	case STU_TAB:
		if (m_checkCam.GetCheck())
		{
			cam_param.move_type = GOHOME;
			cam_param.port_type = STU_PORT;
			cam_param.speed = speed;
			cam_param.type = CAM_MOVE;
			ctrlClient_set_Camera_params(&cam_param, m_Connect_clientHandle);
		}
		else
		{
			m_CameraControl_stu.setMoveSpeed(speed, speed);
			m_CameraControl_stu.home();
		}
		break;
	default:
		break;
	}
}


void DlgCam::OnBnClickedButtonLeftPreset()
{
	// TODO:  在此添加控件通知处理程序代码
	if (numPos <= 0)
	{
		MessageBox("没有设置预置位个数！");
		return;
	}
	HWND hWnd = ::FindWindow(NULL, _T("ITC TrackTools"));
	CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(hWnd);
	if (1==right)
	{
		if (m_checkCam.GetCheck())
		{
			Serial_Position_t cam_pos;
			memset(&cam_pos, 0, sizeof(Serial_Position_t));
			cam_pos.port = TEA_PORT;
			ctrlClient_get_Camera_position(&cam_pos, m_Connect_clientHandle);
		}
		else
		{
			m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		}
		m_txtPreset.SetWindowText("等待获取位置...");
		WaitForSingleObject(camEvent, INFINITE);
		m_txtPreset.SetWindowText("");
		if (m_get_panPosit>0)
		{
			MessageBox("相机位置不在左端。");
			return;
		}
		::ResetEvent(camEvent);
		m_leftPreset = m_get_panPosit;
		DWORD *pParams = new DWORD[2];
		int *params = new int[3];
		params[0] = m_leftPreset;
		params[1] = m_rightPreset;
		params[2] = LeftToRight;
		pParams[0] = (DWORD)this;
		pParams[1] = (DWORD)params;
		//autoPreSet(m_leftPreset, m_rightPreset,LeftToRight);
		disableButton();
		m_hThread = ::CreateThread(NULL, NULL, automaticPreset, (LPVOID*)(pParams), NULL, NULL);
		//WaitForSingleObject(m_hThread, INFINITE);
	}
	else
	{
		if (m_checkCam.GetCheck())
		{
			Serial_Position_t cam_pos;
			memset(&cam_pos, 0, sizeof(Serial_Position_t));
			cam_pos.port = TEA_PORT;
			ctrlClient_get_Camera_position(&cam_pos, m_Connect_clientHandle);
			m_txtPreset.SetWindowText("等待获取位置...");
			WaitForSingleObject(camEvent, INFINITE);
			m_txtPreset.SetWindowText("获取成功，请设置最右端！");
			CString temp;
			temp.Format("pan:%d, til:%d", m_get_panPosit, m_get_tiltPosit);
			m_txtPreset.SetWindowText(temp);
			/*m_get_panPosit = cam_pos.posit_pan;
			m_get_tiltPosit = cam_pos.posit_tilt*/;
		}
		else
		{
			m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		}
		if (m_get_panPosit > 0)
		{
			MessageBox("相机位置不在左端。");
			return;
		}
		::ResetEvent(camEvent);
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
		if (m_checkCam.GetCheck())
		{
			Serial_Position_t cam_pos;
			memset(&cam_pos, 0, sizeof(Serial_Position_t));
			cam_pos.port = TEA_PORT;
			ctrlClient_get_Camera_position(&cam_pos, m_Connect_clientHandle);
			m_get_panPosit = cam_pos.posit_pan;
			m_get_tiltPosit = cam_pos.posit_tilt;
		}
		else
		{
			m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		}
		if (m_get_panPosit < 0)
		{
			MessageBox("相机位置不在右端。");
			return;
		}
		m_rightPreset = m_get_panPosit;

		DWORD *pParams = new DWORD[2];
		int *params = new int[3];
		params[0] = m_leftPreset;
		params[1] = m_rightPreset;
		params[2] = LeftToRight;
		pParams[0] = (DWORD)this;
		pParams[1] = (DWORD)params;
		//autoPreSet(m_leftPreset, m_rightPreset,LeftToRight);
		disableButton();
		m_hThread = ::CreateThread(NULL, NULL, automaticPreset, (LPVOID*)(pParams), NULL, NULL);
		//WaitForSingleObject(m_hThread, INFINITE);
	}
	else
	{
		if (m_checkCam.GetCheck())
		{
			Serial_Position_t cam_pos;
			memset(&cam_pos, 0, sizeof(Serial_Position_t));
			cam_pos.port = TEA_PORT;
			ctrlClient_get_Camera_position(&cam_pos, m_Connect_clientHandle);
			m_get_panPosit = cam_pos.posit_pan;
			m_get_tiltPosit = cam_pos.posit_tilt;
		}
		else
		{
			m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		}
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

void DlgCam::enableButton()
{
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LEFT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ZOOMIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ZOOMOUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LEFT_PRESET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RIGHT_PRESET)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(TRUE);

// 	GetDlgItem(IDC_BUT_CALIBRATION)->ShowWindow(TRUE);
// 	GetDlgItem(IDC_BUT_AGAINCALIB)->ShowWindow(TRUE);
}

void DlgCam::disableButton()
{
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LEFT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ZOOMIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ZOOMOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LEFT_PRESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RIGHT_PRESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(FALSE);

// 	GetDlgItem(IDC_BUT_CALIBRATION)->ShowWindow(FALSE);
// 	GetDlgItem(IDC_BUT_AGAINCALIB)->ShowWindow(FALSE);
}

LRESULT DlgCam::OnUserThreadend(WPARAM wParam, LPARAM lParam)
{
	enableButton();
	return 0;
}

DWORD WINAPI DlgCam::automaticPreset(LPVOID pParam)
{
	DWORD *in = (DWORD*)pParam;
	DlgCam *pDlg = (DlgCam*)(in[0]);
	int *params = (int*)(in[1]);
	int a = params[0];
	int b = params[1];
	int direct = params[2];

	if (a == b)
	{
		pDlg->MessageBox("左右位置都为原点位置！");
		pDlg->left = pDlg->right = 0;
		return false;
	}
	pDlg->m_CameraControl_tch.setMoveSpeed(20, 20);
	int width = (b - a) / (pDlg->numPos - 1);
	int fix = (b - a) % (pDlg->numPos - 1);
	int num = 0;
	CString s;
	for (int i = a; i <= b; i += width)
	{
		s.Format("正在设置%d号预置位...", num);
		pDlg->m_txtPreset.SetWindowText(s);

		if (num == pDlg->numPos - 1)
		{
			while (pDlg->m_get_panPosit != i + fix)
			{
				if (pDlg->m_checkCam.GetCheck())
				{
					Serial_Position_t cam_pos;
					cam_pos.port = TEA_PORT;
					ctrlClient_get_Camera_position(&cam_pos, pDlg->m_Connect_clientHandle);
					pDlg->m_txtPreset.SetWindowText("等待获取位置...");
					WaitForSingleObject(pDlg->camEvent, INFINITE);
					pDlg->m_txtPreset.SetWindowText("");
				}
				else
				{
					pDlg->m_CameraControl_tch.getPosit(&pDlg->m_get_panPosit, &pDlg->m_get_tiltPosit, 500);
					pDlg->m_CameraControl_tch.move(i + fix, pDlg->m_get_tiltPosit, FALSE);
				}
			}
		}
		else
		{
			while (pDlg->m_get_panPosit != i)
			{
				pDlg->m_CameraControl_tch.getPosit(&pDlg->m_get_panPosit, &pDlg->m_get_tiltPosit, 500);
				pDlg->m_CameraControl_tch.move(i, pDlg->m_get_tiltPosit, FALSE);
			}
		}

		pDlg->m_CameraControl_tch.preset(PANandTILT_CTRL_PTZ_SET_PRESET, num + 10);
		Sleep(500);
		s.Format("%d号预置位设置成功...", num);
		pDlg->m_txtPreset.SetWindowText(s);
		Sleep(500);
		num++;
	}
	pDlg->left = 0;
	pDlg->right = 0;
	pDlg->MessageBox("云台摄像机预置位设置成功！");
	pDlg->m_txtPreset.SetWindowText(_T(""));
	::PostMessage(pDlg->GetSafeHwnd(), WM_USER_THREADEND, 0, 0);
	return true;
}

void DlgCam::autoPreSet(int a, int b, int direct)
{
	if (a==b)
	{
		MessageBox("左右位置都为原点位置！");
		left = right = 0;
		return;
	}
	m_CameraControl_tch.setMoveSpeed(20, 20);
	int width = (b - a) / (numPos - 1);
	int fix = (b - a) % (numPos - 1);
	int num = 0;
	CString s;
	for (int i = a; i <= b; i += width)
	{
		s.Format("正在设置%d号预置位...", num);
		m_txtPreset.SetWindowText(s);
		//Sleep(500);
		/*if (num==numPos-1)
		{
			while (m_get_panPosit != i)
			{
				m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
				m_CameraControl.move(i, m_get_tiltPosit, FALSE);
			}
		}
		else
		{
			if (num==0)
			{
				if (direct==LeftToRight)
				{
					while (m_get_panPosit != i)
					{
						m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
						m_CameraControl.move(i, m_get_tiltPosit, FALSE);
					}
				}
				else
				{
					while (m_get_panPosit!=i)
					{
						m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
						m_CameraControl.move(i, m_get_tiltPosit, FALSE);
					}
				}
			}
			else
			{
				while (m_get_panPosit != i)
				{
					m_CameraControl.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
					m_CameraControl.move(i, m_get_tiltPosit, FALSE);
				}
			}
		}*/
		
		if (num==numPos-1)
		{
			while (m_get_panPosit != i + fix)
			{
				m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
				m_CameraControl_tch.move(i + fix, m_get_tiltPosit, FALSE);
			}
		}
		else
		{
			while (m_get_panPosit != i)
			{
				m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
				m_CameraControl_tch.move(i, m_get_tiltPosit, FALSE);
			}
		}
		

		m_CameraControl_tch.preset(PANandTILT_CTRL_PTZ_SET_PRESET, num);
		Sleep(500);
		s.Format("%d号预置位设置成功...", num);
		m_txtPreset.SetWindowText(s);
		Sleep(500);
		num++;
	}
	left = 0;
	right = 0;
	MessageBox("云台摄像机预置位设置成功！");
	m_txtPreset.SetWindowText(_T(""));
}


#define CALIBRATION_FLAG_NULL 0
#define CALIBRATION_FLAG_LEFT_UP 1
#define CALIBRATION_FLAG_RIGHT_UP 2
#define CALIBRATION_FLAG_LEFT_DOWN 4
#define CALIBRATION_FLAG_RIGHT_DOWN 8
#define CALIBRATION_FLAG_ALL 15

static int countCalib = CALIBRATION_FLAG_NULL;
void DlgCam::OnBnClickedButCalibration()
{
	//采集标定点，左上角
	m_CameraControl_stu.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
	m_calibPt[0].x = m_get_panPosit;
	m_calibPt[0].y = m_get_tiltPosit;
	m_CameraControl_stu.getZoom(&m_get_zoomValue, 500);
	m_zoom[0] = m_get_zoomValue;
	countCalib |= CALIBRATION_FLAG_LEFT_UP;
	MessageBox("左上角标定点设置完成！");
	//CString str;
	//if (countCalib == 1)
	//{
	//	str.Format(_T("标定右上角"));
	//}
	//else if (countCalib == 2)
	//{
	//	str.Format(_T("标定右下角"));
	//}
	//else if (countCalib == 3)
	//{
	//	str.Format(_T("标定左下角"));
	//}
	//else if (countCalib == 4)
	//{
	//	str.Format(_T("计算标定参数"));
	//}
	//this->SetDlgItemText(IDC_BUT_CALIBRATION, str);
}


void DlgCam::OnBnClickedButAgaincalib()
{
	//重新标定
	countCalib = CALIBRATION_FLAG_NULL;
	//CString str;
	//str.Format(_T("标定左上角"));
	//this->SetDlgItemText(IDC_BUT_CALIBRATION, str);
}


void DlgCam::OnBnClickedButton2()
{
	//右上角
	m_CameraControl_stu.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
	m_calibPt[1].x = m_get_panPosit;
	m_calibPt[1].y = m_get_tiltPosit;
	m_CameraControl_stu.getZoom(&m_get_zoomValue, 500);
	m_zoom[1] = m_get_zoomValue;
	countCalib |= CALIBRATION_FLAG_RIGHT_UP;
	MessageBox("右上角标定点设置完成！");
}


void DlgCam::OnBnClickedButton3()
{
	//左下角
	m_CameraControl_stu.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
	m_calibPt[3].x = m_get_panPosit;
	m_calibPt[3].y = m_get_tiltPosit;
	m_CameraControl_stu.getZoom(&m_get_zoomValue, 500);
	m_zoom[3] = m_get_zoomValue;
	countCalib |= CALIBRATION_FLAG_LEFT_DOWN;
	MessageBox("左下角标定点设置完成！");
}


void DlgCam::OnBnClickedButton4()
{
	//右下角
	m_CameraControl_stu.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
	m_calibPt[2].x = m_get_panPosit;
	m_calibPt[2].y = m_get_tiltPosit;
	m_CameraControl_stu.getZoom(&m_get_zoomValue, 500);
	m_zoom[2] = m_get_zoomValue;
	countCalib |= CALIBRATION_FLAG_RIGHT_DOWN;
	MessageBox("右下角标定点设置完成！");
}


void DlgCam::OnBnClickedButton5()
{
	//标定
	//计算标定参数
	if ((countCalib&CALIBRATION_FLAG_ALL) == CALIBRATION_FLAG_ALL)
	{
		HWND hWnd = ::FindWindow(NULL, _T("ITC TrackTools"));
		CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(hWnd);

		//StuITRACK_ClientParams_t &stu_params = ((CMFCTrackToolsDlg*)GetDlgItem(IDD_MFCTRACKTOOLS_DIALOG))->dlgStu.stu_params;
		cv::Point2f ptSrc[4];
		ptSrc[0].x = pWnd->dlgStu.stu_params.stuTrack_vertex[0].x;
		ptSrc[0].y = pWnd->dlgStu.stu_params.stuTrack_vertex[0].y;
		ptSrc[1].x = pWnd->dlgStu.stu_params.stuTrack_vertex[1].x;
		ptSrc[1].y = pWnd->dlgStu.stu_params.stuTrack_vertex[1].y;
		ptSrc[2].x = pWnd->dlgStu.stu_params.stuTrack_vertex[2].x;
		ptSrc[2].y = pWnd->dlgStu.stu_params.stuTrack_vertex[2].y;
		ptSrc[3].x = pWnd->dlgStu.stu_params.stuTrack_vertex[3].x;
		ptSrc[3].y = pWnd->dlgStu.stu_params.stuTrack_vertex[3].y;

		cv::Point2f m_ptDst[4];
		m_ptDst[0].x = m_calibPt[0].x;
		m_ptDst[0].y = m_calibPt[0].y;
		m_ptDst[1].x = m_calibPt[1].x;
		m_ptDst[1].y = m_calibPt[1].y;
		m_ptDst[2].x = m_calibPt[2].x;
		m_ptDst[2].y = m_calibPt[2].y;
		m_ptDst[3].x = m_calibPt[3].x;
		m_ptDst[3].y = m_calibPt[3].y;
		cv::Mat transM;
		transM.create(3, 3, CV_64FC1);
		transM = getPerspectiveTransform(ptSrc, m_ptDst);
		pWnd->dlgStu.stu_params.transformationMatrix[0] = transM.at<double>(0, 0);
		pWnd->dlgStu.stu_params.transformationMatrix[1] = transM.at<double>(0, 1);
		pWnd->dlgStu.stu_params.transformationMatrix[2] = transM.at<double>(0, 2);
		pWnd->dlgStu.stu_params.transformationMatrix[3] = transM.at<double>(1, 0);
		pWnd->dlgStu.stu_params.transformationMatrix[4] = transM.at<double>(1, 1);
		pWnd->dlgStu.stu_params.transformationMatrix[5] = transM.at<double>(1, 2);
		pWnd->dlgStu.stu_params.transformationMatrix[6] = transM.at<double>(2, 0);
		pWnd->dlgStu.stu_params.transformationMatrix[7] = transM.at<double>(2, 1);
		pWnd->dlgStu.stu_params.transformationMatrix[8] = transM.at<double>(2, 2);

		int zoom1 = (m_zoom[0] + m_zoom[1]) / 2;
		int zoom2 = (m_zoom[2] + m_zoom[3]) / 2;
		int width1 = (pWnd->dlgStu.stu_params.stuTrack_stuWidth_standard[0] + pWnd->dlgStu.stu_params.stuTrack_stuWidth_standard[1]) / 2;
		int width2 = (pWnd->dlgStu.stu_params.stuTrack_stuWidth_standard[2] + pWnd->dlgStu.stu_params.stuTrack_stuWidth_standard[3]) / 2;
		if (width1 != width2)
		{
			pWnd->dlgStu.stu_params.stretchingAB[0] = ((double)(zoom1 - zoom2)) / (width1 - width2);
			pWnd->dlgStu.stu_params.stretchingAB[1] = zoom1 - pWnd->dlgStu.stu_params.stretchingAB[0] * width1;
		}
		else
		{
			pWnd->dlgStu.stu_params.stretchingAB[0] = 0;
			pWnd->dlgStu.stu_params.stretchingAB[1] = MIN(zoom1, zoom2);
		}
		pWnd->dlgStu.isSetCam = TRUE;
		MessageBox("标定成功，但未保存！");
	}
	else
	{
		if ((countCalib&CALIBRATION_FLAG_LEFT_UP) != CALIBRATION_FLAG_LEFT_UP)
			MessageBox("左上角未设标定点！");
		else if ((countCalib&CALIBRATION_FLAG_RIGHT_UP) != CALIBRATION_FLAG_RIGHT_UP)
			MessageBox("右上角未设标定点！");
		else if ((countCalib&CALIBRATION_FLAG_RIGHT_DOWN) != CALIBRATION_FLAG_RIGHT_DOWN)
			MessageBox("右下角未设标定点！");
		else if ((countCalib&CALIBRATION_FLAG_LEFT_DOWN) != CALIBRATION_FLAG_LEFT_DOWN)
			MessageBox("左下角未设标定点！");
	}
}
