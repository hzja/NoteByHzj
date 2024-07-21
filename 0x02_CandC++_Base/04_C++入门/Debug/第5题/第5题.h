// 第5题.h : main header file for the 第5题 application
//

#if !defined(AFX_5_H__0A97BC66_E514_43C4_8D06_6FE82771ABCA__INCLUDED_)
#define AFX_5_H__0A97BC66_E514_43C4_8D06_6FE82771ABCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy5App:
// See 第5题.cpp for the implementation of this class
//

class CMy5App : public CWinApp
{
public:
	CMy5App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy5App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy5App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_5_H__0A97BC66_E514_43C4_8D06_6FE82771ABCA__INCLUDED_)
