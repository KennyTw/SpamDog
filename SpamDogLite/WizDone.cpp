// WizDone.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "WizDone.h"
#include "WizSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizDone property page

IMPLEMENT_DYNCREATE(CWizDone, CPropertyPageEx)
// CPropertyPageEx(CTestPage1::IDD, 0, IDS_ABOUTBOX,IDS_ABOUTBOX)
CWizDone::CWizDone() : CPropertyPageEx(CWizDone::IDD)
{
	//{{AFX_DATA_INIT(CWizDone)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;

	m_strHeaderTitle = _T("SpamDog 設定完成");
	m_strHeaderSubTitle = _T("謝謝您 ! SpamDog 已設定完成"); 
}

CWizDone::~CWizDone()
{
}

void CWizDone::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizDone)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizDone, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizDone)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizDone message handlers

BOOL CWizDone::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWizSheet* pSheet = (CWizSheet*)GetParent();
 
	pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	return CPropertyPageEx::OnSetActive();
}

BOOL CWizDone::OnWizardFinish() 
{
	
	// TODO: Add your specialized code here and/or call the base class
	CWizSheet* pSheet = (CWizSheet*)GetParent();

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	  
     HINSTANCE hInstance = GetModuleHandle(NULL);
     GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	 _splitpath( path_buffer, drive, dir, fname, ext );

	  char IniPath[_MAX_PATH];	  

	  strcpy(IniPath,drive);
	  strcat(IniPath,dir);	   
	  strcat(IniPath,"SpamDog.ini");

	  char DogIDStr[25];
	  itoa(pSheet->DogId , DogIDStr , 10);

      WritePrivateProfileString("SysSetup","DogId",DogIDStr,IniPath);

	  int ApVersion = pSheet->OutlookType -1;
	  char ApVersionStr[10];
	  itoa(ApVersion , ApVersionStr , 10);
	  WritePrivateProfileString("SysSetup","ApVersion",ApVersionStr,IniPath);
	  WritePrivateProfileString("SysSetup","OutlookProf",pSheet->OutlookProf,IniPath);	



	EndDialog(999);
	
	return CPropertyPageEx::OnWizardFinish();
}
