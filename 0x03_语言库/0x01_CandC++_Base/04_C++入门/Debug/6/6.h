// 6.h : main header file for the 6 application
//

#if !defined(AFX_6_H__B05D7456_DC94_4861_9C9E_BE5C20EE63B1__INCLUDED_)
#define AFX_6_H__B05D7456_DC94_4861_9C9E_BE5C20EE63B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy6App:
// See 6.cpp for the implementation of this class
//

class CMy6App : public CWinApp
{
public:
	CMy6App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy6App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_6_H__B05D7456_DC94_4861_9C9E_BE5C20EE63B1__INCLUDED_)
