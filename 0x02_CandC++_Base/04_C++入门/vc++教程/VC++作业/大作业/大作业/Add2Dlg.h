#pragma once


// CAdd2Dlg �Ի���

class CAdd2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdd2Dlg)

public:
	CAdd2Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdd2Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
