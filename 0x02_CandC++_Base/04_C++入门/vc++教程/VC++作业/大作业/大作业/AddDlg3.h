#pragma once


// CAddDlg3 对话框

class CAddDlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg3)

public:
	CAddDlg3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg3();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
