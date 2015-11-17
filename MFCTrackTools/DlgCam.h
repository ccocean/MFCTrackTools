#pragma once


// DlgCam 对话框

class DlgCam : public CDialog
{
	DECLARE_DYNAMIC(DlgCam)

public:
	DlgCam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgCam();

// 对话框数据
	enum { IDD = IDD_CAMCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
