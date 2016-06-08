
// MFCTrackToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "MFCTrackToolsDlg.h"
#include "afxdialogex.h"

//#include "track_client_commintication.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define UP_PIXEL 20
#define PIC_TOP 60


#pragma comment(lib,"SkinPPWTL.lib")
#include "SkinPPWTL.h"
static Track_cmd_info_t g_track_cmd[] =
{
	{ STU_SETTRACK_CMD, "设置学生参数" },
	{ TEA_SETTRACK_CMD, "设置老师参数" },
	{ STU_GETTRACK_CMD, "获取学生参数" },
	{ TEA_GETTRACK_CMD, "获取老师参数" },
	{ GET_CAMERA_INFO, "获取相机参数" },
	{ SET_TRACK_STATUS_CMD, "设置跟踪状态" },

	{ GET_TRACK_STATUS_CMD, "获取跟踪状态" },
	{ PLC_GETTRACK_CMD, "获取策略状态" },
	{ PLC_SETTRACK_CMD, "设置跟踪状态" },
	{ GET_TRACK_DEBUG_CMD, "获取调试状态"},
	{SET_TRACK_DEBUG_CMD, "设置调试状态"},
	{ TRACK_LOGIN, "登录" },
};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMFCTrackToolsDlg 对话框


//构造函数，初始化值
CMFCTrackToolsDlg::CMFCTrackToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCTrackToolsDlg::IDD, pParent)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_track_clientHandle = NULL;
	m_strSkin = "";
	m_streamTeaHandle = NULL;
	m_streamStuHandle = NULL;

	m_imgbufferYUV.create(HEIGHT * 3 / 2, WIDTH, CV_8UC1);
}

void CMFCTrackToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_picSrc, m_picOverall);

	DDX_Control(pDX, IDC_picCam, m_picFeature);

	//DDX_Control(pDX, IDC_grpBoxCam, m_grpBoxCam);

	DDX_Control(pDX, IDC_tabTrack, m_tabTrack);
	DDX_Control(pDX, IDC_CHECK1, m_check_algFlag);
	DDX_Control(pDX, IDC_CHECK2, m_check_stuFlag);
}

//消息映射
BEGIN_MESSAGE_MAP(CMFCTrackToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabTrack, &CMFCTrackToolsDlg::OnTcnSelchangetabtrack)
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CHECK1, &CMFCTrackToolsDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCTrackToolsDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CMFCTrackToolsDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMFCTrackToolsDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CMFCTrackToolsDlg::OnBnClickedBtnLoad)
END_MESSAGE_MAP()


// CMFCTrackToolsDlg 消息处理程序

BOOL CMFCTrackToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//m_check_algFlag.SetCheck(FALSE);
	//m_check_stuFlag.SetCheck(FALSE);
	//m_isAlgActivity.isTchTrack = m_isAlgActivity.isStuTrack = FALSE;
	// TODO:  在此添加额外的初始化代码
	return initProgramControl();
}
static int video_call_back(Decode_Info_t *pInfo, void*param)
{
	CMFCTrackToolsDlg* pSelf = (CMFCTrackToolsDlg*)param;
	if (pSelf == NULL || pInfo == NULL || pInfo->data == NULL)
	{
		//OutputDebugString("video_call_back is NULL\n");
	}
	pSelf->video_display(pInfo);
	pSelf->showImage();
	return 0;
}
int CMFCTrackToolsDlg::video_display(Decode_Info_t *pInfo)
{
	//OutputDebugString("stream=============\n");
	int nResult = StretchDIBits(hdc,
		0, 0,
		WIDTH, HEIGHT,
		0, 0,
		WIDTH, HEIGHT,
		pInfo->data,
		&m_bmphdr,
		DIB_RGB_COLORS,
		SRCCOPY);

	//pDC->Rectangle(picRect);
	//CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	//CPen *pOldPen = pDC->SelectObject(&pen);
	//pDC->SetDCPenColor(RGB(255, 0, 0));
	trackdraw();
	//showImage();
	pDC->SelectObject(pOldPen);
	

	//pDC->Rectangle(50, 50, 100, 100);
	//pDC->RectVisible(CRect(50, 50, 100, 100));
	//pDC->SelectObject(pOldPen);

	return 0;
}

void  FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void CMFCTrackToolsDlg::showImage()
{
	if (CurSel==TCH_TAB)
	{
		cv::Mat temp = m_tch_cam.getImageBuffer();
		if (!temp.empty())
		{
			cv::resize(temp, m_imgbufferYUV, cv::Size(m_imgbufferYUV.cols, m_imgbufferYUV.rows));
			cvtColor(m_imgbufferYUV, m_imgbufferShow_tch, CV_YUV2BGR_I420);
			IplImage m_showImage(m_imgbufferShow_tch);
			uchar buffer[sizeof(BITMAPINFOHEADER)+1024];
			BITMAPINFO* bmi = (BITMAPINFO*)buffer;
			int bmp_w = m_showImage.width, bmp_h = m_showImage.height;
			int bpp = (m_showImage.depth & 255)*m_showImage.nChannels;
			//代替cvvimage的Bpp()函数
			FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, m_showImage.origin);
			/*CRect ShowRect;
			GetDlgItem(IDC_picFeature)->GetClientRect(&ShowRect);
			CDC *dc = GetDlgItem(IDC_picFeature)->GetDC();*/
			SetStretchBltMode(pDC2->GetSafeHdc(), COLORONCOLOR);
			StretchDIBits(
				pDC2->GetSafeHdc(), showRectFeature.left, showRectFeature.top, showRectFeature.Width(), showRectFeature.Height(), 0, 0, bmp_w, bmp_h,
				m_showImage.imageData, bmi, DIB_RGB_COLORS, SRCCOPY);
		}
		else
		{

			logFile.WriteString("----->教师相机特写没获取到流");
			logFile.Write(("\r\n"), 2);
		}
	}
	else if (STU_TAB==CurSel)
	{
		cv::Mat temp = m_stu_cam.getImageBuffer();
		if (!temp.empty())
		{
			cv::resize(temp, m_imgbufferYUV, cv::Size(m_imgbufferYUV.cols, m_imgbufferYUV.rows));
			cvtColor(m_imgbufferYUV, m_imgbufferShow_tch, CV_YUV2BGR_I420);
			IplImage m_showImage(m_imgbufferShow_tch);
			uchar buffer[sizeof(BITMAPINFOHEADER)+1024];
			BITMAPINFO* bmi = (BITMAPINFO*)buffer;
			int bmp_w = m_showImage.width, bmp_h = m_showImage.height;
			int bpp = (m_showImage.depth & 255)*m_showImage.nChannels;
			//代替cvvimage的Bpp()函数
			FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, m_showImage.origin);
			/*CRect ShowRect;
			GetDlgItem(IDC_picFeature)->GetClientRect(&ShowRect);
			CDC *dc = GetDlgItem(IDC_picFeature)->GetDC();*/
			SetStretchBltMode(pDC2->GetSafeHdc(), COLORONCOLOR);
			StretchDIBits(
				pDC2->GetSafeHdc(), showRectFeature.left, showRectFeature.top, showRectFeature.Width(), showRectFeature.Height(), 0, 0, bmp_w, bmp_h,
				m_showImage.imageData, bmi, DIB_RGB_COLORS, SRCCOPY);
		}
		else
		{
			logFile.WriteString("----->学生相机特写没获取到流");
			logFile.Write(("\r\n"), 2);
		}
		
	}
	//绘制特写镜头的十字光标
	pOldPen = pDC2->SelectObject(&penR);
	pDC2->MoveTo(CPoint(centre_pt.x, centre_pt.y - 10));
	pDC2->LineTo(CPoint(centre_pt.x, centre_pt.y + 10));

	pDC2->MoveTo(CPoint(centre_pt.x - 10, centre_pt.y));
	pDC2->LineTo(CPoint(centre_pt.x + 10, centre_pt.y));

	pDC2->SelectObject(&pOldPen);
}

