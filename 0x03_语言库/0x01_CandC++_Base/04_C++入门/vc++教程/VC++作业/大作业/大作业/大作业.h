
// ����ҵ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C����ҵApp:
// �йش����ʵ�֣������ ����ҵ.cpp
//

class C����ҵApp : public CWinApp
{
public:
	C����ҵApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C����ҵApp theApp;