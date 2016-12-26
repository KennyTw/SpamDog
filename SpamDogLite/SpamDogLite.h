// SpamDog.h : main header file for the SPAMDOG application
//

#if !defined(AFX_SPAMDOG_H__8EBD491B_85CC_4554_B47F_E08EEF4FD526__INCLUDED_)
#define AFX_SPAMDOG_H__8EBD491B_85CC_4554_B47F_E08EEF4FD526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols



/////////////////////////////////////////////////////////////////////////////
// CSpamDogApp:
// See SpamDog.cpp for the implementation of this class
//
LONG __stdcall TheCrashHandlerFunction ( EXCEPTION_POINTERS * pExPtrs );
void SetDefaultMailClient(int ClientCode);
void WindowsOpen(char *Url);

class CSpamDogApp : public CWinApp
{
public:
	CSpamDogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpamDogApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpamDogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPAMDOG_H__8EBD491B_85CC_4554_B47F_E08EEF4FD526__INCLUDED_)
