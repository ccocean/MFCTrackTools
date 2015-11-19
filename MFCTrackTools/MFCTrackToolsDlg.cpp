
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

#define LEFT_PIXEL 1020
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
	m_track_clientHandle = NULL;
	m_strSkin = "";
}

void CMFCTrackToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_picSrc, m_picOverall);

	DDX_Control(pDX, IDC_picCam, m_picFeature);

	//DDX_Control(pDX, IDC_grpBoxCam, m_grpBoxCam);

	DDX_Control(pDX, IDC_tabTrack, m_tabTrack);
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
	return initProgramControl();
}
static int video_call_back(Decode_Info_t *pInfo, void*param)
{
	CMFCTrackToolsDlg* pSelf = (CMFCTrackToolsDlg*)param;
	if (pSelf == NULL || pInfo == NULL || pInfo->data == NULL)
	{
		OutputDebugString("video_call_back is NULL\n");
	}
	pSelf->video_display(pInfo);
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
	pDC->SelectObject(pOldPen);

	//pDC->Rectangle(50, 50, 100, 100);
	//pDC->RectVisible(CRect(50, 50, 100, 100));
	//pDC->SelectObject(pOldPen);

	return 0;
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
	m_connectDialog.DoModal();

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
	init_stream_recv(&recv_stream_handle);
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

	m_picOverall.SetWindowPos(NULL, 40, rectTrackClient.top + 20, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_picFeature.SetWindowPos(NULL, 40, rectTrackClient.top + 350, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_picOverall.GetClientRect(&rs);


	m_tabTrack.InsertItem(TCH_TAB, "教师");
	m_tabTrack.InsertItem(STU_TAB, "学生");
	
	//绑定dlgTch到Tab控件
	dlgTch.Create(IDD_DlgTch, GetDlgItem(IDC_tabTrack));
	dlgStu.Create(IDD_DlgStu, GetDlgItem(IDC_tabTrack));
	

	//移动控件
	m_tabTrack.GetClientRect(&rs);
	cx = rs.right - rs.left;
	cy = rs.bottom - rs.top - 20;
	rs.top += 20;
	dlgTch.MoveWindow(rs);
	dlgTch.ShowWindow(TRUE);
	dlgStu.MoveWindow(rs);
	dlgStu.ShowWindow(FALSE);



	CRect rsDlgcam;
	dlgCam.Create(IDD_CAMCONTROL, GetDlgItem(IDD_CAMCONTROL));
	dlgCam.GetClientRect(rsDlgcam);
	rsDlgcam.left = rectTrackClient.left;
	cx = rectTrackClient.right - rectTrackClient.left;
	rsDlgcam.top = rectTrackClient.bottom;
	cy = rsDlgcam.bottom;
	dlgCam.SetWindowPos(NULL, rsDlgcam.left, rsDlgcam.top, cx, cy, SWP_NOZORDER);
	dlgCam.ShowWindow(TRUE);
	
	m_tabTrack.SetCurSel(0);

	dlgTch.tch_params = { 0 };
	dlgStu.stu_params = { 0 };
	
	//初始化DlgTch中的控件
	dlgTch.m_editPos.SetWindowText(_T("大于滑框宽度"));
	dlgTch.m_comboSlide.InsertString(0,_T("3"));
	dlgTch.m_comboSlide.InsertString(1,_T("5"));
	dlgTch.m_comboSlide.InsertString(2,_T("7"));
	dlgTch.m_comboSlide.InsertString(3,_T("11"));
	dlgTch.m_comboStand.InsertString(0, _T("1秒"));
	dlgTch.m_comboStand.InsertString(1, _T("2秒"));
	dlgTch.m_comboStand.InsertString(2, _T("3秒"));
	dlgTch.m_comboStand.InsertString(3, _T("4秒"));
	dlgTch.m_comboStand.InsertString(4, _T("5秒"));

	//初始化DlgStu中的控件
	dlgStu.m_edtStandAgl.SetWindowText(_T("0~360度"));
	dlgStu.m_edtStandFrm.SetWindowText(_T("3~10帧"));
	dlgStu.m_edtSitFrm.SetWindowText(_T("3~10帧"));
	dlgStu.m_edtMoveDev.SetWindowText(_T("0.2~2.0"));
	dlgStu.m_comboDly.InsertString(0, _T("500ms"));
	dlgStu.m_comboDly.InsertString(1, _T("1000ms"));
	dlgStu.m_comboDly.InsertString(2, _T("1500ms"));
	dlgStu.m_comboDly.InsertString(3, _T("2000ms"));
	dlgStu.m_comboDly.InsertString(4, _T("2500ms"));
	dlgStu.m_comboDly.InsertString(5, _T("3000ms"));

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
	pOldBrush = pDC->SelectObject(pBrush);
	GetModuleFileName(GetModuleHandle(0), m_pExeDir, MAX_PATH);
	CString str(m_pExeDir);
	int n = str.ReverseFind('\\');
	str = str.Left(n);

	memcpy(m_pExeDir, str.GetString(), MAX_PATH);
	m_strSkin = m_pExeDir;
	m_strSkin += SKINNAME;
	skinppLoadSkin((char*)m_strSkin.GetString());
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
	skinppExitSkin();
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
			for (int i = 0; i < Frame_Width; i += (Frame_Width / int_pos))
			{
				//cvLine(srcImg, cvPoint(i, 0), cvPoint(i, Frame_Height), cvScalar(0, 0, 255));
				//cvRectangle(srcImg, cvPoint(camPosSlide.left*(Frame_Width / int_pos), 0), cvPoint((camPosSlide.right + 1) * (Frame_Width / int_pos), Frame_Height), cvScalar(255, 0, 0));//画预置位滑框
			}
		}
		if (mouseStatus == Mouse_LBDOWN)
		{
			if (pt.x > 0 && pt.y > 0)
			{
				pOldPen = pDC->SelectObject(&penR);
				drawRectangle(p1, pt);
			}
			if (mouseCnt == 1)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
			}
			if (mouseCnt == 2)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
				pOldPen = pDC->SelectObject(&penG);
				drawRectangle(CPoint(blk.x, blk.y), CPoint(blk.x + blk.width, blk.y + blk.height));
			}
		}
		else if (mouseStatus == Mouse_LBUP)
		{

			//drawRectangle(p1, p2);
			if (mouseCnt == 1)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
			}
			if (mouseCnt == 2)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
				pOldPen = pDC->SelectObject(&penG);
				drawRectangle(CPoint(blk.x, blk.y), CPoint(blk.x + blk.width, blk.y + blk.height));
			}
			SetDlgItemInt(IDC_editX, p1.x);
			SetDlgItemInt(IDC_editY, p1.y);
			SetDlgItemInt(IDC_editW, p2.x - p1.x);
			SetDlgItemInt(IDC_editH, p2.y - p1.y);
		}
		else if (mouseStatus == Mouse_LBDRAG)
		{
			//drawRectangle(p1, p2);
			if (mouseCnt == 1)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
			}
			if (mouseCnt == 2)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(CPoint(tch.x, tch.y), CPoint(tch.x + tch.width, tch.y + tch.height));
				pOldPen = pDC->SelectObject(&penG);
				drawRectangle(CPoint(blk.x, blk.y), CPoint(blk.x + blk.width, blk.y + blk.height));
			}
		}
	}
	else
	{
		if (mouseStatus==Mouse_LBDOWN)
		{
			if (pt.x > 0 && pt.y > 0)
			{
				pOldPen = pDC->SelectObject(&penY);
				drawRectangle(pa, pt);
			}
		}
		else if (mouseStatus==Mouse_LBUP)
		{
			pOldPen = pDC->SelectObject(&penY);
			drawRectangle(pa, pb, pc, pd);
			drawEndRect(pa,10);
			drawEndRect(pb,10);
			drawEndRect(pc,10);
			drawEndRect(pd,10);
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
			drawRectangle(pa, pb, pc, pd);
			drawEndRect(pa, 10);
			drawEndRect(pb, 10);
			drawEndRect(pc, 10);
			drawEndRect(pd, 10);
			drawLines(DRAW_WIDTH);
			
		}
		if (mouseStatus==Mouse_RBDOWN)
		{
			if (pt.x > 0 && pt.y > 0&&p1.x>0&&p1.y>0)
			{
				pOldPen = pDC->SelectObject(&penB);
				drawLine(p1, pt);
			}
			drawLines(DRAW_ANGLE);
			drawLines(DRAW_WIDTH);
		}
		else if (mouseStatus==Mouse_RBUP)
		{
			drawLines(DRAW_ANGLE);
			drawLines(DRAW_WIDTH);
		}
		else if (mouseStatus==Mouse_RBDRAG)
		{
			drawLines(DRAW_ANGLE);
			drawLines(DRAW_WIDTH);
		}
	}
}

