#pragma once


// CAddDlg8 �Ի���

class CAddDlg8 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg8)

public:
	CAddDlg8(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg8();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
