#pragma once
#include "afxwin.h"


// GlobalContrl �Ի���

typedef struct
{
	int tea_time_min;//��ʦ�����л���С����ʱ��
	int stu_time_min;//ѧ�������л���С����ʱ��
	int ppt_time_min;//ppt�����л���С����ʱ��
	int blb_time_min;//�ڰ廭���л���С����ʱ��
}Time_Set_t;

typedef struct
{
	Time_Set_t time;
	int mut_pic_flag;//�໭���־λ ����Ϊ1��ʾ��Ҫ�໭�棬0��ʾ�رն໭��
}Policy_Set_t;

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
	CButton m_chk_multiple;
	afx_msg void OnBnClickedChkMultiple();
	int m_edt_timeTch;
	int m_edt_timeStu;
	int m_edt_timeBlk;
	int m_edt_timeVGA;
	afx_msg void OnBnClickedBtnCtrlApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
