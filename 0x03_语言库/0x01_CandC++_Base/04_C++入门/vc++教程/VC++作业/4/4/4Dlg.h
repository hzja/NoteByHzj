
// 4Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMy4Dlg �Ի���
class CMy4Dlg : public CDialogEx
{
// ����
public:
	CMy4Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY4_DIALOG };

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
	CEdit m_Edit;
	CButton m_Check;
	afx_msg void OnBnClickedCheck1();
};
