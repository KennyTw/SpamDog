// MainFrm.cpp : implementation file
//


#include "stdafx.h"
#include "SpamDog.h"
#include "MainFrm.h"
#include "SetupFrm.h"

#include <process.h>
//#include "Wizard.h"

//#include "MailExpert.h"
//#include "MailExpert2.h"
#include "MailFolderLearn.h"

#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"

#include "../Swzip/Compress.h"
#include "../Swparser/MailParser.h"
#include "../Swfiledb/DB.h"

#ifdef _CONSOLEDBG
	#include <conio.h>
#endif 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_NOTIFYICON WM_USER+5 //自定?消息的ID 
#define WM_NOTIFYICON2 WM_USER+6 //自定?消息的ID 

#define WM_CLOSESPAMDOG WM_USER+1222

#define WM_LICENSE_GO_AD WM_USER+1223
#define WM_LICENSE_MAC_ERROR WM_USER+1224
#define WM_LICENSE_CHECK_LICENSED WM_USER+1225
#define WM_LICENSE_CONNECT_ERROR WM_USER+1226
#define WM_LICENSE_LOCK WM_USER+1227



#include "MapiEx.h"
#include "OEapi.h"

#include <tlhelp32.h>

//Global Class Definition ///////////////////////////////////////
//#ifdef _OUTLOOK
	 CMapiEx *g_MapiEx;
//#endif

//#ifdef _OUTLOOKEX	 
	 COEapi *g_OEapi;	 	
//#endif   

	 

/////////////////////////////////////////////////////////////////////////////
// CMainFrm dialog

const UINT WM_TASKBARCREATED = 
     ::RegisterWindowMessage(_T("TaskbarCreated"));


/////////////////////////////////////////////////////////////////////////////
// CMainFrm dialog

CMainFrm::CMainFrm(CWnd* pParent /*=NULL*/)
	: CDialog (CMainFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainFrm)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//Create(IDD_SPAMDOG_DIALOG, pParent); 

	InitializeCriticalSection(&m_CritSec); 
	InitializeCriticalSection(&m_RangeDbSec);
	InitializeCriticalSection(&m_DBSec);


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	m_hIcon2 = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_hIcon3 = AfxGetApp()->LoadIcon(IDI_ICON4);
	m_hIcon4 = AfxGetApp()->LoadIcon(IDI_ICON5);
	
	
    

	ReporterThread = NULL;
	SysMailThread = NULL;
	DelTrashMailThread = NULL;
	CheckLicenseThread = NULL;

	HandUpdate = false;

	DogId = 0;
	ClientPid = 0;
	//#ifdef _OUTLOOKEX
	   g_OEapi = NULL;
	//#endif

	//#ifdef _OUTLOOK
	   g_MapiEx = NULL;
	//#endif

	  
	memset(&Settings,0,sizeof(APsetup));

	//GoUpdate = false;
	FirstStart = true;
	NeedUpdateNow = false;
	StopCancelNewMailIcon = false;

#ifdef _CONSOLEDBG
     AllocConsole();
	 hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	 SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
	 SetConsoleTitle("SpamDog Debug Console");
#endif
}

CMainFrm::~CMainFrm()
{
	//#ifdef _OUTLOOKEX
	   if (g_OEapi != NULL)
		   delete g_OEapi;
	//#endif

	//#ifdef _OUTLOOK	   
	    if (g_MapiEx != NULL)
		   delete g_MapiEx;
	//#endif
#ifdef _CONSOLEDBG
	FreeConsole();
	hConsole = NULL;
#endif

	DeleteCriticalSection (&m_CritSec);  
	DeleteCriticalSection (&m_RangeDbSec);  
	DeleteCriticalSection (&m_DBSec);
	
}

void CMainFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainFrm)
	DDX_Control(pDX, IDC_STATIC1, m_MainStatic);
	DDX_Control(pDX, IDC_BUTTON1, m_Trash);
	DDX_Control(pDX, IDC_BUTTON2, m_Setup);
	DDX_Control(pDX, IDC_BUTTON3, m_Close);
	DDX_Control(pDX, IDC_BUTTON4, m_ScanBox);
	DDX_Control(pDX, IDC_BUTTON5, m_LearnWiz);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainFrm, CDialog)
	//{{AFX_MSG_MAP(CMainFrm)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_NOTIFYICON,OnNotifyIcon)
	ON_MESSAGE(WM_NOTIFYICON2,OnNotifyIcon2)
	ON_MESSAGE(WM_CLOSESPAMDOG,OnCloseSpamDog)
	ON_MESSAGE(WM_LICENSE_GO_AD,OnLicenseGoAd)
	ON_MESSAGE(WM_LICENSE_MAC_ERROR ,OnLicenseMacError)
	ON_MESSAGE(WM_LICENSE_CHECK_LICENSED  ,OnLicenseCheckLicensed)
	ON_MESSAGE(WM_LICENSE_CONNECT_ERROR   ,OnLicenseConnectError)
	ON_MESSAGE(WM_LICENSE_LOCK , OnLicenseLock)
	
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENUITEM32774, OnMenuitem32774)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_COMMAND(ID_MENUITEM32771, OnMenuitem32771)
	ON_COMMAND(ID_MENUITEM32772, OnMenuitem32772)
	ON_COMMAND(ID_MENUITEM32773, OnMenuitem32773)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_MENUITEM32775, OnMenuitem32775)
	ON_COMMAND(ID_MENUITEM32776, OnMenuitem32776)
	ON_COMMAND(ID_MENUITEM32777, OnMenuitem32777)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrm message handlers

LRESULT CMainFrm::OnTaskBarCreated(WPARAM wp, LPARAM lp)
{
     AddTrayIcon();
     return 0;
}


BOOL CMainFrm::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	InitialValue();

	Settings.ShowAD = true;
	 


	/*
	CTime t1( 2005, 6, 1, 0, 0, 0 ); 
	CTime t2((time_t) CTime::GetCurrentTime().GetTime());
	
	CTimeSpan ts = t2-t1;
	if (ts.GetSeconds() >= 0)
	{
	
		AfxMessageBox("很抱歉 ! 此版本已過期 , 請至 http://www.softworking.com 下載更新版本 !");
		EndDialog(0);

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



		return false;
	
	}*/


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	

	 

	//MoveWindow(wrect);
	
	// TODO: Add extra initialization here
	//CheckDBVersion();

	m_Trash.SetBitMap(IDB_BITMAP1);
	m_Trash.SetButtonTextColor(RGB(0,0,0),RGB(0,0,0),RGB(255,255,255));	
	m_Trash.SetButtonBgColor(GetSysColor(COLOR_BTNFACE),RGB(152,152,186),RGB(0x66,0x66,0x99));

	m_Setup.SetBitMap(IDB_BITMAP2);
	m_Setup.SetButtonTextColor(RGB(0,0,0),RGB(0,0,0),RGB(255,255,255));	
	m_Setup.SetButtonBgColor(GetSysColor(COLOR_BTNFACE),RGB(152,152,186),RGB(0x66,0x66,0x99));

	m_Close.SetBitMap(IDB_BITMAP4);
	m_Close.SetButtonTextColor(RGB(0,0,0),RGB(0,0,0),RGB(255,255,255));	
	m_Close.SetButtonBgColor(GetSysColor(COLOR_BTNFACE),RGB(152,152,186),RGB(0x66,0x66,0x99));

	m_ScanBox.SetBitMap(IDB_BITMAP5);
	m_ScanBox.SetButtonTextColor(RGB(0,0,0),RGB(0,0,0),RGB(255,255,255));	
	m_ScanBox.SetButtonBgColor(GetSysColor(COLOR_BTNFACE),RGB(152,152,186),RGB(0x66,0x66,0x99));

	m_LearnWiz.SetBitMap(IDB_BITMAP6);
	m_LearnWiz.SetButtonTextColor(RGB(0,0,0),RGB(0,0,0),RGB(255,255,255));	
	m_LearnWiz.SetButtonBgColor(GetSysColor(COLOR_BTNFACE),RGB(152,152,186),RGB(0x66,0x66,0x99));
	

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	 
	
	// TODO: Add extra initialization here

	//m_AutoCheck.SetCheck(1);
	m_MainStatic.SetWindowText("啟動中..");

		//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

	Getini();
				
	
	
	MsgFrm.Create(IDD_MSGFRM_DIALOG,0);
	//WebFrm.Create(IDD_WEBFRM_DIALOG,0);
	 

	//MsgFrm.ShowMsg(RGB(0,0,0),RGB(0,0,100),"test");

	GetIniRule();


   
	
	//SetTimer(1,5000 ,NULL);

		//處理 waitting msg dump
			//	MSG msg;
	/*			while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}*/
 
	char Path[MAX_PATH];
	strcpy(Path,PGPath);
	strcat(Path,"DB");
	CreateDirectory(Path,NULL);

	strcpy(Path,PGPath);
	strcat(Path,"Report");
	CreateDirectory(Path,NULL);
	
	strcpy(Path,PGPath);
	strcat(Path,"Backup");
	CreateDirectory(Path,NULL);

	strcpy(Path,PGPath);
	strcat(Path,"NicData");
	CreateDirectory(Path,NULL);

	CheckDBSize();

	if (GetDogId() == false)
	{
	
		AfxMessageBox("很抱歉 ! 您與主機的網路連線發生問題 , 無法啟用 SpamDog !");
		EndDialog(0);
		return TRUE;
	
	}

	CheckLicenseValid();	
	ExecuteMailClient();

	SetTimer(1,1000 * 60,NULL);

	InitialValue();
	

	unsigned ThreadId;  
	_beginthreadex(NULL, 0, WriteWhoisInfo , (LPVOID) this, 0, &ThreadId);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainFrm::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainFrm::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMainFrm::DelTraceRule(int RuleId)
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
	   
	    
	   strcat(IniPath,"Rule.dat");
	  	
	   WIN32_FIND_DATA FindFileData;
	   HANDLE hFind;
	   bool FoundFile=false;

	   hFind = FindFirstFile(IniPath,&FindFileData);
 	   if (hFind != INVALID_HANDLE_VALUE) 
	   {
			FoundFile = true;
	   }

	   FindClose(hFind);

	   if (!FoundFile) return;

	   

	   FILE *fp = fopen(IniPath,"r+b");
	   

	   RuleFile m_RuleFile;
	   //RuleData m_RuleData;

	   if (fp != NULL)
	   {
		   fseek(fp,sizeof(RuleHeader),SEEK_SET);
		   while(fread(&m_RuleFile,1,sizeof(RuleFile),fp))
		   {
			
				 
				if (m_RuleFile.RuleId == RuleId)
				{
					m_RuleFile.Used = false;
					//移回
					int offset = ftell(fp)-sizeof(RuleFile)+FIELD_OFFSET(RuleFile,Used);
					fseek(fp,offset,SEEK_SET);
					fwrite(&m_RuleFile.Used,1,sizeof(m_RuleFile.Used),fp);
					break;
				}
			    
				
		   }
	   
	   }

	   if (fp != NULL)
	   fclose(fp);
}

bool CMainFrm::CheckReportMailExist()
{

	 bool Found = false;
	 WIN32_FIND_DATA FindFileData;
     HANDLE hFind;

	 char FindPath[_MAX_PATH];
	 strcpy(FindPath,PGPath);
	 strcat(FindPath,"Report//*.*"); 	 

	 hFind = FindFirstFile(FindPath,&FindFileData);
 	 if (hFind != INVALID_HANDLE_VALUE)
	 {
		 do
		 {			
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				Found = true;
				break;
			}		
		 
		 }while(FindNextFile(hFind, &FindFileData) != 0);

		 
		 FindClose(hFind);
		
	 }
	 else
	 {
		return false;
	 }

	  return Found;
}

