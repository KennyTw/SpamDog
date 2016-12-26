// WizWaitMailIdent.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WizWaitMailIdent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizWaitMailIdent property page

IMPLEMENT_DYNCREATE(CWizWaitMailIdent, CPropertyPageEx)

CWizWaitMailIdent::CWizWaitMailIdent() : CPropertyPageEx(CWizWaitMailIdent::IDD)
{
	//{{AFX_DATA_INIT(CWizWaitMailIdent)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CWizWaitMailIdent::~CWizWaitMailIdent()
{
}

void CWizWaitMailIdent::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizWaitMailIdent)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizWaitMailIdent, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizWaitMailIdent)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizWaitMailIdent message handlers
