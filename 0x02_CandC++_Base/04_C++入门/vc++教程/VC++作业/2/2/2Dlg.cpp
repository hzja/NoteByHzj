
// 2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "2.h"
#include "2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy2Dlg 对话框




CMy2Dlg::CMy2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2Dlg::IDD, pParent)
	, m_Group1(FALSE)
	, m_Check(FALSE)
	, m_Edit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_RADIO5, m_Radio1);
	//  DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Radio(pDX, IDC_RADIO5, m_Group1);
	//  DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Check(pDX, IDC_CHECK1, m_Check);
	DDX_Text(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CMy2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMy2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO7, &CMy2Dlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CMy2Dlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_CHECK1, &CMy2Dlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CMy2Dlg 消息处理程序

BOOL CMy2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
   
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2Dlg::OnBnClickedButton2()
{
	OnOK();
	// TODO: 在此添加控件通知处理程序代码
}


void CMy2Dlg::OnBnClickedButton1()
{
    int cj,rs=0;
	int n=GetCheckedRadioButton(IDC_RADIO5,IDC_RADIO9);
	if(n==IDC_RADIO5)
	    cj=100;
	else if(n==IDC_RADIO6)
		cj=90;
	else if(n==IDC_RADIO7)
	    cj=80;
	else if(n==IDC_RADIO8)
	    cj=70;
	else if(n==IDC_RADIO9)
	    cj=60;
	GetDlgItem(IDC_CHECK1)->rs=rs+
		// TODO: 在此添加控件通知处理程序代码
}



void CMy2Dlg::OnBnClickedCheck1()
{

	// TODO: 在此添加控件通知处理程序代码
}