BOOL CMFCTrackToolsDlg::initNetCommuntication()
{
	memset(&m_bmphdr, 0, sizeof(BITMAPINFO));
	DWORD dwBmpHdr = sizeof(BITMAPINFO);
	m_bmphdr.bmiHeader.biBitCount = 24;
	m_bmphdr.bmiHeader.biClrImportant = 0;
	m_bmphdr.bmiHeader.biSize = dwBmpHdr;
	m_bmphdr.bmiHeader.biSizeImage = 0;
	m_bmphdr.bmiHeader.biWidth = 480;//pixel_w;
	//注意BMP在y方向是反着存储的，一次必须设置一个负值，才能使图像正着显示出来  
	m_bmphdr.bmiHeader.biHeight = -264;//-pixel_h;
	m_bmphdr.bmiHeader.biXPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biYPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biClrUsed = 0;
	m_bmphdr.bmiHeader.biPlanes = 1;
	m_bmphdr.bmiHeader.biCompression = BI_RGB;

	
	m_connectDialog.setConectfunCall(ctrlClient_init_trackCommunticationEx, this);
	//INT res = m_connectDialog.DoModal();
	if (m_connectDialog.DoModal() == IDCANCEL)
		exit(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE

}
BOOL CMFCTrackToolsDlg::ctrlClient_init_Stream()
{
	RecvStream_Handle_t recv_stream_handle;
	recv_stream_handle.channel = 0;
	strcpy_s(recv_stream_handle.iP, sizeof(recv_stream_handle.iP), m_trackIp);
	recv_stream_handle.port = TEACH_STREAM_PORT;
	recv_stream_handle.channel = 1;
	recv_stream_handle.param1 = this;
	recv_stream_handle.call_back_fun = video_call_back;
	m_streamTeaHandle = init_stream_recv(&recv_stream_handle);
	logFile.WriteString("----->教师分析流初始化完毕");
	logFile.Write(("\r\n"), 2);
	recv_stream_handle.port = STUDENT_STREAM_PORT;
	m_streamStuHandle = init_stream_recv(&recv_stream_handle);
	logFile.WriteString("----->学生分析流初始化完毕");
	logFile.Write(("\r\n"), 2);
	return TRUE;

}
BOOL CMFCTrackToolsDlg::initProgramControl()
{
	//初始化窗口
	CRect rs1;
	CWnd::GetClientRect(rs1);

	//CWnd::SetWindowPos(NULL, 0, 0, 960, 720, SWP_NOZORDER);//初始化窗口大小
	//CenterWindow();//窗口居中

	//初始化控件
	CRect rs;
	int cx = 0;
	int cy = 0;
	CRect rectTrackClient;
	GetDlgItem(IDC_tabTrack)->GetWindowRect(&rectTrackClient);//获取控件相对于屏幕的位置
	ScreenToClient(rectTrackClient);//转化为对话框上的相对位置

	m_picOverall.SetWindowPos(NULL, MARGIN_LEFT, rectTrackClient.top + 20, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_picFeature.SetWindowPos(NULL, MARGIN_LEFT, rectTrackClient.top + 350, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_picOverall.GetClientRect(&rs);

	pic_top = rectTrackClient.top + 20;

	m_tabTrack.InsertItem(TCH_TAB, "教师");
	m_tabTrack.InsertItem(STU_TAB, "学生");
	m_tabTrack.InsertItem(CTRL_TAB, "设置");
	
	//绑定dlgTch到Tab控件
	dlgTch.Create(IDD_DlgTch, GetDlgItem(IDC_tabTrack));
	dlgStu.Create(IDD_DlgStu, GetDlgItem(IDC_tabTrack));
	dlgCtrl.Create(IDD_DLGGLOBAL, GetDlgItem(IDC_tabTrack));

	initCamDlg(cx, cx, rectTrackClient);
	

	//移动控件
	m_tabTrack.GetClientRect(&rs);
	cx = rs.right - rs.left;
	cy = rs.bottom - rs.top - 20;
	rs.top += 20;
	dlgTch.MoveWindow(rs);
	dlgTch.ShowWindow(TRUE);
	dlgStu.MoveWindow(rs);
	dlgStu.ShowWindow(FALSE);
	dlgCtrl.MoveWindow(rs);
	dlgCtrl.ShowWindow(FALSE);
	
	
	m_tabTrack.SetCurSel(TCH_TAB);

	dlgTch.tch_params = { 0 };
	dlgStu.stu_params = { 0 };
	
	//初始化DlgTch中的控件
	dlgTch.m_editPos.SetWindowText(_T("大于滑框宽度"));
	dlgTch.m_comboSlide.InsertString(0,_T("3"));
	dlgTch.m_comboSlide.InsertString(1,_T("5"));
	dlgTch.m_comboSlide.InsertString(2,_T("7"));
	dlgTch.m_comboSlide.InsertString(3,_T("9"));
	dlgTch.m_comboStand.InsertString(0, _T("1"));
	dlgTch.m_comboStand.InsertString(1, _T("2"));
	dlgTch.m_comboStand.InsertString(2, _T("3"));
	dlgTch.m_comboStand.InsertString(3, _T("4"));
	dlgTch.m_comboStand.InsertString(4, _T("5"));

	//初始化DlgStu中的控件
	dlgStu.m_edtStandAgl.SetWindowText(_T("0~360度"));
	dlgStu.m_edtStandFrm.SetWindowText(_T("3~10帧"));
	dlgStu.m_edtSitFrm.SetWindowText(_T("3~10帧"));
	dlgStu.m_edtMoveDev.SetWindowText(_T("20~200"));
	dlgStu.m_comboDly.InsertString(0, _T("0.5"));
	dlgStu.m_comboDly.InsertString(1, _T("1.0"));
	dlgStu.m_comboDly.InsertString(2, _T("1.5"));
	dlgStu.m_comboDly.InsertString(3, _T("2.0"));
	dlgStu.m_comboDly.InsertString(4, _T("2.5"));
	dlgStu.m_comboDly.InsertString(5, _T("3.0"));
	dlgStu.m_sliderAngle.SetRange(0, 360);
	/*int pos;
	CString tempPos;
	switch (dlgStu.m_radioAgl)
	{
	case 0:
		dlgStu.m_edtRightUpAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderAngle.SetPos(pos);
	case 1:
		dlgStu.m_edtRightUpAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderAngle.SetPos(pos);
	case 2:
		dlgStu.m_edtLeftDnAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderAngle.SetPos(pos);
	case 3:
		dlgStu.m_edtRightDnAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderAngle.SetPos(pos);
	}

	switch (dlgStu.m_radioWid)
	{
	case 0:
		dlgStu.m_edtRightUpWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderWidth.SetPos(pos);
	case 1:
		dlgStu.m_edtRightUpWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderWidth.SetPos(pos);
	case 2:
		dlgStu.m_edtLeftDnWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderWidth.SetPos(pos);
	case 3:
		dlgStu.m_edtRightDnWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);
		dlgStu.m_sliderWidth.SetPos(pos);
	}*/

	//初始化dlgCtrl中的控件
	dlgCtrl.m_chk_multiple.SetCheck(TRUE);
	dlgCtrl.m_chk_multiple.SetWindowText(_T("开启多画面"));
	dlgCtrl.m_chk_stuOverview.SetCheck(TRUE);
	dlgCtrl.m_chk_stuOverview.SetWindowText(_T("开启学生特写"));

	p1.x = 0;
	p1.y = 0;
	p2.x = 0;
	p2.y = 0;


	pDC = GetDlgItem(IDC_picSrc)->GetDC();
	hdc = pDC->GetSafeHdc();
	GetDlgItem(IDC_picSrc)->GetClientRect(&picRect);
	penY.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	penG.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	penR.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	penB.CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	penDB.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	penPink.CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	penPurple.CreatePen(PS_SOLID, 1, RGB(155, 155, 255));
	pOldBrush = pDC->SelectObject(pBrush);

	//用于画标定特写镜头时的中心标志
	pDC2 = GetDlgItem(IDC_picFeature)->GetDC();
	//CRect showRect;
	GetDlgItem(IDC_picFeature)->GetClientRect(&showRectFeature);
	centre_pt.x = showRectFeature.left + showRectFeature.Width() / 2;
	centre_pt.y = showRectFeature.top + showRectFeature.Height() / 2;

	dlgCam.m_txtPreset.SetWindowText("");
	dlgCam.m_checkCam.SetCheck(TRUE);
	//SetTimer(1,40, NULL);

	/*GetModuleFileName(GetModuleHandle(0), m_pExeDir, MAX_PATH);
	CString str(m_pExeDir);
	int n = str.ReverseFind('\\');
	str = str.Left(n);

	memcpy(m_pExeDir, str.GetString(), MAX_PATH);
	m_strSkin = m_pExeDir;
	m_strSkin += SKINNAME;
	skinppLoadSkin((char*)m_strSkin.GetString());*/
	return initNetCommuntication();

}
void CMFCTrackToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCTrackToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		//CWnd::SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
		GetClientRect(&rect);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCTrackToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCTrackToolsDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	g_flag = 1;
	if (MessageBox(_T("确定退出吗"), _T("提示"), MB_YESNO | MB_ICONWARNING)
		== IDNO)
		return;

	//开启跟踪调试
	ctrlClient_set_track_debug(0, m_track_clientHandle);
	communtication_set_handleStatus(m_track_clientHandle, STOP_STATUS);
	stop_stream_stream(m_streamTeaHandle);
	stop_stream_stream(m_streamStuHandle);
	m_tch_cam.StreamStop();
	m_tch_cam.logout();


	m_stu_cam.StreamStop();
	m_stu_cam.logout();


	dlgCam.m_CameraControl_tch.stopControl();
	dlgCam.m_CameraControl_stu.stopControl();

	logFile.Flush();
	logFile.Close();


	
	WSACleanup();			//释放网络连接资源
	skinppExitSkin();
	exit(0);
	CDialogEx::OnClose();
}

void CMFCTrackToolsDlg::drawRectangle(CPoint a, CPoint b, CPoint c, CPoint d)
{
	pDC->MoveTo(a);
	pDC->LineTo(b);
	pDC->MoveTo(b);
	pDC->LineTo(c);
	pDC->MoveTo(c);
	pDC->LineTo(d);
	pDC->MoveTo(d);
	pDC->LineTo(a);
}

void CMFCTrackToolsDlg::drawRectangle(CPoint a, CPoint c)
{
	CPoint b, d;
	b.x = c.x; b.y = a.y;
	d.x = a.x, d.y = c.y;
	pDC->MoveTo(a);
	pDC->LineTo(b);
	pDC->MoveTo(b);
	pDC->LineTo(c);
	pDC->MoveTo(c);
	pDC->LineTo(d);
	pDC->MoveTo(d);
	pDC->LineTo(a);
}

void CMFCTrackToolsDlg::drawLine(CPoint a, CPoint b)
{
	pDC->MoveTo(a);
	pDC->LineTo(b);
}

void CMFCTrackToolsDlg::drawLines(int flag)
{
	if (DRAW_ANGLE==flag)
	{
		pOldPen = pDC->SelectObject(&penB);
		if (pA.x > 0 && pA.y > 0)
		{
			drawLine(pa, pA);
			drawEndRect(pA,10);
		}
		if (pB.x > 0 && pB.y > 0)
		{
			drawLine(pb, pB);
			drawEndRect(pB,10);
		}
		if (pC.x > 0 && pC.y > 0)
		{
			drawLine(pc, pC);
			drawEndRect(pC,10);
		}
		if (pD.x > 0 && pD.y > 0)
		{
			drawLine(pd, pD);
			drawEndRect(pD,10);
		}
	}
	else
	{
		//penG.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		pOldPen = pDC->SelectObject(&penR);
		if (ln1[0].x > 0 && ln1[0].y > 0 && ln1[1].x > 0 && ln1[1].y > 0)
		{
			drawLine(ln1[0], ln1[1]);
			drawEndRect(ln1[1],5);
		}
		if (ln2[0].x > 0 && ln2[0].y > 0 && ln2[1].x > 0 && ln2[1].y > 0)
		{
			drawLine(ln2[0], ln2[1]);
			drawEndRect(ln2[1],5);
		}
		if (ln3[0].x > 0 && ln3[0].y > 0 && ln3[1].x > 0 && ln3[1].y > 0)
		{
			drawLine(ln3[0], ln3[1]);
			drawEndRect(ln3[1],5);
		}
		if (ln4[0].x > 0 && ln4[0].y > 0 && ln4[1].x > 0 && ln4[1].y > 0)
		{
			drawLine(ln4[0], ln4[1]);
			drawEndRect(ln4[1],5);
		}
	}
}

void CMFCTrackToolsDlg::drawEndRect(CPoint center, int size)
{
	//CBrush brush(RGB(0, 255, 0));
	CRect rct = CRect(center.x - size, center.y - size, center.x + size, center.y + size);
	pDC->Rectangle(&rct);
	pOldBrush = pBrush;
}

void CMFCTrackToolsDlg::trackdraw()
{
	if (CurSel == TCH_TAB)
	{
		if (g_drawPS == 1)
		{
			//for (int i = 0; i < Frame_Width; i += (Frame_Width / int_pos))
			//{
			//	cvLine(srcImg, cvPoint(i, 0), cvPoint(i, Frame_Height), cvScalar(0, 0, 255));
			//	cvRectangle(srcImg, cvPoint(camPosSlide.left*(Frame_Width / int_pos), 0), cvPoint((camPosSlide.right + 1) * (Frame_Width / int_pos), Frame_Height), cvScalar(255, 0, 0));//画预置位滑框
			//}
			if (!m_isAlgActivity.isTchTrack)
			{
				if (int_pos > 0)
				{
					pOldPen = pDC->SelectObject(&penDB);
					drawRectangle(CPoint(camPosSlide.left*(tch.width / int_pos)+tch.x, tch.y), CPoint((camPosSlide.right + 1) * (tch.width / int_pos)+tch.x, tch.y + tch.height));

					pOldPen = pDC->SelectObject(&penY);
					drawEndRect(CPoint((tch.width / int_pos) / 2+tch.x, tch.y + tch.height / 2), 10);
					drawEndRect(CPoint(tch.width - (tch.width / int_pos) / 2+tch.x, tch.y + tch.height / 2), 10);

					if (p4.x > 0 && p4.y > 0)
					{
						pOldPen = pDC->SelectObject(&penPink);
						drawRectangle(p3, p4);
					}
				}
			}
			
			//绘制特写镜头的十字光标
			pOldPen = pDC->SelectObject(&penR);
			pDC->MoveTo(CPoint(centre_pt2.x, centre_pt2.y - 10));
			pDC->LineTo(CPoint(centre_pt2.x, centre_pt2.y + 10));

			pDC->MoveTo(CPoint(centre_pt2.x - 10, centre_pt2.y));
			pDC->LineTo(CPoint(centre_pt2.x + 10, centre_pt2.y));

			pDC->SelectObject(&pOldPen);
		}

		pOldPen = pDC->SelectObject(&penY);
		drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
		pOldPen = pDC->SelectObject(&penG);
		drawRectangle(CPoint(blk.x, blk.y), CPoint(blk.x + blk.width, blk.y + blk.height));
		pOldPen = pDC->SelectObject(&penPurple);
		drawLine(pl, pr);
		
		/*pOldPen = pDC->SelectObject(&penPurple);
		drawLine(pl, pr);*/
		if (mouseStatus == Mouse_LBDOWN)
		{
			if (pt.x > 0 && pt.y > 0)
			{
				pOldPen = pDC->SelectObject(&penR);
				drawRectangle(p1, pt);
			}
		}
		else if (mouseStatus == Mouse_LBUP) 
		{
			SetDlgItemInt(IDC_editX, p1.x);
			SetDlgItemInt(IDC_editY, p1.y);
			SetDlgItemInt(IDC_editW, p2.x - p1.x);
			SetDlgItemInt(IDC_editH, p2.y - p1.y);
		}
		if (mouseStatus == Mouse_RBDOWN)
		{
			if (pt.x > 0 && pt.y > 0)
			{
				pOldPen = pDC->SelectObject(&penPink);
				drawRectangle(p3, pt);
			}
		}
	}
	else
	{
		if (mouseStatus == Mouse_LBDOWN)
		{
			if (pt.x > 0 && pt.y > 0)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(pa, pt);
			}
		}
		if (mouseStatus == Mouse_LBUP)
		{
			pOldPen = pDC->SelectObject(&penY);
			if (pa.x > 0 && pa.y > 0 && pc.x > 0 && pc.y > 0)
			{
				drawRectangle(pa, pb, pc, pd);
				drawEndRect(pa, 10);
				drawEndRect(pb, 10);
				drawEndRect(pc, 10);
				drawEndRect(pd, 10);
				
			}
			drawLines(DRAW_WIDTH);
		}
		else if (mouseStatus == Mouse_LBDRAG)
		{
			pOldPen = pDC->SelectObject(&penY);
			drawRectangle(pa, pb, pc, pd);
			drawEndRect(pa, 10);
			drawEndRect(pb, 10);
			drawEndRect(pc, 10);
			drawEndRect(pd, 10);
			drawLines(DRAW_WIDTH);
		}
		else
		{
			pOldPen = pDC->SelectObject(&penY);
			if (pa.x > 0 && pa.y > 0 && pc.x > 0 && pc.y > 0)
			{
				drawRectangle(pa, pb, pc, pd);
				drawEndRect(pa, 10);
				drawEndRect(pb, 10);
				drawEndRect(pc, 10);
				drawEndRect(pd, 10);
				
			}
			drawLines(DRAW_WIDTH);
			//drawLines(DRAW_ANGLE);
		}
		if (mouseStatus == Mouse_RBDOWN)
		{
			if (pt.x > 0 && pt.y > 0 && p1.x > 0 && p1.y > 0)
			{
				pOldPen = pDC->SelectObject(&penB);
				drawLine(p1, pt);
			}
			drawLines(DRAW_ANGLE);
			drawLines(DRAW_WIDTH);
		}
		else if (mouseStatus == Mouse_RBUP)
		{
			drawLines(DRAW_ANGLE);
			drawLines(DRAW_WIDTH);
		}
		else if (mouseStatus == Mouse_RBDRAG)
		{
			drawLines(DRAW_ANGLE);
			drawLines(DRAW_WIDTH);
		}
		else
		{
			//drawLines(DRAW_ANGLE);
			pOldPen = pDC->SelectObject(&penY);
			if (pa.x > 0 && pa.y > 0 && pc.x > 0 && pc.y > 0)
			{
				drawRectangle(pa, pb, pc, pd);
				drawEndRect(pa, 10);
				drawEndRect(pb, 10);
				drawEndRect(pc, 10);
				drawEndRect(pd, 10);
				
			}
			drawLines(DRAW_WIDTH);
			drawLines(DRAW_ANGLE);
		}
	}
}

//int CMFCTrackToolsDlg::minimalDistance(CPoint in)
//{
//	int dist = getDistance(in, pa);
//	int rst = 0;
//	if (getDistance(in,pb)<dist)
//	{
//		dist = getDistance(in, pb);
//		rst = 1;
//	}
//	if (getDistance(in, pc) < dist)
//	{
//		dist = getDistance(in, pc);
//		rst = 2;
//	}
//	if (getDistance(in, pd) < dist)
//	{
//		dist = getDistance(in, pd);
//		rst = 3;
//	}
//	return rst;
//}

int CMFCTrackToolsDlg::getDistance(CPoint a, CPoint b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void CMFCTrackToolsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	if (MARGIN_LEFT <= point.x&&point.x <= MARGIN_LEFT + Frame_Width&&pic_top <= point.y&&point.y <= pic_top + Frame_Height)
	{
		//if (p1.x<point.x - MARGIN_LEFT && point.x - MARGIN_LEFT<p2.x&&p1.y<point.y - pic_top&&point.y - pic_top<p2.y)
		if (CurSel==TCH_TAB)
		{
			if (m_check_algFlag.GetCheck())
			{
				MessageBox(_T("请先关闭教师跟踪！"));
				return;
			}
			if ((tch.x < point.x - MARGIN_LEFT && point.x - MARGIN_LEFT < tch.x + tch.width&&tch.y < point.y - pic_top&&point.y - pic_top < tch.y + tch.height))
			{
				if (isKeyDown==KEY_ERROR)
				{
					MessageBox("组合键错误，不可同时按下CTRL和SHIFT！");
					isKeyDown = CTRL_KEY_UP;
					return;
				}
				if (isKeyDown==CTRL_KEY_DOWN)
				{
					p3 = p4 = { 0 };
					pt = { 0 };
					p3.x = point.x - MARGIN_LEFT;
					p3.y = point.y - pic_top;
					mouseStatus = Mouse_RBDOWN;
				}
				else if (isKeyDown==SHIFT_KEY_DOWN)
				{
					pl.x = 0;
					pl.y = point.y - pic_top;
					mouseStatus = Mouse_RBDOWN;
				}
				else
				{
					pt.x = point.x;
					pt.y = point.y;
					whichRect = 1;
					mouseStatus = Mouse_LBDRAG;
				}
			}
			else if ((blk.x < point.x - MARGIN_LEFT && point.x - MARGIN_LEFT < blk.x + blk.width&&blk.y < point.y - pic_top&&point.y - pic_top < blk.y + blk.height))
			{
				pt.x = point.x;
				pt.y = point.y;
				whichRect = 2;
				mouseStatus = Mouse_LBDRAG;
			}
			else
			{
				whichRect = 0;
				pt.x = 0, pt.y = 0;
				p1.x = point.x - MARGIN_LEFT;
				p1.y = point.y - pic_top;
				//p2.x = 0, p2.y = 0;
				mouseStatus = Mouse_LBDOWN;
				if (mouseCnt == 2)
				{
					mouseCnt = 0;
					tch = { 0 };
					blk = { 0 };
				}
			}
		}
		else
		{
			//学生跟踪框拖拽bug
			/*if (pa.x+10<point.x-MARGIN_LEFT&&pb.x+10<point.x-MARGIN_LEFT&&point.x-MARGIN_LEFT<pc.x-10&&point.x-MARGIN_LEFT<pd.x&&pa.y+10<point.y-pic_top&&pb.y+10<point.y-pic_top&&point.y-pic_top<pc.y-10&&point.y-pic_top<pd.y-10)
			{
				pt.x = point.x;
				pt.y = point.y;
				mouseStatus = Mouse_LBDRAG;
				pA = pB = pC = pD = { 0 };
			}
			else */
			if (pa.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pa.x + 10 && pa.y - 10 <= point.y - pic_top&&point.y - pic_top <= pa.y + 10)
			{
				if (pa.x!=point.x&&pa.y!=point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_A;
					//pA = pB = pC = pD = { 0 };
				}
			}
			else if (pb.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pb.x + 10 && pb.y - 10 <= point.y - pic_top&&point.y - pic_top <= pb.y + 10)
			{
				if (pb.x != point.x&&pb.y != point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_B;
					//pA = pB = pC = pD = { 0 };
				}
			}
			else if (pc.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pc.x + 10 && pc.y - 10 <= point.y - pic_top&&point.y - pic_top <= pc.y + 10)
			{
				if (pc.x != point.x&&pc.y != point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_C;
					//pA = pB = pC = pD = { 0 };
				}
			}
			else if (pd.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pd.x + 10 && pd.y - 10 <= point.y - pic_top&&point.y - pic_top <= pd.y + 10)
			{
				if (pd.x != point.x&&pd.y != point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_D;
					//pA = pB = pC = pD = { 0 };
				}
			}
			else
			{
				//mouseCnt--;
				pa = pb = pc = pd = { 0 };
				pA = pB = pC = pD = { 0 };
				pt.x = 0, pt.y = 0;
				pa.x = point.x - MARGIN_LEFT;
				pa.y = point.y - pic_top;
				mouseStatus = Mouse_LBDOWN;
				pA = pB = pC = pD = { 0 };
				p1 = p2 = { 0 };
				ln1[0] = ln1[1] = ln2[0] = ln2[1] = ln3[0] = ln3[1] = ln4[0] = ln4[1] = { 0 };
			}
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCTrackToolsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (MARGIN_LEFT <= point.x&&point.x <= MARGIN_LEFT + Frame_Width && pic_top <= point.y&&point.y <= pic_top + Frame_Height)
	{
		if (CurSel==TCH_TAB)
		{
			if (mouseStatus != Mouse_LBDRAG)
			{
				if (mouseStatus==Mouse_LBDOWN)
				{
					mouseCnt++;
					if (mouseCnt == 1)
					{
						/*p1.x = 0;
						p2.x = Frame_Width;*/
						p2.x = pt.x;
					}
					if (mouseCnt == 2)
					{
						//p1.x = 0;//修改板书范围
						p2.x = pt.x;
						//p2.x = Frame_Width;
					}
					//p2.x = point.x - MARGIN_LEFT;
					p2.y = point.y - pic_top;
					mouseStatus = Mouse_LBUP;
					pt.x = -1;
					pt.y = -1;

					if (p2.x < p1.x || p2.y<p1.y)
					{
						pt = p2;
						p2 = p1;
						p1 = pt;
					}
					if (p2.x>p1.x&&p2.y < p1.y)
					{
						pt.y = p1.y;
						p1.y = p2.y;
						p2.y = pt.y;
					}
					if (p2.x<p1.x&&p2.y>p1.y)
					{
						pt.x = p1.x;
						p1.x = p2.x;
						p2.x = pt.x;
					}
					if (mouseCnt == 1)
					{
						tch.x = p1.x; tch.y = p1.y;
						tch.width = p2.x - p1.x;
						tch.height = p2.y - p1.y;

						dlgTch.setTrackRects(tch, TCH_RECT);

						tmp = _T(" ,");
						s.Format(_T("%d"), tch.x);
						str += s;
						str += tmp;
						dlgTch.m_editX.SetWindowTextA(s);
						s.Format(_T("%d"), tch.y);
						str += s;
						str += tmp;
						dlgTch.m_editY.SetWindowTextA(s);
						s.Format(_T("%d"), tch.width);
						str += s;
						str += tmp;
						dlgTch.m_editW.SetWindowTextA(s);
						s.Format(_T("%d"), tch.height);
						str += s;
						dlgTch.m_editH.SetWindowTextA(s);
						dlgTch.m_txtTchArg.SetWindowTextA(str);
						str = _T("");
					}
					if (mouseCnt == 2)
					{
						blk.x = p1.x;
						blk.y = p1.y;
						blk.width = p2.x - p1.x;
						blk.height = p2.y - p1.y;

						dlgTch.setTrackRects(blk, BLK_RECT);

						tmp = _T(" ,");
						s.Format(_T("%d"), blk.x);
						str += s;
						str += tmp;
						dlgTch.m_editX.SetWindowTextA(s);
						s.Format(_T("%d"), blk.y);
						str += s;
						str += tmp;
						dlgTch.m_editY.SetWindowTextA(s);
						s.Format(_T("%d"), blk.width);
						str += s;
						str += tmp;
						dlgTch.m_editW.SetWindowTextA(s);
						s.Format(_T("%d"), blk.height);
						str += s;
						dlgTch.m_editH.SetWindowTextA(s);
						dlgTch.m_txtBlkArg.SetWindowTextA(str);
						str = _T("");
					}
				}
				else
				{
					if (isKeyDown==CTRL_KEY_DOWN)
					{
						p4.x = point.x - MARGIN_LEFT;
						p4.y = point.y - pic_top;
						if (p4.x < p3.x || p4.y<p3.y)
						{
							pt = p4;
							p4 = p3;
							p3 = pt;
						}
						if (p4.x>p3.x&&p4.y < p3.y)
						{
							pt.y = p3.y;
							p3.y = p4.y;
							p4.y = pt.y;
						}
						if (p4.x<p3.x&&p4.y>p3.y)
						{
							pt.x = p3.x;
							p3.x = p4.x;
							p4.x = pt.x;
						}
						int s_area = (p4.x - p3.x)*(p4.y - p3.y);
						dlgTch.tch_params.threshold.targetArea = s_area;
						str.Format("%d", s_area);
						dlgTch.m_editTargetArea.SetWindowText(_T(str));
						str = "";
						mouseStatus = Mouse_RBUP;
					}
					if (isKeyDown==SHIFT_KEY_DOWN)
					{
						pr.x = WIDTH;
						pr.y = point.y - pic_top;
						int h = pr.y - tch.y;
						str.Format("%d", h);
						dlgTch.tch_params.threshold.outside = h;
						dlgTch.m_editOutSide.SetWindowText(_T(str));
						str = "";
						mouseStatus = Mouse_RBUP;
					}
					pt = { 0 };
				}
			}
			else
			{
				if (whichRect == 1)
				{
					tmp = _T(" ,");
					s.Format(_T("%d"), tch.x);
					str += s;
					str += tmp;
					s.Format(_T("%d"), tch.y);
					str += s;
					str += tmp;
					s.Format(_T("%d"), tch.width);
					str += s;
					str += tmp;
					s.Format(_T("%d"), tch.height);
					str += s;
					dlgTch.m_txtTchArg.SetWindowTextA(str);
					str = _T("");
					SetDlgItemInt(IDC_editX, tch.x);
					SetDlgItemInt(IDC_editY, tch.y);
					SetDlgItemInt(IDC_editW, tch.width);
					SetDlgItemInt(IDC_editH, tch.height);
				}
				else if (whichRect == 2)
				{
					tmp = _T(" ,");
					s.Format(_T("%d"), blk.x);
					str += s;
					str += tmp;
					s.Format(_T("%d"), blk.y);
					str += s;
					str += tmp;
					s.Format(_T("%d"), blk.width);
					str += s;
					str += tmp;
					s.Format(_T("%d"), blk.height);
					str += s;
					dlgTch.m_txtBlkArg.SetWindowTextA(str);
					str = _T("");
					SetDlgItemInt(IDC_editX, blk.x);
					SetDlgItemInt(IDC_editY, blk.y);
					SetDlgItemInt(IDC_editW, blk.width);
					SetDlgItemInt(IDC_editH, blk.height);
				}
				dlgTch.setTrackRects(tch, TCH_RECT);
				dlgTch.setTrackRects(blk, BLK_RECT);
				mouseStatus = Mouse_LBUP;
			}
			pt = { 0 };
		}
		//学生界面的左键抬起操作
		else
		{
			if (mouseStatus==Mouse_LBDOWN)
			{
				pc.x = point.x - MARGIN_LEFT;
				pc.y = point.y - pic_top;
				mouseStatus = Mouse_LBUP;
				if (pc.x < pa.x || pc.y<pa.y)
				{
					pt = pc;
					pc = pa;
					pa = pt;
				}
				if (pc.x>pa.x&&pc.y < pa.y)
				{
					pt.y = pa.y;
					pa.y = pc.y;
					pc.y = pt.y;
				}
				if (pc.x<pa.x&&pc.y>pa.y)
				{
					pt.x = pa.x;
					pa.x = pc.x;
					pc.x = pt.x;
				}
				pb.x = pc.x; pb.y = pa.y;
				pd.x = pa.x; pd.y = pc.y;
				pA = pa;
				pB = pb;
				pC = pc;
				pD = pd;

				updateParams(PARAM_POSITION);
				updateLines();
				updateParams(PARAM_WIDTH);
				mouseStatus = Mouse_LBUP;
			}
			else
			{
				updateParams(PARAM_POSITION);
				updateLines();
				updateParams(PARAM_WIDTH);
				mouseStatus = Mouse_LBUP;
				//SetDlgItemInt(IDC_EDT_LEFTUP_AGL,mouseStatus);
			}
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCTrackToolsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (MARGIN_LEFT <= point.x&&point.x <= MARGIN_LEFT + Frame_Width && pic_top <= point.y&&point.y <= pic_top + Frame_Height)
	{
		if (CurSel==TCH_TAB)
		{
			if (mouseStatus == Mouse_LBDOWN)
			{
				pt.x = point.x - MARGIN_LEFT;
				pt.y = point.y - pic_top;
			}
			if (mouseStatus == Mouse_LBDRAG)
			{
				/*p1.x += (point.x - pt.x);
				p1.y += (point.y - pt.y);
				p2.x += (point.x - pt.x);
				p2.y += (point.y - pt.y);*/


				if (whichRect == 1)
				{
					tch.x += (point.x - pt.x);
					tch.y += (point.y  - pt.y);
					pl.y += (point.y - pt.y);
					pr.y += (point.y - pt.y);
					centre_pt2.y += (point.y - pt.y);
					if (p3.x>0&&p3.y>0&&p4.x>0&&p4.y>0)
					{
						p3.y += (point.y - pt.y);
						p4.y += (point.y - pt.y);
					}
					/*if (pl.x>0&&pl.y>0&&pr.x>0&&pr.y>0)
					{
						
					}*/
					SetDlgItemInt(IDC_editX, tch.x);
					SetDlgItemInt(IDC_editY, tch.y);
					SetDlgItemInt(IDC_editW, tch.width);
					SetDlgItemInt(IDC_editH, tch.height);
				}
				if (whichRect == 2)
				{
					blk.x += (point.x - pt.x);//修改板书框
					blk.y += (point.y  - pt.y);
					SetDlgItemInt(IDC_editX, blk.x);
					SetDlgItemInt(IDC_editY, blk.y);
					SetDlgItemInt(IDC_editW, blk.width);
					SetDlgItemInt(IDC_editH, blk.height);
				}
				pt.x = point.x;
				pt.y = point.y;
			}
			if (Mouse_RBDOWN == mouseStatus&&isKeyDown == CTRL_KEY_DOWN)
			{
				pt.x = point.x - MARGIN_LEFT;
				pt.y = point.y - pic_top;
			}
		}
		else
		{
			if (mouseStatus == Mouse_LBDOWN)
			{
				pt.x = point.x - MARGIN_LEFT;
				pt.y = point.y - pic_top;
			}
			if (mouseStatus==Mouse_RBDOWN)
			{
				pt.x = point.x - MARGIN_LEFT;
				pt.y = point.y - pic_top;
			}
			if (mouseStatus == Mouse_LBDRAG)
			{
				/*point.x -= MARGIN_LEFT;
				point.y -= pic_top;*/

				pa.x += (point.x  - pt.x);
				pa.y += (point.y  - pt.y);

				/*pt.x += (point.x - pt.x);
				pt.y += (point.y - pt.y);*/

				pb.x += (point.x - pt.x);
				pb.y += (point.y - pt.y);

				pc.x += (point.x - pt.x);
				pc.y += (point.y - pt.y);

				pd.x += (point.x - pt.x);
				pd.y += (point.y - pt.y);

				pt.x = point.x;
				pt.y = point.y;

				updateLines();
			}
			if (mouseStatus == Mouse_ADJUST_A)
			{
				pa.x += (point.x  - pt.x);
				pa.y += (point.y  - pt.y);
				pA.x += (point.x - pt.x);
				pA.y += (point.x - pt.x);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus == Mouse_ADJUST_B)
			{
				pb.x += (point.x  - pt.x);
				pb.y += (point.y  - pt.y);
				pB.x += (point.x - pt.x);
				pB.y += (point.y - pt.y);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus == Mouse_ADJUST_C)
			{
				pc.x += (point.x - pt.x);
				pc.y += (point.y  - pt.y);
				pC.x += (point.x - pt.x);
				pC.y += (point.y - pt.y);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus == Mouse_ADJUST_D)
			{
				pd.x += (point.x  - pt.x);
				pd.y += (point.y  - pt.y);
				pD.x += (point.x - pt.x);
				pD.y += (point.y - pt.y);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus==Mouse_RBDOWN)
			{
				pt.x = point.x - MARGIN_LEFT;
				pt.y = point.y - pic_top;
			}
			if (mouseStatus==Mouse_RBDRAG)
			{
				if (p1==pa)
				{
					pA.x += (point.x - pt.x);
					pA.y += (point.y - pt.y);
					pt.x = point.x;
					pt.y = point.y;
				}
				if (p1 == pb)
				{
					pB.x += (point.x - pt.x);
					pB.y += (point.y - pt.y);
					pt.x = point.x;
					pt.y = point.y;
				}
				if (p1 == pc)
				{
					pC.x += (point.x - pt.x);
					pC.y += (point.y - pt.y);
					pt.x = point.x;
					pt.y = point.y;
				}
				if (p1 == pd)
				{
					pD.x += (point.x - pt.x);
					pD.y += (point.y - pt.y);
					pt.x = point.x;
					pt.y = point.y;
				}
				//判断拖拽宽度
				if (whichVertex == 0)
				{
					ln1[1].x += (point.x - pt.x);
					pt = point;
				}
				if (whichVertex == 1)
				{
					ln2[1].x += (point.x - pt.x);
					pt = point;
				}
				if (whichVertex == 2)
				{
					ln3[1].x += (point.x - pt.x);
					pt = point;
				}
				if (whichVertex == 3)
				{
					ln4[1].x += (point.x - pt.x);
					pt = point;
				}
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCTrackToolsDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//if (MARGIN_LEFT <= point.x&&point.x <= MARGIN_LEFT + Frame_Width && pic_top <= point.y&&point.y <= pic_top + Frame_Height)
	//{
	//	if (CurSel == STU_TAB)
	//	{
	//		//pt = { 0 };
	//		//if (pa.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pa.x + 10 && pa.y - 10 <= point.y - pic_top&&point.y - pic_top <= pa.y + 10)
	//		//{
	//		//	if (pa.x != point.x&&pa.y != point.y)
	//		//	{
	//		//		p1 = pa;
	//		//		mouseStatus = Mouse_RBDOWN;
	//		//		//isRightButton = 1;
	//		//	}
	//		//}
	//		//else if (pb.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pb.x + 10 && pb.y - 10 <= point.y - pic_top&&point.y - pic_top <= pb.y + 10)
	//		//{
	//		//	if (pb.x != point.x&&pb.y != point.y)
	//		//	{
	//		//		p1 = pb;
	//		//		mouseStatus = Mouse_RBDOWN;
	//		//		//isRightButton = 1;
	//		//	}
	//		//}
	//		//else if (pc.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pc.x + 10 && pc.y - 10 <= point.y - pic_top&&point.y - pic_top <= pc.y + 10)
	//		//{
	//		//	if (pc.x != point.x&&pc.y != point.y)
	//		//	{
	//		//		p1 = pc;
	//		//		mouseStatus = Mouse_RBDOWN;
	//		//		//isRightButton = 1;
	//		//	}
	//		//}
	//		//else if (pd.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pd.x + 10 && pd.y - 10 <= point.y - pic_top&&point.y - pic_top <= pd.y + 10)
	//		//{
	//		//	if (pd.x != point.x&&pd.y != point.y)
	//		//	{
	//		//		p1 = pd;
	//		//		mouseStatus = Mouse_RBDOWN;
	//		//		//isRightButton = 1;
	//		//	}
	//		//}
	//		//else if (pA.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pA.x + 10 && pA.y - 10 <= point.y - pic_top&&point.y - pic_top <= pA.y + 10)
	//		//{
	//		//	if (pA.x>0&&pA.y>0)
	//		//	{
	//		//		pt = point;
	//		//		p1 = pa;
	//		//		mouseStatus = Mouse_RBDRAG;
	//		//		whichVertex = -1;
	//		//	}
	//		//}
	//		//else if (pB.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pB.x + 10 && pB.y - 10 <= point.y - pic_top&&point.y - pic_top <= pB.y + 10)
	//		//{
	//		//	if (pB.x>0&&pB.y>0)
	//		//	{
	//		//		pt = point;
	//		//		p1 = pb;
	//		//		mouseStatus = Mouse_RBDRAG;
	//		//		whichVertex = -1;
	//		//	}
	//		//}
	//		//else if (pC.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pC.x + 10 && pC.y - 10 <= point.y - pic_top&&point.y - pic_top <= pC.y + 10)
	//		//{
	//		//	if (pC.x > 0 && pC.y > 0)
	//		//	{
	//		//		pt = point;
	//		//		p1 = pc;
	//		//		mouseStatus = Mouse_RBDRAG;
	//		//		whichVertex = -1;
	//		//	}
	//		//}
	//		//else if (pD.x - 10 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= pD.x + 10 && pD.y - 10 <= point.y - pic_top&&point.y - pic_top <= pD.y + 10)
	//		//{
	//		//	if (pD.x > 0 && pD.y > 0)
	//		//	{
	//		//		pt = point;
	//		//		p1 = pd;
	//		//		mouseStatus = Mouse_RBDRAG;
	//		//		whichVertex = -1;
	//		//	}
	//		//}
	//		////调整四个顶点的宽度
	//		//else if (ln1[1].x - 5 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= ln1[1].x + 5 && ln1[1].y - 5 <= point.y - pic_top&&point.y - pic_top <= ln1[1].y + 5)
	//		//{
	//		//	pt = point;
	//		//	//p2 = pa;
	//		//	whichVertex = 0;
	//		//	mouseStatus = Mouse_RBDRAG;
	//		//}
	//		//else if (ln2[1].x - 5 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= ln2[1].x + 5 && ln2[1].y - 5 <= point.y - pic_top&&point.y - pic_top <= ln2[1].y + 5)
	//		//{
	//		//	pt = point;
	//		//	//p2 = pb;
	//		//	whichVertex = 1;
	//		//	mouseStatus = Mouse_RBDRAG;
	//		//}
	//		//else if (ln3[1].x - 5 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= ln3[1].x + 5 && ln3[1].y - 5 <= point.y - pic_top&&point.y - pic_top <= ln3[1].y + 5)
	//		//{
	//		//	pt = point;
	//		//	//p2 = pc;
	//		//	whichVertex = 2;
	//		//	mouseStatus = Mouse_RBDRAG;
	//		//}
	//		//else if (ln4[1].x - 5 <= point.x - MARGIN_LEFT && point.x - MARGIN_LEFT <= ln4[1].x + 5 && ln4[1].y - 5 <= point.y - pic_top&&point.y - pic_top <= ln4[1].y + 5)
	//		//{
	//		//	pt = point;
	//		//	//p2 = pd;
	//		//	whichVertex = 3;
	//		//	mouseStatus = Mouse_RBDRAG;
	//		//}
	//		//else
	//		//{
	//		//	pA = pB = pC = pD = { 0 };
	//		//	p1 = p2 = pt = { 0 };
	//		//	/*ln1[0] = ln1[1] = { 0 };
	//		//	ln2[0] = ln2[1] = { 0 };
	//		//	ln3[0] = ln3[1] = { 0 };
	//		//	ln4[0] = ln4[1] = { 0 };*/
	//		//	updateLines();
	//		//}
	//	}
	//	else
	//	{
	//		if (m_check_algFlag.GetCheck())
	//		{
	//			MessageBox(_T("请先关闭教师跟踪！"));
	//			return;
	//		}
	//		if (tch.x < point.x&&point.x < tch.x + tch.width&&tch.y < point.y&&point.y < tch.y + tch.height + pic_top)
	//		{
	//			if (isKeyDown == CTRL_KEY_UP)
	//			{
	//				p3 = p4 = { 0 };
	//				pt = { 0 };
	//				p3.x = point.x - MARGIN_LEFT;
	//				p3.y = point.y - pic_top;
	//				mouseStatus = Mouse_RBDOWN;
	//			}
	//			else
	//			{
	//				pl.x = 0;
	//				pl.y = point.y - pic_top;
	//				mouseStatus = Mouse_RBDOWN;
	//			}
	//		}
	//	}
	//}
	CDialogEx::OnRButtonDown(nFlags, point);
}

void CMFCTrackToolsDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//if (MARGIN_LEFT <= point.x&&point.x <= MARGIN_LEFT + Frame_Width && pic_top <= point.y&&point.y <= pic_top + Frame_Height)
	//{
	//	if (CurSel==TCH_TAB)
	//	{
	//		if (tch.x < point.x&&point.x < tch.x + tch.width&&tch.y < point.y&&point.y < tch.y + tch.height + pic_top)
	//		{
	//			if (isKeyDown==CTRL_KEY_UP)
	//			{
	//				p4.x = point.x - MARGIN_LEFT;
	//				p4.y = point.y - pic_top;
	//				if (p4.x < p3.x || p4.y<p3.y)
	//				{
	//					pt = p4;
	//					p4 = p3;
	//					p3 = pt;
	//				}
	//				if (p4.x>p3.x&&p4.y < p3.y)
	//				{
	//					pt.y = p3.y;
	//					p3.y = p4.y;
	//					p4.y = pt.y;
	//				}
	//				if (p4.x<p3.x&&p4.y>p3.y)
	//				{
	//					pt.x = p3.x;
	//					p3.x = p4.x;
	//					p4.x = pt.x;
	//				}
	//				int s_area = (p4.x - p3.x)*(p4.y - p3.y);
	//				dlgTch.tch_params.threshold.targetArea = s_area;
	//				str.Format("%d", s_area);
	//				dlgTch.m_editTargetArea.SetWindowText(_T(str));
	//				str = "";
	//				mouseStatus = Mouse_RBUP;
	//			}
	//			else
	//			{
	//				pr.x = WIDTH;
	//				pr.y = point.y - pic_top;
	//				int h = pr.y - tch.y;
	//				str.Format("%d", h);
	//				dlgTch.tch_params.threshold.outside = h;
	//				dlgTch.m_editOutSide.SetWindowText(_T(str));
	//				str = "";
	//				mouseStatus = Mouse_RBUP;
	//			}
	//			pt = { 0 };
	//		}
	//		/*else
	//		{
	//			p1.x = 0; p1.y = 0;
	//			p2.x = 0; p2.y = 0;
	//			pt.x = 0; pt.y = 0;
	//			tch.x = 0; tch.y = 0;
	//			tch.width = 0; tch.height = 0;
	//			blk.x = 0; blk.y = 0;
	//			blk.width = 0; blk.height = 0;
	//			p3 = p4 = { 0 };

	//			dlgTch.m_txtTchArg.SetWindowText(_T(""));
	//			dlgTch.m_txtBlkArg.SetWindowText(_T(""));
	//			mouseCnt = 0;
	//			mouseStatus = Mouse_RBUP;
	//		}*/
	//		
	//	}
	//	else
	//	{
	//		//if (p1 == pa)
	//		//{
	//		//	p2.x = point.x - MARGIN_LEFT;
	//		//	p2.y = point.y - pic_top;
	//		//	CPoint p = p2 - p1;
	//		//	angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
	//		//	angle = angle < 0 ? angle + 360 : angle;
	//		//	s.Format("%d", angle);
	//		//	if (pa.x>0 && pa.y > 0)
	//		//	{
	//		//		pA = p2;
	//		//		p1 = p2 = { 0 };
	//		//		dlgStu.m_edtLeftUpAgl.SetWindowText(s);
	//		//		dlgStu.stu_params.stuTrack_direct_standard[0] = angle;
	//		//	}
	//		//	else
	//		//	{
	//		//		p2 = { 0 };
	//		//	}
	//		//}
	//		//if (p1 == pb)
	//		//{
	//		//	p2.x = point.x - MARGIN_LEFT;
	//		//	p2.y = point.y - pic_top;
	//		//	CPoint p = p2 - p1;
	//		//	angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
	//		//	angle = angle < 0 ? angle + 360 : angle;
	//		//	s.Format("%d", angle);
	//		//	if (pb.x > 0 && pb.y > 0)
	//		//	{
	//		//		pB = p2;
	//		//		p1 = p2 = { 0 };
	//		//		dlgStu.m_edtRightUpAgl.SetWindowText(s);
	//		//		dlgStu.stu_params.stuTrack_direct_standard[1] = angle;
	//		//	}
	//		//	else
	//		//	{
	//		//		p2 = { 0 };
	//		//	}
	//		//}
	//		//if (p1 == pc)
	//		//{
	//		//	p2.x = point.x - MARGIN_LEFT;
	//		//	p2.y = point.y - pic_top;
	//		//	CPoint p = p2 - p1;
	//		//	angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
	//		//	angle = angle < 0 ? angle + 360 : angle;
	//		//	s.Format("%d", angle);
	//		//	if (pc.x > 0 && pc.y > 0)
	//		//	{
	//		//		pC = p2;
	//		//		p1 = p2 = { 0 };
	//		//		dlgStu.m_edtRightDnAgl.SetWindowText(s);
	//		//		dlgStu.stu_params.stuTrack_direct_standard[2] = angle;
	//		//	}
	//		//	else
	//		//	{
	//		//		p2 = { 0 };
	//		//	}
	//		//}
	//		//if (p1 == pd)
	//		//{
	//		//	p2.x = point.x - MARGIN_LEFT;
	//		//	p2.y = point.y - pic_top;
	//		//	CPoint p = p2 - p1;
	//		//	angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
	//		//	angle = angle < 0 ? angle + 360 : angle;
	//		//	s.Format("%d", angle);
	//		//	if (pd.x > 0 && pd.y > 0)
	//		//	{
	//		//		pD = p2;
	//		//		p1 = p2 = { 0 };
	//		//		dlgStu.m_edtLeftDnAgl.SetWindowText(s);
	//		//		dlgStu.stu_params.stuTrack_direct_standard[3] = angle;
	//		//	}
	//		//	else
	//		//	{
	//		//		p2 = { 0 };
	//		//	}
	//		//}
	//		//if (whichVertex == 0)
	//		//{
	//		//	ln1[1].x += (point.x - pt.x);
	//		//	dist = getDistance(ln1[0], ln1[1]);
	//		//	s.Format("%d", dist);
	//		//	dlgStu.m_edtLeftUpWid.SetWindowText(s);
	//		//	whichVertex = -1;
	//		//	dlgStu.stu_params.stuTrack_stuWidth_standard[0] = dist;
	//		//	//pt = point;
	//		//}
	//		//if (whichVertex == 1)
	//		//{
	//		//	ln2[1].x += (point.x - pt.x);
	//		//	dist = getDistance(ln2[0], ln2[1]);
	//		//	s.Format("%d", dist);
	//		//	dlgStu.m_edtRightUpWid.SetWindowText(s);
	//		//	whichVertex = -1;
	//		//	dlgStu.stu_params.stuTrack_stuWidth_standard[1] = dist;
	//		//}
	//		//if (whichVertex == 2)
	//		//{
	//		//	ln3[1].x += (point.x - pt.x);
	//		//	dist = getDistance(ln3[0], ln3[1]);
	//		//	s.Format("%d", dist);
	//		//	dlgStu.m_edtRightDnWid.SetWindowText(s);
	//		//	whichVertex = -1;
	//		//	dlgStu.stu_params.stuTrack_stuWidth_standard[2] = dist;
	//		//}
	//		//if (whichVertex == 3)
	//		//{
	//		//	ln4[1].x += (point.x - pt.x);
	//		//	dist = getDistance(ln4[0], ln4[1]);
	//		//	s.Format("%d", dist);
	//		//	dlgStu.m_edtLeftDnWid.SetWindowText(s);
	//		//	whichVertex = -1;
	//		//	dlgStu.stu_params.stuTrack_stuWidth_standard[3] = dist;
	//		//}
	//		//mouseStatus = Mouse_RBUP;
	//		////isRightButton = 0;
	//	}
	//}
	CDialogEx::OnRButtonUp(nFlags, point);
}

void CMFCTrackToolsDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (MARGIN_LEFT <= point.x&&point.x <= MARGIN_LEFT + WIDTH && pic_top <= point.y&&point.y <= pic_top + HEIGHT)
	{
		if (CurSel==TCH_TAB)
		{
			p1.x = -10; p1.y = -10;
			p2.x = -10; p2.y = -10;
			pt.x = -10; pt.y = -10;
			tch.x = -10; tch.y = -10;
			tch.width = -10; tch.height = -10;
			blk.x = -10; blk.y = -10;
			blk.width = -10; blk.height = -10;
			p3 = p4 = { -10 };
			pl = pr = { -10 };
			centre_pt2 = { -10 };

			dlgTch.m_txtTchArg.SetWindowText(_T(""));
			dlgTch.m_txtBlkArg.SetWindowText(_T(""));
			dlgTch.m_editOutSide.SetWindowText(_T(""));
			dlgTch.m_editTargetArea.SetWindowText(_T(""));
			mouseCnt = 0;
			mouseStatus = Mouse_RBUP;
		}
		
	}
	CDialogEx::OnRButtonDblClk(nFlags, point);
}

void CMFCTrackToolsDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CMFCTrackToolsDlg::updateLines()
{
	ln1[0] = pa;
	ln1[1].x = pa.x + (pb.x - pa.x)*0.2;
	ln1[1].y = pa.y + (pb.y - pa.y)*0.2;
	ln2[0] = pb;
	ln2[1].x = pb.x + (pa.x - pb.x)*0.2;
	ln2[1].y = pb.y + (pa.y - pb.y)*0.2;
	ln3[0] = pc;
	ln3[1].x = pc.x + (pd.x - pc.x)*0.2;
	ln3[1].y = pc.y + (pd.y - pc.y)*0.2;
	ln4[0] = pd;
	ln4[1].x = pd.x + (pc.x - pd.x)*0.2;
	ln4[1].y = pd.y + (pc.y - pd.y)*0.2;
}

void CMFCTrackToolsDlg::updateParams(int flag)
{
	if (PARAM_POSITION == flag)
	{
		dlgStu.stu_params.stuTrack_vertex[0].x = pa.x;
		dlgStu.stu_params.stuTrack_vertex[0].y = pa.y;

		dlgStu.stu_params.stuTrack_vertex[1].x = pb.x;
		dlgStu.stu_params.stuTrack_vertex[1].y = pb.y;

		dlgStu.stu_params.stuTrack_vertex[2].x = pc.x;
		dlgStu.stu_params.stuTrack_vertex[2].y = pc.y;

		dlgStu.stu_params.stuTrack_vertex[3].x = pd.x;
		dlgStu.stu_params.stuTrack_vertex[3].y = pd.y;

		tmp = _T(" ,");
		s.Format(_T("%d"), pa.x);
		str += s;
		str += tmp;
		s.Format(_T("%d"), pa.y);
		str += s;
		dlgStu.m_edtLeftUpPos.SetWindowTextA(str);
		str = _T("");

		tmp = _T(" ,");
		s.Format(_T("%d"), pb.x);
		str += s;
		str += tmp;
		s.Format(_T("%d"), pb.y);
		str += s;
		dlgStu.m_edtRightUpPos.SetWindowTextA(str);
		str = _T("");

		tmp = _T(" ,");
		s.Format(_T("%d"), pd.x);
		str += s;
		str += tmp;
		s.Format(_T("%d"), pd.y);
		str += s;
		dlgStu.m_edtLeftDnPos.SetWindowTextA(str);
		str = _T("");

		tmp = _T(" ,");
		s.Format(_T("%d"), pc.x);
		str += s;
		str += tmp;
		s.Format(_T("%d"), pc.y);
		str += s;
		dlgStu.m_edtRightDnPos.SetWindowTextA(str);
		str = _T("");
	}
	else
	{
		/*dist = getDistance(ln1[0], ln1[1]);
		s.Format("%d", dist);
		dlgStu.m_edtLeftUpWid.SetWindowText(s);
		dlgStu.stu_params.stuTrack_stuWidth_standard[0] = dist;

		dist = getDistance(ln2[0], ln2[1]);
		s.Format("%d", dist);
		dlgStu.m_edtRightUpWid.SetWindowText(s);
		dlgStu.stu_params.stuTrack_stuWidth_standard[1] = dist;

		dist = getDistance(ln3[0], ln3[1]);
		s.Format("%d", dist);
		dlgStu.m_edtRightDnWid.SetWindowText(s);
		dlgStu.stu_params.stuTrack_stuWidth_standard[2] = dist;

		dist = getDistance(ln4[0], ln4[1]);
		s.Format("%d", dist);
		dlgStu.m_edtLeftDnWid.SetWindowText(s);
		dlgStu.stu_params.stuTrack_stuWidth_standard[3] = dist;*/

		CPoint v1, v2;
		int mod1 = 0, mod2 = 0;
		double v1x, v1y, v2x, v2y;
		v1 = pb - pa; v2 = pd - pc;
		mod1 = sqrt(v1.x*v1.x + v1.y*v1.y);
		mod2 = sqrt(v2.x*v2.x + v2.y*v2.y);
		v1x = ((double)v1.x) / mod1; v1y = ((double)v1.y) / mod1;
		v2x = ((double)v2.x) / mod2; v2y = ((double)v2.y) / mod2;
		int width;

		//GetDlgItem(IDC_EDT_LEFTUP_WID)->GetWindowText(str);
		width = dlgStu.stu_params.stuTrack_stuWidth_standard[0];
		s.Format("%d", width);
		dlgStu.m_edtLeftUpWid.SetWindowText(s);
		ln1[0] = pa;
		ln1[1].x = (int)(pa.x + v1x*width + 0.5);
		ln1[1].y = (int)(pa.y + v1y*width + 0.5);

		//GetDlgItem(IDC_EDT_RIGHTUP_WID)->GetWindowText(str);
		width = dlgStu.stu_params.stuTrack_stuWidth_standard[1];
		s.Format("%d", width);
		dlgStu.m_edtRightUpWid.SetWindowText(s);
		ln2[0] = pb;
		ln2[1].x = (int)(pb.x - v1x*width + 0.5);
		ln2[1].y = (int)(pb.y - v1y*width + 0.5);

		//GetDlgItem(IDC_EDT_LEFTDN_WID)->GetWindowText(str);
		width = dlgStu.stu_params.stuTrack_stuWidth_standard[3];
		s.Format("%d", width);
		dlgStu.m_edtLeftDnWid.SetWindowText(s);
		ln4[0] = pd;
		ln4[1].x = (int)(pd.x - v2x*width + 0.5);
		ln4[1].y = (int)(pd.y - v2y*width + 0.5);

		//GetDlgItem(IDC_EDT_RIGHTDN_WID)->GetWindowText(str);
		width = dlgStu.stu_params.stuTrack_stuWidth_standard[2];
		s.Format("%d", width);
		dlgStu.m_edtRightDnWid.SetWindowText(s);
		ln3[0] = pc;
		ln3[1].x = (int)(pc.x + v2x*width + 0.5);
		ln3[1].y = (int)(pc.y + v2y*width + 0.5);
	}
}

void CMFCTrackToolsDlg::loadParamsFromTch(TeaITRACK_Params* params)
{
	dlgTch.tch_params = *params;
	int index;
	//g_drawPS = 1;
	int_pos = params->numOfPos;
	int_slide = params->numOfSlide;
	tch.x = params->tch.x;
	tch.y = params->tch.y;
	tch.width = params->tch.width;
	tch.height = params->tch.height;
	blk.x = params->blk.x;
	blk.y = params->blk.y;
	blk.width = params->blk.width;
	blk.height = params->blk.height;
	s.Format("%d", params->threshold.outside);
	dlgTch.m_editOutSide.SetWindowText(s);

	s.Format("%d", (int)(params->threshold.stand/1000));
	index=dlgTch.m_comboStand.FindString(0, s);
	if (index!=CB_ERR)
	{
		dlgTch.m_comboStand.SetCurSel(index);
	}

	s.Format("%d", params->numOfSlide);
	index = dlgTch.m_comboSlide.FindString(0, s);
	if (index != CB_ERR)
	{
		dlgTch.m_comboSlide.SetCurSel(index);
	}

	s.Format("%d", int_pos);
	dlgTch.m_editPos.SetWindowText(s);
	s.Format("%d", params->threshold.targetArea);
	dlgTch.m_editTargetArea.SetWindowText(s);

	int _maxArea = (int)(params->maxArea * 100);
	s.Format("%d", _maxArea);
	dlgTch.m_editMaxArea.SetWindowText(s);

	camPosSlide.center = int_pos / 2;
	camPosSlide.width = int_slide / 2;
	camPosSlide.left = camPosSlide.center - camPosSlide.width;
	camPosSlide.right = camPosSlide.center + camPosSlide.width;

	if (int_pos!=0)
	{
		centre_pt2.x = (camPosSlide.left + int_slide / 2 + 0.5)*(tch.width / int_pos) + tch.x;
		centre_pt2.y = tch.y + tch.height / 2;
	}	

	pl = { 0, params->threshold.outside + tch.y };
	pr = { WIDTH, params->threshold.outside + tch.y };

	dlgCam.setNumOfPreset(params->numOfPos);

	tmp = _T(" ,");
	s.Format(_T("%d"), tch.x);
	str += s;
	str += tmp;
	dlgTch.m_editX.SetWindowTextA(s);
	s.Format(_T("%d"), tch.y);
	str += s;
	str += tmp;
	dlgTch.m_editY.SetWindowTextA(s);
	s.Format(_T("%d"), tch.width);
	str += s;
	str += tmp;
	dlgTch.m_editW.SetWindowTextA(s);
	s.Format(_T("%d"), tch.height);
	str += s;
	dlgTch.m_editH.SetWindowTextA(s);
	dlgTch.m_txtTchArg.SetWindowTextA(str);
	str = _T("");

	s.Format(_T("%d"), blk.x);
	str += s;
	str += tmp;
	s.Format(_T("%d"), blk.y);
	str += s;
	str += tmp;
	s.Format(_T("%d"), blk.width);
	str += s;
	str += tmp;
	s.Format(_T("%d"), blk.height);
	str += s;
	dlgTch.m_txtBlkArg.SetWindowTextA(str);
	str = _T("");

	dlgTch.setParams(params);

	mouseCnt = 2;
}

void CMFCTrackToolsDlg::loadParamsFromStu(StuITRACK_ClientParams_t* params)
{
	dlgStu.stu_params = *params;
	int index;
	//g_drawPS = 0;
	//先载入四个顶角位置
	pa.x = params->stuTrack_vertex[0].x;
	pa.y = params->stuTrack_vertex[0].y;

	pb.x = params->stuTrack_vertex[1].x;
	pb.y = params->stuTrack_vertex[1].y;

	pc.x = params->stuTrack_vertex[2].x;
	pc.y = params->stuTrack_vertex[2].y;

	pd.x = params->stuTrack_vertex[3].x;
	pd.y = params->stuTrack_vertex[3].y;

	//载入四个宽度
	CPoint v1, v2;
	int mod1 = 0, mod2 = 0;
	v1 = pb - pa; v2 = pd - pc;
	mod1 = sqrt(v1.x*v1.x + v1.y*v1.y);
	mod2 = sqrt(v2.x*v2.x + v2.y*v2.y);

	if (mod1 == 0 || mod2 == 0)
	{
		ln1[0] = ln1[1] = ln2[0] = ln2[1] = ln3[0] = ln3[1] = ln4[0] = ln4[1] = { 0 };
		v1 = v2 = { 0 };
	}
	else
	{
		v1.x = v1.x / mod1; v1.y = v1.y / mod1;
		v2.x = v2.x / mod2; v2.y = v2.y / mod2;

		ln1[0] = pa;
		ln1[1].x = pa.x + v1.x*params->stuTrack_stuWidth_standard[0];
		ln1[1].y = pa.y + v1.y*params->stuTrack_stuWidth_standard[0];

		ln2[0] = pb;
		ln2[1].x = pb.x - v1.x*params->stuTrack_stuWidth_standard[1];
		ln2[1].y = pb.y - v1.y*params->stuTrack_stuWidth_standard[1];

		ln3[0] = pc;
		ln3[1].x = pc.x + v2.x*params->stuTrack_stuWidth_standard[2];
		ln3[1].y = pc.y + v2.y*params->stuTrack_stuWidth_standard[2];

		ln4[0] = pd;
		ln4[1].x = pd.x - v2.x*params->stuTrack_stuWidth_standard[3];
		ln4[1].y = pd.y - v2.y*params->stuTrack_stuWidth_standard[3];
	}
	

	/*switch (dlgStu.m_radioWid)
	{
	case 0:
	dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[0]);
	break;
	case 1:
	dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[1]);
	break;
	case 2:
	dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[3]);
	break;
	case 3:
	dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[2]);
	break;
	default:
	break;
	}*/

	//载入四个角度值
	//int temp = 0;

	//temp = pa.x - 10;
	pA.x = pa.x + cos(params->stuTrack_direct_standard[0] * ITC_ANGLE_TO_RADIAN) * 50;
	//temp = pa.y - 10;
	pA.y = pa.y + sin(params->stuTrack_direct_standard[0] * ITC_ANGLE_TO_RADIAN) * 50;

	//temp = pb.x - 10;
	pB.x = pb.x + cos(params->stuTrack_direct_standard[1] * ITC_ANGLE_TO_RADIAN) * 50;
	//temp = pb.y - 10;
	pB.y = pb.y + sin(params->stuTrack_direct_standard[1] * ITC_ANGLE_TO_RADIAN) * 50;

	//temp = pc.x - 10;
	pC.x = pc.x + cos(params->stuTrack_direct_standard[2] * ITC_ANGLE_TO_RADIAN) * 80;
	//temp = pc.y - 10;
	pC.y = pc.y + sin(params->stuTrack_direct_standard[2] * ITC_ANGLE_TO_RADIAN) * 80;

	//temp = pd.x - 10;
	pD.x = pd.x + cos(params->stuTrack_direct_standard[3] * ITC_ANGLE_TO_RADIAN) * 80;
	//temp = pd.y - 10;
	pD.y = pd.y + sin(params->stuTrack_direct_standard[3] * ITC_ANGLE_TO_RADIAN) * 80;

	s.Format("%d", params->stuTrack_direct_standard[0]);
	dlgStu.m_edtLeftUpAgl.SetWindowText(s);

	s.Format("%d", params->stuTrack_direct_standard[1]);
	dlgStu.m_edtRightUpAgl.SetWindowText(s);

	s.Format("%d", params->stuTrack_direct_standard[2]);
	dlgStu.m_edtRightDnAgl.SetWindowText(s);

	s.Format("%d", params->stuTrack_direct_standard[3]);
	dlgStu.m_edtLeftDnAgl.SetWindowText(s);

	//载入显示参数
	s.Format("%d", params->stuTrack_direct_range);
	dlgStu.m_edtStandAgl.SetWindowText(s);

	s.Format("%d", params->stuTrack_standCount_threshold);
	dlgStu.m_edtStandFrm.SetWindowText(s);

	s.Format("%d", params->stuTrack_sitdownCount_threshold);
	dlgStu.m_edtSitFrm.SetWindowText(s);

	s.Format("%d", (int)(params->stuTrack_move_threshold*100));
	dlgStu.m_edtMoveDev.SetWindowText(s);

	s.Format("%.1f", params->stuTrack_moveDelayed_threshold/1000.0);
	index=dlgStu.m_comboDly.FindString(0, s);
	if (index!=CB_ERR)
	{
		dlgStu.m_comboDly.SetCurSel(index);
	}
	

	/*int pos;
	CString tempPos;*/
	switch (dlgStu.m_radioAgl)
	{
	case 0:
		/*dlgStu.m_edtRightUpAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderAngle.SetPos(params->stuTrack_direct_standard[0]);
		break;
	case 1:
		/*dlgStu.m_edtRightUpAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderAngle.SetPos(params->stuTrack_direct_standard[1]);
		break;
	case 2:
		/*dlgStu.m_edtLeftDnAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderAngle.SetPos(params->stuTrack_direct_standard[3]);
		break;
	case 3:
		/*dlgStu.m_edtRightDnAgl.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderAngle.SetPos(params->stuTrack_direct_standard[2]);
		break;
	}

	switch (dlgStu.m_radioWid)
	{
	case 0:
		/*dlgStu.m_edtRightUpWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[0]);
		break;
	case 1:
		/*dlgStu.m_edtRightUpWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[1]);
		break;
	case 2:
		/*dlgStu.m_edtLeftDnWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[3]);
		break;
	case 3:
		/*dlgStu.m_edtRightDnWid.GetWindowText(tempPos);
		pos = _ttoi(tempPos);*/
		dlgStu.m_sliderWidth.SetPos(params->stuTrack_stuWidth_standard[2]);
		break;
	}

	dlgStu.setParams(params);
	updateParams(PARAM_POSITION);
	updateParams(PARAM_WIDTH);
}
void CMFCTrackToolsDlg::loadParamsFromPlc(Policy_Set_t* params)
{
	str.Format(_T("%d"), params->time.blb_time_min);
	dlgCtrl.m_edt_timeBlk.SetWindowText(str);
	dlgCtrl.ctrl_params.time.blb_time_min = params->time.blb_time_min;

	str.Format(_T("%d"), params->time.stu_time_min);
	dlgCtrl.m_edt_timeStu.SetWindowText(str);
	dlgCtrl.ctrl_params.time.stu_time_min = params->time.stu_time_min;

	str.Format(_T("%d"), params->time.tea_time_min);
	dlgCtrl.m_edt_timeTch.SetWindowText(str);
	dlgCtrl.ctrl_params.time.tea_time_min = params->time.tea_time_min;

	str.Format(_T("%d"), params->time.ppt_time_min);
	dlgCtrl.m_edt_timeVGA.SetWindowText(str);
	dlgCtrl.ctrl_params.time.ppt_time_min = params->time.ppt_time_min;


	dlgCtrl.m_chk_multiple.SetCheck(params->mut_pic_flag);
	dlgCtrl.ctrl_params.mut_pic_flag = params->mut_pic_flag;
	dlgCtrl.m_chk_stuOverview.SetCheck(params->stu_feature_flag);
	dlgCtrl.ctrl_params.stu_feature_flag = params->stu_feature_flag;
	str.Format(_T(""));
}
void CMFCTrackToolsDlg::updateParamsFromStu(StuITRACK_ClientParams_t* params)
{
	loadParamsFromStu(params);
}
static  inline char *get_track_cmd_name(int cmd)
{
	int i = 0;
	while (g_track_cmd[i].cmd != -1)
	{
		if (cmd == g_track_cmd[i].cmd)
			return g_track_cmd[i].cmd_name;
		i++;
	}
	return "unknown name";
}
//回调心跳函数处理
static int ctrlClient_process_trackHeartEx(char *buff, void * param)
{
	CMFCTrackToolsDlg* pTrackDlg = (CMFCTrackToolsDlg*)param;
	if (param == NULL)
	{
		AfxMessageBox(_T("接收失败"));
	}
	
	return pTrackDlg->ctrlClient_process_trackHeart(buff);
}
int CMFCTrackToolsDlg::ctrlClient_process_trackHeart(char *buff)
{
	//OutputDebugString(_T("ctrlClient_process_trackHeart=====================\n"));
	return 0;
}
static int ctrl_connect_status(Connect_Status status, void * param)
{
	CMFCTrackToolsDlg* pTrackDlg = (CMFCTrackToolsDlg*)param;
	if (pTrackDlg == NULL || status == CONNECT_FAIL)
	{
		if (pTrackDlg)
		{
			free(pTrackDlg->m_track_clientHandle);
			pTrackDlg->m_track_clientHandle = NULL;
		}

		AfxMessageBox(_T("连接失败"));
	}
	else if (status == DISCONNECT_SUCCESS)
	{
		if (pTrackDlg)
		{
			free(pTrackDlg->m_track_clientHandle);
			pTrackDlg->m_track_clientHandle = NULL;

		}
		AfxMessageBox(_T("服务器断开连接"));
	}
	else
	{	//用户名密码登录
		pTrackDlg->dlgCtrl.setConnectHandle(pTrackDlg->m_track_clientHandle);
		ctrlClient_login(&(pTrackDlg->m_loginInfo), pTrackDlg->m_track_clientHandle);
	}
	pTrackDlg->dlgTch.setConnectHandle(pTrackDlg->m_track_clientHandle);
	pTrackDlg->dlgStu.setConnectHandle(pTrackDlg->m_track_clientHandle);
	pTrackDlg->dlgCtrl.setConnectHandle(pTrackDlg->m_track_clientHandle);
	pTrackDlg->dlgCam.setConnectHandle(pTrackDlg->m_track_clientHandle);
	return  0;
}
//回调函数处理
static int ctrlClient_process_trackMsgEx(Communtication_Head_t *head, void *msg, Commutication_Handle_t handle, void * param)
{
	CMFCTrackToolsDlg* pTrackDlg = (CMFCTrackToolsDlg*)param;
	if (param == NULL)
	{
		AfxMessageBox(_T("接收失败"));
	}
	return pTrackDlg->ctrlClient_process_trackMsg(head, msg, handle);
}
int CMFCTrackToolsDlg::ctrlClient_process_trackMsg(Communtication_Head_t *head, void *msg, Commutication_Handle_t handle)
{

 	char errMsg[128] = { 0 };
	if (NULL == head || NULL == msg || NULL == handle) {
		logFile.WriteString("----->接收信息失败！");
		logFile.Write(("\r\n"), 2);
		AfxMessageBox(_T("接收信息失败"));
		return -1;
	}
	int return_code = 0;
	return_code = head->return_code;

	if (return_code != 0)
	{
		sprintf_s(errMsg, sizeof(errMsg), "%s失败", get_track_cmd_name(head->cmd));
		CString logStr;
		logStr.Format("head cmd: %d", head->cmd);
		logFile.WriteString("----->命令信息，");
		logFile.WriteString(logStr);
		logFile.Write(("\r\n"), 2);
		AfxMessageBox(_T(errMsg));
		return -1;
	}
	switch (head->cmd)
	{
	case STU_SETTRACK_CMD:
	{
							 MessageBox("学生设置成功！");
							 break;
	}
	case TEA_SETTRACK_CMD:
	{
							 MessageBox("教师设置成功！");
							 break;
	}
	case SET_TRACK_STATUS_CMD:
	{
							 break;
	}
	case PLC_SETTRACK_CMD:
	{
							 MessageBox("设置成功，重启生效！");
							 break;
	}
	case STU_GETTRACK_CMD:
	{
		if (head->total_len != sizeof(StuITRACK_ClientParams_t))
		{
			logFile.WriteString("----->学生数据信息不匹配！");
			logFile.Write(("\r\n"), 2);
		}
		else
		{
			logFile.WriteString("----->学生数据获取成功！");
			logFile.Write(("\r\n"), 2);
			StuITRACK_ClientParams_t * stu_params = (StuITRACK_ClientParams_t *)msg;
			loadParamsFromStu(stu_params);
			ctrlClient_set_stream_display(m_streamStuHandle, m_streamTeaHandle, STU_CHANNL);
		}
		break;
	}
	case TEA_GETTRACK_CMD:
	{
		if (head->total_len != sizeof(TeaITRACK_Params))
		{
			logFile.WriteString("----->教师数据信息不匹配！");
			logFile.Write(("\r\n"), 2);
		}
		else
		{
			logFile.WriteString("----->教师数据获取成功！");
			logFile.Write(("\r\n"), 2);
			TeaITRACK_Params * tea_params = (TeaITRACK_Params *)msg;
			loadParamsFromTch(tea_params);
			ctrlClient_set_stream_display(m_streamStuHandle, m_streamTeaHandle, TEACH_CHANNL);
		}

		break;
	}
	case PLC_GETTRACK_CMD:
	{
			if (head->total_len != sizeof(Policy_Set_t))
			{
				logFile.WriteString("----->策略数据信息不匹配！");
				logFile.Write(("\r\n"), 2);
			}
			else
			{
				logFile.WriteString("----->策略数据获取成功！");
				logFile.Write(("\r\n"), 2);
				Policy_Set_t * plc_params = (Policy_Set_t *)msg;
				loadParamsFromPlc(plc_params);
				//loadParamsFromTch(plc_params);
				//ctrlClient_set_stream_display(m_streamStuHandle, m_streamTeaHandle, TEACH_CHANNL);

			}

			break;
	}
	case GET_CAMERA_INFO:
	{
		
		if (head->total_len != sizeof(Panoramic_Camera_Info))
		{
			logFile.WriteString("----->结构体头大小不一致！");
			logFile.Write(("\r\n"), 2);

		}
		else
		{
			
			Panoramic_Camera_Info * cameras_params = (Panoramic_Camera_Info *)msg;
			memcpy(&m_cameraInfo, cameras_params, sizeof(Panoramic_Camera_Info));
			//获取相机ip
			if (m_cameraInfo.ip[TCH_FEATURE_CAM]!=NULL&&m_cameraInfo.ip[STU_FEATURE_CAM]!=NULL)
			{
				logFile.WriteString("----->教师相机：");
				logFile.WriteString(m_cameraInfo.ip[TCH_FEATURE_CAM]);
				logFile.Write(("\r\n"), 2);
				logFile.WriteString("----->学生相机：");
				logFile.WriteString(m_cameraInfo.ip[STU_FEATURE_CAM]);
				logFile.Write(("\r\n"), 2);
				m_cameraInfo.nPort[TCH_FEATURE_CAM] = FEATURE_CAM_PORT;
				m_cameraInfo.nPort[STU_FEATURE_CAM] = FEATURE_CAM_PORT;
				const char* vlcArgs[] =
				{
					"-I",
					"-dummy",
					"--ignore-config",
					"--network-caching=500",
					"--ffmpeg-hw",
					"--http-reconnect",
					"--postproc-q=1"
				};
				/*m_cameraInfo.nControPort[TCH_FEATURE_CAM] = 1259;
				m_cameraInfo.nControPort[STU_FEATURE_CAM] = 1259;*/
				connectCam();
			}
			else
			{
				MessageBox("获取相机IP地址失败！");
				logFile.WriteString("----->获取相机IP地址失败！");
				logFile.Write(("\r\n"), 2);
			}
		}
		break;
	}
	case GET_TRACK_STATUS_CMD:
	{
			if (head->total_len != sizeof(Track_Status_t))
			{
				logFile.WriteString("----->跟踪数据信息不匹配！");
				logFile.Write(("\r\n"), 2);
			}
			else
			{
				logFile.WriteString("----->跟踪数据获取成功！");
				logFile.Write(("\r\n"), 2);
				Track_Status_t * trackstatus_params = (Track_Status_t *)msg;
				//trackstatus_params->isStuTrack = !trackstatus_params->isStuTrack;
				//trackstatus_params->isTchTrack = !trackstatus_params->isTchTrack;
				memcpy(&m_isAlgActivity, trackstatus_params, sizeof(Track_Status_t));
				m_check_algFlag.SetCheck(m_isAlgActivity.isTchTrack);
				if (m_isAlgActivity.isTchTrack)
				{
					g_drawPS = 0;
				}
				else
				{
					g_drawPS = 1;
				}
				m_check_stuFlag.SetCheck(m_isAlgActivity.isStuTrack);
			}
			break;
	}
	case	SET_TRACK_DEBUG_CMD:
	{
		if (head->total_len != sizeof(int))
		{
			
		}
		else
		{

		}
		break;
	}
	case TRACK_LOGIN:
	{
		//登陆成功，开启跟踪调试
		CString logStr;
		logFile.WriteString("----->登陆成功！");
		logFile.Write(("\r\n"), 2);
		logStr.Format("登陆IP地址为：");
		logStr += m_track_clientHandle->ip;
		logFile.WriteString(logStr);
		logFile.Write(("\r\n"), 2);

		Login_t * pLogin_info = (Login_t*)msg;
		ctrlClient_set_track_debug(1, m_track_clientHandle);
		ctrlClient_init_Stream();
		ctrlClient_get_track_status(m_track_clientHandle);
		
		ctrlClient_get_policy_params(m_track_clientHandle);
		ctrlClient_get_stu_params(m_track_clientHandle);
		ctrlClient_get_teach_params(m_track_clientHandle);
		ctrlClient_get_camera_params(m_track_clientHandle);
		//::PostMessage(m_connectDialog.GetSafeHwnd(), WM_CLOSE, NULL, NULL);
		::PostMessage(m_connectDialog.GetSafeHwnd(), WM_USER_LOGIN, NULL, NULL);
		//::SetEvent(m_connectDialog.loginSuccess);
		break;
	}
	case CAM_SET_CMD:
	{
						::SetEvent(dlgCam.goEvent);
						break;
	}
	case CAM_GET_CMD:
	{
						Serial_Position_t *pCamPos = (Serial_Position_t *)msg;
						CString s;
						//s.Format("%d,%d$$$$$$$$$$$$\r\n", pCamPos->posit_pan, pCamPos->posit_tilt);
						//OutputDebugString(s);
						dlgCam.m_get_panPosit = pCamPos->posit_pan;
						dlgCam.m_get_tiltPosit = pCamPos->posit_tilt;
						dlgCam.m_get_zoomValue = pCamPos->coefficient;
						//MessageBox(temp);
						::SetEvent(dlgCam.camEvent);
							break;
	}
	default:
	{
			   MessageBox("命令无效");
			   break;
	}
	}
	sprintf_s(errMsg, sizeof(errMsg), "%s成功", get_track_cmd_name(head->cmd));
	logFile.WriteString(errMsg);
	logFile.Write(("\r\n"), 2);
	//AfxMessageBox(_T(errMsg));
	return 0;
}
int CMFCTrackToolsDlg::ctrlClient_init_trackCommunticationEx(void*param, Net_Info * pnetInfo)
{
	CMFCTrackToolsDlg* pTrackDialog = (CMFCTrackToolsDlg*)param;
	if (pTrackDialog == NULL || pnetInfo == NULL)
	{
		AfxMessageBox("程序出错");
	}
	
	strcpy(pTrackDialog->m_trackIp, pnetInfo->Ip);
	memcpy(&((pTrackDialog->m_loginInfo).username), pnetInfo->username, sizeof((pTrackDialog->m_loginInfo).username));
	memcpy(&((pTrackDialog->m_loginInfo).passwd), pnetInfo->passwd, sizeof((pTrackDialog->m_loginInfo).passwd));
	return pTrackDialog->ctrlClient_init_trackCommuntication();
}
int CMFCTrackToolsDlg::ctrlClient_init_trackCommuntication()
{

	if (m_track_clientHandle != NULL)
	{
		//用户名密码登录
		ctrlClient_login(&m_loginInfo, m_track_clientHandle);
		//OutputDebugString(_T("ctrlClient_init_trackCommuntication is init"));
		return -1;
	}

	m_track_clientHandle = communtication_create_clientHandle(m_trackIp, C_CONTROL_TRACK,
		ctrlClient_process_trackMsgEx, ctrlClient_process_trackHeartEx, ctrl_connect_status, this);
	if (m_track_clientHandle == NULL) {
		AfxMessageBox(TEXT("创建客户端失败"));
		return -1;
	}
	return  0;
}
void CMFCTrackToolsDlg::OnTcnSelchangetabtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	CurSel = m_tabTrack.GetCurSel();

	switch (CurSel)
	{
	case TCH_TAB:
		dlgTch.ShowWindow(TRUE);
		dlgStu.ShowWindow(FALSE);
		dlgCtrl.ShowWindow(FALSE);
		pa.x = -10; pa.y = -10;
		pb.x = -10; pb.y = -10;
		pc.x = -10; pc.y = -10;
		pd.x = -10; pd.y = -10;
		pt.x = -10; pt.y = -10;
		p1.x = -10; p1.y = -10;
		p2.x = -10; p2.y = -10;
		p3 = p4 = pl = pr = { -10 };
		mouseCnt = 0;

		dlgCam.GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_LEFT)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_ZOOMIN)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_ZOOMOUT)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_LEFT_PRESET)->ShowWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_RIGHT_PRESET)->ShowWindow(TRUE);
		dlgCam.GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(TRUE);

		dlgCam.GetDlgItem(IDC_BUT_CALIBRATION)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUT_AGAINCALIB)->ShowWindow(FALSE);

		dlgCam.GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON3)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON4)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON5)->ShowWindow(FALSE);
		ctrlClient_get_teach_params(m_track_clientHandle);
		break;
	case STU_TAB:
		dlgTch.ShowWindow(FALSE);
		dlgStu.ShowWindow(TRUE);
		dlgCtrl.ShowWindow(FALSE);
		tch.x = -10; tch.y = -10;
		tch.width = -10; tch.height = -10;
		blk.x = -10; blk.y = -10;
		blk.width = -10; blk.height = -10;
		pt.x = -10; pt.y = -10;
		mouseCnt = 0;

		dlgCam.GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_LEFT)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_ZOOMIN)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_ZOOMOUT)->EnableWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(TRUE);
		//屏蔽教师预置位设置
		dlgCam.GetDlgItem(IDC_BUTTON_LEFT_PRESET)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_RIGHT_PRESET)->ShowWindow(FALSE);
		//dlgTch.dlgCam.GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(FALSE);

		dlgCam.GetDlgItem(IDC_BUT_CALIBRATION)->ShowWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUT_AGAINCALIB)->ShowWindow(TRUE);

		dlgCam.GetDlgItem(IDC_BUTTON2)->ShowWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);
		dlgCam.GetDlgItem(IDC_BUTTON5)->ShowWindow(TRUE);
		ctrlClient_get_stu_params(m_track_clientHandle);
		break;
	case CTRL_TAB:
		pa = pb = pc = pd = { -10 };
		pA = pB = pC = pD = { -10 };
		ln1[0] = ln2[0] = ln3[0] = ln4[0] = { -10 };
		ln1[1] = ln2[1] = ln3[1] = ln4[1] = { -10 };
		dlgTch.ShowWindow(FALSE);
		dlgStu.ShowWindow(FALSE);
		dlgCtrl.ShowWindow(TRUE);

		dlgCam.GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_LEFT)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_ZOOMIN)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_ZOOMOUT)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_LEFT_PRESET)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON_RIGHT_PRESET)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(FALSE);

		dlgCam.GetDlgItem(IDC_BUT_CALIBRATION)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUT_AGAINCALIB)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON3)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON4)->ShowWindow(FALSE);
		dlgCam.GetDlgItem(IDC_BUTTON5)->ShowWindow(FALSE);
		ctrlClient_get_policy_params(m_track_clientHandle);
	default:
		;
	}

	*pResult = 0;
}


