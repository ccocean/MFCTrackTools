#pragma once
#include "stuTrack_settings_parameter.h"
#include "afxwin.h"
#include "track_client_commintication.h"
#include "afxcmn.h"

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

	int isSetCam = FALSE;
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
	CSliderCtrl m_sliderAngle;
	CButton m_rdoAglLeftUp;
	int m_radioAgl;
	CString str;
	CSliderCtrl m_sliderWidth;
	int m_radioWid;
	void updateAngle();
	void updateWidth();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	//从本地加载或保存设置参数
	bool save_local_Parameter(std::string filePath, StuITRACK_ClientParams_t* stu_params);
	bool load_local_Parameter(std::string filePath, StuITRACK_ClientParams_t* stu_params);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedRadioAglLeftup();
	afx_msg void OnBnClickedRadioAglRightup();
	afx_msg void OnBnClickedRadioAglLeftdn();
	afx_msg void OnBnClickedRadioAglRightdn();
	afx_msg void OnBnClickedRadioWidthLeftup();
	afx_msg void OnBnClickedRadioWidthRightup();
	afx_msg void OnBnClickedRadioWidthLeftdn();
	afx_msg void OnBnClickedRadioWidthRightdn();
};
