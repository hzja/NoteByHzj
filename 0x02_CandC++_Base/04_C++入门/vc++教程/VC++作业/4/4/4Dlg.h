
// 4Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMy4Dlg 对话框
class CMy4Dlg : public CDialogEx
{
// 构造
public:
	CMy4Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit;
	CButton m_Check;
	afx_msg void OnBnClickedCheck1();
};
