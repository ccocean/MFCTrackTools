#pragma once
#include "afxwin.h"
#include "track_client_commintication.h"
#include "track_ctrl_params.h"

// GlobalContrl 对话框



class GlobalContrl : public CDialog
{
	DECLARE_DYNAMIC(GlobalContrl)

public:
	GlobalContrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GlobalContrl();

	//控制全局的参数
	Policy_Set_t ctrl_params;

// 对话框数据
	enum { IDD = IDD_DLGGLOBAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	Commutication_Handle_t m_Connect_clientHandle;
	void setConnectHandle(Commutication_Handle_t pConnect_clientHandle);
	CButton m_chk_multiple;
	afx_msg void OnBnClickedChkMultiple();
	int m_edt_timeTch;
	int m_edt_timeStu;
	int m_edt_timeBlk;
	int m_edt_timeVGA;
	afx_msg void OnBnClickedBtnCtrlApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int checkParameters();
};
