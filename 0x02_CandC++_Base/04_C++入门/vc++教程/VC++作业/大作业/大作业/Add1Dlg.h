#pragma once


// CAdd1Dlg �Ի���

class CAdd1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdd1Dlg)

public:
	CAdd1Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdd1Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
