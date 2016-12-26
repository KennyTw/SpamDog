// LiveUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "LiveUpdate.h"
#include "TurnDB.h"
#include "SetupFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdate dialog


CLiveUpdate::CLiveUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CLiveUpdate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLiveUpdate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
 
}


void CLiveUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiveUpdate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_STATIC_CUTVALUE, m_DbCutValue);
		DDX_Control(pDX, IDC_SLIDER_CUTDB, m_DbCut);
		//DDX_Control(pDX, IDC_CHECK_AUTOUPDATE, m_AutoUpdate);
		//DDX_Control(pDX, IDC_CHECK_AUTOBACKUP, m_AutoBackup);
		//DDX_Control(pDX, IDC_FILEEDIT, m_FilePath);
		DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLiveUpdate, CDialog)
	//{{AFX_MSG_MAP(CLiveUpdate)
	//ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CUTDB, OnCustomdrawSliderCutdb)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdate message handlers

BOOL CLiveUpdate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_status.SetWindowText("");

	m_DbCut.SetRange(0,3);
	m_DbCut.SetTicFreq(1);
	
	// TODO: Add extra initialization here

	GetIni();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CLiveUpdate::GetIni()
{
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

   /* int AutoUpdate = GetPrivateProfileInt("SysSetup","AutoUpdate",1,IniPath);
	if (AutoUpdate == 1)
	{
		m_AutoUpdate.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_AutoUpdate.SetCheck(BST_UNCHECKED);
	}

	int AutoBackup = GetPrivateProfileInt("SysSetup","AutoBackup",1,IniPath);
	if (AutoBackup == 1)
	{
		m_AutoBackup.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_AutoBackup.SetCheck(BST_UNCHECKED);
	}*/

}
/*
void CLiveUpdate::OnButton1() 
{
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   char EXEPath[_MAX_PATH];	   

	   strcpy(EXEPath,drive);
	   strcat(EXEPath,dir);	   
//#ifdef _OUTLOOK
	   strcat(EXEPath,"SpamdogReporter.exe");
//#endif
//#ifdef _OUTLOOKEX//
	   //strcat(EXEPath,"LiveUpdateOE.exe");
//#endif

	    

    CString Wndstr;
	Wndstr.Format("%d %d",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId());
	//AfxMessageBox(Wndstr);

	ShellExecute(AfxGetMainWnd()->m_hWnd,"open",EXEPath,Wndstr,"",SW_SHOW);
	
 

	if (GetParent() != NULL)
	{
		 
			CSetupFrm *pfrm = NULL;
			pfrm = (CSetupFrm *) GetParent()->GetParent();
			if (pfrm != NULL)
			  pfrm->EndDialog(0);
		 
	}
	
}*/

void CLiveUpdate::OnButton7() 
{
	CTurnDB tdb;
	tdb.FixDB(&m_status);

	AfxMessageBox("作業完成!");	
}

void CLiveUpdate::OnButton4() 
{
	CTurnDB tdb;
	
	tdb.TruncDB(m_DbCut.GetPos(),&m_status);

	AfxMessageBox("作業完成!");	
}

/*
void CLiveUpdate::OnButton6() 
{
	static char  szFilters[]=  "SpamDog Key Files (*.sdb)|*.sdb||";

   // Create an Open dialog; the default file name extension is ".my".
     CFileDialog fileDlg (TRUE, "sdb", "*.sdb",
      OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);


   if(fileDlg.DoModal() ==IDOK )
   {
      CString pathName = fileDlg.GetPathName();
   
      // Implement opening and reading file in here.
      m_FilePath.SetWindowText(pathName);
      //Change the window's title to the opened file's title.
      //CString fileName = fileDlg.GetFileTitle ();
   
      

   }

 	
}

void CLiveUpdate::OnButton5() 
{
	// TODO: Add your control notification handler code here
	CTurnDB tdb;

	CString path;
	m_FilePath.GetWindowText(path);

	if (!path.IsEmpty())
	{
	
		tdb.ParseKeyFile(path.GetBuffer(path.GetLength()));

		path.ReleaseBuffer();

		AfxMessageBox("完成匯入");
	}

}*/

void CLiveUpdate::OnCustomdrawSliderCutdb(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int m_pos = m_DbCut.GetPos();

	
	CString tempstr;
	if (m_pos == 0)
	{
		tempstr = "刪除未使用";
	}
	else 
	{
	    tempstr.Format("刪除使用次數低於 %d 次",m_pos);
	}
	

	m_DbCutValue.SetWindowText(tempstr);

	*pResult = 0;
}

