#pragma once


// CAddDlg5 �Ի���

class CAddDlg5 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg5)

public:
	CAddDlg5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg5();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
