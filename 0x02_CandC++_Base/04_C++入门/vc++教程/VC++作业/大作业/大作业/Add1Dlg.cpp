// Add1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "大作业.h"
#include "Add1Dlg.h"
#include "afxdialogex.h"


// CAdd1Dlg 对话框

IMPLEMENT_DYNAMIC(CAdd1Dlg, CDialogEx)

CAdd1Dlg::CAdd1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdd1Dlg::IDD, pParent)
{

}

CAdd1Dlg::~CAdd1Dlg()
{
}

void CAdd1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdd1Dlg, CDialogEx)
END_MESSAGE_MAP()


// CAdd1Dlg 消息处理程序
