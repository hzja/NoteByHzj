
// VC++����ҵ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVC����ҵApp:
// �йش����ʵ�֣������ VC++����ҵ.cpp
//

class CVC����ҵApp : public CWinApp
{
public:
	CVC����ҵApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVC����ҵApp theApp;