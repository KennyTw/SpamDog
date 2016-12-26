// WizOther.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WizOther.h"
#include "WizSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizOther property page

IMPLEMENT_DYNCREATE(CWizOther, CPropertyPageEx)

CWizOther::CWizOther() : CPropertyPageEx(CWizOther::IDD)
{
	//{{AFX_DATA_INIT(CWizOther)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		m_psp.dwFlags &= ~PSP_HASHELP;

	m_strHeaderTitle = _T("垃圾郵件說明");
	m_strHeaderSubTitle = _T("請務必遵照原則處理信件,如違反原則者,將直接停權"); 
}

CWizOther::~CWizOther()
{
}

void CWizOther::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizOther)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizOther, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizOther message handlers

BOOL CWizOther::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizSheet* pSheet = (CWizSheet*)GetParent();
	pSheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPageEx::OnSetActive();
}

BOOL CWizOther::OnInitDialog() 
{
	LOGFONT lf;                        // Used to create the CFont.
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
	CFont *pFont = GetDlgItem(IDC_STATIC1)->GetFont();
	 
	pFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	//SetFont(&lf, bRedraw);

   	
   //lf.lfHeight = 20;                  // Request a 20-pixel-high font
   //strcpy(lf.lfFaceName, "Arial");    //    with face name "Arial".
   m_font.CreateFontIndirect(&lf);    // Create the font.

   // Use the font to paint a control. This code assumes
   // a control named IDC_TEXT1 in the dialog box.
   GetDlgItem(IDC_STATIC1)->SetFont(&m_font);
   GetDlgItem(IDC_STATIC2)->SetFont(&m_font);
   GetDlgItem(IDC_STATIC3)->SetFont(&m_font);
   //GetDlgItem(IDC_STATIC4)->SetFont(&m_font);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