void CMainFrm::CheckLicenseValid()
{

	//檢查 valid time 未失效則 turn on licensed func
	CTime t1((time_t) CTime::GetCurrentTime().GetTime());
	CTime t2((time_t) Settings.LicenseValidTime);

	CTimeSpan ts = t2-t1;

	if (ts.GetTotalSeconds() > 0)
	{
	
		TurnOnLicensedFunc();
	}
	else
	{
		TurnOffLicensedFunc();
	}
 
	if (CheckLicenseThread  == NULL)
	{
		unsigned ThreadId;  
		CheckLicenseThread = (HANDLE)_beginthreadex(NULL, 0, CheckLicense , (LPVOID) this, 0, &ThreadId);
	
	}

}
void CMainFrm::CheckDBSize()
{

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
	   strcat(IniPath,"DB//102db1.db");

	   WIN32_FIND_DATA FindFileData;
	   HANDLE hFind;

	   hFind = FindFirstFile(IniPath,&FindFileData);
 	   if (hFind != INVALID_HANDLE_VALUE) 
	   {

			FILE *fp = fopen(IniPath,"rb");
			fseek(fp,0,SEEK_END);

			int dbsize = ftell(fp);
			
			fclose(fp);

			if (dbsize > 1024 * 1024 * 1024)
			{
				
				AfxMessageBox("提醒您!您的資料庫大小已超過 1 GB , \n請重組您的資料庫 , 謝謝 !");				
			}
	   }

	   FindClose(hFind);
		   
	   

}

void CMainFrm::GetIniProf(char *prof,int size)
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

	   
	   GetPrivateProfileString("SysSetup","OutlookProf","",prof,size,IniPath);

	   //

	   
}

void CMainFrm::Getini(char statuscode)
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
	   char EXEPath[_MAX_PATH];	 

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");

	   strcpy(EXEPath,drive);
	   strcat(EXEPath,dir);	   
	    

//#ifdef _OUTLOOK
	   strcat(EXEPath,"LiveUpdate.exe");
//#endif
//#ifdef _OUTLOOKEX
//	   strcat(EXEPath,"LiveUpdateOE.exe");
//#endif

	   strcpy(PGPath,drive);
	   strcat(PGPath,dir);


    char MailPath[_MAX_PATH];
	strcpy(MailPath,PGPath);
	strcat(MailPath,"MailPath2.dat");

	GetMacAddress(Settings.MAC);
 
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

	//int CurrentVersion = GetPrivateProfileInt("LiveUpdate","CurrentVersion",0,IniPath);

	/*int updatekey = GetPrivateProfileInt("LiveUpdate","UpdateKey",0,IniPath);
 
	//Version 差異的 initial
	if (updatekey == 0)
	{
		DeleteFile(MailPath);
		WritePrivateProfileString("LiveUpdate","UpdateKey","1",IniPath); //last key : 1

	}*/



	

	hFind = FindFirstFile(MailPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{     
		FILE *fp = fopen(MailPath,"w+b");
		fclose(fp);
	}   

	FindClose(hFind);

    

	if (statuscode != 1) //1 須要下次起動更改
	{
	 Settings.APVersion = GetPrivateProfileInt("SysSetup","ApVersion",1,IniPath);
	}

	GetPrivateProfileString("SysSetup","DnsIp","168.95.1.1",Settings.DnsIp,sizeof(Settings.DnsIp),IniPath); 
	

	int SysOkMailColor = GetPrivateProfileInt("SysAlertColor","SysOkMailColor",16744448,IniPath);
	int SpamMailColor = GetPrivateProfileInt("SysAlertColor","SpamMailColor",32896,IniPath);
	int UnknownColor = GetPrivateProfileInt("SysAlertColor","UnknownColor",12632256,IniPath);
	int VirusColor = GetPrivateProfileInt("SysAlertColor","VirusColor",255,IniPath);
	int TextColor = GetPrivateProfileInt("SysAlertColor","TextColor",0,IniPath);

	if (Settings.APVersion == AP_OUTLOOK && g_MapiEx == NULL)
	{
		g_MapiEx = new CMapiEx();
		//g_MapiEx->SetStatic(&m_MainStatic);
		//g_MapiEx->SetParentObj((void *) this);

		char profstr[50];
		GetIniProf(profstr,sizeof(profstr));

		if (g_MapiEx->Login(2,profstr) == S_OK)
		{
			g_MapiEx->NotificationOn();
			
			m_MainStatic.SetWindowText("OutLook 監控中..");
		}
		else
		{	
			m_MainStatic.SetWindowText("啟動失敗..");
		}
	} else if (Settings.APVersion == AP_OUTLOOKEX && g_OEapi == NULL)
	{
  
		g_OEapi = new COEapi();
		HRESULT hr = g_OEapi->Login();
		hr = g_OEapi->NotificationOn();

		if (hr == S_OK)
		{
			m_MainStatic.SetWindowText("Express 監控中..");
		}
		else
		{
			m_MainStatic.SetWindowText("啟動失敗..");
		}
 
	}
	



	if (Settings.APVersion == AP_OUTLOOK)
	{
		if (g_MapiEx != NULL)
		{
			g_MapiEx->SysOkMailColor = SysOkMailColor;
			g_MapiEx->SpamMailColor = SpamMailColor;
			g_MapiEx->UnknownColor = UnknownColor;
			g_MapiEx->VirusColor = VirusColor;
			g_MapiEx->TextColor = TextColor;
		}
	}
	else if (Settings.APVersion == AP_OUTLOOKEX)
	{
	 
		if (g_OEapi != NULL)
		{
			g_OEapi->SysOkMailColor = SysOkMailColor;
			g_OEapi->SpamMailColor = SpamMailColor;
			g_OEapi->UnknownColor = UnknownColor;
			g_OEapi->VirusColor = VirusColor;
			g_OEapi->TextColor = TextColor;
		}
	}

	Settings.LicenseValidTime = GetPrivateProfileInt("SysSetup","LicenseTime",0,IniPath) ;
	//Settings.LicenseLastCheckTime = GetPrivateProfileInt("SysSetup","LicenseLastTime",0,IniPath);
	
	Settings.LastUpdateTime = GetPrivateProfileInt("LiveUpdate","LastUpdate",0,IniPath);
	if (GetPrivateProfileInt("SysSetup","IpSourceIdent",1,IniPath) == 1)
	{
		Settings.IpSourceIdent = true;
	}
	else
	{
		Settings.IpSourceIdent = false;
	}

	if (GetPrivateProfileInt("SysSetup","IpStrIdent",0,IniPath) == 1)
	{
		Settings.IpStrIdent = true;
	}
	else
	{
		Settings.IpStrIdent = false;
	}

	if (GetPrivateProfileInt("SysSetup","NeedReport",1,IniPath) == 1)
	{
		Settings.NeedReport = true;
	}
	else
	{
		Settings.NeedReport = false;
	}

	if (GetPrivateProfileInt("SysSetup","NoDogNews",0,IniPath) == 1)
	{
		Settings.NoDogNews = true;
	}
	else
	{
		Settings.NoDogNews = false;
	}

	/*if (GetPrivateProfileInt("SysSetup","ReportWithDel",0,IniPath) == 1)
	{
		Settings.ReportWithDel = true;
	}
	else
	{
		Settings.ReportWithDel = false;
	}*/


	/*if (Settings.LastUpdateTime == 0)
	{
	
	   
			CString Wndstr;
			Wndstr.Format("%d %d 2",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId());
			//送出 handle
			ShellExecute(AfxGetMainWnd()->m_hWnd,"open",EXEPath,Wndstr,"",SW_SHOWNOACTIVATE);
			CTime ct;
			ct = CTime::GetCurrentTime();
			long tvalue = (long) ct.GetTime();

			Settings.LastUpdateTime = tvalue;	  
	 
	
	}*/

	/*Settings.LastReportTime =  GetPrivateProfileInt("LiveUpdate","LastReport",0,IniPath);
	if (Settings.LastReportTime == 0)
	{
	
	    //if (ReporterThread == NULL)
		//{
			char EXEPath[_MAX_PATH];	 

			strcpy(EXEPath,drive);
			strcat(EXEPath,dir);	   
			strcat(EXEPath,"SpamdogReporter.exe");

			CString Wndstr;
			Wndstr.Format("%d %d",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId());
			//送出 handle
			ShellExecute(AfxGetMainWnd()->m_hWnd,"open",EXEPath,Wndstr,"",SW_HIDE);

			
			CTime ct;
			ct = CTime::GetCurrentTime();
			long tvalue = (long) ct.GetTime();

			Settings.LastUpdateTime = tvalue;	  
		//}
	
	}*/


	

	Settings.LastBackTime = GetPrivateProfileInt("SysSetup","LastBackup",0,IniPath);
	//if (Settings.LastUpdateTime == 0)
	//{
	
		//執行備份
	
	//}

	if (GetPrivateProfileInt("SysSetup","ShowTrashMsg",1,IniPath) == 1)	
	{
		Settings.ShowTrashMsg = true;
	}
	else
	{
		Settings.ShowTrashMsg = false;
	}


	Settings.MsgDelayTime = GetPrivateProfileInt("SysSetup","MsgDelayTime",3000,IniPath);
	Settings.MsgSpeed = GetPrivateProfileInt("SysSetup","MsgSpeed",200,IniPath);
	Settings.UnknownRate = (float) GetPrivateProfileInt("SysSetup","UnknownRate",5,IniPath) / 10;

	/*if (GetPrivateProfileInt("SysSetup","AllowNullContent",0,IniPath) == 1)	
	{
		Settings.AllowNullContent = true;
	}
	else
	{
		Settings.AllowNullContent = false;
	}*/

	if (GetPrivateProfileInt("SysSetup","AllowNullSender",0,IniPath) == 1)	
	{
		Settings.AllowNullSender = true;
	}
	else
	{
		Settings.AllowNullSender = false;
	}
	 
	if (GetPrivateProfileInt("SysSetup","AllowNullSubject",0,IniPath) == 1)	
	{
		Settings.AllowNullSubject = true;
	}
	else
	{
		Settings.AllowNullSubject = false;
	}
	 
	if (GetPrivateProfileInt("SysSetup","BootStart",0,IniPath) == 1)	
	{
		Settings.BootStart = true;

		//如果沒有則註冊
		
		HKEY RegKey;
		RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);
		 
		RegSetValueEx(RegKey,"SpamDog",0,REG_SZ,(const unsigned char *)path_buffer,strlen(path_buffer));
	  
		RegCloseKey(RegKey);



	}
	else
	{
		Settings.BootStart = false;

	
	}

	if (GetPrivateProfileInt("SysSetup","AutoUpdate",1,IniPath) == 1)	
	{
		Settings.AutoUpdate = true;
	}
	else
	{
		Settings.AutoUpdate = false;
	}

	if (GetPrivateProfileInt("SysSetup","AutoBackup",1,IniPath) == 1)	
	{
		Settings.AutoBackup = true;
	}
	else
	{
		Settings.AutoBackup = false;
	}

	if (GetPrivateProfileInt("SysSetup","SpamAddress",0,IniPath) == 1)	
	{
		Settings.SpamAddress = true;
	}
	else
	{
		Settings.SpamAddress = false;
	}

	if (GetPrivateProfileInt("SysSetup","SpamSubject",1,IniPath) == 1)	
	{
		Settings.SpamSubject = true;
	}
	else
	{
		Settings.SpamSubject = false;
	}

	//GetPrivateProfileString("SysSetup","IPident","",Settings.IPident,sizeof(Settings.IPident),IniPath);
	GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",Settings.ServerIp,sizeof(Settings.ServerIp),IniPath);
	GetPrivateProfileString("SysSetup","ArrangeMailFolder","[我的整理郵件]",Settings.ArrangeMailFolder,sizeof(Settings.ArrangeMailFolder),IniPath);
	GetPrivateProfileString("SysSetup","IpKey","",Settings.IpKeyStr,sizeof(Settings.IpKeyStr),IniPath);

	

	if (GetPrivateProfileInt("SysSetup","MailTracer",0,IniPath) == 1)	
	{
		Settings.MailTracer = true;
	}
	else
	{
		Settings.MailTracer = false;
	}

	 
	Settings.TracerExpire = GetPrivateProfileInt("SysSetup","TracerExpire",7,IniPath); // 預設 7 日 , 基本單位: 日

	if (GetPrivateProfileInt("SysSetup","BootWithAp",0,IniPath) == 1)	
	{
		Settings.BootWithAp = true;
	}
	else
	{
		Settings.BootWithAp = false;
	}

	if (GetPrivateProfileInt("SysSetup","ShowMsg",1,IniPath) == 1)	
	{
		Settings.ShowMsg = true;
	}
	else
	{
		Settings.ShowMsg = false;
	}

	Settings.BigMailSize = GetPrivateProfileInt("SysSetup","BigMailSize",1048576,IniPath);  //size limite at 1MB 

	if (GetPrivateProfileInt("SysSetup","SysAutoDel",0,IniPath) == 1)	
	{
		Settings.SysAutoDel = true;
	}
	else
	{
		Settings.SysAutoDel = false;
	}

	Settings.SysDelDay = GetPrivateProfileInt("SysSetup","SysDelDay",5,IniPath);


	if (GetPrivateProfileInt("SysSetup","AuthEx",0,IniPath) == 1)	
	{
		Settings.AuthEx = true;
	}
	else
	{
		Settings.AuthEx = false;
	}

	if (GetPrivateProfileInt("SysSetup","AutoDel",0,IniPath) == 1)	
	{
		Settings.AutoDel = true;
	}
	else
	{
		Settings.AutoDel = false;
	}

	if (GetPrivateProfileInt("SysSetup","AutoShowReport",1,IniPath) == 1)	
	{
		Settings.AutoShowReport = true;
	}
	else
	{
		Settings.AutoShowReport = false;
	}

	
	Settings.AutoShowReportFreq = GetPrivateProfileInt("SysSetup","AutoShowReportFreq",30,IniPath);

	//Settings.SysMailId = GetPrivateProfileInt("LiveUpdate","SysMailId",0,IniPath);

	//check rbl enable
	char tempstr[50];
	tempstr[0] = 0;
	GetPrivateProfileString("SysSetup","RBL1","rbl.softworking.com",tempstr,50,IniPath);

	if (tempstr[0] == 0)
	{
	
		Settings.bRBL = false;
	}
	else
	{
		Settings.bRBL = true;
	}
	

	

}

