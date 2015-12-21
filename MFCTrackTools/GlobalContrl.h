#pragma once
#include "afxwin.h"


// GlobalContrl 对话框

typedef struct
{
	int tea_time_min;//教师画面切换最小保持时间
	int stu_time_min;//学生画面切换最小保持时间
	int ppt_time_min;//ppt画面切换最小保持时间
	int blb_time_min;//黑板画面切换最小保持时间
}Time_Set_t;

typedef struct
{
	Time_Set_t time;
	int mut_pic_flag;//多画面标志位 设置为1表示需要多画面，0表示关闭多画面
}Policy_Set_t;

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
	CButton m_chk_multiple;
	afx_msg void OnBnClickedChkMultiple();
	int m_edt_timeTch;
	int m_edt_timeStu;
	int m_edt_timeBlk;
	int m_edt_timeVGA;
	afx_msg void OnBnClickedBtnCtrlApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
