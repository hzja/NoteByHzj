
// 1Dlg.h : ͷ�ļ�
//

#pragma once


// CMy1Dlg �Ի���
class CMy1Dlg : public CDialogEx
{
// ����
public:
	CMy1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY1_DIALOG };

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
	int m_Edit1;
	int m_Edit2;
	afx_msg void OnBnClickedButton1();
};