int CMFCTrackToolsDlg::minimalDistance(CPoint in)
{
	int dist = getDistance(in, pa);
	int rst = 0;
	if (getDistance(in,pb)<dist)
	{
		dist = getDistance(in, pb);
		rst = 1;
	}
	if (getDistance(in, pc) < dist)
	{
		dist = getDistance(in, pc);
		rst = 2;
	}
	if (getDistance(in, pd) < dist)
	{
		dist = getDistance(in, pd);
		rst = 3;
	}
	return rst;
}

int CMFCTrackToolsDlg::getDistance(CPoint a, CPoint b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void CMFCTrackToolsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	if (40 <= point.x&&point.x <= 40 + Frame_Width&&PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		//if (p1.x<point.x - 40 && point.x - 40<p2.x&&p1.y<point.y - PIC_TOP&&point.y - PIC_TOP<p2.y)
		if (CurSel==TCH_TAB)
		{
			if ((tch.x < point.x - 40 && point.x - 40 < tch.x + tch.width&&tch.y < point.y - PIC_TOP&&point.y - PIC_TOP < tch.y + tch.height))
			{
				pt.x = point.x;
				pt.y = point.y;
				whichRect = 1;
				mouseStatus = Mouse_LBDRAG;
			}
			else if ((blk.x < point.x - 40 && point.x - 40 < blk.x + blk.width&&blk.y < point.y - PIC_TOP&&point.y - PIC_TOP < blk.y + blk.height))
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
				p1.x = point.x - 40;
				p1.y = point.y - PIC_TOP;
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
			if (pa.x+10<point.x-40&&point.x-40<pc.x-10&&pa.y+10<point.y-PIC_TOP&&point.y-PIC_TOP<pc.y-10)
			{
				pt.x = point.x;
				pt.y = point.y;
				mouseStatus = Mouse_LBDRAG;
				pA = pB = pC = pD = { 0 };
			}
			else if (pa.x - 10 <= point.x - 40 && point.x - 40 <= pa.x + 10 && pa.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pa.y + 10)
			{
				if (pa.x!=point.x&&pa.y!=point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_A;
					pA = pB = pC = pD = { 0 };
				}
			}
			else if (pb.x - 10 <= point.x - 40 && point.x - 40 <= pb.x + 10 && pb.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pb.y + 10)
			{
				if (pb.x != point.x&&pb.y != point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_B;
					pA = pB = pC = pD = { 0 };
				}
			}
			else if (pc.x - 10 <= point.x - 40 && point.x - 40 <= pc.x + 10 && pc.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pc.y + 10)
			{
				if (pc.x != point.x&&pc.y != point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_C;
					pA = pB = pC = pD = { 0 };
				}
			}
			else if (pd.x - 10 <= point.x - 40 && point.x - 40 <= pd.x + 10 && pd.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pd.y + 10)
			{
				if (pd.x != point.x&&pd.y != point.y)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_D;
					pA = pB = pC = pD = { 0 };
				}
			}
			else
			{
				mouseCnt--;
				pt.x = 0, pt.y = 0;
				pa.x = point.x - 40;
				pa.y = point.y - PIC_TOP;
				mouseStatus = Mouse_LBDOWN;
				pA = pB = pC = pD = { 0 };
				p1 = p2 = { 0 };
			}
			/*if (mouseCnt == 1)
			{
				if (pa.x - 10 <= point.x - 40 && point.x - 40 <= pa.x + 10 && pa.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pa.y + 10)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_A;
				}
				else if (pb.x - 10 <= point.x - 40 && point.x - 40 <= pb.x + 10 && pb.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pb.y + 10)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_B;
				}
				else if (pc.x - 10 <= point.x - 40 && point.x - 40 <= pc.x + 10 && pc.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pc.y + 10)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_C;
				}
				else if (pd.x - 10 <= point.x - 40 && point.x - 40 <= pd.x + 10 && pd.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pd.y + 10)
				{
					pt.x = point.x;
					pt.y = point.y;
					mouseStatus = Mouse_ADJUST_D;
				}
			}*/
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCTrackToolsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (40 <= point.x&&point.x <= 40 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		if (CurSel==TCH_TAB)
		{
			if (mouseStatus != Mouse_LBDRAG)
			{
				//p2.x = point.x - 40;
				p2.x = Frame_Width;
				p1.x = 0;
				p2.y = point.y - PIC_TOP;
				mouseStatus = Mouse_LBUP;
				pt.x = -1;
				pt.y = -1;
				mouseCnt++;
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
					blk.x = p1.x; blk.y = p1.y;
					blk.width = p2.x - p1.x;
					blk.height = p2.y - p1.y;

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
				mouseStatus = Mouse_LBUP;
			}
		}
		//学生界面的左键抬起操作
		else
		{
			if (mouseStatus==Mouse_LBDOWN)
			{
				pc.x = point.x - 40;
				pc.y = point.y - PIC_TOP;
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

				dlgStu.stu_params.stuTrack_vertex[0].x = pa.x;
				dlgStu.stu_params.stuTrack_vertex[0].y = pa.y;

				dlgStu.stu_params.stuTrack_vertex[1].x = pb.x;
				dlgStu.stu_params.stuTrack_vertex[1].y = pb.y;

				dlgStu.stu_params.stuTrack_vertex[2].x = pc.x;
				dlgStu.stu_params.stuTrack_vertex[2].y = pc.y;

				dlgStu.stu_params.stuTrack_vertex[3].x = pd.x;
				dlgStu.stu_params.stuTrack_vertex[3].y = pd.y;

				updateLines();

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
				mouseCnt++;
			}
			else
			{
				dlgStu.stu_params.stuTrack_vertex[0].x = pa.x;
				dlgStu.stu_params.stuTrack_vertex[0].y = pa.y;

				dlgStu.stu_params.stuTrack_vertex[1].x = pb.x;
				dlgStu.stu_params.stuTrack_vertex[1].y = pb.y;

				dlgStu.stu_params.stuTrack_vertex[2].x = pc.x;
				dlgStu.stu_params.stuTrack_vertex[2].y = pc.y;

				dlgStu.stu_params.stuTrack_vertex[3].x = pd.x;
				dlgStu.stu_params.stuTrack_vertex[3].y = pd.y;
				updateLines();
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
	if (40 <= point.x&&point.x <= 40 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		if (CurSel==TCH_TAB)
		{
			if (mouseStatus == Mouse_LBDOWN)
			{
				pt.x = point.x - 40;
				pt.y = point.y - PIC_TOP;
			}
			if (mouseStatus == Mouse_LBDRAG)
			{
				/*p1.x += (point.x - pt.x);
				p1.y += (point.y - pt.y);
				p2.x += (point.x - pt.x);
				p2.y += (point.y - pt.y);*/


				if (whichRect == 1)
				{
					//tch.x += (point.x - pt.x);
					tch.y += (point.y  - pt.y);
					SetDlgItemInt(IDC_editX, tch.x);
					SetDlgItemInt(IDC_editY, tch.y);
					SetDlgItemInt(IDC_editW, tch.width);
					SetDlgItemInt(IDC_editH, tch.height);
				}
				if (whichRect == 2)
				{
					//blk.x += (point.x - pt.x);
					blk.y += (point.y  - pt.y);
					SetDlgItemInt(IDC_editX, blk.x);
					SetDlgItemInt(IDC_editY, blk.y);
					SetDlgItemInt(IDC_editW, blk.width);
					SetDlgItemInt(IDC_editH, blk.height);
				}
				pt.x = point.x;
				pt.y = point.y;
			}
		}
		else
		{
			if (mouseStatus == Mouse_LBDOWN)
			{
				pt.x = point.x - 40;
				pt.y = point.y - PIC_TOP;
			}
			if (mouseStatus==Mouse_RBDOWN)
			{
				pt.x = point.x - 40;
				pt.y = point.y - PIC_TOP;
			}
			if (mouseStatus == Mouse_LBDRAG)
			{
				/*point.x -= 40;
				point.y -= PIC_TOP;*/
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
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus == Mouse_ADJUST_B)
			{
				pb.x += (point.x  - pt.x);
				pb.y += (point.y  - pt.y);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus == Mouse_ADJUST_C)
			{
				pc.x += (point.x - pt.x);
				pc.y += (point.y  - pt.y);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus == Mouse_ADJUST_D)
			{
				pd.x += (point.x  - pt.x);
				pd.y += (point.y  - pt.y);
				pt.x = point.x;
				pt.y = point.y;
			}
			if (mouseStatus==Mouse_RBDOWN)
			{
				pt.x = point.x - 40;
				pt.y = point.y - PIC_TOP;
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
	if (40 <= point.x&&point.x <= 40 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		if (CurSel == STU_TAB)
		{
			pt = { 0 };
			if (pa.x - 10 <= point.x - 40 && point.x - 40 <= pa.x + 10 && pa.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pa.y + 10)
			{
				if (pa.x != point.x&&pa.y != point.y)
				{
					p1 = pa;
					mouseStatus = Mouse_RBDOWN;
					isRightButton = 1;
				}
			}
			else if (pb.x - 10 <= point.x - 40 && point.x - 40 <= pb.x + 10 && pb.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pb.y + 10)
			{
				if (pb.x != point.x&&pb.y != point.y)
				{
					p1 = pb;
					mouseStatus = Mouse_RBDOWN;
					isRightButton = 1;
				}
			}
			else if (pc.x - 10 <= point.x - 40 && point.x - 40 <= pc.x + 10 && pc.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pc.y + 10)
			{
				if (pc.x != point.x&&pc.y != point.y)
				{
					p1 = pc;
					mouseStatus = Mouse_RBDOWN;
					isRightButton = 1;
				}
			}
			else if (pd.x - 10 <= point.x - 40 && point.x - 40 <= pd.x + 10 && pd.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pd.y + 10)
			{
				if (pd.x != point.x&&pd.y != point.y)
				{
					p1 = pd;
					mouseStatus = Mouse_RBDOWN;
					isRightButton = 1;
				}
			}
			else if (pA.x - 10 <= point.x - 40 && point.x - 40 <= pA.x + 10 && pA.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pA.y + 10)
			{
				if (pA.x>0&&pA.y>0)
				{
					pt = point;
					p1 = pa;
					mouseStatus = Mouse_RBDRAG;
					whichVertex = -1;
				}
			}
			else if (pB.x - 10 <= point.x - 40 && point.x - 40 <= pB.x + 10 && pB.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pB.y + 10)
			{
				if (pB.x>0&&pB.y>0)
				{
					pt = point;
					p1 = pb;
					mouseStatus = Mouse_RBDRAG;
					whichVertex = -1;
				}
			}
			else if (pC.x - 10 <= point.x - 40 && point.x - 40 <= pC.x + 10 && pC.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pC.y + 10)
			{
				if (pC.x > 0 && pC.y > 0)
				{
					pt = point;
					p1 = pc;
					mouseStatus = Mouse_RBDRAG;
					whichVertex = -1;
				}
			}
			else if (pD.x - 10 <= point.x - 40 && point.x - 40 <= pD.x + 10 && pD.y - 10 <= point.y - PIC_TOP&&point.y - PIC_TOP <= pD.y + 10)
			{
				if (pD.x > 0 && pD.y > 0)
				{
					pt = point;
					p1 = pd;
					mouseStatus = Mouse_RBDRAG;
					whichVertex = -1;
				}
			}
			//调整四个顶点的宽度
			else if (ln1[1].x - 5 <= point.x - 40 && point.x - 40 <= ln1[1].x + 5 && ln1[1].y - 5 <= point.y - PIC_TOP&&point.y - PIC_TOP <= ln1[1].y + 5)
			{
				pt = point;
				//p2 = pa;
				whichVertex = 0;
				mouseStatus = Mouse_RBDRAG;
			}
			else if (ln2[1].x - 5 <= point.x - 40 && point.x - 40 <= ln2[1].x + 5 && ln2[1].y - 5 <= point.y - PIC_TOP&&point.y - PIC_TOP <= ln2[1].y + 5)
			{
				pt = point;
				//p2 = pb;
				whichVertex = 1;
				mouseStatus = Mouse_RBDRAG;
			}
			else if (ln3[1].x - 5 <= point.x - 40 && point.x - 40 <= ln3[1].x + 5 && ln3[1].y - 5 <= point.y - PIC_TOP&&point.y - PIC_TOP <= ln3[1].y + 5)
			{
				pt = point;
				//p2 = pc;
				whichVertex = 2;
				mouseStatus = Mouse_RBDRAG;
			}
			else if (ln4[1].x - 5 <= point.x - 40 && point.x - 40 <= ln4[1].x + 5 && ln4[1].y - 5 <= point.y - PIC_TOP&&point.y - PIC_TOP <= ln4[1].y + 5)
			{
				pt = point;
				//p2 = pd;
				whichVertex = 3;
				mouseStatus = Mouse_RBDRAG;
			}
			else
			{
				pA = pB = pC = pD = { 0 };
				p1 = p2 = pt = { 0 };
				/*ln1[0] = ln1[1] = { 0 };
				ln2[0] = ln2[1] = { 0 };
				ln3[0] = ln3[1] = { 0 };
				ln4[0] = ln4[1] = { 0 };*/
				updateLines();
			}
		}
	}
	CDialogEx::OnRButtonDown(nFlags, point);
}

void CMFCTrackToolsDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (40 <= point.x&&point.x <= 40 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		if (CurSel==TCH_TAB)
		{
			p1.x = 0; p1.y = 0;
			p2.x = 0; p2.y = 0;
			pt.x = 0; pt.y = 0;
			tch.x = 0; tch.y = 0;
			tch.width = 0; tch.height = 0;
			blk.x = 0; blk.y = 0;
			blk.width = 0; blk.height = 0;

			dlgTch.m_txtTchArg.SetWindowText(_T(""));
			dlgTch.m_txtBlkArg.SetWindowText(_T(""));
			mouseCnt = 0;
			mouseStatus = Mouse_RBUP;
		}
		else
		{
			if (p1 == pa)
			{
				p2.x = point.x - 40;
				p2.y = point.y - PIC_TOP;
				CPoint p = p2 - p1;
				angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
				angle = angle < 0 ? angle + 360 : angle;
				s.Format("%d", angle);
				if (pa.x>0 && pa.y > 0)
				{
					pA = p2;
					p1 = p2 = { 0 };
					dlgStu.m_edtLeftUpAgl.SetWindowText(s);
					dlgStu.stu_params.stuTrack_direct_standard[0] = angle;
				}
				else
				{
					p2 = { 0 };
				}
			}
			if (p1 == pb)
			{
				p2.x = point.x - 40;
				p2.y = point.y - PIC_TOP;
				CPoint p = p2 - p1;
				angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
				angle = angle < 0 ? angle + 360 : angle;
				s.Format("%d", angle);
				if (pb.x > 0 && pb.y > 0)
				{
					pB = p2;
					p1 = p2 = { 0 };
					dlgStu.m_edtRightUpAgl.SetWindowText(s);
					dlgStu.stu_params.stuTrack_direct_standard[1] = angle;
				}
				else
				{
					p2 = { 0 };
				}
			}
			if (p1 == pc)
			{
				p2.x = point.x - 40;
				p2.y = point.y - PIC_TOP;
				CPoint p = p2 - p1;
				angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
				angle = angle < 0 ? angle + 360 : angle;
				s.Format("%d", angle);
				if (pc.x > 0 && pc.y > 0)
				{
					pC = p2;
					p1 = p2 = { 0 };
					dlgStu.m_edtRightDnAgl.SetWindowText(s);
					dlgStu.stu_params.stuTrack_direct_standard[2] = angle;
				}
				else
				{
					p2 = { 0 };
				}
			}
			if (p1 == pd)
			{
				p2.x = point.x - 40;
				p2.y = point.y - PIC_TOP;
				CPoint p = p2 - p1;
				angle = (int)(atan2(p.y, p.x) * ITC_RADIAN_TO_ANGLE);
				angle = angle < 0 ? angle + 360 : angle;
				s.Format("%d", angle);
				if (pd.x > 0 && pd.y > 0)
				{
					pD = p2;
					p1 = p2 = { 0 };
					dlgStu.m_edtLeftDnAgl.SetWindowText(s);
					dlgStu.stu_params.stuTrack_direct_standard[3] = angle;
				}
				else
				{
					p2 = { 0 };
				}
			}
			if (whichVertex == 0)
			{
				ln1[1].x += (point.x - pt.x);
				dist = getDistance(ln1[0], ln1[1]);
				s.Format("%d", dist);
				dlgStu.m_edtLeftUpWid.SetWindowText(s);
				whichVertex = -1;
				dlgStu.stu_params.stuTrack_stuWidth_standard[0] = dist;
				//pt = point;
			}
			if (whichVertex == 1)
			{
				ln2[1].x += (point.x - pt.x);
				dist = getDistance(ln2[0], ln2[1]);
				s.Format("%d", dist);
				dlgStu.m_edtRightUpWid.SetWindowText(s);
				whichVertex = -1;
				dlgStu.stu_params.stuTrack_stuWidth_standard[1] = dist;
			}
			if (whichVertex == 2)
			{
				ln3[1].x += (point.x - pt.x);
				dist = getDistance(ln3[0], ln3[1]);
				s.Format("%d", dist);
				dlgStu.m_edtRightDnWid.SetWindowText(s);
				whichVertex = -1;
				dlgStu.stu_params.stuTrack_stuWidth_standard[2] = dist;
			}
			if (whichVertex == 3)
			{
				ln4[1].x += (point.x - pt.x);
				dist = getDistance(ln4[0], ln4[1]);
				s.Format("%d", dist);
				dlgStu.m_edtLeftDnWid.SetWindowText(s);
				whichVertex = -1;
				dlgStu.stu_params.stuTrack_stuWidth_standard[3] = dist;
			}
			mouseStatus = Mouse_RBUP;
			isRightButton = 0;
		}
	}
	CDialogEx::OnRButtonUp(nFlags, point);
}

void CMFCTrackToolsDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (40 <= point.x&&point.x <= 40 + Frame_Width && PIC_TOP <= point.y&&point.y <= PIC_TOP + Frame_Height)
	{
		
	}
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
	OutputDebugString(_T("ctrlClient_process_trackHeart=====================\n"));
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
	{
		pTrackDlg->ctrlClient_init_Stream();
		PostMessage(pTrackDlg->m_connectDialog.GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	}
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
		AfxMessageBox(_T("接收信息失败"));
		return -1;
	}
	int return_code = 0;
	return_code = head->return_code;

	if (return_code != 0)
	{
		sprintf_s(errMsg, sizeof(errMsg), "%s失败", get_track_cmd_name(head->cmd));
		AfxMessageBox(_T(errMsg));
		return -1;
	}
	switch (head->cmd)
	{
	case STU_SETTRACK_CMD:
	{
							 break;
	}
	case TEA_SETTRACK_CMD:
	{
							 break;
	}
	case STU_GETTRACK_CMD:
	{
							 break;
	}
	case TEA_GETTRACK_CMD:
	{
							 break;
	}
	}
	sprintf_s(errMsg, sizeof(errMsg), "%s成功", get_track_cmd_name(head->cmd));
	AfxMessageBox(_T(errMsg));
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
	return pTrackDialog->ctrlClient_init_trackCommuntication();

}
int CMFCTrackToolsDlg::ctrlClient_init_trackCommuntication()
{

	if (m_track_clientHandle != NULL)
	{
		OutputDebugString(_T("ctrlClient_init_trackCommuntication is init"));
		return -1;
	}

	m_track_clientHandle = communtication_create_clientHandle(m_trackIp, C_CONTROL_TRACK,
		ctrlClient_process_trackMsgEx, ctrlClient_process_trackHeartEx, ctrl_connect_status, this);
	if (m_track_clientHandle == NULL) {
		AfxMessageBox(TEXT("创建客户端失败"));
		return -1;
	}
	dlgTch.setConnectHandle(m_track_clientHandle);
	dlgStu.setConnectHandle(m_track_clientHandle);
	return  0;
}

void CMFCTrackToolsDlg::OnTcnSelchangetabtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	CurSel = m_tabTrack.GetCurSel();

	switch (CurSel)
	{
	case 0:
		dlgTch.ShowWindow(TRUE);
		dlgStu.ShowWindow(FALSE);
		pa.x = 0; pa.y = 0;
		pb.x = 0; pb.y = 0;
		pc.x = 0; pc.y = 0;
		pd.x = 0; pd.y = 0;
		pt.x = 0; pt.y = 0;
		p1.x = 0; p1.y = 0;
		p2.x = 0; p2.y = 0;
		mouseCnt = 0;
		break;
	case 1:
		dlgTch.ShowWindow(FALSE);
		dlgStu.ShowWindow(TRUE);
		tch.x = 0; tch.y = 0;
		tch.width = 0; tch.height = 0;
		blk.x = 0; blk.y = 0;
		blk.width = 0; blk.height = 0;
		pt.x = 0; pt.y = 0;
		mouseCnt = 0;
		break;
	default:
		;
	}

	*pResult = 0;
}


BOOL CMFCTrackToolsDlg::PreTranslateMessage(MSG* pMsg)
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
	return CDialogEx::PreTranslateMessage(pMsg);
}














