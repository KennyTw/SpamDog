// WizSheet.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WizSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizSheet

IMPLEMENT_DYNAMIC(CWizSheet, CPropertySheetEx)
/*
CWizSheet::CWizSheet(UINT nIDCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(nIDCaption, pParentWnd, iSelectPage,
				  hbmWatermark, hpalWatermark, hbmHeader)
{
//	AddPage(&m_Welcome);	
//	AddPage(&m_GetDogId);
//	AddPage(&m_SetOutlook);
	AddPage(&m_SetEmailIdent);
	AddPage(&m_WaitMailIdent);
	AddPage(&m_WizDone);

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;	
    m_psh.dwFlags &= ~(PSH_HASHELP) ;

}
*/
CWizSheet::CWizSheet( CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(IDS_MAINFRM_TRASH, pParentWnd, iSelectPage,
					  hbmWatermark, hpalWatermark, hbmHeader)
{
	AddPage(&m_Welcome);	
	AddPage(&m_GetDogId);
	AddPage(&m_SetOutlook);
	//AddPage(&m_SetEmailIdent);
	//AddPage(&m_WaitMailIdent);	
	AddPage(&m_LearnTest);
	AddPage(&m_WizOther);
	AddPage(&m_WizDone);

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;	
    m_psh.dwFlags &= ~(PSH_HASHELP) ;

}

CWizSheet::~CWizSheet()
{
}


BEGIN_MESSAGE_MAP(CWizSheet, CPropertySheetEx)
	//{{AFX_MSG_MAP(CWizSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizSheet message handlers
