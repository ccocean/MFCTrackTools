#pragma once
#include "stuTrack_settings_parameter.h"
#include "afxwin.h"

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
	int checkParameters();
	void getParameters();
	
	CEdit m_edtLfetUpPos;
	CEdit m_edtLeftDnPos;
	CEdit m_edtRightUpPos;
	CEdit m_edtRightDnPos;
	CEdit m_edtLeftUpAgl;
	CEdit m_edtLeftDnAgl;
	CEdit m_edtRightUpAgl;
	CEdit m_edtRightDnAgl;
	CEdit m_edtLeftUpWid;
	CEdit m_edtLeftDnWid;
	CEdit m_edtRightUpWid;
	CEdit m_edtRightDnWid;
	CEdit m_edtStandAgl;
	CEdit m_edtStandFrm;
	CEdit m_edtSitFrm;
	CEdit m_edtMoveDev;
	CEdit m_edtMoveDly;
};
