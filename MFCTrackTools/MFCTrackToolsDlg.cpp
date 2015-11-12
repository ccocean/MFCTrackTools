
// MFCTrackToolsDlg.cpp : ʵ���ļ�
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
#define PIC_TOP 20


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCTrackToolsDlg �Ի���


//���캯������ʼ��ֵ
CMFCTrackToolsDlg::CMFCTrackToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCTrackToolsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTrackToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_Exit, m_Exit);
	DDX_Control(pDX, IDC_picSrc, m_picOverall);
	DDX_Control(pDX, IDC_listErr, m_listErr);
	DDX_Control(pDX, IDC_picCam, m_picFeature);
	DDX_Control(pDX, IDC_btnUp, m_btnUp);
	DDX_Control(pDX, IDC_btnLeft, m_btnLeft);
	DDX_Control(pDX, IDC_btnDown, m_btnDown);
	DDX_Control(pDX, IDC_btnRight, m_btnRight);
	DDX_Control(pDX, IDC_grpBoxCam, m_grpBoxCam);
	DDX_Control(pDX, IDC_txtTune, m_txtTune);
	DDX_Control(pDX, IDC_txtFocus, m_txtFocus);
	DDX_Control(pDX, IDC_btnOrigin, m_btnOrigin);
	DDX_Control(pDX, IDC_btnTuneAsd, m_btnTuneAsd);
	DDX_Control(pDX, IDC_btnTuneDsd, m_btnTuneDsd);
	DDX_Control(pDX, IDC_btnTuneStop, m_btnTuneStop);
	DDX_Control(pDX, IDC_btnFocusAsd, m_btnFocusAsd);
	DDX_Control(pDX, IDC_btnFocusStop, m_btnFocusStop);
	DDX_Control(pDX, IDC_btnFocusDsd, m_btnFocusDsd);
	DDX_Control(pDX, IDC_tabTrack, m_tabTrack);
}

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CMFCTrackToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMFCTrackToolsDlg ��Ϣ�������

BOOL CMFCTrackToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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
	//ע��BMP��y�����Ƿ��Ŵ洢�ģ�һ�α�������һ����ֵ������ʹͼ��������ʾ����  
	m_bmphdr.bmiHeader.biHeight = -264;//-pixel_h;
	m_bmphdr.bmiHeader.biXPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biYPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biClrUsed = 0;
	m_bmphdr.bmiHeader.biPlanes = 1;
	m_bmphdr.bmiHeader.biCompression = BI_RGB;


	ctrlClient_init_trackCommuntication();
	RecvStream_Handle_t recv_stream_handle;
	recv_stream_handle.channel = 0;
	strcpy_s(recv_stream_handle.iP, sizeof(recv_stream_handle.iP), _T("192.168.11.140"));
	recv_stream_handle.port = TEACH_STREAM_PORT;
	recv_stream_handle.channel = 1;
	recv_stream_handle.param1 = this;
	recv_stream_handle.call_back_fun = video_call_back;
	init_stream_recv(&recv_stream_handle);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE

}
BOOL CMFCTrackToolsDlg::initProgramControl()
{

	//��ʼ������
	CWnd::SetWindowPos(NULL, 0, 0, 1280, 720, SWP_NOZORDER);//��ʼ�����ڴ�С
	CenterWindow();//���ھ���

	//��ʼ���ؼ�

	//���ٲ����ؼ�
	m_tabTrack.SetWindowPos(NULL, LEFT_PIXEL-20, UP_PIXEL - 10, 240, 640, SWP_NOZORDER);
	m_tabTrack.InsertItem(0, "��ʦ");
	m_tabTrack.InsertItem(1, "ѧ��");

	//��dlgTch��Tab�ؼ�
	dlgTch.Create(IDD_DlgTch, GetDlgItem(IDC_tabTrack));

	CRect rs;
	m_tabTrack.GetClientRect(&rs);
	rs.top += 20;
	rs.bottom -= 0;
	rs.left += 0;
	rs.right -= 0;
	dlgTch.SetWindowPos(NULL, 0, 0, 220, 400, SWP_NOZORDER);
	dlgTch.MoveWindow(rs);
	dlgTch.ShowWindow(true);

	//��ʾ�ؼ�����־�ؼ�
	m_picOverall.SetWindowPos(NULL, 40, PIC_TOP + 40, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_picFeature.SetWindowPos(NULL, 40, PIC_TOP + 350, Frame_Width, Frame_Height, SWP_NOZORDER);
	m_listErr.SetWindowPos(NULL, 600, PIC_TOP + 50, 360, 240, SWP_NOZORDER);

	//
	m_grpBoxCam.SetWindowPos(NULL, 600, PIC_TOP + 360, 360, 240, SWP_NOZORDER);
	m_btnUp.SetWindowPos(NULL, 600 + 25 + 32, PIC_TOP + 360 + 25, 30, 30, SWP_NOZORDER);
	m_btnLeft.SetWindowPos(NULL, 600 + 25, PIC_TOP + 360 + 25 + 32, 30, 30, SWP_NOZORDER);
	m_btnDown.SetWindowPos(NULL, 600 + 25 + 32, PIC_TOP + 360 + 25 + 64, 30, 30, SWP_NOZORDER);
	m_btnRight.SetWindowPos(NULL, 600 + 25 + 64, PIC_TOP + 360 + 25 + 32, 30, 30, SWP_NOZORDER);
	m_btnOrigin.SetWindowPos(NULL, 600 + 25 + 28, PIC_TOP + 360 + 25 + 100, 40, 20, SWP_NOZORDER);

	m_txtTune.SetWindowPos(NULL, 600 + 10, PIC_TOP + 360 + 25 + 140, 40, 30, SWP_NOZORDER);
	m_btnTuneAsd.SetWindowPos(NULL, 600 + 45, PIC_TOP + 360 + 25 + 138, 20, 20, SWP_NOZORDER);
	m_btnTuneStop.SetWindowPos(NULL, 600 + 65, PIC_TOP + 360 + 25 + 138, 40, 20, SWP_NOZORDER);
	m_btnTuneDsd.SetWindowPos(NULL, 600 + 105, PIC_TOP + 360 + 25 + 138, 20, 20, SWP_NOZORDER);

	m_txtFocus.SetWindowPos(NULL, 600 + 10, PIC_TOP + 360 + 25 + 180, 40, 30, SWP_NOZORDER);
	m_btnFocusAsd.SetWindowPos(NULL, 600 + 45, PIC_TOP + 360 + 25 + 178, 20, 20, SWP_NOZORDER);
	m_btnFocusStop.SetWindowPos(NULL, 600 + 65, PIC_TOP + 360 + 25 + 178, 40, 20, SWP_NOZORDER);
	m_btnFocusDsd.SetWindowPos(NULL, 600 + 105, PIC_TOP + 360 + 25 + 178, 20, 20, SWP_NOZORDER);

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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCTrackToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		//CWnd::SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
		GetClientRect(&rect);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCTrackToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCTrackToolsDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_flag = 1;
	if (MessageBox(_T("ȷ���˳���"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING)
		== IDNO)
		return;
	CDialogEx::OnClose();
}


void CMFCTrackToolsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//			//cvRectangle(srcImg, cvPoint(camPosSlide.left*(Frame_Width / int_pos), 0), cvPoint((camPosSlide.right + 1) * (Frame_Width / int_pos), Frame_Height), cvScalar(255, 0, 0));//��Ԥ��λ����
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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




