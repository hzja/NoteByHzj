#pragma once


// CAddDlg4 �Ի���

class CAddDlg4 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg4)

public:
	CAddDlg4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg4();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
