// AddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "����ҵ.h"
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


// CAddDlg �Ի���

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


// CAddDlg ��Ϣ�������


BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_zggdsdmz.AddString (_T("��¥��"));
	m_zggdsdmz.AddString (_T("ˮ䰴�"));
	m_zggdsdmz.AddString (_T("��������"));
	m_zggdsdmz.AddString (_T("���μ�"));
	m_zggdsdfm.AddString (_T("��ֽ��"));
    m_zggdsdfm.AddString (_T("ӡˢ��"));
    m_zggdsdfm.AddString (_T("��ҩ"));
    m_zggdsdfm.AddString (_T("ָ����"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAddDlg::OnDblclkList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString str;
	int i;
	i=m_zggdsdmz.GetCurSel();
	m_zggdsdmz.GetText(i,str);
	if(str=="���μ�")
	{
		CAddDlg5 dlg;
		dlg.DoModal();
	}
	if(str=="��¥��")
	{
		CAddDlg6 dlg;
		dlg.DoModal();
	}
	if(str=="ˮ䰴�")
	{
		CAddDlg7 dlg;
		dlg.DoModal();
	}
	if(str=="��������")
	{
		CAddDlg8 dlg;
		dlg.DoModal();
	}

}


void CAddDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}


void CAddDlg::OnDblclkList2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	int i;
	i=m_zggdsdfm.GetCurSel();
	m_zggdsdfm.GetText(i,str);
	if(str=="��ֽ��")
	{
		CAddDlg5 dlg;
		dlg.DoModal();
	}
	if(str=="ӡˢ��")
	{
		CAddDlg6 dlg;
		dlg.DoModal();
	}
	if(str=="��ҩ")
	{
		CAddDlg7 dlg;
		dlg.DoModal();
	}
	if(str=="ָ����")
	{
		CAddDlg8 dlg;
		dlg.DoModal();
	}
}


void CAddDlg::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
