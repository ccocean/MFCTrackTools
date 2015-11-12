#pragma once
#include "stuTrack_settings_parameter.h"

// DlgStu 对话框

class DlgStu : public CDialog
{
	DECLARE_DYNAMIC(DlgStu)

public:
	DlgStu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgStu();

// 对话框数据
	enum { IDD = IDD_DlgStu };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	StuITRACK_ClientParams_t stu_params;
	afx_msg void OnBnClickedBtnstuapply();

	
};
