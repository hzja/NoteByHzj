
// 3Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMy3Dlg 对话框
class CMy3Dlg : public CDialogEx
{
// 构造
public:
	CMy3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
//	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_Button;
	afx_msg void OnClickedCheck1();
	CEdit m_Edit;
//	CButton m_check1;
	afx_msg void OnBnClickedCheck2();
	CButton m_check;
};