void CMainFrm::GetIniRule()
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
	   
	   //#ifdef _OUTLOOK
	   strcat(IniPath,"Rule.dat");
	   //#endif

	   //#ifdef _OUTLOOKEX
	   //strcat(IniPath,"RuleOE.dat");
	   //#endif

	   	if (Settings.APVersion == AP_OUTLOOK)
		{
	       if (g_MapiEx != NULL)
			g_MapiEx->m_RuleList.clear();
		}
		else if (Settings.APVersion == AP_OUTLOOKEX)
		{	   
		   if(g_OEapi != NULL)		   
			g_OEapi->m_RuleList.clear();
		 
		}

	   FILE *fp = fopen(IniPath,"rb");
	   

	   RuleFile m_RuleFile;
	   //RuleData m_RuleData;

	   if (fp != NULL)
	   {
		   fseek(fp,sizeof(RuleHeader),SEEK_SET);
		
		   while(fread(&m_RuleFile,1,sizeof(RuleFile),fp))
		   {
			
				/*
			   wchar_t *SenderRuleW = new wchar_t[strlen(m_RuleFile.Rule)+1];
								
			   int WideSize=MultiByteToWideChar(CP_ACP,0,m_RuleFile.Rule,strlen(m_RuleFile.Rule),SenderRuleW,strlen(m_RuleFile.Rule)+1);
			   SenderRuleW[WideSize] = 0;
			
			   wcscpy(m_RuleData.Rule,SenderRuleW);				
			   delete SenderRuleW;		    
			
			   m_RuleData.ShowColor = m_RuleFile.ShowColor;
			   */

			   if (m_RuleFile.Used) //有效鍵
			   {
					//檢查失效
				    if (m_RuleFile.ExpireTime != 0)
					{
						 CTime t1((time_t) CTime::GetCurrentTime().GetTime());
						 CTime t2((time_t) m_RuleFile.ExpireTime);
						 CTimeSpan ts = t1-t2;
						 if (ts.GetTotalSeconds() >= 0)
						 {
							 DelTraceRule(m_RuleFile.RuleId);
							 continue;						 
						 }

					}
				   
				    if (Settings.APVersion == AP_OUTLOOK)
					{
						if (g_MapiEx != NULL) 
							g_MapiEx->m_RuleList.insert(g_MapiEx->m_RuleList.end(),m_RuleFile);
					}
					else if (Settings.APVersion == AP_OUTLOOKEX)
					{
						if(g_OEapi != NULL)
						{
							g_OEapi->m_RuleList.insert(g_OEapi->m_RuleList.end(),m_RuleFile);
						}	
					}
			   }

										
			    
				
		   }
	   
	   }

	   if (fp != NULL)
	   fclose(fp);
 
	  
}

void CMainFrm::SendMailNow()
{

	 if (!ProcessExists("LiveUpdate.exe"))
	 {
		 	char Path[_MAX_PATH];
			strcpy(Path,PGPath);
 
			strcat(Path,"LiveUpdate.exe");		 
		
			CString Wndstr;
			
			Wndstr.Format("%d %d 1",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId()); //上傳 mail			
			ShellExecute(AfxGetMainWnd()->m_hWnd,"open",Path,Wndstr,"",SW_SHOWMINNOACTIVE); //SW_SHOWNOACTIVATE

	 }

}

void CMainFrm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
  if (Settings.AutoUpdate)
  {
	CTime t1((time_t) CTime::GetCurrentTime().GetTime());
	CTime t2((time_t) Settings.LastUpdateTime);
	//CTime t3((time_t) Settings.LastReportTime);

	CTimeSpan ts = t1-t2;

	if (Settings.LastProcessTime == 0 ) 
		 Settings.LastProcessTime = CTime::GetCurrentTime();

	CTimeSpan ts2 = t1 - Settings.LastProcessTime; //大於 60 sec 的閒滯 

	//CTimeSpan ts3 = t1 - t3;

	//if (GoUpdate && ReporterThread == NULL)
	//{
			//GoUpdate = false;
			//MsgFrm.ShowMsg(16744448,RGB(0,0,0),"\r\n資料更新完畢!",0);		
	//}
 

	if ((ts.GetTotalHours() >= 1 || NeedUpdateNow ) && ts2.GetTotalSeconds() >= 30)
	//if (ts.GetTotalMinutes() >= 5 && ts2.GetTotalSeconds() > 60)
	//if (ts.GetTotalHours() >= 1 )
	//if (ts2.GetTotalSeconds() > 60)
    {
		
	
		
		//AfxMessageBox("Start Update");
		if (!ProcessExists("LiveUpdate.exe"))
		{
			NeedUpdateNow = false;
			char Path[_MAX_PATH];
			strcpy(Path,PGPath);

 
			strcat(Path,"LiveUpdate.exe");

			//MsgFrm.ShowMsg(16744448,RGB(0,0,0),"\r\n資料更新中!",0);
			//GoUpdate = true;
		
			//DWORD ThreadId;
			//ReporterThread = CreateThread(NULL, 0, MailReporter , (LPVOID) this, 0, &ThreadId);
		
			CString Wndstr;
			Wndstr.Format("%d %d 2",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId()); //下載更新檔 , 不上傳
			//MsgFrm.ShowMsg(16744448,RGB(0,0,0),"連線起動更新中!",0);
			//送出 handle
			ShellExecute(AfxGetMainWnd()->m_hWnd,"open",Path,Wndstr,"",SW_SHOWMINNOACTIVE);

		

	    
			CTime ct;
			ct = CTime::GetCurrentTime();
			long tvalue = (long) ct.GetTime();

			//Settings.LastReportTime = tvalue;
			Settings.LastUpdateTime = tvalue;

			
		}

		

		 
			
	}
	
  }


  CTime tnow((time_t) CTime::GetCurrentTime().GetTime());
  if ( tnow.GetMinute() % 5 == 0)
  {
  
		 unsigned ThreadId;

	
		 if (Settings.SysAutoDel)
		 {		 
			if (Settings.APVersion ==  AP_OUTLOOKEX)
			{
				g_OEapi->DelSysFolderMail(Settings.SysDelDay);

			}
			else if (Settings.APVersion ==  AP_OUTLOOK)
			{
		 
				g_MapiEx->DelSysFolderMail(Settings.SysDelDay);
			}
		 }
	
		 if (SysMailThread == NULL)
		     SysMailThread = (HANDLE)_beginthreadex(NULL, 0, GetPmail , (LPVOID) this, 0, &ThreadId);

		if(Settings.Licensed  && Settings.bRBL)
		{
			if (Settings.APVersion ==  AP_OUTLOOKEX)
			{
				g_OEapi->ScanUnknownBox();

			}
			else if (Settings.APVersion ==  AP_OUTLOOK)
			{
		 
				g_MapiEx->ScanUnknownBox();
			}
		}

  
  
  }

		TrayWnd mTrayWnd;
		if (Settings.Licensed && StopCancelNewMailIcon && ClientPid != 0)
		{
			//等待 icon 消失
			if (GetAPTrayWnd(ClientPid,&mTrayWnd) != S_OK)
			{
				StopCancelNewMailIcon = false;
			}

		}

   //Check Server Donate //一天一次 確認到期時間 use thread check
   	CTime t_LicenseLastCheck((time_t) Settings.LicenseLastCheckTime);
	CTimeSpan ts = tnow-t_LicenseLastCheck;

	if ( ts.GetTotalHours() >= 1)
	{
	
			CheckLicenseValid();

		/*	CTime ct;
			ct = CTime::GetCurrentTime();
			long tvalue = (long) ct.GetTime();
			Settings.LicenseLastCheckTime = tvalue;

			//write ini last check time
			CString tmpstr;
			tmpstr.Format("%d",tvalue);

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
		 
			WritePrivateProfileString("SysSetup","LicenseLastTime",tmpstr,IniPath);*/


	
	}


	if ((!Settings.Licensed) || (Settings.Licensed && Settings.NeedReport))
	{
		if (Settings.LastReportTime == 0 ) 
		{
			 Settings.LastReportTime = CTime::GetCurrentTime();
		}
		else
		{
		
			CTimeSpan ts = tnow-Settings.LastReportTime ;

			if (Settings.AutoShowReport && ts.GetTotalMinutes() >= Settings.AutoShowReportFreq)
			{
				//show report frm
				if (CheckReportMailExist())
				{
					if (!mMailReportFrm.m_InModal)
					{

						KillTimer(nIDEvent);//避免時間太長相撞
						//if have report mail
						//CReportFrm mMailReportFrm(NULL);
						//CMailReportFrm mMailReportFrm;
						
						mMailReportFrm.AutoClose = true;
						int rc = mMailReportFrm.DoModal();

					
									
					
						if (rc == 20 || rc == 2)
						{
							 SendMailNow();	
							 
							 if (rc == 20)
							 {
						
								 if (Settings.APVersion == AP_OUTLOOK)
								 {
										g_MapiEx->EmptySpamFolder();
								 }
								 else if (Settings.APVersion == AP_OUTLOOKEX)
								 {
	  									g_OEapi->EmptySpamFolder();
								 }
				
				 				 ChangeTrashIcon(0);
							 }
						
						}

						SetTimer(1,1000 * 60,NULL);
					}


				}

				Settings.LastReportTime = CTime::GetCurrentTime();
			}
		}
	}




	if (!Settings.Licensed && Settings.ShowAD)
	{
		 
		//CTimeSpan ts = tnow-Settings.LastProcessTime;
	
		//if (ts.GetTotalSeconds() >= 60)
		//{
			Settings.ShowAD = false;
			if (Settings.ADText != 0)
			{
			   
			   ShowToolTip(Settings.ADTitle,Settings.ADText, 10 * 1000,1);		
			}

		//}

	}

	CDialog::OnTimer(nIDEvent);
}









