
// jsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CjsDlg �Ի���
class CjsDlg : public CDialogEx
{
// ����
public:
	CjsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_JS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CButton m_Check1;
//	CButton m_Chevk2;
	CButton m_Check3;
	CButton m_Check4;
	CButton m_Check5;
	CButton m_Check2;
//	CEdit m_Edit1;
//	CEdit m_Edit1;
	int m_Edit1;
	CEdit m_cEdit;
	afx_msg void OnEnChangeEdit1();
};
