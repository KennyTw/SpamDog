// OtherSetupFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "OtherSetupFrm.h"
#include "MapiProf.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherSetupFrm dialog


COtherSetupFrm::COtherSetupFrm(CWnd* pParent /*=NULL*/)
	: CDialog(COtherSetupFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherSetupFrm)
	//}}AFX_DATA_INIT
}


void COtherSetupFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherSetupFrm)
	DDX_Control(pDX, IDC_EDIT_ARRANGEMIN, m_ArrangeMin);
	DDX_Control(pDX, IDC_CHECK_AUTOARRANGE, m_AutoArrange);
	DDX_Control(pDX, IDC_CHECK_SYSAUTODEL, m_SysAutoDel);
	DDX_Control(pDX, IDC_EDIT_SYSDELDAY, m_SysDelDay);
	DDX_Control(pDX, IDC_EDIT_ARRANGEFOLDER, m_ArrangeFolder);
	DDX_Control(pDX, IDC_CHECK_STARTWITHAP, m_StartWithAP);
	DDX_Control(pDX, IDC_COMBO2, m_ApVersion);
	DDX_Control(pDX, IDC_CHECK_BOOTSTART, m_BootStart);
	DDX_Control(pDX, IDC_COMBO1, m_ProfList);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherSetupFrm, CDialog)
	//{{AFX_MSG_MAP(COtherSetupFrm)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherSetupFrm message handlers

BOOL COtherSetupFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ProfList.ResetContent();
	m_ProfList.AddString("不設定");
	
	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	if (mfrm->Settings.APVersion == AP_OUTLOOK)
	{
		CMapiProf mp;
		mp.ListProf(&m_ProfList);
	}

	m_ProfList.SetCurSel(0);

	if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
	{
		  m_ProfList.EnableWindow(false);
		  //m_AuthEx.EnableWindow(false);
		  m_ApVersion.SetCurSel(1);
	}
	else
	{
		  m_ApVersion.SetCurSel(0);
	}
	
	GetIni();
	// TODO: Add extra initialization here

	/*if (mfrm->Settings.Licensed)
	{
	
		m_NoDogNews.EnableWindow(true);
	
	}*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COtherSetupFrm::GetIni()
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

    char tempstr[50];
	GetPrivateProfileString("SysSetup","OutlookProf","",tempstr,50,IniPath);

	m_ProfList.SelectString(-1,tempstr);

	int BootStart = GetPrivateProfileInt("SysSetup","BootStart",0,IniPath);
	if (BootStart == 1)
	{
		m_BootStart.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_BootStart.SetCheck(BST_UNCHECKED);
	}

	int StartWithAp = GetPrivateProfileInt("SysSetup","BootWithAp",0,IniPath);
	if (StartWithAp == 1)
	{
		m_StartWithAP.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_StartWithAP.SetCheck(BST_UNCHECKED);
	}

	

	/*int AuthEx = GetPrivateProfileInt("SysSetup","AuthEx",0,IniPath);
	if (AuthEx == 1)
	{
		m_AuthEx.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_AuthEx.SetCheck(BST_UNCHECKED);
	}*/

	int SysAutoDel = GetPrivateProfileInt("SysSetup","SysAutoDel",0,IniPath);
	if (SysAutoDel == 1)
	{
		m_SysAutoDel.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_SysAutoDel.SetCheck(BST_UNCHECKED);
	}

	char tempstr2[255];
	GetPrivateProfileString("SysSetup","ArrangeMailFolder","[我的整理郵件]",tempstr2,255,IniPath);
	m_ArrangeFolder.SetWindowText(tempstr2);

	char tempstr3[255];
	GetPrivateProfileString("SysSetup","SysDelDay","5",tempstr3,255,IniPath);
	m_SysDelDay.SetWindowText(tempstr3);


	int AutoArrange = GetPrivateProfileInt("SysSetup","AutoArrange",0,IniPath);
	if (AutoArrange == 1)
	{
		m_AutoArrange.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_AutoArrange.SetCheck(BST_UNCHECKED);
	}

	char tempstr4[255];
	GetPrivateProfileString("SysSetup","ArrangeMin","30",tempstr4,255,IniPath);
	m_ArrangeMin.SetWindowText(tempstr4);


	/*int NoDogNews = GetPrivateProfileInt("SysSetup","NoDogNews",0,IniPath);
	if (NoDogNews == 1)
	{
		m_NoDogNews.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_NoDogNews.SetCheck(BST_UNCHECKED);
	}*/


}

void COtherSetupFrm::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CMainFrm *mainfrm = (CMainFrm *) AfxGetMainWnd();
	mainfrm->HandUpdate = true;
	mainfrm->CheckLicenseValid();

	CDialog *pDialog = (CDialog *) this->GetParent()->GetParent();
	pDialog->EndDialog(0);
}

