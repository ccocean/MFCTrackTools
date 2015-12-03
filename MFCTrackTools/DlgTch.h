#pragma once
#include "afxwin.h"
#include "tch_params.h"
#include "track_client_commintication.h"
#include "DlgCam.h"
// DlgTch 对话框

#define TCH_RECT 0
#define BLK_RECT 1

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
	CEdit m_editTargetArea;
	CEdit m_editOutSide;

	TeaITRACK_Params tch_params;

	

	afx_msg void OnBnClickedbtnapply();
	int checkParameters();
	void getParameters();
public:
	void setConnectHandle(Commutication_Handle_t pConnect_clientHandle);
	Commutication_Handle_t m_Connect_clientHandle;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_txtTchArg;
	CStatic m_txtBlkArg;
	CComboBox m_comboSlide;
	CComboBox m_comboStand;
	void setTrackRects(Tch_Rect_t rc,int flag);
	void setParams(TeaITRACK_Params* params);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