BOOL CMFCTrackToolsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//CString str;
	//str.Format("message:%x, Param:%x\r\n", pMsg->message,pMsg->wParam);
	//OutputDebugString(str);
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽  
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_CONTROL)
	{
		isKeyDown = CTRL_KEY_DOWN;
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_SHIFT)
	{
		isKeyDown = SHIFT_KEY_DOWN;
	}
	if (pMsg->message == WM_KEYUP&&pMsg->wParam == VK_CONTROL)
	{
		isKeyDown = CTRL_KEY_UP;
	}
	if (pMsg->message == WM_KEYUP&&pMsg->wParam == VK_SHIFT)
	{
		isKeyDown = CTRL_KEY_UP;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_SHIFT) < 0)
		{
			isKeyDown = KEY_ERROR;
			//return TRUE;
		}
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == this->m_hWnd)
		{
			//MessageBox("Fuck!");
			this->SetFocus();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
BOOL CMFCTrackToolsDlg::connectCam()
{
	//连接相机
	int iResult = 0;
	WSADATA wsaData = { 0 };
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	BOOL ret;
	if (m_tch_cam.login(&m_uiHandle_tch, CAM_USER, CAM_PSWD, m_cameraInfo.ip[TCH_FEATURE_CAM], m_cameraInfo.nPort[TCH_FEATURE_CAM]))
	{
		dlgCam.m_CameraControl_tch.startControl(m_cameraInfo.ip[TCH_FEATURE_CAM], m_cameraInfo.nControPort[TCH_FEATURE_CAM]);
		dlgCam.m_CameraControl_tch.keepInstruct(PANandTILT_CTRL_PTZ_FOCUSAUTO);//设置相机为自动对焦
		logFile.WriteString("----->教师相机连接成功！");
		logFile.Write(("\r\n"), 2);
		HI_S32 camRes = m_tch_cam.StreamStart();
		if (camRes == HI_SUCCESS)
		{
			logFile.WriteString("----->教师相机流打开成功！");
			logFile.Write(("\r\n"), 2);
			ret = TRUE;
		}
		else
		{
			logFile.WriteString("----->教师相机流打开失败！");
			CString temp;
			temp.Format("教师特写，err code: %d", camRes);
			logFile.WriteString(temp);
			logFile.Write(("\r\n"), 2);
			MessageBox(temp);
			ret = FALSE;
		}
	}
	else
	{
		logFile.WriteString("----->教师相机连接失败！");
		logFile.Write(("\r\n"), 2);
		MessageBox("教师相机连接失败！");
		ret = FALSE;
	}

	if (m_stu_cam.login(&m_uiHandle_tch, CAM_USER, CAM_PSWD, m_cameraInfo.ip[STU_FEATURE_CAM], m_cameraInfo.nPort[STU_FEATURE_CAM]))
	{
		dlgCam.m_CameraControl_stu.startControl(m_cameraInfo.ip[STU_FEATURE_CAM], m_cameraInfo.nControPort[STU_FEATURE_CAM]);
		dlgCam.m_CameraControl_stu.keepInstruct(PANandTILT_CTRL_PTZ_FOCUSAUTO);//设置相机为自动对焦
		logFile.WriteString("----->学生相机连接成功！");
		logFile.Write(("\r\n"), 2);
		HI_S32 camRes = m_stu_cam.StreamStart();
		if (camRes == HI_SUCCESS)
		{
			logFile.WriteString("----->学生相机流打开成功！");
			logFile.Write(("\r\n"), 2);
			ret = TRUE;
		}
		else
		{
			logFile.WriteString("----->学生相机流打开失败！");
			CString temp;
			temp.Format("学生特写，err code: %d", camRes);
			logFile.WriteString(temp);
			logFile.Write(("\r\n"), 2);
			MessageBox(temp);
			ret = FALSE;
		}
		
	}
	else
	{
		MessageBox("学生相机连接失败！");
		logFile.WriteString("----->学生相机连接失败！");
		logFile.Write(("\r\n"), 2);
		ret = FALSE;
	}
	
	
	//showImage();
	return ret;
}
void CMFCTrackToolsDlg::initCamDlg(int cx,int cy, CRect rct)
{
	CRect rsDlgcam;
	dlgCam.setMainDlg(this);
	dlgCam.Create(IDD_CAMCONTROL, GetDlgItem(IDD_CAMCONTROL));
	dlgCam.GetClientRect(rsDlgcam);
	rsDlgcam.left = rct.left;
	cx = rct.right - rct.left;
	rsDlgcam.top = rct.bottom;
	cy = rsDlgcam.bottom;
	dlgCam.SetWindowPos(NULL, rsDlgcam.left, rsDlgcam.top, cx, cy, SWP_NOZORDER);
	dlgCam.ShowWindow(TRUE);

	dlgCam.m_comboSpeed.InsertString(0, "5");
	dlgCam.m_comboSpeed.InsertString(1, "12");
	dlgCam.m_comboSpeed.InsertString(2, "20");

	dlgCam.m_comboSpeed.SetCurSel(1);
	//dlgCam.setNumOfPreset(10);
	
	//connectCam();
	
}
void CMFCTrackToolsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==1)
	{
		showImage();
	}
	CDialogEx::OnTimer(nIDEvent);
}