afx_msg void CMainFrm::OnNotifyIcon(WPARAM wParam, LPARAM lParam) 
{ 
  
	

	if ((wParam == IDR_MAINFRAME || wParam == IDI_ICON1 ))
	{
		if (lParam == WM_LBUTTONDOWN) 
		{
			if (FirstStart)
			{
			
				RECT rWorkArea;
				CRect wrect;

				SystemParametersInfo(SPI_GETWORKAREA,  
						sizeof(RECT),
						&rWorkArea,
						0);    

				GetWindowRect(&wrect);

				SetWindowPos(NULL,rWorkArea.right - wrect.Width() ,rWorkArea.bottom - wrect.Height() ,wrect.Width(),wrect.Height(),SWP_SHOWWINDOW | SWP_NOZORDER);

				FirstStart = false;
			}
			
			ShowWindow(SW_RESTORE);
		} 
		else if (lParam == WM_RBUTTONUP) 
		{
			CMenu menu;
			
			if (!menu.LoadMenu(IDR_MENU1))
			return ;
			CMenu* pSubMenu = menu.GetSubMenu(0);
			
			if (!pSubMenu)
			return ;

			//SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);
			CPoint pos;
			GetCursorPos(&pos);		
	
			CWnd *mainWnd = AfxGetMainWnd();
			mainWnd->SetForegroundWindow();
			
			pSubMenu->TrackPopupMenu(TPM_RIGHTALIGN|TPM_LEFTBUTTON
				|TPM_RIGHTBUTTON, pos.x, pos.y, mainWnd, NULL);
		}
		else if (lParam == TTM_DELTOOL)//A || lParam == TTM_DELTOOLW)
		{
			//
			if (Settings.LastTipType == 1 && Settings.ADUrl[0] != 0 )
			{
				//ShellExecute(this->m_hWnd,"open", Settings.ADUrl, NULL, NULL, SW_SHOW );
				
				WindowsOpen(Settings.ADUrl);
			}
		}
	 
	}

} 


afx_msg void CMainFrm::OnNotifyIcon2(WPARAM wParam, LPARAM lParam) 
{ 
  
	if ((wParam == IDI_ICON2 ))
	{
		//if (lParam ==WM_RBUTTONDOWN) // WM_LBUTTONDOWN ) 
		//{
			//MessageBeep(MB_OK);
			//ShowWindow(SW_SHOWNORMAL);
			//OnMenuitem32772();

			/*if (Settings.APVersion == AP_OUTLOOK)
			{
				MsgFrm.ShowMsg(g_MapiEx->SysOkMailColor, g_MapiEx->TextColor,"\r\n清除垃圾郵件匣完成 !");
			}
			else if (Settings.APVersion == AP_OUTLOOKEX)
			{
	  			
				MsgFrm.ShowMsg(g_OEapi->SysOkMailColor, g_OEapi->TextColor,"\r\n清除垃圾郵件匣完成 !");
			}*/

			//MsgFrm.ShowMsg(16744448,RGB(0,0,0),"\r\n清除垃圾郵件匣完成 !",0);
		//} 
		//else 
		if (lParam == WM_LBUTTONDOWN) 
		{
			

			if((!Settings.Licensed && CheckReportMailExist() && !mMailReportFrm.m_InModal ) ||
				(Settings.Licensed  && Settings.NeedReport  && CheckReportMailExist() && !mMailReportFrm.m_InModal))
			//if (CheckReportMailExist())
			{
				//CMailReportFrm mMailReportFrm;
				//CReportFrm mMailReportFrm(NULL);
			 
				//if (!mMailReportFrm.m_InModal)
				//{

				//close ad show
				//if (!Settings.Licensed )
				//	Settings.ShowAD = true;

					mMailReportFrm.AutoClose = false;
					int rc = mMailReportFrm.DoModal();

					if (rc == 2 || rc == 20)
					{	//正常
						SendMailNow();
					}

					if (rc == 20)
					{
					
						if (Settings.APVersion == AP_OUTLOOK)
						{
						  g_MapiEx->EmptySpamFolder();
						} 
						else if (Settings.APVersion == AP_OUTLOOKEX)
						{
						  g_OEapi->EmptySpamFolder();
						}
						ChangeTrashIcon(0);
					
					}
				//}
			}
			else if (Settings.Licensed  && !Settings.NeedReport) //有贊助且不回報
			{
			
						if (Settings.APVersion == AP_OUTLOOK)
						{
						  g_MapiEx->EmptySpamFolder();
						} 
						else if (Settings.APVersion == AP_OUTLOOKEX)
						{
						  g_OEapi->EmptySpamFolder();
						}
						ChangeTrashIcon(0);
			}
			else if (!mMailReportFrm.m_InModal)
			{
			
				AfxMessageBox("目前回報的垃圾郵件數不足,請稍後再試,謝謝!");
			}
		}
	 
	}

} 

void CMainFrm::GetMacAddress(char *Mac)
{

	Mac[0] = 0;

	lpfnGetAdaptersInfo m_pGetAdaptersInfo;
	HMODULE hIpHlpApi;

	hIpHlpApi = LoadLibrary("iphlpapi.dll");

	//Finding function
	m_pGetAdaptersInfo = 
		(lpfnGetAdaptersInfo)GetProcAddress(hIpHlpApi,"GetAdaptersInfo" );

	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information			(*)
											 // for up to 16 NICs			(*)
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer	(*)

	DWORD dwStatus = m_pGetAdaptersInfo(      // Call GetAdapterInfo		(+)
	  AdapterInfo,                 // [out] buffer to receive data			(*)
	  &dwBufLen);                  // [in] size of receive data buffer		(*)
	
	if(!dwStatus == ERROR_SUCCESS)
	{
		return ;
	}
										// valid, no buffer overflow		(*)

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
												 // current adapter info
												// current adapter info
 
	if (!pAdapterInfo || pAdapterInfo->Address==NULL) return ;	

	 

		sprintf(Mac,"%x%x%x%x%x%x",
			    *(pAdapterInfo->Address),
				*(pAdapterInfo->Address+1),
				*(pAdapterInfo->Address+2),
				*(pAdapterInfo->Address+3),
				*(pAdapterInfo->Address+4),
				*(pAdapterInfo->Address+5)); 

	FreeLibrary(hIpHlpApi);



}

bool CMainFrm::GetDogId()
{
	bool DogIdOK = false;
	char inipath[_MAX_PATH];
	strcpy(inipath,PGPath);
	strcat(inipath,"/SpamDog.ini");
	char EXEPath[_MAX_PATH];
	strcpy(EXEPath,PGPath);
	strcat(EXEPath,"LiveUpdate.exe");

	DogId = GetPrivateProfileInt("SysSetup","DogId",0,inipath);

	if (DogId == 0)
	{
	
		m_MainStatic.SetWindowText("首次啟用中..");
		//連線取 ID
		CCoreSocket::Startup();

		CHTTPclient http; 
 
		
		//#ifdef _LOCALLAN
			HANDLE hand = http.IPGET(Settings.ServerIp,"www.softworking.com","/GetDogID.asp",60 * 1000, 60 * 1000);
		//#else
		//	HANDLE hand = http.IPGET("61.62.70.233","www.softworking.com","/GetDogID.asp",60 * 1000, 60 * 1000);
		//#endif
			

		DWORD rtn = WaitForSingleObject(hand,5);
		while (rtn != WAIT_OBJECT_0)
		{ 
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
		}
			
		
		FILE *fp = NULL;
		fp = fopen(http.GetTempFilePath() , "rb");

		int NewDogID=0;
		fscanf(fp,"%d\n",&NewDogID);

		if (NewDogID > 0)
		{ 

			char DogIDStr[25];
			itoa(NewDogID , DogIDStr , 10);

			WritePrivateProfileString("SysSetup","DogId",DogIDStr,inipath);

			DogIdOK = true;

		}
		
		fclose(fp);

		DeleteFile(http.GetTempFilePath());
	
		CCoreSocket::Cleanup();

		if (Settings.APVersion == AP_OUTLOOK)
		{
			m_MainStatic.SetWindowText("OutLook 監控中..");
		}
		else
		{
			m_MainStatic.SetWindowText("Express 監控中..");
		}

		CString Wndstr;
		Wndstr.Format("%d %d 2",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId());
		//送出 handle
		ShellExecute(AfxGetMainWnd()->m_hWnd,"open",EXEPath,Wndstr,"",SW_SHOWMINNOACTIVE);
		CTime ct;
		ct = CTime::GetCurrentTime();
		long tvalue = (long) ct.GetTime();

		Settings.LastUpdateTime = tvalue;	  
	}
	else
	{
		DogIdOK = true;
	}

	
	return DogIdOK;

}

void CMainFrm::GetClientProcessId()
{

	if (ClientPid == 0)
	{
		if (Settings.APVersion == AP_OUTLOOK)
		{
			ProcessExists("OUTLOOK.EXE",&ClientPid) ;
		}
		else if  (Settings.APVersion == AP_OUTLOOKEX)
		{
			ProcessExists("msimn.exe",&ClientPid) ;
		}
	}
}

void CMainFrm::ExecuteMailClient()
{

    //檢查是否執行
	if (!Settings.BootWithAp) return;

	if (Settings.APVersion == AP_OUTLOOK)
	{
	   if (ProcessExists("OUTLOOK.EXE")) return;
	}
	else if  (Settings.APVersion == AP_OUTLOOKEX)
	{
	   if (ProcessExists("msimn.exe")) return;
	}

	
	

	
	HKEY RegKey;
	
	if (Settings.APVersion == AP_OUTLOOK)
	{
	  RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\OUTLOOK.EXE",&RegKey);
	}
	else if  (Settings.APVersion == AP_OUTLOOKEX)
	{
	  RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msimn.exe",&RegKey);
	}
	
	DWORD dwType, dwLen;

	char *TempRegPath;
	TempRegPath = new char[MAX_PATH];
	dwLen = MAX_PATH;

	RegQueryValueEx(RegKey,"", NULL, &dwType, (unsigned char *) TempRegPath, &dwLen);	
	
	char ExePath[MAX_PATH];
    ExpandEnvironmentStrings(TempRegPath,ExePath,MAX_PATH);
	
	//AfxMessageBox(ExePath);
	ShellExecute(NULL,"open",ExePath,"","",SW_SHOWMINNOACTIVE);
	
	delete TempRegPath;
	RegCloseKey(RegKey);

	 

}

bool CMainFrm::ProcessExists(char *szExename, int* Pid)
{
  HANDLE hProcessSnap;  
  PROCESSENTRY32 pe32;  
  bool ret;
  
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
    return( FALSE );
  }

  pe32.dwSize = sizeof( PROCESSENTRY32 );
  if( !Process32First( hProcessSnap, &pe32 ) )
  {    
    CloseHandle( hProcessSnap );
    return( FALSE );
  }

  ret = false;
  int ilen = strlen(szExename);
  do
  {  
    // since pe32.szExeFile is MAX_PATH bytes, we can 
    // adjust the compare string starting position to use under 
    // win95/98/me (because the pe32.szExeFile is full name not 
    // only filename under win95/98/me)
    int ipos = strlen(pe32.szExeFile)-ilen;  
    if(0 == stricmp(szExename, pe32.szExeFile + ipos))
    {
       if (Pid != NULL)
		   *Pid = pe32.th32ProcessID;
	   ret = true;
       break;
    }
  } while( Process32Next( hProcessSnap, &pe32 ) );

  CloseHandle( hProcessSnap );
  return( ret );



}

void  CMainFrm::TurnOnLicensedFunc()
{

	if (!Settings.Licensed)
	{
	
	
		Settings.Licensed = true;
	}
}
void  CMainFrm::TurnOffLicensedFunc()
{
	if (Settings.Licensed)
	{
	
	
		Settings.Licensed = false;
	}
}

void CMainFrm::ChangeIcon(char Status)
{
    if (Status == 0 )
	{

		//tnd.uID=IDR_MAINFRAME;//IDR_MAINFRAME;
		tnd.uFlags =NIF_ICON;
		tnd.hIcon=m_hIcon;

		Shell_NotifyIcon(NIM_MODIFY,&tnd);
    } 
	else 
	{
		//tnd.uID=IDI_ICON1;//IDR_MAINFRAME;
		tnd.uFlags = NIF_ICON;
		tnd.hIcon= m_hIcon2;		
		Shell_NotifyIcon(NIM_MODIFY,&tnd);
	}
} 


void CMainFrm::ChangeTrashIcon(char Status)
{
    if (Status == 0 )
	{

		//tnd.uID=IDR_MAINFRAME;//IDR_MAINFRAME;
		tnd2.uFlags =NIF_ICON;
		tnd2.hIcon=m_hIcon3;

		Shell_NotifyIcon(NIM_MODIFY,&tnd2);
    } 
	else 
	{
		//tnd.uID=IDI_ICON1;//IDR_MAINFRAME;
		tnd2.uFlags = NIF_ICON;
		tnd2.hIcon= m_hIcon4;		
		Shell_NotifyIcon(NIM_MODIFY,&tnd2);
	}
} 




void CMainFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnOK();
	CDialog::OnClose();
}
void CMainFrm::OnOK()
{


}

void CMainFrm::OnCancel()
{

}

void CMainFrm::OnMenuitem32774() 
{
	// TODO: Add your command handler code here
		//DestroyWindow();

	OnCloseSpamDog(0,0);
}

void CMainFrm::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default

	if(nID==SC_MINIMIZE)
	{ 
	
		
		ShowWindow(SW_HIDE);	
	}
	else
	{
	CDialog::OnSysCommand(nID,lParam);
	}
}


void CMainFrm::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	//if(Settings.ReportWithDel && CheckReportMailExist() && !mMailReportFrm.m_InModal)
	//if((!Settings.Licensed && CheckReportMailExist() && !mMailReportFrm.m_InModal ) || (Settings.ReportWithDel  && CheckReportMailExist() && !mMailReportFrm.m_InModal))
	if((!Settings.Licensed && CheckReportMailExist() && !mMailReportFrm.m_InModal ) ||
				(Settings.Licensed  && Settings.NeedReport  && CheckReportMailExist() && !mMailReportFrm.m_InModal))
	{
		//CReportFrm mMailReportFrm(NULL);
		mMailReportFrm.AutoClose = false;
		int rc = mMailReportFrm.DoModal();

		if (rc == 2 || rc == 20)
		{	//正常
			SendMailNow();

			if (Settings.APVersion == AP_OUTLOOK)
			{
			  g_MapiEx->EmptySpamFolder();
			}
			else if (Settings.APVersion == AP_OUTLOOKEX)
			{
	  			g_OEapi->EmptySpamFolder();
			}
			
			ChangeTrashIcon(0);
		}		
		else
		{
			//report later 
			//return;
		}

	}
	else if (Settings.Licensed && !Settings.NeedReport)
	{
	
		 if (Settings.APVersion == AP_OUTLOOK)
		{
		  g_MapiEx->EmptySpamFolder();
		}
		else if (Settings.APVersion == AP_OUTLOOKEX)
		{
	  		g_OEapi->EmptySpamFolder();
		}
		
		ChangeTrashIcon(0);
	
	}
	else if (!mMailReportFrm.m_InModal)
	{
			
				AfxMessageBox("目前回報的垃圾郵件數不足,請稍後再試,謝謝!");
	}


 
   

	

 
}

void CMainFrm::OnButton2() 
{
	// TODO: Add your control notification handler code here
	 
	CSetupFrm setupfrm(NULL);
 
    int RtnCode = setupfrm.DoModal();
	if (RtnCode == 1)
	{

		Getini();
		GetIniRule();
	} else if  (RtnCode == 3)
	{
		Getini(1);//不改變 AP version
		GetIniRule();
		
		AfxMessageBox("收信環境改變,將在下一次啟動時生效\n請重新起動SpamDog !");
	}
}

void CMainFrm::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	//if (m_AutoCheck.GetCheck() == BST_CHECKED) 
	//{
		//MsgFrm.SetEnable(true);
	//}
	//else
	//{
		//MsgFrm.SetEnable(false);	
	//}
}

void CMainFrm::OnMenuitem32771() 
{
	// TODO: Add your command handler code here
	if (Settings.APVersion == AP_OUTLOOK)
	{
		
		g_MapiEx->ScanAllBox();
	} 
	else if (Settings.APVersion == AP_OUTLOOKEX)
	{
	    g_OEapi->ScanAllBox();
	}

 
}

void CMainFrm::OnMenuitem32772() 
{
	// TODO: Add your command handler code here


	if((!Settings.Licensed && CheckReportMailExist() && !mMailReportFrm.m_InModal ) ||
				(Settings.Licensed  && Settings.NeedReport  && CheckReportMailExist() && !mMailReportFrm.m_InModal))
	{
		//CReportFrm mMailReportFrm(NULL);
		mMailReportFrm.AutoClose = false;
		int rc = mMailReportFrm.DoModal();

		if (rc == 2 || rc == 20)
		{	//正常
			SendMailNow();

				if (Settings.APVersion == AP_OUTLOOK)
				{
				  g_MapiEx->EmptySpamFolder();
				} 
				else if (Settings.APVersion == AP_OUTLOOKEX)
				{
				  g_OEapi->EmptySpamFolder();
				}
				ChangeTrashIcon(0);
		}		
		else
		{
			//report later 
			//return;
		}

	}
	else if (Settings.Licensed  && !Settings.NeedReport)
	{
			if (Settings.APVersion == AP_OUTLOOK)
			{
			  g_MapiEx->EmptySpamFolder();
			} 
			else if (Settings.APVersion == AP_OUTLOOKEX)
			{
			  g_OEapi->EmptySpamFolder();
			}
			ChangeTrashIcon(0);
	
	
	}
	else if (!mMailReportFrm.m_InModal)
	{
			
				AfxMessageBox("目前回報的垃圾郵件數不足,請稍後再試,謝謝!");
	}



	/*if(Settings.Licensed && Settings.NeedReport)
	{
		
		SendMailNow();
	}*/



 
}

void CMainFrm::OnMenuitem32773() 
{
	// TODO: Add your command handler code here
	OnButton2();
}

afx_msg void CMainFrm::OnLicenseGoAd(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(Settings.ADStr);
	//WebFrm.m_Web.Navigate(_T("http://www.softworking.com"), NULL, NULL, NULL, NULL);

	//if (Settings.ADText != 0)
	//	ShowToolTip(Settings.ADTitle,Settings.ADText, 5 * 60 * 1000);

	//if (Settings.ADText != 0)
	//{
			
	//		ShowToolTip(Settings.ADTitle,Settings.ADText, 1 * 60 * 1000,1);
	//}
	if (Settings.ShowAD)
	{
	
			Settings.ShowAD = false;
			if (Settings.ADText != 0)
			{
			   
			   ShowToolTip(Settings.ADTitle,Settings.ADText, 10 * 1000,1);		
			}


	}else
	if (HandUpdate) 
	{
		HandUpdate = false;
		Settings.LastTipType = 0;
		ShowToolTip("SpamDog 訊息","手動更新完畢!",5 * 1000,0);
	}

	TurnOffLicensedFunc();
}
afx_msg void CMainFrm::OnLicenseMacError(WPARAM wParam, LPARAM lParam)
{

	AfxMessageBox("SpamDog 贊助版同時只能在一台電腦使用 !");
	TurnOffLicensedFunc();
} 

afx_msg void CMainFrm::OnLicenseLock(WPARAM wParam, LPARAM lParam)
{

	 if (g_OEapi != NULL)
	 {
		   delete g_OEapi;
		   g_OEapi = NULL;
	 }
	 
	 if (g_MapiEx != NULL)
	 {
		   delete g_MapiEx;
		   g_MapiEx = NULL;

	 }

	AfxMessageBox("您的 SpamDog 因特殊原因已被主機鎖住,目前無法使用,煩請與我們連絡,造成不便請多見諒,謝謝 !");
	TurnOffLicensedFunc();
	//DestroyWindow();
	OnCloseSpamDog(0,0);

}

afx_msg void CMainFrm::OnLicenseCheckLicensed(WPARAM wParam, LPARAM lParam)
{



	//AfxMessageBox("OnLicenseCheckLicensed");

//	if (!Settings.NoDogNews && !HandUpdate)
//	WebFrm.m_Web.Navigate(_T("http://www.softworking.com"), NULL, NULL, NULL, NULL);
	

	if (HandUpdate) 
	{
		HandUpdate = false;
		//AfxMessageBox("手動更新完畢!");
		Settings.LastTipType = 0;
		ShowToolTip("SpamDog 訊息","手動更新完畢!",5 * 1000,0);
	}
	//WebFrm.ShowWindow(SW_SHOW);

 
	TurnOnLicensedFunc();

	CTime t1((time_t) CTime::GetCurrentTime().GetTime());
	CTime t2((time_t) Settings.LicenseValidTime);

	CTimeSpan ts = t2-t1;

	if (ts.GetTotalHours() <= 24 )
	{
			//AfxMessageBox("提醒您! SpamDog 贊助功能即將到期!");
		ShowToolTip("SpamDog 提醒","提醒您! SpamDog 贊助功能版即將到期!", 5 * 1000,1);

	}

} 
afx_msg void CMainFrm::OnLicenseConnectError(WPARAM wParam, LPARAM lParam)
{

	
	ShowToolTip("SpamDog 訊息","SpamDog 連線主機發生問題 , 將關閉贊助功能 \n請先確定連線後手動更新或與我們連絡",20 * 1000,0);
	//AfxMessageBox("SpamDog 連線主機發生問題 , 將關閉贊助功能 \n請先確定連線後手動更新或與我們連絡");
	TurnOffLicensedFunc();
} 

afx_msg void CMainFrm::OnCloseSpamDog(WPARAM wParam, LPARAM lParam)
{
	//if (GoUpdate)
	//{
	
		//if (MessageBox("連線更新中 , 是否立即中斷連線?" , NULL,MB_OKCANCEL) == IDOK)
		//{
		  //DestroyWindow();
		//}
	
//	}
	//else
	//{
		 DestroyWindow();
	//}
	
}



void CMainFrm::OnButton3() 
{
	// TODO: Add your control notification handler code here
	//DestroyWindow();
	OnCloseSpamDog(0,0);
}

void CMainFrm::OnButton4() 
{
	// TODO: Add your control notification handler code here
	if (Settings.APVersion == AP_OUTLOOK)
	{		
		g_MapiEx->ScanAllBox();
	} 
	else if (Settings.APVersion == AP_OUTLOOKEX)
	{
	    g_OEapi->ScanAllBox();
	}
}

