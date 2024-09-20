
// 2Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMy2Dlg 对话框
class CMy2Dlg : public CDialogEx
{
// 构造
public:
	CMy2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY2_DIALOG };

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
	afx_msg void OnBnClickedButton2();
//	CButton m_Radio1;
//	CButton m_Check1;
	BOOL m_Group1;
//	CEdit m_Edit1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	BOOL m_Check;
	CString m_Edit;
	afx_msg void OnBnClickedCheck1();
};
