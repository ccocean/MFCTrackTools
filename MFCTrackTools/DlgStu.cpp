// DlgStu.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgStu.h"
#include "afxdialogex.h"
#include "stuTrack_settings_parameter.h"
#include "MFCTrackToolsDlg.h"


// DlgStu 对话框

IMPLEMENT_DYNAMIC(DlgStu, CDialog)



DlgStu::DlgStu(CWnd* pParent /*=NULL*/)
	: CDialog(DlgStu::IDD, pParent)
	, m_radioAgl(0)
	, m_radioWid(0)
{

}

DlgStu::~DlgStu()
{
}

void DlgStu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_LEFTUP_POS, m_edtLeftUpPos);
	DDX_Control(pDX, IDC_EDT_LEFTDN_POS, m_edtLeftDnPos);
	DDX_Control(pDX, IDC_EDT_RIGHTUP_POS, m_edtRightUpPos);
	DDX_Control(pDX, IDC_EDT_RIGHTDN_POS, m_edtRightDnPos);
	DDX_Control(pDX, IDC_EDT_LEFTUP_AGL, m_edtLeftUpAgl);
	DDX_Control(pDX, IDC_EDT_LEFTDN_AGL, m_edtLeftDnAgl);
	DDX_Control(pDX, IDC_EDT_RIGHTUP_AGL, m_edtRightUpAgl);
	DDX_Control(pDX, IDC_EDT_RIGHTDN_AGL, m_edtRightDnAgl);
	DDX_Control(pDX, IDC_EDT_LEFTUP_WID, m_edtLeftUpWid);
	DDX_Control(pDX, IDC_EDT_LEFTDN_WID, m_edtLeftDnWid);
	DDX_Control(pDX, IDC_EDT_RIGHTUP_WID, m_edtRightUpWid);
	DDX_Control(pDX, IDC_EDT_RIGHTDN_WID, m_edtRightDnWid);
	DDX_Control(pDX, IDC_EDT_STAND_AGL, m_edtStandAgl);
	DDX_Control(pDX, IDC_EDT_STAND_FRM, m_edtStandFrm);
	DDX_Control(pDX, IDC_EDT_SIT_FRM, m_edtSitFrm);
	DDX_Control(pDX, IDC_EDT_MOVE_DEV, m_edtMoveDev);
	DDX_Control(pDX, IDC_COMBO_MOVE_DLY, m_comboDly);
	DDX_Control(pDX, IDC_SLIDER_ANGLE, m_sliderAngle);
	DDX_Control(pDX, IDC_RADIO_AGL_LEFTUP, m_rdoAglLeftUp);
	DDX_Radio(pDX, IDC_RADIO_AGL_LEFTUP, m_radioAgl);
	DDX_Control(pDX, IDC_SLIDER_WIDTH, m_sliderWidth);
	DDX_Radio(pDX, IDC_RADIO_WIDTH_LEFTUP, m_radioWid);
}


BEGIN_MESSAGE_MAP(DlgStu, CDialog)
	ON_BN_CLICKED(IDC_BTNSTUAPPLY, &DlgStu::OnBnClickedBtnstuapply)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, &DlgStu::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &DlgStu::OnBnClickedButton3)
END_MESSAGE_MAP()


// DlgStu 消息处理程序

void DlgStu::getParameters()
{
	int ret = 0;
	ret = ctrlClient_get_stu_params(m_Connect_clientHandle);
	if (ret != 0)
	{
		OutputDebugString("获取学生参数失败！");
	}
	return;
}

