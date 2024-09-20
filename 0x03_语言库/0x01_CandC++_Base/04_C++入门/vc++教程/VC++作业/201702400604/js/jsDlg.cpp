
// jsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "js.h"
#include "jsDlg.h"
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


// CjsDlg �Ի���




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


// CjsDlg ��Ϣ�������

BOOL CjsDlg::OnInitDialog()
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
	CheckRadioButton(IDC_RADIO1,IDC_RADIO5,IDC_RADIO1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CjsDlg::OnPaint()
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
HCURSOR CjsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CjsDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}


void CjsDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
