
// 2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2.h"
#include "2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy2Dlg �Ի���




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


// CMy2Dlg ��Ϣ�������

BOOL CMy2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
   
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2Dlg::OnBnClickedButton2()
{
	OnOK();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		// TODO: �ڴ���ӿؼ�֪ͨ����������
}



void CMy2Dlg::OnBnClickedCheck1()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
