#pragma once


// CAddDlg5 对话框

class CAddDlg5 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg5)

public:
	CAddDlg5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg5();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
