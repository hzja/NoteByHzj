#pragma once
#include "afxwin.h"


// CAddDlg �Ի���

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
