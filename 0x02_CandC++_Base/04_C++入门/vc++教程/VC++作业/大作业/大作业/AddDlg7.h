#pragma once


// CAddDlg7 �Ի���

class CAddDlg7 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg7)

public:
	CAddDlg7(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg7();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