void CMainFrm::OnButton5() 
{
 
	//ShowToolTip("SpamDog 提醒","要記得贊助喔<A>link</A>",60 * 1000 );



	if (Settings.APVersion == AP_OUTLOOK)
	{	
	   g_MapiEx->ArrangeMail(NULL,NULL,NULL,NULL);	   

	}
	else if (Settings.APVersion == AP_OUTLOOKEX)
	{
	   g_OEapi->ArrangeMail(NULL,NULL,0);	  
	}

/*

		    

			//MsgFrm.ShowMsg(16744448,RGB(0,0,0),"\r\n資料更新中!",0);
			//GoUpdate = true;
		
			//DWORD ThreadId;
			//ReporterThread = CreateThread(NULL, 0, MailReporter , (LPVOID) this, 0, &ThreadId);
		
			CString Wndstr;
			Wndstr.Format("%d %d 2",AfxGetMainWnd()->m_hWnd,GetCurrentProcessId()); //下載更新檔 , 不上傳
			//MsgFrm.ShowMsg(16744448,RGB(0,0,0),"連線起動更新中!",0);
			//送出 handle
			ShellExecute(AfxGetMainWnd()->m_hWnd,"open","D:\\MB\\SpamDog\\LiveUpdate.exe",Wndstr,"",SW_SHOWMINNOACTIVE);

 */
	
}
/*
void CMainFrm::GetSysMsg()
{

	 CCoreSocket::Startup();

 
	
	 char IniPath[_MAX_PATH];
	 strcpy(IniPath,PGPath);
	 strcat(IniPath,"SpamDog.ini");

 
  	 char DogIdStr[255];
	 itoa(DogId,DogIdStr,10);

	 
	 int lastver = 0;
	 lastver = GetPrivateProfileInt("LiveUpdate","SysMailId",0,IniPath);

	 char GetVerPath[_MAX_PATH];
	 strcpy(GetVerPath , "/GetSysMail.asp?ver=");
	 char verstr[25];
	 itoa(lastver,verstr,10);
	 strcat(GetVerPath , verstr);
	 strcat(GetVerPath , "&DogId=");
	 strcat(GetVerPath , DogIdStr);


	 #ifdef _LOCALLAN
		char *ServerIP = "192.168.1.2";
	 #else
		char *ServerIP = "61.62.70.233";
	 #endif
	 
	 CHTTPclient http; 
	 HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",GetVerPath,60 * 1000, 60 * 1000);

	 DWORD rtn = WaitForSingleObject(hand,5);

	    while (rtn != WAIT_OBJECT_0)
		{
			  
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
		} 
	 
     

	   
	   FILE *fp = fopen(http.GetTempFilePath(),"rb");
	   if (fp != NULL)
	   {
		    int ServerVer=0;
			int UpdateItem=0;
			char itemstr[255];

			fscanf(fp,"%d\n",&ServerVer); //取版本

			if (ServerVer > lastver )
			{
				fscanf(fp,"%d\n",&UpdateItem); //取數目
				bool TransOK = false;

				for (int i= 0 ; i < UpdateItem ; i++)
				{				
					fscanf(fp,"%s\n",itemstr);				
				

					string srvpath;
					srvpath = "//SysMail//";					 
					srvpath += itemstr; 
					 
					CHTTPclient http; 
					 
				    HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",(char *) srvpath.c_str(),60 * 1000, 60 * 1000);
				    DWORD rtn = WaitForSingleObject(hand,5);			
				 
					
					while (rtn != WAIT_OBJECT_0)
					{
					 
						//處理 waitting msg dump
						MSG msg;
						while (::PeekMessage(&msg, NULL,   // pump message until none
								NULL, NULL, PM_REMOVE))   // are left in the queue
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}

						rtn = WaitForSingleObject(hand,5);
					}
 

					if (http.DoTransSucceed())
					{
					    
						TransOK = true;	 

						FILE *fp = fopen(http.GetTempFilePath(),"rb");
						if (fp != NULL)
						{
						
						 
									ZipHeader zipheader;
									fread(&zipheader,1,sizeof(zipheader),fp);

									char* DataBuffer = new char[zipheader.OrgLen];
		  
									int BufferSize=0;
									
									if (zipheader.isCompress)
									{	 
						    
										CCompress pzip;			

										char *DataTemp = new char[zipheader.ThisLen];
										int readsize = fread(DataTemp,1,zipheader.ThisLen,fp);

										if (readsize  == zipheader.ThisLen)
										{
											pzip.DeCompress((unsigned char *) DataTemp,zipheader.ThisLen,(unsigned char *)DataBuffer,&zipheader.OrgLen);
				  
										}

										BufferSize = zipheader.OrgLen;

										
										delete DataTemp;

				
									}
									else
									{

											int readsize = fread(DataBuffer,1,zipheader.OrgLen,fp);
											if (readsize  != zipheader.ThisLen)
											{
												//DataBuffer[0] = 0 ;
											}
											else
											{
												BufferSize = readsize;
					 
											}
		 
									}

									if (BufferSize > 0)
									{
										if (Settings.APVersion == AP_OUTLOOKEX)
										{
												g_OEapi->CreateSysMsg(DataBuffer,BufferSize);
										
										}
											
									}

									delete DataBuffer;
							 

							fclose(fp);
						
						}
					}

					DeleteFile(http.GetTempFilePath());
				 

					 
					
				}

				 
				 
				//更新 ini 版本
			 
				if (TransOK)
				{
					char VerStr[25];
					itoa(ServerVer , VerStr , 10);

					WritePrivateProfileString("LiveUpdate","SysMailId",VerStr,IniPath);
				}

			}

			fclose(fp);
	   }

	   DeleteFile(http.GetTempFilePath()); 
	   CCoreSocket::Cleanup();


}*/



unsigned __stdcall  GetPmail(LPVOID lpParam)
{
 
	 CCoreSocket::Startup();

	

	 CMainFrm  *mainfrm=NULL; 
	 mainfrm = (CMainFrm *)lpParam;

 
	
	 char IniPath[_MAX_PATH];
	 strcpy(IniPath,mainfrm->PGPath);
	 strcat(IniPath,"SpamDog.ini");

	 int DogId = mainfrm->DogId;
  	 char DogIdStr[255];
	 itoa(DogId,DogIdStr,10);

	 
	 //int lastver = mainfrm->Settings.SysMailId;
	// int lastver = GetPrivateProfileInt("LiveUpdate","SysMailId",0,IniPath);

	 

	 char GetVerPath[_MAX_PATH];
	 strcpy(GetVerPath , "/GetPmail2.asp?");
	 strcat(GetVerPath , "DogId=");
	 strcat(GetVerPath , DogIdStr);


	 char ServerIP[16];

	 strcpy(ServerIP,mainfrm->Settings.ServerIp);
	 /*
	 #ifdef _LOCALLAN	 
		strcpy(ServerIP,"192.168.1.2");
	 #else
	 
		strcpy(ServerIP,"61.62.70.233");
	 #endif*/
	 
	 CHTTPclient http; 
	 HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",GetVerPath,60 * 1000, 60 * 1000);

	 DWORD rtn = WaitForSingleObject(hand,5);

	    while (rtn != WAIT_OBJECT_0)
		{
			  
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
		} 
	 
     

	   
	   FILE *fp = fopen(http.GetTempFilePath(),"rb");
	   if (fp != NULL)
	   {
		    
		    
			int UpdateItem=0;
			unsigned int OrgLen = 0;
			unsigned int ThisLen = 0;
			//char itemstr[255];
			int IsCompress = 0;

			 
			fscanf(fp,"%d\n",&UpdateItem); //取數目

			for (int i= 0 ; i < UpdateItem ; i++)
			{				
				fscanf(fp,"%d\n",&IsCompress); //是否壓縮
				fscanf(fp,"%d\n",&OrgLen); 	//讀原始大小
				fscanf(fp,"%d\n",&ThisLen); 	//讀資料大小
				
				char *buff = new char[ThisLen+1];
				memset(buff,0,ThisLen+1);

				fread(buff,1,ThisLen,fp);
				char tmp[2];
				fread(tmp,1,2,fp); //skip /r/n

				//for (int p = 0 ; p < 
				//fscanf(fp,"%dc\n",10,buff); //讀內容
				

				char *Decbuff = NULL;
				char *De64Buffer = NULL;

				unsigned int DecBuffSize = 0;

				CMailCodec MailCodec;
			    MailCodec.Base64Decode2(buff,&De64Buffer,&DecBuffSize);

			    delete buff;

				
				
				

				if (IsCompress == 1)
				{				
				  Decbuff = new char[OrgLen+1];
				  memset(Decbuff,0,OrgLen+1);

				  CCompress pzip;
				  
				  pzip.DeCompress((unsigned char *) De64Buffer,DecBuffSize,(unsigned char *)Decbuff,&OrgLen);  

				  MailCodec.FreeBase64Decode(De64Buffer);

				   
				  
				} 
				else
				{
				  Decbuff = De64Buffer;
				}

									if (OrgLen > 0)
									{
										//DataBuffer[BufferSize] = 0;
											char *Subject = NULL;
											char *FromEmail = NULL;
											int HeaderEndPos = 0;

											//解 header 
											for(int i = 0 ; i < OrgLen ; i ++)
											{
												if (strnicmp(Decbuff + i , "subject: ",9) == 0 )
												{
													i = i + 8;
													int SavePos = i;
													while (i < OrgLen)
													{
														if (Decbuff[i] == char(13))
														{
															if (i-SavePos > 0)
															{
																Subject = new char[i-SavePos+1];
																memset(Subject,0,i-SavePos+1);
																strncpy(Subject,Decbuff+SavePos,i-SavePos);
															}

															i --;
															break;
														}
														else
														{
														   i++;
														}

													}
												} 
												else if (strnicmp(Decbuff + i , "from: ",6) == 0 )
												{
													//先不猜解 name <aaa@email.com>
													i = i + 5;
													int SavePos = i;
													while (i < OrgLen)
													{
														if (Decbuff[i] == char(13))
														{
															if (i-SavePos > 0)
															{
																FromEmail = new char[i-SavePos+1];
																memset(FromEmail,0,i-SavePos+1);
																strncpy(FromEmail,Decbuff+SavePos,i-SavePos);
															}

															int CutStart = 0;
															int CutEnd = 0;

															for (int c = 0 ; c < i-SavePos ; c++)
															{
															
																if (FromEmail[c] == '<')
																{
																	CutStart = c + 1;
																}
																else if (CutStart > 0 && FromEmail[c] == '>')
																{
																	if (c - CutStart > 0)
																	{
																		strncpy(FromEmail,FromEmail + CutStart , c - CutStart);
																		FromEmail[c - CutStart] = 0;
																	}
																	break;
																}
															}

															i --;
															break;
														}
														else
														{
														   i++;
														}

													}

												} else if (Decbuff[i] == char(13) && 
													       Decbuff[i+1] == char(10) &&
														   Decbuff[i+2] == char(13) &&
														   Decbuff[i+3] == char(10) )
														  // DataBuffer[i+4] == char(13) &&
														   //DataBuffer[i+5] == char(10))
												{
												
													HeaderEndPos = i + 4;
													break;
												
												}

											}
										
										

										if (mainfrm->Settings.APVersion == AP_OUTLOOKEX)
										{
											
											 g_OEapi->CreateSysMsg(Decbuff,OrgLen);
										
										}
										else if (mainfrm->Settings.APVersion == AP_OUTLOOK)
										{
										
											if (HeaderEndPos > 0)
											{
												char *headerbuff = new char[HeaderEndPos+1];
												headerbuff[HeaderEndPos] = 0;

												memcpy(headerbuff,Decbuff,HeaderEndPos);
												g_MapiEx->CreateSysMsg(headerbuff,Decbuff+HeaderEndPos,FromEmail,Subject);

												delete [] headerbuff;
											
											}
											
											

											
										}

										CString sendstr;
										sendstr = "發現系統訊息 \r\n\r\n主旨: ";
										sendstr += Subject;

										if (mainfrm->Settings.APVersion == AP_OUTLOOK)
										{
											mainfrm->MsgFrm.ShowMsg(g_MapiEx->SysOkMailColor, g_MapiEx->TextColor,sendstr.GetBuffer(sendstr.GetLength()));
										}
										else if (mainfrm->Settings.APVersion == AP_OUTLOOKEX)
										{
											mainfrm->MsgFrm.ShowMsg(g_OEapi->SysOkMailColor, g_OEapi->TextColor,sendstr.GetBuffer(sendstr.GetLength()));
										}	

										if (Subject != NULL) delete Subject;
										if (FromEmail != NULL) delete FromEmail ;
									}

									if (IsCompress == 1)
									{
										delete Decbuff;
									}
									else
									{
										MailCodec.FreeBase64Decode(Decbuff);
									}
		
			 
			}

		

			fclose(fp);
	   }

	 
	   DeleteFile(http.GetTempFilePath());
	   CCoreSocket::Cleanup(); 
 

	   

	   CloseHandle(mainfrm->SysMailThread);
	   mainfrm->SysMailThread = NULL;

	   


	   _endthreadex( 0 );
	    return 0;

}