void CMFCTrackToolsDlg::OnBnClickedCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_isAlgActivity.isTchTrack = m_check_algFlag.GetCheck();
	if (m_isAlgActivity.isTchTrack)
	{
		g_drawPS = 0;
	}
	else
	{
		g_drawPS = 1;
	}
	ctrlClient_set_track_status(&m_isAlgActivity, m_track_clientHandle);
}


void CMFCTrackToolsDlg::OnBnClickedCheck2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_isAlgActivity.isStuTrack = m_check_stuFlag.GetCheck();
	ctrlClient_set_track_status(&m_isAlgActivity, m_track_clientHandle);
}


void CMFCTrackToolsDlg::OnBnClickedButtonAbout()
{
	// TODO:  在此添加控件通知处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	this->SetFocus();
}

void CMFCTrackToolsDlg::OnBnClickedBtnSave()
{
	// TODO:  在此添加控件通知处理程序代码
	//保存到本地
	//CString filter;
	//CString fileName;
	//switch (CurSel)
	//{
	//	/*case TCH_TAB:
	//		fileName = _T("tch_params.yml");
	//		break;
	//		case STU_TAB:
	//		fileName = _T("stu_params.yml");
	//		break;
	//		case CTRL_TAB:
	//		fileName = _T("plc_params.yml");
	//		break;*/
	//case TCH_TAB:
	//	fileName = _T("tch_params.tch");
	//	filter = _T("文件 (*.tch)|*.tch||");
	//	break;
	//case STU_TAB:
	//	fileName = _T("stu_params.stu");
	//	filter = _T("文件 (*.stu)|*.stu||");
	//	break;
	//case CTRL_TAB:
	//	fileName = _T("plc_params.plc");
	//	filter = _T("文件 (*.plc)|*.plc||");
	//	break;
	//default:
	//	break;
	//}
	CString fileName = _T("params.yml");//默认打开的文件名  
	CString filter = _T("文件 (*.yml)|*.yml|文件（*.xml)|*.xml||");		//文件过虑的类型  
	CFileDialog openFileDlg(FALSE, "yml", fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		CString FilePathName = openFileDlg.GetPathName();
		save_Parameter(FilePathName.GetBuffer(0)/*, &dlgStu.stu_params, &dlgTch.tch_params, &dlgCtrl.ctrl_params*/);
		this->SetFocus();
	}
}

