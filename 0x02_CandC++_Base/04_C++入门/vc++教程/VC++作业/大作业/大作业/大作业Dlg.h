
// ����ҵDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// C����ҵDlg �Ի���
class C����ҵDlg : public CDialogEx
{
// ����
public:
	C����ҵDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

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
	afx_msg void OnBnClickedButton3();
	CButton m_mmts;
	CEdit m_zh;
	CEdit m_mm;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
