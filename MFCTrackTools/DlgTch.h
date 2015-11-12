#pragma once
#include "afxwin.h"
#include "tch_params.h"

// DlgTch 对话框

class DlgTch : public CDialog
{
	DECLARE_DYNAMIC(DlgTch)

public:
	DlgTch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgTch();

// 对话框数据
	enum { IDD = IDD_DlgTch };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editX;
	CEdit m_editY;
	CEdit m_editW;
	CEdit m_editH;
	CEdit m_editPos;
	CEdit m_editSlide;
	CEdit m_editStand;
	CEdit m_editTargetArea;
	CEdit m_editOutSide;

	TeaITRACK_Params tch_params;

	afx_msg void OnBnClickedbtnapply();
};
