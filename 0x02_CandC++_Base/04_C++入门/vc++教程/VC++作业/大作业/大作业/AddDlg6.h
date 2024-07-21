#pragma once


// CAddDlg6 对话框

class CAddDlg6 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg6)

public:
	CAddDlg6(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg6();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
