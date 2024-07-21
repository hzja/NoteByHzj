// 6Dlg.h : header file
//

#if !defined(AFX_6DLG_H__FD288090_D4E3_4261_BD40_C388EDA9893A__INCLUDED_)
#define AFX_6DLG_H__FD288090_D4E3_4261_BD40_C388EDA9893A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy6Dlg dialog

class CMy6Dlg : public CDialog
{
// Construction
public:
	CMy6Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy6Dlg)
	enum { IDD = IDD_MY6_DIALOG };
	CEdit	m_e2;
	CEdit	m_e1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_6DLG_H__FD288090_D4E3_4261_BD40_C388EDA9893A__INCLUDED_)