unsigned __stdcall  CheckLicense(LPVOID lpParam)
{
 
	 CCoreSocket::Startup();

	 CMainFrm  *mainfrm=NULL; 
	 mainfrm = (CMainFrm *)lpParam; 

	 CTime ct;
	 ct = CTime::GetCurrentTime();
	 long tvalue = (long) ct.GetTime();
	 mainfrm->Settings.LicenseLastCheckTime = tvalue; 
	 
	 char IniPath[_MAX_PATH];
	 strcpy(IniPath,mainfrm->PGPath);
	 strcat(IniPath,"SpamDog.ini");
	 /*CString tmpstr;
	 tmpstr.Format("%d",tvalue);
	
	

	 WritePrivateProfileString("SysSetup","LicenseLastTime",tmpstr,IniPath);*/

	 int DogId = mainfrm->DogId;
  	 char DogIdStr[255];
	 itoa(DogId,DogIdStr,10);

	 
	 //int lastver = mainfrm->Settings.SysMailId;
	// int lastver = GetPrivateProfileInt("LiveUpdate","SysMailId",0,IniPath);
	 char GetVerPath[_MAX_PATH];
	 strcpy(GetVerPath , "/CheckLicense.asp?");
	 strcat(GetVerPath , "Ver=2&DogId=");
	 strcat(GetVerPath , DogIdStr);
	 strcat(GetVerPath , "&M=");
	 strcat(GetVerPath , mainfrm->Settings.MAC);

	 char ServerIP[16];

	 strcpy(ServerIP,mainfrm->Settings.ServerIp);
	 /*
	 #ifdef _LOCALLAN	 
		strcpy(ServerIP,"192.168.1.2");
	 #else
	 
		strcpy(ServerIP,"61.62.70.233");
	 #endif*/
	 
	 CHTTPclient http; 
	 HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",GetVerPath,60 * 1000, 60 * 1000);

	 DWORD rtn = WaitForSingleObject(hand,5);

	    while (rtn != WAIT_OBJECT_0)
		{
			  
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
		} 

	   FILE *fp = fopen(http.GetTempFilePath(),"rb");
	   if (fp != NULL)
	   {
			 
				int CheckCode = 0;		   
		   
				int status = 0;
				fscanf(fp,"%d\n",&status); //取status

				char ValidDateStr[255];
				double ValidDate = 0;
				fscanf(fp,"%s\n",&ValidDateStr); //取ValidDate				 

				fscanf(fp,"%s\n",&mainfrm->Settings.ADTitle); //ADTitle
				fscanf(fp,"%s\n",&mainfrm->Settings.ADText); //ADText
				fscanf(fp,"%s\n",&mainfrm->Settings.ADUrl); //ADUrl

				fscanf(fp,"%d\n",&CheckCode); //取CheckCode

				if (CheckCode == 991)
				{

					ValidDate = atof(ValidDateStr);

					if (ValidDate > 0)
					{
						ValidDate -= 25569;
						ValidDate *= 86400;
					}
		
					mainfrm->Settings.LicenseValidTime = (long) ValidDate;

						//write to ini valid date			
					char tmpstr[255];
					itoa(ValidDate  ,tmpstr,10);
					WritePrivateProfileString("SysSetup","LicenseTime",tmpstr,IniPath);

					if (status == 0)
					{
						//Send AD msg
						SendMessage(mainfrm->m_hWnd,WM_LICENSE_GO_AD ,0,0);
					}
					else if (status == 1)
					{
						//check ok   
						SendMessage(mainfrm->m_hWnd,WM_LICENSE_CHECK_LICENSED  ,0,0);
					
					}
					else if (status == -1)
					{
					   //Mac Error
						char tmpstr[1];
						tmpstr[0] = 0;
						WritePrivateProfileString("SysSetup","LicenseTime",tmpstr,IniPath);	

						SendMessage(mainfrm->m_hWnd,WM_LICENSE_MAC_ERROR   ,0,0);
					}
					else if (status == -999)
					{
					
						char tmpstr[1];
						tmpstr[0] = 0;
						WritePrivateProfileString("SysSetup","LicenseTime",tmpstr,IniPath);	

					

						//Lock Client
						SendMessage(mainfrm->m_hWnd,WM_LICENSE_LOCK   ,0,0);
					}
				} else if (CheckCode == 992)
				{
					//System Fix
					//Nothing To Do
				
				}
				else
				{
						char tmpstr[1];
						tmpstr[0] = 0;
						WritePrivateProfileString("SysSetup","LicenseTime",tmpstr,IniPath);	

						mainfrm->Settings.ADTitle[0] = 0;
						mainfrm->Settings.ADText[0] = 0;


						SendMessage(mainfrm->m_hWnd,WM_LICENSE_CONNECT_ERROR    ,0,0);		   
				}

			
		  

		   fclose(fp);
	   }
	   else
	   {
				//Connect False
		        //reset Valid Date
		   char tmpstr[1];
		   tmpstr[0] = 0;
		   WritePrivateProfileString("SysSetup","LicenseTime",tmpstr,IniPath);
		   SendMessage(mainfrm->m_hWnd,WM_LICENSE_CONNECT_ERROR    ,0,0);
	   
	   }

	 
	   DeleteFile(http.GetTempFilePath());
	   CCoreSocket::Cleanup(); 
 

	   

	   CloseHandle(mainfrm->CheckLicenseThread);
	   mainfrm->CheckLicenseThread = NULL;

	   


	   _endthreadex( 0 );
	    return 0;

}


unsigned __stdcall  WriteWhoisInfo(LPVOID lpParam)
{
 
	  // CloseHandle(GetCurrentThread());
	  // _endthreadex( 0 );
	  //  return 0;
		
	   //char IP[16];
	   //strcpy(IP,(char *)lpParam);

	   //delete lpParam;
	   //HANDLE pHeap = GetProcessHeap();
	   //CHeap::FreeHeap(lpParam,&pHeap);

		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT]; 

       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   	char RangeDBFile[_MAX_PATH];
	    char RangeDBData[_MAX_PATH];

		strcpy(RangeDBFile,drive);
		strcat(RangeDBFile,dir);	 	

		strcpy(RangeDBData,drive);
		strcat(RangeDBData,dir);

	

		strcat(RangeDBFile,"DB//Contact.db");
		strcat(RangeDBData,"DB//ContactData.db");





  while(1)
  {

	 

	  Sleep(10 * 1000);
		  
	  CMainFrm *mainfrm = (CMainFrm *) AfxGetMainWnd();
	  // EnterCriticalSection(&mainfrm->m_CritSec);
	   

	   	CCoreSocket::Startup();

	//RebuldDB	
	//EnterCriticalSection(&mainfrm->m_RangeDbSec);

	 CTreeRangeDB *ContactDB;
	 ContactDB = new CTreeRangeDB(&mainfrm->m_RangeDbSec,RangeDBFile,RangeDBData);
	 ContactDB->OpenDB();
	 ContactDB->RebuildDB(60 * 60 * 24 * 30); //one month
	 ContactDB->CloseDB();
	 delete ContactDB;

	// LeaveCriticalSection(&mainfrm->m_RangeDbSec);

	


		char NicDataPath[_MAX_PATH];
		

		strcpy(NicDataPath,drive);
		strcat(NicDataPath,dir);	 	
		strcat(NicDataPath,"NicData\\*.nic");
 
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
	//check contact db
    hFind = FindFirstFile(NicDataPath, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{		 
	
			char NicFile[_MAX_PATH];
			strcpy(NicFile,drive);
			strcat(NicFile,dir);
			strcat(NicFile,"NicData\\");
			strcat(NicFile,FindFileData.cFileName);

			FILE *fp = NULL;
			fp = fopen(NicFile,"r+b");
			if (fp == NULL) 
			{
				//DeleteFile(FindFileData.cFileName);
				continue;
			}

			char IP[16];
			IP[15] = 0;

			fscanf(fp,"%16s\r\n",IP);
			fclose(fp);

				CSpamContact sc;
				sc.SetIp(IP);

				//EnterCriticalSection(&mainfrm->m_RangeDbSec);

				CTreeRangeDB *ContactDB;
				ContactDB = new CTreeRangeDB(&mainfrm->m_RangeDbSec,RangeDBFile,RangeDBData);
				ContactDB->OpenDB();
				int DataFilePos = ContactDB->Search(sc.GetIPValue(),ContactDB->FirstRecPos);
				if ( DataFilePos == -1)
				{
				
					//找不到 db

					//先 release db
					ContactDB->CloseDB();
					delete ContactDB;				

					//LeaveCriticalSection(&mainfrm->m_RangeDbSec);


					char InetNumS[16];
					char InetNumE[16];
					char netname[255];
					char country[3];
					char contact[255];

					memset(&contact,0,sizeof(contact));
					memset(&netname,0,sizeof(netname));
					memset(&country,0,sizeof(country));
					memset(&InetNumS,0,sizeof(InetNumS));
					memset(&InetNumE,0,sizeof(InetNumE));

					
					 
					sc.GetContact(InetNumS,InetNumE,netname,country,contact);
					
					if (strstr(contact,"abuse") == NULL &&
					   strstr(contact,"spam") == NULL)
					{
						strcpy(contact,sc.GetSpamCop());		
					}	

					if ( contact[0] != 0 && InetNumS[0] != 0 && InetNumE[0] != 0)
					{
						/*CString str;
						str.Format("SpamMail Info \r\n\r\nNetName: %s,country: %s,Contact: %s",netname,country,contact);
						//manfrm->ShowToolTip("Spam Mail Info",str.GetBuffer(str.GetLength()),10 * 1000);
						 

						if (mainfrm->Settings.APVersion == AP_OUTLOOK)
						{
								mainfrm->MsgFrm.ShowMsg(g_MapiEx->SysOkMailColor, g_MapiEx->TextColor,str.GetBuffer(str.GetLength()));
						}
						else if (mainfrm->Settings.APVersion == AP_OUTLOOKEX)
						{
								mainfrm->MsgFrm.ShowMsg(g_OEapi->SysOkMailColor, g_OEapi->TextColor,str.GetBuffer(str.GetLength()));
						}	

						str.ReleaseBuffer();*/
						
						Contact ct;
						memset(&ct,0,sizeof(Contact));

						strcpy(ct.country,country);
						strcpy(ct.Email,contact);
						strcpy(ct.netname,netname);

						//EnterCriticalSection(&mainfrm->m_RangeDbSec);

						ContactDB = new CTreeRangeDB(&mainfrm->m_RangeDbSec,RangeDBFile,RangeDBData);
						ContactDB->OpenDB();
						int DataFilePos =	ContactDB->InsertData((char *) &ct,sizeof(ct));
						if (DataFilePos >= 0)
						{
						
							CSpamContact m_sc1;	
							m_sc1.SetIp(InetNumS);

							CSpamContact m_sc2;	
							m_sc2.SetIp(InetNumE);

							unsigned int IPs = m_sc1.GetIPValue();
							unsigned int IPe = m_sc2.GetIPValue();

							ContactDB->InserKey(IPs,IPe,ContactDB->FirstRecPos,DataFilePos);
							ContactDB->CloseDB();
							delete ContactDB;
							//LeaveCriticalSection(&mainfrm->m_RangeDbSec);

							//Prepare Send Data to Server
								char SendStr[1024 * 5];
								char IPss[255];
								char IPse[255];

							 
								ultoa(IPs,IPss,10);
								ultoa(IPe,IPse,10);
								

								strcpy(SendStr,"IPs=");
								strcat(SendStr,IPss);
								strcat(SendStr,"&IPe=");
								strcat(SendStr,IPse);
								strcat(SendStr,"&NetName=");
								strcat(SendStr,netname);
								strcat(SendStr,"&Country=");
								strcat(SendStr,country);
								strcat(SendStr,"&Contact=");
								strcat(SendStr,contact);

								CHTTPclient http;
								HANDLE hand = http.IPPOST(mainfrm->Settings.ServerIp,"www.softworking.com","/ReportNic.asp",SendStr,5* 60 * 1000, 5 * 60 * 1000);

								DWORD rtn = WaitForSingleObject(hand, 30000); 

								DeleteFile(http.GetTempFilePath());
				 
						}
						else
						{
							ContactDB->CloseDB();
							delete ContactDB;
							//LeaveCriticalSection(&mainfrm->m_RangeDbSec);
						}

						

					
					}
						
				}
				else
				{
					ContactDB->CloseDB();
					delete ContactDB;
					//LeaveCriticalSection(&mainfrm->m_RangeDbSec);
				}
			 
				DeleteFile(NicFile);

		} while (FindNextFile(hFind, &FindFileData) != 0) ;
    
      
		FindClose(hFind);		
		
	}
	

	 
	CCoreSocket::Cleanup();
	//LeaveCriticalSection(&mainfrm->m_CritSec);  
	

  }
 
	   CloseHandle(GetCurrentThread());


	   _endthreadex( 0 );
	    return 0;

}
/*
DWORD WINAPI DelSysMail(LPVOID lpParam)
{

	CMainFrm  *mainfrm=NULL; 
	mainfrm = (CMainFrm *)lpParam;

    if (mainfrm->Settings.APVersion ==  AP_OUTLOOKEX)
	{
			g_OEapi->DelSysFolderMail();
	}

    
	
	
	CloseHandle(mainfrm->DelSysMailThread);
	mainfrm->DelSysMailThread = NULL;

	ExitThread(0);
	return 0;

}


DWORD WINAPI DelTrashMail(LPVOID lpParam)
{

	CMainFrm  *mainfrm=NULL; 
	mainfrm = (CMainFrm *)lpParam;

    if (mainfrm->Settings.APVersion ==  AP_OUTLOOKEX)
	{
			g_OEapi->DelSpamFolderMail();
	}

    
	
	
	CloseHandle(mainfrm->DelTrashMailThread);
	mainfrm->DelTrashMailThread = NULL;

	ExitThread(0);
	return 0;

}
*/
int CMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	//SetWindowPos(0,0,0, 0,0,SWP_HIDEWINDOW  );
	ToolTipCtrl.Create( this, 
						 // the ToolTip control's style 
		                 TTS_NOPREFIX | // prevents the system from stripping the ampersand (&) 
						                // character from a string

						 TTS_BALLOON  |//TTS_BALLOON  | // the ToolTip control has the appearance of
						 // 0x40        // a cartoon "balloon," with rounded corners 
										// and a stem pointing to the item. 

						 TTS_ALWAYSTIP   // the ToolTip will appear when the
	                                    // cursor is on a tool, regardless of 
	                                    // whether the ToolTip control's owner
	                                    // window is active or inactive


					   );

	

 


	


	::SendMessage( (HWND)   ToolTipCtrl.m_hWnd, 
				          (UINT)   TTM_SETTITLE, // Adds a standard icon and title string to a ToolTip    
				          (WPARAM) TTI_INFO, 
							// TTI_NONE    = 0 - no icon 
							// TTI_INFO    = 1 - information icon 
							// TTI_WARNING = 2 - warning icon 
							// TTI_ERROR   = 3 - error icon 
						  (LPARAM) (LPCTSTR) "SpamDog Tip" ); 

	

	 

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//ModifyStyle(WS_VISIBLE,0,SWP_NOACTIVATE);
	AddTrayIcon();
	
	 
	  //ChangeIcon(1);
	/*#ifdef _OUTLOOK
	  AfxGetMainWnd()->SetWindowText (_T("SpamDog beta-1") ); 
	#else
		#ifdef _OUTLOOKEX
			AfxGetMainWnd()->SetWindowText (_T("SpamDog OE beta-1") ); 
		#endif
	#endif	
    */


	return 0;
}

