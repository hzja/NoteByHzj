
// 3Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMy3Dlg �Ի���
class CMy3Dlg : public CDialogEx
{
// ����
public:
	CMy3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
//	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
