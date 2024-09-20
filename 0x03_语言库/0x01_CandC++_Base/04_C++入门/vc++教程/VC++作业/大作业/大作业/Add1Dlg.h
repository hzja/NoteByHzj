#pragma once


// CAdd1Dlg 对话框

class CAdd1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdd1Dlg)

public:
	CAdd1Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdd1Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