void COtherSetupFrm::OnButton3() 
{
 
	if (MessageBox("是否確定移除 SpamDog ?\r\n移除完成後,您可以直接將 Outlook 中的相關信件匣刪除即可","移除 SpamDog",MB_OKCANCEL) != IDOK)
	return;
	
	// TODO: Add your control notification handler code here
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	CMainFrm *mainfrm = (CMainFrm *) AfxGetMainWnd();

	//關閉 registry 開機啟動
		//如果有則刪除
			HKEY RegKey;
			RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);		
	
			DWORD m_dwCount;
			RegQueryValueEx(RegKey,"SpamDog",0,NULL,NULL,&m_dwCount);
			if (m_dwCount > 0)
			{		
				//RegSetValueEx(RegKey,"SpamDog",0,REG_SZ,(const unsigned char *)path_buffer,strlen(path_buffer));
				RegDeleteValue(RegKey,"SpamDog");

			}
        
			RegCloseKey(RegKey);
	

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];
	 char BatchFile[_MAX_PATH];

	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );


	   

			   char DogExecPath[_MAX_PATH];	 
			   char WorkPath[_MAX_PATH];	 
			   char ReportPath[_MAX_PATH];
			   char DBPath[_MAX_PATH];
			   char NicDataPath[_MAX_PATH];
			   char BackupPath[_MAX_PATH];
			   char TempPath[_MAX_PATH];
			   

			   strcpy(DogExecPath,drive);
			   strcat(DogExecPath,dir);	   
			   strcat(DogExecPath,"SpamDogLite.exe");

			   strcpy(WorkPath,drive);
			   strcat(WorkPath,dir);	  

			   strcpy(BatchFile,drive);
			   strcat(BatchFile,"\\UninstallSpamDog.bat");

			   strcpy(ReportPath,WorkPath);
			   strcat(ReportPath,"Report");

			   strcpy(NicDataPath,WorkPath);
			   strcat(NicDataPath,"NicData");			   

			   strcpy(DBPath,WorkPath);
			   strcat(DBPath,"DB");

			   strcpy(BackupPath,WorkPath);
			   strcat(BackupPath,"Backup");

			   
			
			//Create Del Batch
			char Batch[10240];

			strcpy(Batch,"cd \\\r\n:Repeat\r\n");
			strcat(Batch,"del \"");
			strcat(Batch,DogExecPath);
			strcat(Batch,"\" \r\n");
			strcat(Batch,"if exist \"");
			strcat(Batch,DogExecPath);
			strcat(Batch,"\" goto Repeat\r\n");

			strcat(Batch,"rmdir /S /Q \"");
			strcat(Batch,ReportPath);
			strcat(Batch,"\" \r\n");

			strcat(Batch,"rmdir /S /Q \"");
			strcat(Batch,DBPath);
			strcat(Batch,"\" \r\n");

			strcat(Batch,"rmdir /S /Q \"");
			strcat(Batch,BackupPath);
			strcat(Batch,"\" \r\n");

			strcat(Batch,"rmdir /S /Q \"");
			strcat(Batch,NicDataPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swspam.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swspam.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files SwOLApi.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"SwOLApi.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swfiledb.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swfiledb.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swzip.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swzip.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swparser.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swparser.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swmatcher.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swmatcher.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swsocket.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swsocket.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Swvirus.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Swvirus.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files SpamDog.ini
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"SpamDog.ini");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files Rule.dat
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Rule.dat");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files MailPath.dat
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"MailPath.dat");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files MailPath2.dat
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"MailPath2.dat");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Delete Files LiveUpdate.exe
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"LiveUpdate.exe");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//dbghelp.dll			
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"dbghelp.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Mfc42d.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Mfc42d.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Mfco42d.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Mfco42d.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Msvcp60d.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Msvcp60d.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//Msvcrtd.dll
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"Msvcrtd.dll");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//SpamDog.chm
			strcpy(TempPath,WorkPath);
			strcat(TempPath,"SpamDog.chm");

			strcat(Batch,"del /Q \"");
			strcat(Batch,TempPath);
			strcat(Batch,"\" \r\n");

			//rm dir
			strcat(Batch,"rmdir /Q \"");
			strcat(Batch,WorkPath);
			strcat(Batch,"\" \r\n");


			strcat(Batch,"del ");
			strcat(Batch,BatchFile);
			strcat(Batch," \r\n");

			FILE *fp;
			fp = fopen(BatchFile,"w+b");
			fwrite(Batch,1,strlen(Batch),fp);
			fclose(fp);

	    


	//ShellExecute(NULL, "open", "c:\\UnstallSpamDog.bat", NULL, NULL, SW_HIDE);

 
	 // Get ready to spawn the batch file we just created.
      ZeroMemory(&si, sizeof(si));
      si.cb = sizeof(si);

      // We want its console window to be invisible to the user.
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE;

      // Spawn the batch file with low-priority and suspended.
      if (CreateProcess(NULL, BatchFile, NULL, NULL, FALSE,
         CREATE_SUSPENDED | IDLE_PRIORITY_CLASS, NULL, 
         __TEXT("\\"), &si, &pi)) {

         // Lower the batch file's priority even more.
         SetThreadPriority(pi.hThread, THREAD_PRIORITY_IDLE);

         // Raise our priority so that we terminate as quickly as possible.
         SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
         SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

         // Allow the batch file to run and clean-up our handles.
         CloseHandle(pi.hProcess);
         ResumeThread(pi.hThread);
         // We want to terminate right away now so that we can be deleted
         CloseHandle(pi.hThread);
      }

  	CDialog *pDialog = (CDialog *) this->GetParent()->GetParent();
	pDialog->EndDialog(0);

	mainfrm->OnCloseSpamDog(0,0);

 	
}
