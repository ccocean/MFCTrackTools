#pragma once
#include "stuTrack_settings_parameter.h"
#include "afxwin.h"
#include "track_client_commintication.h"
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
	void setParams(StuITRACK_ClientParams_t* params);
	CEdit m_edtLeftUpPos;
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
public:
	void setConnectHandle(Commutication_Handle_t pConnect_clientHandle);
	Commutication_Handle_t m_Connect_clientHandle;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_comboDly;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
