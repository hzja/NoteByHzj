#pragma once


// CAddDlg3 �Ի���

class CAddDlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg3)

public:
	CAddDlg3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg3();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
