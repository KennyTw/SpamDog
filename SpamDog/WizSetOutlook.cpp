// WizSetOutlook.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WizSetOutlook.h"
#include "WizSheet.h"
#include "MapiProf.h"

#include "../SwOlApi/OlApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizSetOutlook property page

IMPLEMENT_DYNCREATE(CWizSetOutlook, CPropertyPageEx)

CWizSetOutlook::CWizSetOutlook() : CPropertyPageEx(CWizSetOutlook::IDD)
{
	//{{AFX_DATA_INIT(CWizSetOutlook)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;

	m_strHeaderTitle = _T("�]�w�w�]���l��n��");
	m_strHeaderSubTitle = _T("�]�w�w�]���n��i�H�� SpamDog �Ұʮɪ����ʱ�"); 
}

CWizSetOutlook::~CWizSetOutlook()
{
}

void CWizSetOutlook::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizSetOutlook)
	DDX_Control(pDX, IDC_COMBO_OLTYPE, m_OlType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizSetOutlook, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizSetOutlook)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizSetOutlook message handlers

BOOL CWizSetOutlook::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWizSheet* pSheet = (CWizSheet*)GetParent();
	pSheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPageEx::OnSetActive(); 
}

LRESULT CWizSetOutlook::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizSheet* pSheet = (CWizSheet*)GetParent();

	
	
	if (IsDlgButtonChecked(IDC_RADIO2))
	{
			pSheet->OutlookType = 2; //OE
			SetDefaultMailClient(2);
	}
	else
	{
		//outlook
		int curidx = m_OlType.GetCurSel();
		if ( curidx > 0)
		{
		 	
			m_OlType.GetLBText(curidx,pSheet->OutlookProf);
 
		}

		pSheet->OutlookType = 1; //OL
		SetDefaultMailClient(1);
	}

	
	return CPropertyPageEx::OnWizardNext();
}

void CWizSetOutlook::OnRadio1() 
{
	// TODO: Add your control notification handler code here

	
}

void CWizSetOutlook::OnButton1() 
{
	
	SetDefaultMailClient(1);

	m_OlType.ResetContent();
	m_OlType.AddString("���]�w");

	HRESULT hr = OlLogin("Outlook");
	if (hr == S_OK)
	{
 
		CMapiProf mp;
		mp.ListProf(&m_OlType);
		m_OlType.SetCurSel(1);

		OlLogout();	
	}
	else
	{
			AfxMessageBox("�ܳ��p ! SpamDog �b�pôOutlook�ɵo�Ϳ��~!\n�нT�w�z IE ��[���ں����ﶵ][�{����][�q�l�l���}]�����w�� Outlook �νЭ��s�}���A���а���@��");	
	}
}

BOOL CWizSetOutlook::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);

	m_OlType.ResetContent();
	m_OlType.AddString("���]�w");
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
