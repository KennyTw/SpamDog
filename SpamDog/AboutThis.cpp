// AboutThis.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDog.h"
#include "AboutThis.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutThis dialog


CAboutThis::CAboutThis(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutThis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutThis)
	//}}AFX_DATA_INIT
}


void CAboutThis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutThis)
	DDX_Control(pDX, IDC_STATIC_LICENSED, m_Licensed);
	DDX_Control(pDX, IDC_STATIC_DOGID, m_DogId);
	DDX_Control(pDX, IDC_STATIC_APLASTUPDATE, m_ApLastUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutThis, CDialog)
	//{{AFX_MSG_MAP(CAboutThis)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutThis message handlers

void CAboutThis::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	WindowsOpen("http://www.softworking.com");
}

void CAboutThis::OnButton2() 
{
	// TODO: Add your control notification handler code here
	//ShellExecute(AfxGetMainWnd()->m_hWnd,"open","http://www.softworking.com/dogmanual.asp",NULL,"",SW_SHOW);



	//EndDialog(0);
}

void CAboutThis::OnButton13() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(AfxGetMainWnd()->m_hWnd,"open","mailto:service@softworking.com",NULL,"",SW_SHOW);
	
}

BOOL CAboutThis::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   char IniPath[_MAX_PATH];	   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");

    /*char tempstr[50];
	GetPrivateProfileString("LiveUpdate","CurrentVersion","",tempstr,50,IniPath);
	m_VersionNow.SetWindowText(tempstr);*/

	int LastUpdateTime = GetPrivateProfileInt("LiveUpdate","LastUpdate",0,IniPath);
	CTime t1((time_t) LastUpdateTime);

	m_ApLastUpdate.SetWindowText(t1.Format("%Y/%m/%d %H:%M:%S"));

	char tempstr2[50];
	GetPrivateProfileString("SysSetup","DogId","",tempstr2,50,IniPath);	
	m_DogId.SetWindowText(tempstr2);

	int LicenseTime = GetPrivateProfileInt("SysSetup","LicenseTime",0,IniPath);
	if (LicenseTime > 0)
	{
		CTime t2((time_t) LicenseTime);	
		m_Licensed.SetWindowText(t2.Format("[ 贊助版 到期日 %Y/%m/%d ]"));
	}
	else
	{
		m_Licensed.SetWindowText("");
	}

 
	//int LastReportTime = GetPrivateProfileInt("LiveUpdate","LastReport",0,IniPath);
	//CTime t2((time_t) LastReportTime);

	//m_DataLastUpdate.SetWindowText(t2.Format("%Y/%m/%d %H:%M:%S"));
 
	
	  
		 
	 
	
 

	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutThis::OnButton3() 
{
	

}

void CAboutThis::OnButton16() 
{
	// TODO: Add your control notification handler code here
		//ShellExecute(AfxGetMainWnd()->m_hWnd,"opennew","http://www.softworking.com/Donate.asp",NULL,"",SW_SHOW);
	WindowsOpen("http://www.softworking.com/Donate.asp");
}
