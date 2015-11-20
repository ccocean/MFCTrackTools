// DlgCam.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTrackTools.h"
#include "DlgCam.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(DlgCam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_UP, &DlgCam::OnBnClickedButtonUp)
END_MESSAGE_MAP()

//CString	g_strPath = "";
////“英文字符串”转换为“当前语言类型的字符串”
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
//		//如果ini文件中不存在对应的字符串，设置为默认值（英文）
//		strRet = strText;
//	}
//	delete[] val;
//	return strRet;
//}

// DlgCam 消息处理程序
void DlgCam::OnBnClickedButtonUp()
{
	// TODO:  在此添加控件通知处理程序代码
	HI_S32 s32Ret = HI_FAILURE;

	if (m_uiHandle == 100)
	{
		MessageBox(_T("No connection"), _T("msg"), MB_ICONEXCLAMATION);
		return;
	}

	HI_NET_DEV_PTZ_Ctrl_Standard(m_uiHandle, HI_NET_DEV_CTRL_PTZ_UP, m_comboSpeed.GetCurSel());
}
