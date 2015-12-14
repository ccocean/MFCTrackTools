#pragma once


// GlobalContrl 对话框

class GlobalContrl : public CDialog
{
	DECLARE_DYNAMIC(GlobalContrl)

public:
	GlobalContrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GlobalContrl();

// 对话框数据
	enum { IDD = IDD_DLGGLOBAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
