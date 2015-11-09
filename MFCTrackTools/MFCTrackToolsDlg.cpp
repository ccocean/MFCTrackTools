
// MFCTrackToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "MFCTrackToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LEFT_PIXEL 1020
#define UP_PIXEL 20
#define PIC_TOP 20

CMFCTrackToolsDlg *g_pDlg;

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
//	afx_msg void OnClose();
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
}

void CMFCTrackToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_Exit, m_Exit);
	DDX_Control(pDX, IDC_grpBoxTrack, m_grpBoxTrack);
	DDX_Control(pDX, IDC_txtX, m_txtX);
	DDX_Control(pDX, IDC_editX, m_editX);
	DDX_Control(pDX, IDC_txtY, m_txtY);
	DDX_Control(pDX, IDC_editY, m_editY);
	DDX_Control(pDX, IDC_txtTch, m_txtTch);
	DDX_Control(pDX, IDC_txtW, m_txtW);
	DDX_Control(pDX, IDC_editW, m_editW);
	DDX_Control(pDX, IDC_txtH, m_txtH);
	DDX_Control(pDX, IDC_editH, m_editH);
	DDX_Control(pDX, IDC_btnSaveTch, m_btnSaveTch);
	DDX_Control(pDX, IDC_btnSaveBlk, m_btnSaveBlk);
	DDX_Control(pDX, IDC_txtT, m_txtT);
	DDX_Control(pDX, IDC_txtB, m_txtB);
	DDX_Control(pDX, IDC_txtTchArg, m_txtTchArg);
	DDX_Control(pDX, IDC_txtBlkArg, m_txtBlkArg);
	DDX_Control(pDX, IDC_grpBoxPos, m_grpBoxPos);
	DDX_Control(pDX, IDC_txtPos, m_txtPos);
	DDX_Control(pDX, IDC_txtSlide, m_txtSlide);
	DDX_Control(pDX, IDC_editPos, m_editPos);
	DDX_Control(pDX, IDC_editSlide, m_editSlide);
	DDX_Control(pDX, IDC_btnSet, m_btnSet);
	DDX_Control(pDX, IDC_btnSavePos, m_btnSavePos);
	DDX_Control(pDX, IDC_picSrc, m_picSrc);
	DDX_Control(pDX, IDC_listErr, m_listErr);
	DDX_Control(pDX, IDC_btnSaveTrack, m_btnSaveTrack);
	DDX_Control(pDX, IDC_txtStand, m_txtStand);
	DDX_Control(pDX, IDC_txtTargetArea, m_txtTargetArea);
	DDX_Control(pDX, IDC_txtOutSide, m_txtOutSide);
	DDX_Control(pDX, IDC_editStand, m_editStand);
	DDX_Control(pDX, IDC_editTargetArea, m_editTargetArea);
	DDX_Control(pDX, IDC_editOutSide, m_editOutSide);
	DDX_Control(pDX, IDC_grpBoxThreshold, m_grpBoxThreshold);
	DDX_Control(pDX, IDC_btnSaveThreshold, m_btnSaveThreshold);
	DDX_Control(pDX, IDC_btnApply, m_btnApply);
	DDX_Control(pDX, IDC_btnDefault, m_btnDefault);
	DDX_Control(pDX, IDC_picCam, m_picCam);
}