void CLiveUpdate::OnButton2() 
{
	// TODO: Add your control notification handler code here
		  char IniPath[_MAX_PATH];

		  char db1Path[_MAX_PATH]; 
		  char db2Path[_MAX_PATH]; 
		  char db3Path[_MAX_PATH]; 
		  //char ipdb1Path[_MAX_PATH]; 
		  //char ipdb2Path[_MAX_PATH]; 
		  //char ipdb3Path[_MAX_PATH]; 

		  char todb1Path[_MAX_PATH]; 
		  char todb2Path[_MAX_PATH]; 
		  char todb3Path[_MAX_PATH]; 
		  //char toipdb1Path[_MAX_PATH]; 
		  //char toipdb2Path[_MAX_PATH]; 
		  //char toipdb3Path[_MAX_PATH]; 

		  CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

		  strcpy(db1Path,mfrm->PGPath);
		  strcpy(db2Path,mfrm->PGPath);
		  strcpy(db3Path,mfrm->PGPath);
		  //strcpy(ipdb1Path,mfrm->PGPath);
		  //strcpy(ipdb2Path,mfrm->PGPath);
		  //strcpy(ipdb3Path,mfrm->PGPath);
		  strcpy(IniPath,mfrm->PGPath);

		  strcpy(todb1Path,mfrm->PGPath);
		  strcpy(todb2Path,mfrm->PGPath);
		  strcpy(todb3Path,mfrm->PGPath);
		  //strcpy(toipdb1Path,mfrm->PGPath);
		  //strcpy(toipdb2Path,mfrm->PGPath);
		  //strcpy(toipdb3Path,mfrm->PGPath);

		  strcat(todb1Path,"Backup//");
		  //CreateDirectory(todb1Path,NULL);
		  strcat(todb1Path,CTime::GetCurrentTime().Format("%Y%m%d"));
		  strcat(todb1Path,"102db1.db");

		  strcat(todb2Path,"Backup//");
		  strcat(todb2Path,CTime::GetCurrentTime().Format("%Y%m%d"));
		  strcat(todb2Path,"102db2.db");

		  strcat(todb3Path,"Backup//");
		  strcat(todb3Path,CTime::GetCurrentTime().Format("%Y%m%d"));
		  strcat(todb3Path,"102db3.db");


		  //strcat(toipdb1Path,"Backup//");
		  //CreateDirectory(todb1Path,NULL);
		  //strcat(toipdb1Path,CTime::GetCurrentTime().Format("%Y%m%d"));
		  //strcat(toipdb1Path,"ipdb1.db");

		  //strcat(toipdb2Path,"Backup//");
		  //strcat(toipdb2Path,CTime::GetCurrentTime().Format("%Y%m%d"));
		  //strcat(toipdb2Path,"ipdb2.db");

		  //strcat(toipdb3Path,"Backup//");
		  //strcat(toipdb3Path,CTime::GetCurrentTime().Format("%Y%m%d"));
		  //strcat(toipdb3Path,"ipdb3.db");

		 

		  strcat(db1Path,"DB//102db1.db");
		  strcat(db2Path,"DB//102db2.db");
		  strcat(db3Path,"DB//102db3.db");

		  //strcat(ipdb1Path,"DB//ipdb1.db");
		  //strcat(ipdb2Path,"DB//ipdb2.db");
		  //strcat(ipdb3Path,"DB//ipdb3.db");

	
		  strcat(IniPath,"SpamDog.ini");

		  
		  CopyFile(db1Path,todb1Path,false);		  
		  CopyFile(db2Path,todb2Path,false);	  
		  CopyFile(db3Path,todb3Path,false);		
		  
		 // CopyFile(ipdb1Path,toipdb1Path,false);		  
		  //CopyFile(ipdb2Path,toipdb2Path,false);	  
		  //CopyFile(ipdb3Path,toipdb3Path,false);	
		

	 
	    CTime ct;
	    ct = CTime::GetCurrentTime();
	    long tvalue = (long) ct.GetTime();

		

	    mfrm->Settings.LastBackTime = tvalue;

		CString tmpstr;
		tmpstr.Format("%d",tvalue);

		WritePrivateProfileString("SysSetup","LastBackup",tmpstr,IniPath);

		AfxMessageBox("備份完成!");
}
