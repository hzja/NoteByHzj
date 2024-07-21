#pragma once
#include "afxwin.h"


// CAddDlg 对话框

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_zggdsdmz;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1();
	CListBox m_zggdsdfm;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnDblclkList2();
	afx_msg void OnLbnSelchangeList1();
};