void DlgStu::setParams(StuITRACK_ClientParams_t* params)
{
	stu_params.flag_setting = params->flag_setting;
	stu_params.height = params->height;
	stu_params.stuTrack_debugMsg_flag = params->stuTrack_debugMsg_flag;
	stu_params.stuTrack_direct_range = params->stuTrack_direct_range;
	stu_params.stuTrack_direct_standard[0] = params->stuTrack_direct_standard[0];
	stu_params.stuTrack_direct_standard[1] = params->stuTrack_direct_standard[1];
	stu_params.stuTrack_direct_standard[2] = params->stuTrack_direct_standard[2];
	stu_params.stuTrack_direct_standard[3] = params->stuTrack_direct_standard[3];
	stu_params.stuTrack_Draw_flag = params->stuTrack_Draw_flag;
	stu_params.stuTrack_move_threshold = params->stuTrack_move_threshold;
	stu_params.stuTrack_moveDelayed_threshold = params->stuTrack_moveDelayed_threshold;
	stu_params.stuTrack_sitdownCount_threshold = params->stuTrack_sitdownCount_threshold;
	stu_params.stuTrack_standCount_threshold = params->stuTrack_standCount_threshold;
	stu_params.stuTrack_stuWidth_standard[0] = params->stuTrack_stuWidth_standard[0];
	stu_params.stuTrack_stuWidth_standard[1] = params->stuTrack_stuWidth_standard[1];
	stu_params.stuTrack_stuWidth_standard[2] = params->stuTrack_stuWidth_standard[2];
	stu_params.stuTrack_stuWidth_standard[3] = params->stuTrack_stuWidth_standard[3];
	stu_params.stuTrack_vertex[0] = params->stuTrack_vertex[0];
	stu_params.stuTrack_vertex[1] = params->stuTrack_vertex[1];
	stu_params.stuTrack_vertex[2] = params->stuTrack_vertex[2];
	stu_params.stuTrack_vertex[3] = params->stuTrack_vertex[3];
	stu_params.width = params->width;
	
	stu_params.stretchingAB[0] = params->stretchingAB[0];
	stu_params.stretchingAB[1] = params->stretchingAB[1];

	for (int i = 0; i < 9; i++)
	{
		stu_params.transformationMatrix[i] = params->transformationMatrix[i];
	}
	isSetCam = TRUE;
}

int DlgStu::checkParameters()
{
	CString standAgl, standFrm, sitFrm, moveDev, moveDly;
	int _standAgl, _standFrm, _sitFrm,  _moveDly;
	double _moveDev;
	if (stu_params.stuTrack_vertex[0].x <= 0 || stu_params.stuTrack_vertex[0].y <= 0)
	{
		MessageBox("顶点位置错误！");
		return -1;
	}
	if (stu_params.stuTrack_direct_standard[0] <= 0 || stu_params.stuTrack_direct_standard[1] <= 0
		|| stu_params.stuTrack_direct_standard[2] <= 0 || stu_params.stuTrack_direct_standard[3] <= 0)
	{
		MessageBox("顶点角度错误！");
		return -1;
	}
	if (stu_params.stuTrack_stuWidth_standard[0] <= 0 || stu_params.stuTrack_stuWidth_standard[1] <= 0
		|| stu_params.stuTrack_stuWidth_standard[2] <= 0 || stu_params.stuTrack_stuWidth_standard[3] <= 0)
	{
		MessageBox("顶点宽度错误！");
		return -1;
	}
	m_edtStandAgl.GetWindowText(standAgl);
	m_edtStandFrm.GetWindowText(standFrm);
	m_edtSitFrm.GetWindowText(sitFrm);
	m_edtMoveDev.GetWindowText(moveDev);
	/*_moveDly = m_comboDly.GetCurSel();*/
	m_comboDly.GetWindowText(moveDly);
	//m_edtMoveDly.GetWindowText(moveDly);
	if (standAgl.IsEmpty()||standFrm.IsEmpty()||sitFrm.IsEmpty()||moveDev.IsEmpty()||moveDly.IsEmpty())
	{
		MessageBox("数据不能为空！");
		return -1;
	}
	else
	{
		_standAgl = _ttoi(standAgl);
		_standFrm = _ttoi(standFrm);
		_sitFrm = _ttoi(sitFrm);
		_moveDev = _ttof(moveDev)/100;
		_moveDly = (int)(_ttof(moveDly)*1000);
		if (_standAgl<0)
		{
			MessageBox("起立角度偏移数据错误！");
			return -1;
		}
		if (_standFrm<=0)
		{
			MessageBox("起立阈值数据错误！");
			return -1;
		}
		if (_sitFrm <= 0)
		{
			MessageBox("坐下阈值数据错误！");
			return -1;
		}
		if (_moveDev <= 0)
		{
			MessageBox("移动偏离数据错误！");
			return -1;
		}
		if (_moveDly <= 0)
		{
			MessageBox("移动延时数据错误！");
			return -1;
		}
	}
	if (FALSE==isSetCam)
	{
		MessageBox(_T("相机参数还未设置，请设置相机参数！"));
		return -1;
	}
	stu_params.stuTrack_direct_range = _standAgl;
	stu_params.stuTrack_standCount_threshold = _standFrm;
	stu_params.stuTrack_sitdownCount_threshold = _sitFrm;
	stu_params.stuTrack_move_threshold = _moveDev;
	stu_params.stuTrack_moveDelayed_threshold = _moveDly;
	stu_params.width = WIDTH;
	stu_params.height = HEIGHT;
	stu_params.flag_setting = TRUE;
	stu_params.stuTrack_debugMsg_flag = 1;
	stu_params.stuTrack_Draw_flag = TRUE;
	return 0;
}
void DlgStu::setConnectHandle(Commutication_Handle_t pConnect_clientHandle)
{
	m_Connect_clientHandle = pConnect_clientHandle;
}
void DlgStu::OnBnClickedBtnstuapply()
{
	// TODO:  在此添加控件通知处理程序代码
	if (checkParameters()==0)
	{
		if (m_Connect_clientHandle)
		{
			ctrlClient_set_stu_params(&stu_params, m_Connect_clientHandle);
		}
		else
		{
			MessageBox("未连接服务器！");
		}
		
	}
}


