// Upgrade.h : main header file for the UPGRADE application
//

#if !defined(AFX_UPGRADE_H__8ED694C7_3747_43CC_83E5_BE852AB16D99__INCLUDED_)
#define AFX_UPGRADE_H__8ED694C7_3747_43CC_83E5_BE852AB16D99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUpgradeApp:
// See Upgrade.cpp for the implementation of this class
//

class CUpgradeApp : public CWinApp
{
public:
	CUpgradeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgradeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUpgradeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADE_H__8ED694C7_3747_43CC_83E5_BE852AB16D99__INCLUDED_)