//消息映射
BEGIN_MESSAGE_MAP(CMFCTrackToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//ON_BN_CLICKED(IDC_btnPlay, &CMFCTrackToolsDlg::OnBnClickedbtnplay)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_btnSaveTch, &CMFCTrackToolsDlg::OnBnClickedbtnsavetch)
	ON_BN_CLICKED(IDC_btnSaveBlk, &CMFCTrackToolsDlg::OnBnClickedbtnsaveblk)
	ON_BN_CLICKED(IDC_btnSet, &CMFCTrackToolsDlg::OnBnClickedbtnset)
	ON_BN_CLICKED(IDC_btnSavePos, &CMFCTrackToolsDlg::OnBnClickedbtnsavepos)
	ON_BN_CLICKED(IDC_btnSaveTrack, &CMFCTrackToolsDlg::OnBnClickedbtnsavetrack)
	ON_BN_CLICKED(IDC_btnSaveThreshold, &CMFCTrackToolsDlg::OnBnClickedbtnsavethreshold)
	ON_BN_CLICKED(IDC_btnApply, &CMFCTrackToolsDlg::OnBnClickedbtnapply)
	ON_BN_CLICKED(IDC_btnDefault, &CMFCTrackToolsDlg::OnBnClickedbtndefault)
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

	// TODO:  在此添加额外的初始化代码

	//初始化窗口
	CWnd::SetWindowPos(NULL, 0, 0, 1280, 720, SWP_NOZORDER);//初始化窗口大小
	CenterWindow();//窗口居中

	//初始化控件

	//跟踪参数控件
	m_grpBoxTrack.SetWindowPos(NULL, LEFT_PIXEL, UP_PIXEL, 220, 300, SWP_NOZORDER);
	m_txtTch.SetWindowPos(NULL, LEFT_PIXEL+20, 50, 50, 20, SWP_NOZORDER);
	m_txtX.SetWindowPos(NULL, LEFT_PIXEL + 20, 70, 20, 20, SWP_NOZORDER);
	m_editX.SetWindowPos(NULL, LEFT_PIXEL + 40, 70, 60, 20, SWP_NOZORDER);
	m_txtY.SetWindowPos(NULL, LEFT_PIXEL + 120, 70, 20, 20, SWP_NOZORDER);
	m_editY.SetWindowPos(NULL, LEFT_PIXEL + 140, 70, 60, 20, SWP_NOZORDER);

	m_txtW.SetWindowPos(NULL, LEFT_PIXEL + 20, 100, 20, 20, SWP_NOZORDER);
	m_editW.SetWindowPos(NULL, LEFT_PIXEL + 40, 100, 60, 20, SWP_NOZORDER);
	m_txtH.SetWindowPos(NULL, LEFT_PIXEL + 120, 100, 20, 20, SWP_NOZORDER);
	m_editH.SetWindowPos(NULL, LEFT_PIXEL + 140, 100, 60, 20, SWP_NOZORDER);

	m_btnSaveTch.SetWindowPos(NULL, LEFT_PIXEL + 20, 130, 80, 20, SWP_NOZORDER);
	m_btnSaveBlk.SetWindowPos(NULL, LEFT_PIXEL + 120, 130, 80, 20, SWP_NOZORDER);

	m_txtT.SetWindowPos(NULL, LEFT_PIXEL + 20, 170, 80, 20, SWP_NOZORDER);
	m_txtB.SetWindowPos(NULL, LEFT_PIXEL + 20, 230, 80, 20, SWP_NOZORDER);

	m_txtTchArg.SetWindowPos(NULL, LEFT_PIXEL + 40, 200, 150, 20, SWP_NOZORDER);
	m_txtTchArg.SetWindowTextW(_T("Rect(0, 0, 0, 0)"));
	m_txtBlkArg.SetWindowPos(NULL, LEFT_PIXEL + 40, 260, 150, 20, SWP_NOZORDER);
	m_txtBlkArg.SetWindowTextW(_T("Rect(0, 0, 0, 0)"));
	m_btnSaveTrack.SetWindowPos(NULL, LEFT_PIXEL + 120, 280, 80, 20, SWP_NOZORDER);

	//预置位参数控件
	m_grpBoxPos.SetWindowPos(NULL, LEFT_PIXEL, UP_PIXEL + 305, 220, 150, SWP_NOZORDER);
	m_txtPos.SetWindowPos(NULL, LEFT_PIXEL + 20, UP_PIXEL + 325, 80, 20, SWP_NOZORDER);
	m_editPos.SetWindowPos(NULL, LEFT_PIXEL + 90, UP_PIXEL + 325, 80, 20, SWP_NOZORDER);
	m_txtSlide.SetWindowPos(NULL, LEFT_PIXEL + 20, UP_PIXEL + 355, 80, 20, SWP_NOZORDER);
	m_editSlide.SetWindowPos(NULL, LEFT_PIXEL + 90, UP_PIXEL + 355, 80, 20, SWP_NOZORDER);

	m_btnSet.SetWindowPos(NULL, LEFT_PIXEL + 20, UP_PIXEL + 390, 80, 20, SWP_NOZORDER);
	m_btnSavePos.SetWindowPos(NULL, LEFT_PIXEL + 110, UP_PIXEL + 390, 80, 20, SWP_NOZORDER);

	//阈值参数控件
	m_grpBoxThreshold.SetWindowPos(NULL, LEFT_PIXEL, UP_PIXEL + 460, 220, 150, SWP_NOZORDER);
	m_txtStand.SetWindowPos(NULL, LEFT_PIXEL + 20, UP_PIXEL + 490, 80, 20, SWP_NOZORDER);
	m_editStand.SetWindowPos(NULL, LEFT_PIXEL + 80, UP_PIXEL + 490, 80, 20, SWP_NOZORDER);
	m_editStand.SetWindowTextW(_T("以秒为单位"));
	m_txtTargetArea.SetWindowPos(NULL, LEFT_PIXEL + 20, UP_PIXEL + 520, 80, 20, SWP_NOZORDER);
	m_editTargetArea.SetWindowPos(NULL, LEFT_PIXEL + 80, UP_PIXEL + 520, 80, 20, SWP_NOZORDER);
	m_editTargetArea.SetWindowTextW(_T("以像素为单位"));
	m_txtOutSide.SetWindowPos(NULL, LEFT_PIXEL + 20, UP_PIXEL + 550, 80, 20, SWP_NOZORDER);
	m_editOutSide.SetWindowPos(NULL, LEFT_PIXEL + 80, UP_PIXEL + 550, 80, 20, SWP_NOZORDER);
	m_editOutSide.SetWindowTextW(_T("以像素为单位"));
	m_btnSaveThreshold.SetWindowPos(NULL, LEFT_PIXEL + 100, UP_PIXEL + 580, 80, 20, SWP_NOZORDER);
	m_btnApply.SetWindowPos(NULL, LEFT_PIXEL + 120, UP_PIXEL + 630, 80, 20, SWP_NOZORDER);
	m_btnDefault.SetWindowPos(NULL, LEFT_PIXEL + 10, UP_PIXEL + 630, 100, 20, SWP_NOZORDER);

	//显示控件及日志控件
	m_picSrc.SetWindowPos(NULL, 40, PIC_TOP+40, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_picCam.SetWindowPos(NULL, 40, PIC_TOP + 350, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_listErr.SetWindowPos(NULL, 600, PIC_TOP+50, 360, 240, SWP_NOZORDER);

	pDC = GetDlgItem(IDC_picSrc)->GetDC();
	hdc = pDC->GetSafeHdc();
	GetDlgItem(IDC_picSrc)->GetClientRect(&picRect);
	//g_video = cvCaptureFromFile(g_videoPath.c_str());

	//fps = cvGetCaptureProperty(g_video, CV_CAP_PROP_FPS);
	//vfps = 1000 / fps;
	SetTimer(1, fps, NULL);

	p1.x = 0;
	p1.y = 0;
	p2.x = 0;
	p2.y = 0;


	g_pDlg = this;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	CDialogEx::OnClose();
}


void CMFCTrackToolsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*CDC *pDC = GetDlgItem(IDC_picSrc)->GetDC();
	HDC hdc = pDC->GetSafeHdc();*/
	//if (g_video == NULL)
	//{
	//	printf("No video!");
	//}
	//frame = cvQueryFrame(g_video);
	//if (frame)
	//{
	//	//cvResize(frame, srcImg, 1);
	//	if (g_drawPS==1)
	//	{
	//		for (int i = 0; i < Frame_Width; i += (Frame_Width / int_pos))
	//		{
	//			//cvLine(srcImg, cvPoint(i, 0), cvPoint(i, Frame_Height), cvScalar(0, 0, 255));
	//			//cvRectangle(srcImg, cvPoint(camPosSlide.left*(Frame_Width / int_pos), 0), cvPoint((camPosSlide.right + 1) * (Frame_Width / int_pos), Frame_Height), cvScalar(255, 0, 0));//画预置位滑框
	//		}
	//	}
	//	if (g_drawTch==1)
	//	{
	//		//cvRectangle(srcImg, cvPoint(tch.x, tch.y), cvPoint(tch.x + tch.width, tch.y + tch.height), cvScalar(0, 255, 0));
	//		//cvRectangle(srcImg, cvPoint(blk.x, blk.y), cvPoint(blk.x + blk.width, blk.y + blk.height), cvScalar(0, 255, 0));
	//		/*p1.x = -1; p1.y = -1;
	//		p2.x = -1; p2.y = -1;
	//		pt.x = -1; pt.y = -1;*/
	//	}
	//	if (g_drawBlk==1)
	//	{
	//		//cvRectangle(srcImg, cvPoint(blk.x, blk.y), cvPoint(blk.x + blk.width, blk.y + blk.height), cvScalar(0, 255, 0));
	//	}
	//	if (mouseStatus == Mouse_LBDOWN)
	//	{
	//		if (pt.x > 0 && pt.y > 0)
	//		{
	//			//cvRectangle(srcImg, p1, pt, cvScalar(0, 255, 255), 1, 8, 0);
	//		}
	//	}
	//	else if (mouseStatus==Mouse_LBUP)
	//	{
	//		if (p2.x<p1.x||p2.y<p1.y)
	//		{
	//			pt = p2;
	//			p2 = p1;
	//			p1 = pt;
	//		}
	//		if (p2.x>p1.x&&p2.y<p1.y)
	//		{
	//			pt.y = p1.y;
	//			p1.y = p2.y;
	//			p2.y = pt.y;
	//		}
	//		if (p2.x<p1.x&&p2.y>p1.y)
	//		{
	//			pt.x = p1.x;
	//			p1.x = p2.x;
	//			p2.x = pt.x;
	//		}
	//		//cvRectangle(srcImg, p1, p2, cvScalar(0, 255, 255), 1, 8, 0);
	//		SetDlgItemInt(IDC_editX, p1.x);
	//		SetDlgItemInt(IDC_editY, p1.y);
	//		SetDlgItemInt(IDC_editW, p2.x-p1.x);
	//		SetDlgItemInt(IDC_editH, p2.y-p1.y);
	//	}
	//	else if (mouseStatus==Mouse_DRAG)
	//	{
	//		//cvRectangle(srcImg, p1, p2, cvScalar(0, 255, 255), 1, 8, 0);
	//	}
	//	//cimg.CopyOf(srcImg, srcImg->nChannels);
	//	//cimg.DrawToHDC(hdc, &picRect);
	//}
	//CDialogEx::OnTimer(nIDEvent);
}


void CMFCTrackToolsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	if (20 <= point.x&&point.x <= 20 + Frame_Width&&PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		if (p1.x<point.x - 20 && point.x - 20<p2.x&&p1.y<point.y - PIC_TOP&&point.y - PIC_TOP<p2.y)
		{
			pt.x = point.x;
			pt.y = point.y;
			mouseStatus = Mouse_DRAG;
		}
		else
		{
			pt.x = 0, pt.y = 0;
			p1.x = point.x - 20;
			p1.y = point.y - PIC_TOP;
			//p2.x = 0, p2.y = 0;
			mouseStatus = Mouse_LBDOWN;
		}
		
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCTrackToolsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (20 <= point.x&&point.x <= 20 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		//m_txtTchArg.SetWindowTextW(_T("Rect(0,0,0,0)"));
		if (mouseStatus!=Mouse_DRAG)
		{
			p2.x = point.x - 20;
			p2.y = point.y - PIC_TOP;
			mouseStatus = Mouse_LBUP;
			pt.x = -1;
			pt.y = -1;
		}
		else
		{
			if (p1.x < point.x&&point.x < p2.x&&p1.y < point.y&&point.y < p2.y)
			{
				mouseStatus = Mouse_LBUP;
			}
			else
			{
				/*p1.x = 0, p1.y = 0;
				p2.x = 0, p2.y = 0;
				pt.x = point.x, pt.y = point.y;*/
				mouseStatus = Mouse_LBUP;
			}
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCTrackToolsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (20 <= point.x&&point.x <= 20 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		//m_txtTchArg.SetWindowTextW(_T("Rect(0,0,0,0)"));
		if (mouseStatus==Mouse_LBDOWN)
		{
			pt.x = point.x - 20;
			pt.y = point.y - PIC_TOP;
		}
		if (mouseStatus==Mouse_DRAG)
		{
			p1.x += (point.x - pt.x);
			p1.y += (point.y - pt.y);
			p2.x += (point.x - pt.x);
			p2.y += (point.y - pt.y);
			pt.x = point.x;
			pt.y = point.y;
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCTrackToolsDlg::OnBnClickedbtnsavetch()
{
	if (p1.x <= 0 || p1.y <= 0 || p2.x <= 0 || p2.y <= 0)
	{
		AfxMessageBox(_T("没有获得数据！"));
	}
	else
	{
		tch.x = p1.x;
		tch.y = p1.y;
		tch.width = p2.x - p1.x;
		tch.height = p2.y - p1.y;
		params.tch = tch;
		CString a=_T(", ");
		tmp.Format(_T("%d"), p1.x);
		str += tmp;
		str += a;
		tmp.Format(_T("%d"), p1.y);
		str += tmp;
		str += a;
		tmp.Format(_T("%d"), p2.x-p1.x);
		str += tmp;
		str += a;
		tmp.Format(_T("%d"), p2.y-p1.y);
		str += tmp;
		//LPCTSTR str;
		m_txtTchArg.SetWindowTextW(str);
		str = _T("");
		g_drawTch = 1;
		p1.x = 0, p1.y = 0;
		p2.x = 0, p2.y = 0;
		
	}
	// TODO:  在此添加控件通知处理程序代码
}


void CMFCTrackToolsDlg::OnBnClickedbtnsaveblk()
{
	if (p1.x <= 0 || p1.y <= 0 || p2.x <= 0 || p2.y <= 0)
	{
		AfxMessageBox(_T("没有获得数据！"));
	}
	else
	{
		blk.x = p1.x;
		blk.y = p1.y;
		blk.width = p2.x - p1.x;
		blk.height = p2.y - p1.y;
		params.blk = blk;
		CString a = _T(", ");
		tmp.Format(_T("%d"), p1.x);
		str += tmp;
		str += a;
		tmp.Format(_T("%d"), p1.y);
		str += tmp;
		str += a;
		tmp.Format(_T("%d"), p2.x - p1.x);
		str += tmp;
		str += a;
		tmp.Format(_T("%d"), p2.y - p1.y);
		str += tmp;
		//LPCTSTR str;
		m_txtBlkArg.SetWindowTextW(str);
		str = _T("");
		g_drawBlk = 1;
		p1.x = 0, p1.y = 0;
		p2.x = 0, p2.y = 0;
	}
	// TODO:  在此添加控件通知处理程序代码
}


void CMFCTrackToolsDlg::OnBnClickedbtnset()
{
	// TODO:  在此添加控件通知处理程序代码
	CString position, slide;
	
	m_editPos.GetWindowTextW(position);
	m_editSlide.GetWindowTextW(slide);
	if (position.IsEmpty()||slide.IsEmpty())
	{
		AfxMessageBox(_T("没有获得数据！"));
	}
	else
	{
		int_pos = _ttoi(position);
		int_slide = _ttoi(slide);
		if (int_slide > int_pos)
		{
			AfxMessageBox(_T("数据不正确，预置位数目必须大于滑框宽度！"));
		}
		else if (int_slide<=0)
		{
			AfxMessageBox(_T("数据不正确。"));
		}
		else
		{
			g_drawPS = 1;
			camPosSlide.width = (int_slide - 1) / 2;
			camPosSlide.center = int_pos / 2;
			camPosSlide.left = camPosSlide.center - camPosSlide.width;
			camPosSlide.right = camPosSlide.center + camPosSlide.width;
		}
	}
	
}


void CMFCTrackToolsDlg::OnBnClickedbtnsavepos()
{
	// TODO:  在此添加控件通知处理程序代码
	
	//m_editPos.IsWindowEnabled();
	if (m_editPos.IsWindowEnabled()&&m_editSlide.IsWindowEnabled())
	{
		m_editPos.EnableWindow(FALSE);
		m_editSlide.EnableWindow(FALSE);
		m_btnSavePos.SetWindowTextW(_T("修改"));
		params.numOfPos = int_pos;
		params.numOfSlide = int_slide;
	}
	else
	{
		m_editPos.EnableWindow(TRUE);
		m_editSlide.EnableWindow(TRUE);
		m_btnSavePos.SetWindowTextW(_T("保存"));

	}
}


void CMFCTrackToolsDlg::OnBnClickedbtnsavetrack()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (0==g_drawRct)
	{
		g_drawRct = 1;
		m_btnSaveTrack.SetWindowTextW(_T("修改"));
	}
	else
	{
		g_drawRct = 0;
		m_btnSaveTrack.SetWindowTextW(_T("保存"));
	}*/
	g_drawBlk = 0;
	g_drawTch = 0;
}


void CMFCTrackToolsDlg::OnBnClickedbtnsavethreshold()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stand, targetArea, outside;
	int _stand, _targetArea, _outside;
	m_editStand.GetWindowTextW(stand);
	m_editTargetArea.GetWindowTextW(targetArea);
	m_editOutSide.GetWindowTextW(outside);
	if (stand.IsEmpty() || targetArea.IsEmpty() || outside.IsEmpty())
	{
		AfxMessageBox(_T("没有获得数据！"));
	}
	else
	{
		_stand = _ttoi(stand);
		_targetArea = _ttoi(targetArea);
		_outside = _ttoi(outside);
		if (_stand<=0||_targetArea<=0||_outside<=0)
		{
			AfxMessageBox(_T("数据错误！"));
		}
		else
		{
			params.threshold.stand = _stand;
			params.threshold.targetArea = _targetArea;
			params.threshold.outside = _outside;
		}
	}
	
}


void CMFCTrackToolsDlg::OnBnClickedbtnapply()
{
	// TODO:  在此添加控件通知处理程序代码
	CString text;
	m_btnDefault.GetWindowTextW(text);
	params.frame.width = 480;
	params.frame.height = 264;
	if (text=="使用默认参数")
	{
		params.isSetParams = 0;
	}
	else
	{
		params.isSetParams = 1;
		if (params.tch.x <= 0 || params.tch.y <= 0 || params.tch.width <= 0 || params.tch.height <= 0)
		{
			AfxMessageBox(_T("教师跟踪框数据错误！"));
		}
		if (params.blk.x <= 0 || params.blk.y <= 0 || params.blk.width <= 0 || params.blk.height <= 0)
		{
			AfxMessageBox(_T("黑板跟踪框数据错误！"));
		}
		if (params.numOfPos <= 0 || params.numOfSlide <= 0 || params.numOfPos < params.numOfSlide)
		{
			AfxMessageBox(_T("教师跟踪框数据错误！"));
		}
		if (params.threshold.outside <= 0 || params.threshold.stand <= 0 || params.threshold.targetArea <= 0)
		{
			AfxMessageBox(_T("教师跟踪框数据错误！"));
		}
	}
	
}


void CMFCTrackToolsDlg::OnBnClickedbtndefault()
{
	// TODO:  在此添加控件通知处理程序代码
	CString text;
	m_btnDefault.GetWindowTextW(text);
	if (text=="使用默认参数")
	{
		m_btnDefault.SetWindowTextW(_T("自定义参数"));
		m_btnSaveTch.EnableWindow(FALSE);
		m_btnSaveBlk.EnableWindow(FALSE);
		m_btnSavePos.EnableWindow(FALSE);
		m_btnSaveTrack.EnableWindow(FALSE);
		m_btnSet.EnableWindow(FALSE);
		m_btnSaveThreshold.EnableWindow(FALSE);
		m_editOutSide.EnableWindow(FALSE);
		m_editPos.EnableWindow(FALSE);
		m_editSlide.EnableWindow(FALSE);
		m_editStand.EnableWindow(FALSE);
		m_editTargetArea.EnableWindow(FALSE);
	}
	else
	{
		m_btnDefault.SetWindowTextW(_T("使用默认参数"));
		m_btnSaveTch.EnableWindow(TRUE);
		m_btnSaveBlk.EnableWindow(TRUE);
		m_btnSavePos.EnableWindow(TRUE);
		m_btnSaveTrack.EnableWindow(TRUE);
		m_btnSet.EnableWindow(TRUE);
		m_btnSaveThreshold.EnableWindow(TRUE);
		m_editOutSide.EnableWindow(TRUE);
		m_editPos.EnableWindow(TRUE);
		m_editSlide.EnableWindow(TRUE);
		m_editStand.EnableWindow(TRUE);
		m_editTargetArea.EnableWindow(TRUE);
	}
}