BOOL DlgStu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽    
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;

	if (pMsg->message == WM_LBUTTONUP)
	{
		int agl;
		int temp = 0;
		if (pMsg->hwnd == GetDlgItem(IDC_SLIDER_ANGLE)->m_hWnd)
		{
			updateAngle();
		}
		if ((pMsg->hwnd == GetDlgItem(IDC_SLIDER_WIDTH)->m_hWnd))
		{
			updateWidth();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void DlgStu::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
}



void DlgStu::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (pScrollBar != NULL)
	{
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		if (pSlider->GetDlgCtrlID() == IDC_SLIDER_ANGLE)
		{
			updateAngle();
		}
		if (pSlider->GetDlgCtrlID() == IDC_SLIDER_WIDTH)
		{
			updateWidth();
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

bool DlgStu::save_local_Parameter(std::string filePath, StuITRACK_ClientParams_t* stu_params)
{
	cv::FileStorage fs(filePath, cv::FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "height" << stu_params->height;
		fs << "width" << stu_params->width;
		fs << "stuTrack_debugMsg_flag" << stu_params->stuTrack_debugMsg_flag;
		fs << "stuTrack_Draw_flag" << stu_params->stuTrack_Draw_flag;

		fs << "stuTrack_direct_standard" << "[";
		fs << stu_params->stuTrack_direct_standard[0] << stu_params->stuTrack_direct_standard[1] << stu_params->stuTrack_direct_standard[2] << stu_params->stuTrack_direct_standard[3];
		fs << "]";

		fs << "stuTrack_stuWidth_standard" << "[";
		fs << stu_params->stuTrack_stuWidth_standard[0] << stu_params->stuTrack_stuWidth_standard[1] << stu_params->stuTrack_stuWidth_standard[2] << stu_params->stuTrack_stuWidth_standard[3];
		fs << "]";

		fs << "stuTrack_direct_range" << stu_params->stuTrack_direct_range;
		fs << "stuTrack_standCount_threshold" << stu_params->stuTrack_standCount_threshold;
		fs << "stuTrack_sitdownCount_threshold" << stu_params->stuTrack_sitdownCount_threshold;
		fs << "stuTrack_moveDelayed_threshold" << stu_params->stuTrack_moveDelayed_threshold;
		fs << "stuTrack_move_threshold" << stu_params->stuTrack_move_threshold;

		fs << "stuTrack_vertex" << "[";
		for (int i = 0; i < 4; i++)
		{
			fs << "{";
			fs << "x" << stu_params->stuTrack_vertex[i].x;
			fs << "y" << stu_params->stuTrack_vertex[i].y;
			fs << "}";
		}
		fs << "]";

		fs << "transformationMatrix" << "[";
		for (int i = 0; i < 9; i++)
		{
			fs << stu_params->transformationMatrix[i];
		}
		fs << "]";

		fs << "stretchingAB" << "[";
		fs << stu_params->stretchingAB[0] << stu_params->stretchingAB[1];
		fs << "]";

		fs.release();
		return true;
	}
	else
	{
		return false;
	}
}

bool DlgStu::load_local_Parameter(std::string filePath, StuITRACK_ClientParams_t* stu_params)
{
	cv::FileStorage fs(filePath, cv::FileStorage::READ);
	if (fs.isOpened())
	{
		int i = 0;
		fs["height"] >> stu_params->height;
		fs["width"] >> stu_params->width;
		fs["stuTrack_debugMsg_flag"] >> stu_params->stuTrack_debugMsg_flag;
		fs["stuTrack_Draw_flag"] >> stu_params->stuTrack_Draw_flag;

		cv::FileNode node = fs["stuTrack_direct_standard"];
		if (node.type() != cv::FileNode::SEQ)
		{
			return false;
		}
		cv::FileNodeIterator it = node.begin(), it_end = node.end();
		for (i = 0; it != it_end; ++it, i++)
		{
			stu_params->stuTrack_direct_standard[i] = *it;
		}
		if (i != 4)
		{
			::MessageBox(NULL, _T("角度方向参数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
		}

		node = fs["stuTrack_stuWidth_standard"];
		if (node.type() != cv::FileNode::SEQ)
		{
			return false;
		}
		it = node.begin(), it_end = node.end();
		for (i = 0; it != it_end; ++it, i++)
		{
			stu_params->stuTrack_stuWidth_standard[i] = *it;
		}
		if (i != 4)
		{
			::MessageBox(NULL, _T("学生宽度参数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
		}

		fs["stuTrack_direct_range"] >> stu_params->stuTrack_direct_range;
		fs["stuTrack_standCount_threshold"] >> stu_params->stuTrack_standCount_threshold;
		fs["stuTrack_sitdownCount_threshold"] >> stu_params->stuTrack_sitdownCount_threshold;
		fs["stuTrack_moveDelayed_threshold"] >> stu_params->stuTrack_moveDelayed_threshold;
		fs["stuTrack_move_threshold"] >> stu_params->stuTrack_move_threshold;

		node = fs["stuTrack_vertex"];
		if (node.type() != cv::FileNode::SEQ)
		{
			return false;
		}
		it = node.begin(), it_end = node.end();
		for (i = 0; it != it_end; ++it, i++)
		{
			stu_params->stuTrack_vertex[i].x = (*it)["x"];
			stu_params->stuTrack_vertex[i].y = (*it)["y"];
		}
		if (i != 4)
		{
			::MessageBox(NULL, _T("角点位置参数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
		}

		node = fs["transformationMatrix"];
		if (node.type() != cv::FileNode::SEQ)
		{
			return false;
		}
		it = node.begin(), it_end = node.end();
		for (i = 0; it != it_end; ++it, i++)
		{
			stu_params->transformationMatrix[i] = *it;
		}
		if (i != 9)
		{
			::MessageBox(NULL, _T("变换矩阵参数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
		}

		node = fs["stretchingAB"];
		if (node.type() != cv::FileNode::SEQ)
		{
			return false;
		}
		it = node.begin(), it_end = node.end();
		for (i = 0; it != it_end; ++it, i++)
		{
			stu_params->stretchingAB[i] = *it;
		}
		if (i != 2)
		{
			::MessageBox(NULL, _T("拉伸系数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
		}

		fs.release();
		return true;
	}
	else
	{
		return false;
	}
}

void DlgStu::updateAngle()
{
	HWND p_hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
	CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);

	if (pWnd->pa.x == 0 && pWnd->pa.y == 0 && pWnd->pb.x == 0 && pWnd->pb.y && pWnd->pc.x && pWnd->pc.y && pWnd->pd.x && pWnd->pd.y)
	{
		MessageBox(_T("没有画出跟踪区域，请先设置跟踪区域。"));
		return;
	}

	int agl, temp;

	UpdateData(TRUE);
	switch (m_radioAgl)
	{
	case 0:
		agl = m_sliderAngle.GetPos();
		str.Format(_T("%d"), agl);
		GetDlgItem(IDC_EDT_LEFTUP_AGL)->SetWindowText(str);

		//temp = pWnd->pa.x - 10;
		pWnd->pA.x = pWnd->pa.x + cos(agl * ITC_ANGLE_TO_RADIAN) * 50;
		//temp = pWnd->pa.y - 10;
		pWnd->pA.y = pWnd->pa.y + sin(agl * ITC_ANGLE_TO_RADIAN) * 50;

		break;
	case 1:
		agl = m_sliderAngle.GetPos();
		str.Format(_T("%d"), agl);
		GetDlgItem(IDC_EDT_RIGHTUP_AGL)->SetWindowText(str);

		//temp = pWnd->pb.x - 10;
		pWnd->pB.x = pWnd->pb.x + cos(agl * ITC_ANGLE_TO_RADIAN) * 50;
		//temp = pWnd->pb.y - 10;
		pWnd->pB.y = pWnd->pb.y + sin(agl * ITC_ANGLE_TO_RADIAN) * 50;

		break;
	case 2:
		agl = m_sliderAngle.GetPos();
		str.Format(_T("%d"), agl);
		GetDlgItem(IDC_EDT_LEFTDN_AGL)->SetWindowText(str);

		//temp = pWnd->pd.x - 10;
		pWnd->pD.x = pWnd->pd.x + cos(agl * ITC_ANGLE_TO_RADIAN) * 80;
		//temp = pWnd->pd.y - 10;
		pWnd->pD.y = pWnd->pd.y + sin(agl * ITC_ANGLE_TO_RADIAN) * 80;

		break;
	case 3:
		agl = m_sliderAngle.GetPos();
		str.Format(_T("%d"), agl);
		GetDlgItem(IDC_EDT_RIGHTDN_AGL)->SetWindowText(str);

		//temp = pWnd->pc.x - 10;
		pWnd->pC.x = pWnd->pc.x + cos(agl * ITC_ANGLE_TO_RADIAN) * 80;
		//temp = pWnd->pc.y - 10;
		pWnd->pC.y = pWnd->pc.y + sin(agl * ITC_ANGLE_TO_RADIAN) * 80;

		break;
	default:
		break;
	}
}

void DlgStu::updateWidth()
{
	HWND p_hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
	CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(p_hWnd);

	if (pWnd->pa.x == 0 && pWnd->pa.y == 0 && pWnd->pb.x == 0 && pWnd->pb.y && pWnd->pc.x && pWnd->pc.y && pWnd->pd.x && pWnd->pd.y)
	{
		MessageBox(_T("没有画出跟踪区域，请先设置跟踪区域。"));
		return;
	}

	CPoint v1, v2;
	int mod1 = 0, mod2 = 0;
	v1 = pWnd->pb - pWnd->pa; v2 = pWnd->pd - pWnd->pc;
	mod1 = sqrt(v1.x*v1.x + v1.y*v1.y);
	mod2 = sqrt(v2.x*v2.x + v2.y*v2.y);
	v1.x = v1.x / mod1; v1.y = v1.y / mod1;
	v2.x = v2.x / mod2; v2.y = v2.y / mod2;
	CString str;
	int width;

	UpdateData(TRUE);
	switch (m_radioWid)
	{
	case 0:
		m_sliderWidth.SetRange(0, mod1);
		GetDlgItem(IDC_WIDTH_LEFT)->ShowWindow(TRUE);
		GetDlgItem(IDC_WIDTH_RIGHT)->ShowWindow(TRUE);

		str.Format(_T("%d"), mod1);
		GetDlgItem(IDC_WIDTH_LEFT)->SetWindowText(_T("0"));
		GetDlgItem(IDC_WIDTH_RIGHT)->SetWindowText(str);

		width = m_sliderWidth.GetPos();
		str.Format(_T("%d"), width);
		GetDlgItem(IDC_EDT_LEFTUP_WID)->SetWindowText(str);

		pWnd->ln1[0] = pWnd->pa;
		pWnd->ln1[1].x = pWnd->pa.x + v1.x*width;
		pWnd->ln1[1].y = pWnd->pa.y + v1.y*width;

		break;
	case 1:
		m_sliderWidth.SetRange(0, mod1);
		GetDlgItem(IDC_WIDTH_LEFT)->ShowWindow(TRUE);
		GetDlgItem(IDC_WIDTH_RIGHT)->ShowWindow(TRUE);

		str.Format(_T("%d"), mod1);
		GetDlgItem(IDC_WIDTH_LEFT)->SetWindowText(_T("0"));
		GetDlgItem(IDC_WIDTH_RIGHT)->SetWindowText(str);

		width = m_sliderWidth.GetPos();
		str.Format(_T("%d"), width);
		GetDlgItem(IDC_EDT_RIGHTUP_WID)->SetWindowText(str);

		pWnd->ln2[0] = pWnd->pb;
		pWnd->ln2[1].x = pWnd->pb.x - v1.x*width;
		pWnd->ln2[1].y = pWnd->pb.y - v1.y*width;
		break;
	case 2:
		m_sliderWidth.SetRange(0, mod2);
		GetDlgItem(IDC_WIDTH_LEFT)->ShowWindow(TRUE);
		GetDlgItem(IDC_WIDTH_RIGHT)->ShowWindow(TRUE);

		str.Format(_T("%d"), mod2);
		GetDlgItem(IDC_WIDTH_LEFT)->SetWindowText(_T("0"));
		GetDlgItem(IDC_WIDTH_RIGHT)->SetWindowText(str);

		width = m_sliderWidth.GetPos();
		str.Format(_T("%d"), width);
		GetDlgItem(IDC_EDT_LEFTDN_WID)->SetWindowText(str);

		pWnd->ln4[0] = pWnd->pd;
		pWnd->ln4[1].x = pWnd->pd.x - v2.x*width;
		pWnd->ln4[1].y = pWnd->pd.y - v2.y*width;
		break;
	case 3:
		m_sliderWidth.SetRange(0, mod2);
		GetDlgItem(IDC_WIDTH_LEFT)->ShowWindow(TRUE);
		GetDlgItem(IDC_WIDTH_RIGHT)->ShowWindow(TRUE);

		str.Format(_T("%d"), mod2);
		GetDlgItem(IDC_WIDTH_LEFT)->SetWindowText(_T("0"));
		GetDlgItem(IDC_WIDTH_RIGHT)->SetWindowText(str);

		width = m_sliderWidth.GetPos();
		str.Format(_T("%d"), width);
		GetDlgItem(IDC_EDT_RIGHTDN_WID)->SetWindowText(str);

		pWnd->ln3[0] = pWnd->pc;
		pWnd->ln3[1].x = pWnd->pc.x + v2.x*width;
		pWnd->ln3[1].y = pWnd->pc.y + v2.y*width;
		
		break;
	default:
		break;
	}
}


void DlgStu::OnBnClickedButton2()
{
	//保存到本地
	CString fileName = _T("paraStu.yml");								//默认打开的文件名  
	CString filter = _T("文件 (*.yml)|*.yml|文件（*.xml)|*.xml||");		//文件过虑的类型  
	CFileDialog openFileDlg(FALSE, NULL, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		CString FilePathName = openFileDlg.GetPathName();
		save_local_Parameter(FilePathName.GetBuffer(0), &stu_params);
	}
}


void DlgStu::OnBnClickedButton3()
{
	//从本地加载
	CString fileName = _T("paraStu.yml");								//默认打开的文件名  
	CString filter = _T("文件 (*.yml)|*.yml|文件（*.xml)|*.xml||");		//文件过虑的类型  
	CFileDialog openFileDlg(TRUE, NULL, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		CString FilePathName = openFileDlg.GetPathName();
		StuITRACK_ClientParams_t temp;
		memset(&temp, 0, sizeof(StuITRACK_ClientParams_t));
		load_local_Parameter(FilePathName.GetBuffer(0), &temp);
		HWND hWnd = ::FindWindow(NULL, _T("MFCTrackTools"));
		CMFCTrackToolsDlg *pWnd = (CMFCTrackToolsDlg *)FromHandle(hWnd);
		pWnd->updateParamsFromStu(&temp);
	}
}
