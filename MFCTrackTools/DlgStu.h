#pragma once
#include "stuTrack_settings_parameter.h"

// DlgStu �Ի���

class DlgStu : public CDialog
{
	DECLARE_DYNAMIC(DlgStu)

public:
	DlgStu(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgStu();

// �Ի�������
	enum { IDD = IDD_DlgStu };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	StuITRACK_ClientParams_t stu_params;
	afx_msg void OnBnClickedBtnstuapply();

	
};
