#pragma once


// CAddDlg6 �Ի���

class CAddDlg6 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg6)

public:
	CAddDlg6(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg6();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
