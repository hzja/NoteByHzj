#pragma once


// CAddDlg7 对话框

class CAddDlg7 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg7)

public:
	CAddDlg7(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg7();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