void CMFCTrackToolsDlg::save_Parameter(std::string filePath/*, StuITRACK_ClientParams_t* stu_params, TeaITRACK_Params* tch_params, Policy_Set_t* ctrl_params*/)
{
	cv::FileStorage fs(filePath, cv::FileStorage::WRITE);
	if (fs.isOpened())
	{
		//存储教师的参数
		fs << "tch_height" << dlgTch.tch_params.tch.height;
		fs << "tch_width" << dlgTch.tch_params.tch.width;
		fs << "tch_x" << dlgTch.tch_params.tch.x;
		fs << "tch_y" << dlgTch.tch_params.tch.y;

		fs << "blk_height" << dlgTch.tch_params.blk.height;
		fs << "blk_width" << dlgTch.tch_params.blk.width;
		fs << "blk_x" << dlgTch.tch_params.blk.x;
		fs << "blk_y" << dlgTch.tch_params.blk.y;

		fs << "numOfPos" << dlgTch.tch_params.numOfPos;
		fs << "numOfSlide" << dlgTch.tch_params.numOfSlide;
		fs << "outside" << dlgTch.tch_params.threshold.outside;
		fs << "stand" << dlgTch.tch_params.threshold.stand;
		fs << "targetArea" << dlgTch.tch_params.threshold.targetArea;
		fs << "maxArea" << dlgTch.tch_params.maxArea;


		//存储学生参数
		fs << "height" << dlgStu.stu_params.height;
		fs << "width" << dlgStu.stu_params.width;
		fs << "stuTrack_debugMsg_flag" << dlgStu.stu_params.stuTrack_debugMsg_flag;
		fs << "stuTrack_Draw_flag" << dlgStu.stu_params.stuTrack_Draw_flag;

		fs << "stuTrack_direct_standard" << "[";
		fs << dlgStu.stu_params.stuTrack_direct_standard[0] << dlgStu.stu_params.stuTrack_direct_standard[1] << dlgStu.stu_params.stuTrack_direct_standard[2] << dlgStu.stu_params.stuTrack_direct_standard[3];
		fs << "]";

		fs << "stuTrack_stuWidth_standard" << "[";
		fs << dlgStu.stu_params.stuTrack_stuWidth_standard[0] << dlgStu.stu_params.stuTrack_stuWidth_standard[1] << dlgStu.stu_params.stuTrack_stuWidth_standard[2] << dlgStu.stu_params.stuTrack_stuWidth_standard[3];
		fs << "]";

		fs << "stuTrack_direct_range" << dlgStu.stu_params.stuTrack_direct_range;
		fs << "stuTrack_standCount_threshold" << dlgStu.stu_params.stuTrack_standCount_threshold;
		fs << "stuTrack_sitdownCount_threshold" << dlgStu.stu_params.stuTrack_sitdownCount_threshold;
		fs << "stuTrack_moveDelayed_threshold" << dlgStu.stu_params.stuTrack_moveDelayed_threshold;
		fs << "stuTrack_deleteTime_threshold" << dlgStu.stu_params.stuTrack_deleteTime_threshold;
		fs << "stuTrack_move_threshold" << dlgStu.stu_params.stuTrack_move_threshold;
		fs << "stuTrack_standup_threshold" << dlgStu.stu_params.stuTrack_standup_threshold;

		fs << "stuTrack_vertex" << "[";
		for (int i = 0; i < 4; i++)
		{
			fs << "{";
			fs << "x" << dlgStu.stu_params.stuTrack_vertex[i].x;
			fs << "y" << dlgStu.stu_params.stuTrack_vertex[i].y;
			fs << "}";
		}
		fs << "]";

		fs << "transformationMatrix" << "[";
		for (int i = 0; i < 9; i++)
		{
			fs << dlgStu.stu_params.transformationMatrix[i];
		}
		fs << "]";

		fs << "stretchingAB" << "[";
		fs << dlgStu.stu_params.stretchingAB[0] << dlgStu.stu_params.stretchingAB[1];
		fs << "]";

		fs << "mut_pic_flag" << dlgCtrl.ctrl_params.mut_pic_flag;
		fs << "stu_feature_flag" << dlgCtrl.ctrl_params.stu_feature_flag;
		fs << "blb_time_min" << dlgCtrl.ctrl_params.time.blb_time_min;
		fs << "ppt_time_min" << dlgCtrl.ctrl_params.time.ppt_time_min;
		fs << "stu_time_min" << dlgCtrl.ctrl_params.time.stu_time_min;
		fs << "tea_time_min" << dlgCtrl.ctrl_params.time.tea_time_min;
		fs.release();
	}
	else
	{
		MessageBox(_T("文件打开错误！"));
		fs.release();
	}
}

