// WizWelcome.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WizWelcome.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizWelcome property page

IMPLEMENT_DYNCREATE(CWizWelcome, CPropertyPageEx)

CWizWelcome::CWizWelcome() : CPropertyPageEx(CWizWelcome::IDD)
{
	//{{AFX_DATA_INIT(CWizWelcome)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;
	//m_psp.pszTitle = "Welcome To SpamDog --!";
//	m_psp.pszHeaderSubTitle  = "Subtitle";

	m_strHeaderTitle = _T("歡迎使用 SpamDog");
	m_strHeaderSubTitle = _T("請依照步驟完成初次使用的設定"); 


}

CWizWelcome::~CWizWelcome()
{
}

void CWizWelcome::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizWelcome)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CWizWelcome, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizWelcome)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizWelcome message handlers

BOOL CWizWelcome::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPageEx::OnSetActive();
}
