// DlgCam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgCam.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(DlgCam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_UP, &DlgCam::OnBnClickedButtonUp)
END_MESSAGE_MAP()

//CString	g_strPath = "";
////��Ӣ���ַ�����ת��Ϊ����ǰ�������͵��ַ�����
//CString ConvertString(CString strText)
//{
//	char *val = new char[200];
//	CString strIniPath, strRet;
//
//	if (g_strPath != "")
//		strIniPath = g_strPath + "\\langchn.ini";
//	else
//		strIniPath = "./langchn.ini";
//	memset(val, 0, 200);
//	GetPrivateProfileString("String", strText, "",
//		val, 200, strIniPath);
//	//printf("%s\n", val);
//	strRet = val;
//	if (strRet.GetLength() == 0)
//	{
//		//���ini�ļ��в����ڶ�Ӧ���ַ���������ΪĬ��ֵ��Ӣ�ģ�
//		strRet = strText;
//	}
//	delete[] val;
//	return strRet;
//}

// DlgCam ��Ϣ�������
void DlgCam::OnBnClickedButtonUp()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HI_S32 s32Ret = HI_FAILURE;

	if (m_uiHandle == 100)
	{
		MessageBox(_T("No connection"), _T("msg"), MB_ICONEXCLAMATION);
		return;
	}

	HI_NET_DEV_PTZ_Ctrl_Standard(m_uiHandle, HI_NET_DEV_CTRL_PTZ_UP, m_comboSpeed.GetCurSel());
}
