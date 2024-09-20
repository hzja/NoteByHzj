
// jsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "js.h"
#include "jsDlg.h"
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


// CjsDlg 对话框




CjsDlg::CjsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CjsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Edit1 = 0;
}

void CjsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	//  DDX_Control(pDX, IDC_CHECK2, m_Chevk2);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK4, m_Check4);
	DDX_Control(pDX, IDC_CHECK5, m_Check5);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	//  DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	//  DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT1, m_cEdit);
}

BEGIN_MESSAGE_MAP(CjsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CjsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CjsDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CjsDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CjsDlg 消息处理程序

BOOL CjsDlg::OnInitDialog()
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
	CheckRadioButton(IDC_RADIO1,IDC_RADIO5,IDC_RADIO1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CjsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CjsDlg::OnPaint()
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
HCURSOR CjsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CjsDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CjsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int i=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO5),
		j1=m_Check1.GetCheck (),
		j2=m_Check2.GetCheck (),
		j3=m_Check3.GetCheck (),
		j4=m_Check4.GetCheck (),
		j5=m_Check5.GetCheck ();
	if(i==IDC_RADIO1)
		m_Edit1=100*(j1*10+j2*9+j3*8+j4*7+j5*6);
	else if(i==IDC_RADIO2)
		m_Edit1=90*(j1*10+j2*9+j3*8+j4*7+j5*6);
	else if(i==IDC_RADIO3)
		m_Edit1=80*(j1*10+j2*9+j3*8+j4*7+j5*6);
	else if(i==IDC_RADIO4)
		m_Edit1=70*(j1*10+j2*9+j3*8+j4*7+j5*6);
	else if(i==IDC_RADIO5)
		m_Edit1=60*(j1*10+j2*9+j3*8+j4*7+j5*6);
	UpdateData(FALSE);
	
}


void CjsDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
