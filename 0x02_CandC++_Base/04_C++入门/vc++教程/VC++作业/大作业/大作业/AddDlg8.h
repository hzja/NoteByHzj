#pragma once


// CAddDlg8 对话框

class CAddDlg8 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg8)

public:
	CAddDlg8(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg8();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