bool CMFCTrackToolsDlg::load_Parameter(std::string filePath)
{
	cv::FileStorage fs(filePath, cv::FileStorage::READ);
	if (fs.isOpened())
	{
		if (CurSel == TCH_TAB)
		{
			TeaITRACK_Params tch_params;
			//memset(&tch_params, 0, sizeof(TeaITRACK_Params));
			fs["tch_height"] >> tch_params.tch.height;
			fs["tch_width"] >> tch_params.tch.width;
			fs["tch_x"] >> tch_params.tch.x;
			fs["tch_y"] >> tch_params.tch.y;
			fs["blk_height"] >> tch_params.blk.height;
			fs["blk_width"] >> tch_params.blk.width;
			fs["blk_x"] >> tch_params.blk.x;
			fs["blk_y"] >> tch_params.blk.y;
			fs["numOfPos"] >> tch_params.numOfPos;
			fs["numOfSlide"] >> tch_params.numOfSlide;
			fs["outside"] >> tch_params.threshold.outside;
			fs["stand"] >> tch_params.threshold.stand;
			fs["targetArea"] >> tch_params.threshold.targetArea;
			fs["maxArea"] >> tch_params.maxArea;
			fs.release();
			loadParamsFromTch(&tch_params);
		}
		if (CurSel == STU_TAB)
		{
			StuITRACK_ClientParams_t stu_params;
			int i = 0;
			fs["height"] >> stu_params.height;
			fs["width"] >> stu_params.width;
			fs["stuTrack_debugMsg_flag"] >> stu_params.stuTrack_debugMsg_flag;
			fs["stuTrack_Draw_flag"] >> stu_params.stuTrack_Draw_flag;

			cv::FileNode node = fs["stuTrack_direct_standard"];
			if (node.type() != cv::FileNode::SEQ)
			{
				return false;
			}
			cv::FileNodeIterator it = node.begin(), it_end = node.end();
			for (i = 0; it != it_end; ++it, i++)
			{
				stu_params.stuTrack_direct_standard[i] = *it;
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
				stu_params.stuTrack_stuWidth_standard[i] = *it;
			}
			if (i != 4)
			{
				::MessageBox(NULL, _T("学生宽度参数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
			}

			fs["stuTrack_direct_range"] >> stu_params.stuTrack_direct_range;
			fs["stuTrack_standCount_threshold"] >> stu_params.stuTrack_standCount_threshold;
			fs["stuTrack_sitdownCount_threshold"] >> stu_params.stuTrack_sitdownCount_threshold;
			fs["stuTrack_moveDelayed_threshold"] >> stu_params.stuTrack_moveDelayed_threshold;
			fs["stuTrack_deleteTime_threshold"] >> stu_params.stuTrack_deleteTime_threshold;
			fs["stuTrack_move_threshold"] >> stu_params.stuTrack_move_threshold;
			fs["stuTrack_standup_threshold"] >> stu_params.stuTrack_standup_threshold;

			node = fs["stuTrack_vertex"];
			if (node.type() != cv::FileNode::SEQ)
			{
				return false;
			}
			it = node.begin(), it_end = node.end();
			for (i = 0; it != it_end; ++it, i++)
			{
				stu_params.stuTrack_vertex[i].x = (*it)["x"];
				stu_params.stuTrack_vertex[i].y = (*it)["y"];
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
				stu_params.transformationMatrix[i] = *it;
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
				stu_params.stretchingAB[i] = *it;
			}
			if (i != 2)
			{
				::MessageBox(NULL, _T("拉伸系数载入出错！"), NULL, MB_OK | MB_ICONWARNING);
			}
			fs.release();
			loadParamsFromStu(&stu_params);
		}
		if (CurSel == CTRL_TAB)
		{
			Policy_Set_t plc;
			fs["mut_pic_flag"] >> plc.mut_pic_flag;
			fs["stu_feature_flag"] >> plc.stu_feature_flag;
			fs["blb_time_min"] >> plc.time.blb_time_min;
			fs["ppt_time_min"] >> plc.time.ppt_time_min;
			fs["stu_time_min"] >> plc.time.stu_time_min;
			fs["tea_time_min"] >> plc.time.tea_time_min;
			fs.release();
			loadParamsFromPlc(&plc);
		}
		return true;
	}
	else
	{
		fs.release();
		return false;
	}
}


void CMFCTrackToolsDlg::OnBnClickedBtnLoad()
{
	// TODO:  在此添加控件通知处理程序代码
	//CString fileName = _T("params.yml");
	/*switch (CurSel)
	{
	case TCH_TAB:
		fileName = _T("tch_params.tch");
		filter = _T("文件 (*.tch)|*.tch||");
		break;
	case STU_TAB:
		fileName = _T("stu_params.stu");
		filter = _T("文件 (*.stu)|*.stu||");
		break;
	case CTRL_TAB:
		fileName = _T("plc_params.plc");
		filter = _T("文件 (*.plc)|*.plc||");
		break;
	default:
		break;
	}*/
	CString filter = _T("文件 (*.yml)|*.yml|文件（*.xml)|*.xml||");		//文件过虑的类型  
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		CString FilePathName = openFileDlg.GetPathName();
		load_Parameter(FilePathName.GetBuffer(0));
		this->SetFocus();
	}
}
