#pragma once


// CAddDlg4 对话框

class CAddDlg4 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg4)

public:
	CAddDlg4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg4();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