void CMainFrm::OnDestroy() 
{
	MsgFrm.DestroyWindow();
	//WebFrm.DestroyWindow();
	//MailReportFrm.DestroyWindow();

	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE,&tnd);
	Shell_NotifyIcon(NIM_DELETE,&tnd2);

	KillTimer(1);
	
}

void CMainFrm::OnMenuitem32775() 
{
	// TODO: Add your command handler code here

	//if(Settings.ReportWithDel  && CheckReportMailExist() && !mMailReportFrm.m_InModal)
	if((!Settings.Licensed && CheckReportMailExist() && !mMailReportFrm.m_InModal ) ||
				(Settings.Licensed  && Settings.NeedReport  && CheckReportMailExist() && !mMailReportFrm.m_InModal))
	{
		//CReportFrm mMailReportFrm(NULL);
		mMailReportFrm.AutoClose = false;
		int rc = mMailReportFrm.DoModal();

		if (rc == 2 || rc == 20)
		{	//正常
			SendMailNow();

			if (Settings.APVersion == AP_OUTLOOK)
			{
			  g_MapiEx->EmptySpamFolder();
			}
			else if (Settings.APVersion == AP_OUTLOOKEX)
			{
	  			g_OEapi->EmptySpamFolder();
			}

				ChangeTrashIcon(0);
		}		
		else
		{
			//report later 
			//return;
		}

	}
	else if (Settings.Licensed && !Settings.NeedReport)
	{
	
		if (Settings.APVersion == AP_OUTLOOK)
		{
		  g_MapiEx->EmptySpamFolder();
		}
		else if (Settings.APVersion == AP_OUTLOOKEX)
		{
	  		g_OEapi->EmptySpamFolder();
		}

			ChangeTrashIcon(0);

	}
	else if (!mMailReportFrm.m_InModal)
	{
			
				AfxMessageBox("目前回報的垃圾郵件數不足,請稍後再試,謝謝!");
	}

	 

	/*if(Settings.Licensed && Settings.NeedReport)
	{
      SendMailNow();
	}*/
}

void CMainFrm::OnMenuitem32776() 
{
	// TODO: Add your command handler code here
	//clear system mail
	if (Settings.APVersion == AP_OUTLOOK)
	{
	  g_MapiEx->EmptySysFolder();
	} 
	else if (Settings.APVersion == AP_OUTLOOKEX)
	{
	  g_OEapi->EmptySysFolder();
	}
}

void CMainFrm::OnMenuitem32777() 
{
	// TODO: Add your command handler code here
	if (Settings.APVersion == AP_OUTLOOK)
	{	
	   g_MapiEx->ArrangeMail(NULL,NULL,NULL,NULL);	   

	}
	else if (Settings.APVersion == AP_OUTLOOKEX)
	{
	   g_OEapi->ArrangeMail(NULL,NULL,0);	  
	}	
}

void CMainFrm::InitialValue()
{
	 CTime ct;
	 ct = CTime::GetCurrentTime();
	 long tvalue = (long) ct.GetTime();

	 Settings.LicenseLastCheckTime = tvalue;

}

void CMainFrm::ShowToolTip(char* Title,char* Text , int Timeout,char ShowType)
{
	 
	 Settings.LastTipType = ShowType;

	 tnd.dwInfoFlags      = NIIF_INFO;
	 tnd.uTimeout         = Timeout;
	 tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP|NIF_INFO  ;

	 strcpy( tnd.szInfoTitle, Title );


	 strcpy( tnd.szInfo,      Text      );

	

	 
	

	 Shell_NotifyIcon(NIM_MODIFY,&tnd);
	

}

void CMainFrm::AddTrayIcon()
{

	  memset(&tnd,0,sizeof(tnd));
	  
	  tnd.cbSize=sizeof(NOTIFYICONDATA);
	  tnd.hWnd=this->m_hWnd;
	  tnd.uID=IDR_MAINFRAME;//IDR_MAINFRAME;
	  tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP|NIF_INFO;

	 

	  tnd.uCallbackMessage=WM_NOTIFYICON;
	  tnd.hIcon=m_hIcon;
	  strcpy(tnd.szTip,"SpamDog");
	  Shell_NotifyIcon(NIM_ADD,&tnd);


	  tnd2.cbSize=sizeof(NOTIFYICONDATA);
	  tnd2.hWnd=this->m_hWnd;
	  tnd2.uID=IDI_ICON2;
	  tnd2.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	  tnd2.uCallbackMessage=WM_NOTIFYICON2;
	  tnd2.hIcon=m_hIcon3;
	  strcpy(tnd2.szTip,"回報且清除垃圾郵件匣");
	  Shell_NotifyIcon(NIM_ADD,&tnd2);

}

HWND CMainFrm::GetTrayNotifyWnd()
{

	HWND hWndTrayNotifyWnd = NULL;

	CWnd *findWnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	
    HWND hWndShellTrayWnd = findWnd->m_hWnd;
    if (hWndShellTrayWnd)    
	{        
		EnumChildWindows(hWndShellTrayWnd, CMainFrm::FindTrayWnd, (LPARAM)&hWndTrayNotifyWnd);   
		
		if(hWndTrayNotifyWnd && IsWindow(hWndTrayNotifyWnd))
		{
			HWND hWndToolBarWnd = NULL;
			EnumChildWindows(hWndTrayNotifyWnd, CMainFrm::FindToolBarInTrayWnd, (LPARAM)&hWndToolBarWnd);   
			if(hWndToolBarWnd)
			{
				return hWndToolBarWnd;
			}
		}

		return hWndTrayNotifyWnd;
	}  

	return hWndShellTrayWnd;

}

BOOL CALLBACK CMainFrm::FindTrayWnd(HWND hwnd, LPARAM lParam)
{    
	TCHAR szClassName[256];
    GetClassName(hwnd, szClassName, 255);    // Did we find the Main System Tray? If so, then get its size and quit
	if (_tcscmp(szClassName, _T("TrayNotifyWnd")) == 0)    
	{        
		HWND* pWnd = (HWND*)lParam;
		*pWnd = hwnd;
        return FALSE;    
	}    
	
	//Oryginal code I found on Internet were seeking here for system clock and it was assumming that clock is on the right side of tray.
	//After that calculated size of tray was adjusted by removing space occupied by clock.
	//This is not a good idea - some clocks are ABOVE or somewhere else on the screen. I found that is far safer to just ignore clock space.
	
	return TRUE;
}

BOOL CALLBACK CMainFrm::FindToolBarInTrayWnd(HWND hwnd, LPARAM lParam)
{    
	TCHAR szClassName[256];
    GetClassName(hwnd, szClassName, 255);    // Did we find the Main System Tray? If so, then get its size and quit
	if (_tcscmp(szClassName, _T("ToolbarWindow32")) == 0)    
	{        
		HWND* pWnd = (HWND*)lParam;
		*pWnd = hwnd;
        return FALSE;    
	}    
	return TRUE;
}

HRESULT CMainFrm::GetAPTrayWnd(DWORD pid , TrayWnd* traywnd)
{
	HRESULT res= S_FALSE;

	HWND hWndTray = GetTrayNotifyWnd();
	if (hWndTray == NULL)
	{
		return S_FALSE;
	}

	//now we have to get an ID of the parent process for system tray
	DWORD dwTrayProcessID = -1;
	GetWindowThreadProcessId(hWndTray, &dwTrayProcessID);
	if(dwTrayProcessID <= 0)
	{
		return S_FALSE;
	}

	HANDLE hTrayProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dwTrayProcessID);
	if(hTrayProc == NULL)
	{
		return S_FALSE;
	}

	

	int iButtonsCount = ::SendMessage(hWndTray, TB_BUTTONCOUNT, 0, 0);
	
	//We want to get data from another process - it's not possible to just send messages like TB_GETBUTTON with a localy
	//allocated buffer for return data. Pointer to localy allocated data has no usefull meaning in a context of another
	//process (since Win95) - so we need to allocate some memory inside Tray process.
	//We allocate sizeof(TBBUTTON) bytes of memory - because TBBUTTON is the biggest structure we will fetch. But this buffer
	//will be also used to get smaller pieces of data like RECT structures.

	LPVOID lpData = VirtualAllocEx(hTrayProc, NULL, sizeof(TBBUTTON), MEM_COMMIT, PAGE_READWRITE);
	if( lpData == NULL || iButtonsCount < 1 )
	{
		CloseHandle(hTrayProc);
		return S_FALSE;
	}

	for(int iButton=0; iButton<iButtonsCount; iButton++)
	{
		DWORD dwBytesRead = -1;
		TBBUTTON buttonData;
		::SendMessage(hWndTray, TB_GETBUTTON, iButton, (LPARAM)lpData);
		ReadProcessMemory(hTrayProc, lpData, &buttonData, sizeof(TBBUTTON), &dwBytesRead);
		if(dwBytesRead < sizeof(TBBUTTON))
		{
			continue;
		}

		//now let's read extra data associated with each button: there will be a HWND of the window that created an icon and icon ID
		DWORD dwExtraData[2] = { 0,0 };
		ReadProcessMemory(hTrayProc, (LPVOID)buttonData.dwData, dwExtraData, sizeof(dwExtraData), &dwBytesRead);
		if(dwBytesRead < sizeof(dwExtraData))
		{
			continue;
		}

		HWND hWndOfIconOwner = (HWND) dwExtraData[0];
		int  iIconId		 = (int)  dwExtraData[1];		

		DWORD dwProcessId = 0;
		GetWindowThreadProcessId(hWndOfIconOwner,&dwProcessId);

		if (dwProcessId == pid)
		{
		
			traywnd->hTrayWnd = hWndTray;
			traywnd->hWnd = hWndOfIconOwner;
			traywnd->IconId = iIconId;
			traywnd->Index = iButton;

			VirtualFreeEx(hTrayProc, lpData, NULL, MEM_RELEASE);
			CloseHandle(hTrayProc);

			return S_OK;
		}


	}

	VirtualFreeEx(hTrayProc, lpData, NULL, MEM_RELEASE);
	CloseHandle(hTrayProc);

	return res;

}



