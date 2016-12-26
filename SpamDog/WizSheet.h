#if !defined(AFX_WIZSHEET_H__3E6A3F79_E91E_40D6_95E2_43E12C95B582__INCLUDED_)
#define AFX_WIZSHEET_H__3E6A3F79_E91E_40D6_95E2_43E12C95B582__INCLUDED_

 
	#include "WizWelcome.h"
	#include "WizGetDogId.h"
	#include "WizSetOutlook.h"
	#include "WizLearnTest.h"
	#include "WizDone.h"
	#include "WizOther.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizSheet

class CWizSheet : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CWizSheet)

// Construction
public:
/*	CWizSheet(UINT nIDCaption, CWnd* pParentWnd = NULL,
			UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
			HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);*/
	CWizSheet(CWnd* pParentWnd = NULL,
			UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
			HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);

// Attributes
public:

	CWizWelcome m_Welcome;
	CWizGetDogId m_GetDogId;	
	CWizSetOutlook m_SetOutlook;
	CWizLearnTest m_LearnTest;
	CWizDone m_WizDone;
	CWizOther m_WizOther;
	char OutlookType;
	CString OutlookProf;
	int DogId;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWizSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWizSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZSHEET_H__3E6A3F79_E91E_40D6_95E2_43E12C95B582__INCLUDED_)
