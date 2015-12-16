// DlgCam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "MFCTrackToolsDlg.h"
#include "DlgCam.h"
#include "afxdialogex.h"
#include "DlgStu.h"


// DlgCam �Ի���

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
}


BEGIN_MESSAGE_MAP(DlgCam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &DlgCam::OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PRESET, &DlgCam::OnBnClickedButtonLeftPreset)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PRESET, &DlgCam::OnBnClickedButtonRightPreset)
	ON_BN_CLICKED(IDC_BUT_CALIBRATION, &DlgCam::OnBnClickedButCalibration)
	ON_BN_CLICKED(IDC_BUT_AGAINCALIB, &DlgCam::OnBnClickedButAgaincalib)
END_MESSAGE_MAP()


// DlgCam ��Ϣ�������

BOOL DlgCam::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //��ESC������Ϣ�滻Ϊ�س�������Ϣ����������ESC��ʱ��  
		//Ҳ��ȥ����OnOK��������OnOKʲôҲ����������ESCҲ������   
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
		HWND p_hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
		CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			if (pWnd->CurSel==TCH_TAB)
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_UP);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_UP);
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_LEFT);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_LEFT);
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_RIGHT);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_RIGHT);
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_DOWN);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_DOWN);
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOMIN)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMIN);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMIN);
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOMOUT)->m_hWnd)
		{
			if (pWnd->CurSel == TCH_TAB)
			{
				m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMOUT);
			}
			else
			{
				m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMOUT);
			}
		}
		//m_btnUp.SetState(TRUE);
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		HWND p_hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
		CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);
		if (pWnd->CurSel == TCH_TAB)
		{
			m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_STOP);
			m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMSTOP);
		}
		else
		{
			m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_STOP);
			m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_ZOOMSTOP);
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

//void DlgCam::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	//MessageBox("fuck");
//	whichBtn = 0;
//	//SetTimer(1, 1, NULL);
//}
//
//
//void DlgCam::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//HI_NET_DEV_PTZ_Ctrl_Standard(m_uiHandle, HI_NET_DEV_CTRL_PTZ_HOME, m_comboSpeed.GetCurSel());
	HWND p_hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
	CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);
	m_comboSpeed.GetWindowText(str);
	speed = _ttoi(str);
	switch (pWnd->CurSel)
	{
	case TCH_TAB:
		m_CameraControl_tch.setMoveSpeed(speed, speed);
		m_CameraControl_tch.home();
	case STU_TAB:
		m_CameraControl_stu.setMoveSpeed(speed, speed);
		m_CameraControl_stu.home();
	default:
		break;
	}
	
}


void DlgCam::OnBnClickedButtonLeftPreset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (numPos <= 0)
	{
		MessageBox("û������Ԥ��λ������");
		return;
	}
	if (1==right)
	{
		m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit>0)
		{
			MessageBox("���λ�ò�����ˡ�");
			return;
		}
		m_leftPreset = m_get_panPosit;

		autoPreSet(m_leftPreset, m_rightPreset,LeftToRight);
	}
	else
	{
		m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit > 0)
		{
			MessageBox("���λ�ò�����ˡ�");
			return;
		}
		m_leftPreset = m_get_panPosit;
		left = 1;
	}
}


void DlgCam::OnBnClickedButtonRightPreset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (numPos<=0)
	{
		MessageBox("û������Ԥ��λ������");
		return;
	}
	if (1==left)
	{
		m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit < 0)
		{
			MessageBox("���λ�ò����Ҷˡ�");
			return;
		}
		m_rightPreset = m_get_panPosit;

		autoPreSet(m_leftPreset, m_rightPreset,RightToLeft);
	}
	else
	{
		m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		if (m_get_panPosit < 0)
		{
			MessageBox("���λ�ò����Ҷˡ�");
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
	if (a==b)
	{
		MessageBox("����λ�ö�Ϊԭ��λ�ã�");
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
		s.Format("��������%d��Ԥ��λ...", num);
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
		s.Format("%d��Ԥ��λ���óɹ�...", num);
		m_txtPreset.SetWindowText(s);
		Sleep(500);
		num++;
	}
	left = 0;
	right = 0;
	MessageBox("��̨�����Ԥ��λ���óɹ���");
	m_txtPreset.SetWindowText(_T(""));
}

#include "opencv2/opencv.hpp"
#ifdef _DEBUG
#pragma comment(lib,"opencv/opencv_core249d.lib")
#pragma comment(lib,"opencv/opencv_imgproc249d.lib")
#endif
#ifndef _DEBUG
#pragma comment(lib,"opencv/opencv_core249.lib")
#pragma comment(lib,"opencv/opencv_imgproc249.lib")
#endif
static int countCalib = 0;
void DlgCam::OnBnClickedButCalibration()
{
	//�ɼ��궨��
	if (countCalib < 4)
	{
		m_CameraControl_tch.getPosit(&m_get_panPosit, &m_get_tiltPosit, 500);
		m_calibPt[countCalib].x = m_get_panPosit;
		m_calibPt[countCalib].y = m_get_tiltPosit;
		m_CameraControl_tch.getZoom(&m_get_zoomValue, 500);
		m_zoom[countCalib] = m_get_zoomValue;
		countCalib++;
		CString str;
		if (countCalib == 1)
		{
			str.Format(_T("�궨���Ͻ�"));
		}
		else if (countCalib == 2)
		{
			str.Format(_T("�궨���½�"));
		}
		else if (countCalib == 3)
		{
			str.Format(_T("�궨���½�"));
		}
		else if (countCalib == 4)
		{
			str.Format(_T("����궨����"));
		}
		this->SetDlgItemText(IDC_BUT_AGAINCALIB, str);
	}
	else
	{
		//����궨����
		HWND hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
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
	}
}


void DlgCam::OnBnClickedButAgaincalib()
{
	//���±궨
	countCalib = 0;
	CString str;
	str.Format(_T("�궨���Ͻ�"));
	this->SetDlgItemText(IDC_BUT_AGAINCALIB, str);
}
