#pragma once
#include "afxwin.h"
#include "track_client_commintication.h"
#include "track_ctrl_params.h"

// GlobalContrl �Ի���



class GlobalContrl : public CDialog
{
	DECLARE_DYNAMIC(GlobalContrl)

public:
	GlobalContrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GlobalContrl();

	//����ȫ�ֵĲ���
	Policy_Set_t ctrl_params;
	

// �Ի�������
	enum { IDD = IDD_DLGGLOBAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Commutication_Handle_t m_Connect_clientHandle;
	void setConnectHandle(Commutication_Handle_t pConnect_clientHandle);
	CButton m_chk_multiple;
	afx_msg void OnBnClickedChkMultiple();
	afx_msg void OnBnClickedBtnCtrlApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int checkParameters();
	CEdit m_edt_timeTch;
	CEdit m_edt_timeStu;
	CEdit m_edt_timeBlk;
	CEdit m_edt_timeVGA;
	CButton m_chk_trackStu;
	CButton m_chk_stuOverview;
	afx_msg void OnBnClickedChkTrackstu();
	afx_msg void OnBnClickedChkStuoverview();
};
