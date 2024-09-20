// AddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "大作业.h"
#include "AddDlg.h"
#include "afxdialogex.h"
#include "Add1Dlg.h"
#include "Add2Dlg.h"
#include "AddDlg3.h"
#include "AddDlg4.h"
#include "AddDlg5.h"
#include "AddDlg6.h"
#include "AddDlg7.h"
#include "AddDlg8.h"


// CAddDlg 对话框

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDlg::IDD, pParent)
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_zggdsdmz);
	DDX_Control(pDX, IDC_LIST2, m_zggdsdfm);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &CAddDlg::OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON3, &CAddDlg::OnBnClickedButton3)
	ON_LBN_DBLCLK(IDC_LIST2, &CAddDlg::OnDblclkList2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAddDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CAddDlg 消息处理程序


BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_zggdsdmz.AddString (_T("红楼梦"));
	m_zggdsdmz.AddString (_T("水浒传"));
	m_zggdsdmz.AddString (_T("三国演义"));
	m_zggdsdmz.AddString (_T("西游记"));
	m_zggdsdfm.AddString (_T("造纸术"));
    m_zggdsdfm.AddString (_T("印刷术"));
    m_zggdsdfm.AddString (_T("火药"));
    m_zggdsdfm.AddString (_T("指南针"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAddDlg::OnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString str;
	int i;
	i=m_zggdsdmz.GetCurSel();
	m_zggdsdmz.GetText(i,str);
	if(str=="西游记")
	{
		CAddDlg5 dlg;
		dlg.DoModal();
	}
	if(str=="红楼梦")
	{
		CAddDlg6 dlg;
		dlg.DoModal();
	}
	if(str=="水浒传")
	{
		CAddDlg7 dlg;
		dlg.DoModal();
	}
	if(str=="三国演义")
	{
		CAddDlg8 dlg;
		dlg.DoModal();
	}

}


void CAddDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CAddDlg::OnDblclkList2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int i;
	i=m_zggdsdfm.GetCurSel();
	m_zggdsdfm.GetText(i,str);
	if(str=="造纸术")
	{
		CAddDlg5 dlg;
		dlg.DoModal();
	}
	if(str=="印刷术")
	{
		CAddDlg6 dlg;
		dlg.DoModal();
	}
	if(str=="火药")
	{
		CAddDlg7 dlg;
		dlg.DoModal();
	}
	if(str=="指南针")
	{
		CAddDlg8 dlg;
		dlg.DoModal();
	}
}


void CAddDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}
