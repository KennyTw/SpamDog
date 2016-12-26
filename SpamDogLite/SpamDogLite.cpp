// SpamDog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "MainFrm.h"

#include "WizSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"

#include "../Swzip/Compress.h"
#include "../Swparser/MailParser.h"
#include "Dbghelp.h"

#include <string>
#pragma warning(disable: 4251)
using namespace std;



/////////////////////////////////////////////////////////////////////////////
// CSpamDogApp

BEGIN_MESSAGE_MAP(CSpamDogApp, CWinApp)
	//{{AFX_MSG_MAP(CSpamDogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpamDogApp construction

CSpamDogApp::CSpamDogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpamDogApp object

CSpamDogApp theApp;
CRITICAL_SECTION m_CrashSec;

CMainFrm *dlg = NULL;
/////////////////////////////////////////////////////////////////////////////
// CSpamDogApp initialization
void WindowsOpen(char *Url)
{

   HKEY hKey = NULL;
   // Open the registry
   if (::RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("http\\shell\\open\\command"),
     0, KEY_READ, &hKey) == ERROR_SUCCESS)
   {
		// Data size
		DWORD cbData = 0;
		// Get the default value
		if (::RegQueryValueEx(hKey, NULL, NULL, NULL, NULL, &cbData) 
			== ERROR_SUCCESS && cbData > 0)
		{
			// Allocate a suitable buffer
			TCHAR* psz = new TCHAR [cbData];
			// Success?
			if (psz != NULL)
			{
				if (::RegQueryValueEx(hKey, NULL, NULL,
				NULL, (LPBYTE)psz, &cbData) ==
				ERROR_SUCCESS)
				{
					CString m_strBrowser(psz);
					// Success!
					 //m_strBrowser = psz;
					 //ShellExecute(NULL, NULL, psz, Url, NULL, SW_SHOWNORMAL);

					if (m_strBrowser.GetLength() > 0)
					{
					 
						int nStart = m_strBrowser.Find('"');
						int nEnd = m_strBrowser.ReverseFind('"');
						if (nStart>=0 && nEnd>=0)
						{
							m_strBrowser.MakeUpper();
							if(nEnd-nStart>3)
							{ // IE & Opera, their pathnames do not include "%1"
								m_strBrowser = m_strBrowser.Mid(nStart + 1, nEnd - nStart - 1);
							}
							else // FireFox & Netscape & Mozilla
							{
								m_strBrowser = m_strBrowser.Left(m_strBrowser.Find(".EXE")+4);
							} 
						}

						ShellExecute(NULL, NULL, m_strBrowser, Url, NULL, SW_SHOWNORMAL);
						

					}
				}
		
				delete [] psz;
			}
		}
		::RegCloseKey(hKey);
   }



}

LONG __stdcall TheCrashHandlerFunction ( EXCEPTION_POINTERS * pExPtrs )
{

	EnterCriticalSection(&m_CrashSec);

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	  
     HINSTANCE hInstance = GetModuleHandle(NULL);
     GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	 _splitpath( path_buffer, drive, dir, fname, ext );

	 char DumpPath[_MAX_PATH];	 
	   

	   strcpy(DumpPath,drive);
	   strcat(DumpPath,dir);	   
	   strcat(DumpPath,"SpamDogServerCrash.dmp");
	  
	  HANDLE hMiniDumpFile = CreateFile(
		DumpPath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);
	  

	  MINIDUMP_EXCEPTION_INFORMATION eInfo;
      eInfo.ThreadId = GetCurrentThreadId();
      eInfo.ExceptionPointers = pExPtrs;
      eInfo.ClientPointers = TRUE;

	  MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hMiniDumpFile,
            MiniDumpNormal ,
            &eInfo,
            NULL,
            NULL);


	  CloseHandle(hMiniDumpFile);

#ifdef SPAMDOGFULL

	  //Get Dogid
	   char IniPath[_MAX_PATH];	  

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");

	   char DogId[50];	   
	   GetPrivateProfileString("SysSetup","DogId","",DogId,sizeof(DogId),IniPath);

	   char ServerIp[16];
	   GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",ServerIp,sizeof(ServerIp),IniPath);

	  
	  //Report Crash File

	  	  FILE *fp= NULL;
		  fp = fopen(DumpPath , "rb");

		  if (fp != NULL)
		  {
			  fseek(fp,0,SEEK_END);

			  int filesize = ftell(fp);

			  char *buff = new char[filesize];
			  //memset(buff,0,filesize+6);

			  //strcpy(buff,"mail=");

			  fseek(fp,0,SEEK_SET);
			  fread(buff,1,filesize,fp);

			  CCompress cc;	 	 
			  unsigned int outsize = cc.EstimateOutBuffSize(filesize);
		
			  char *outbuff = new char[outsize];
			  
			  char *FinalBuff = NULL;
			  int FinalSize = 0;	
			  bool bCompress = false;

			  HRESULT res;
			  if ( (res = cc.DoCompress((unsigned char*)buff,filesize,(unsigned char *) outbuff,&outsize)) == S_OK)
			  {
					FinalBuff = outbuff;
					FinalSize = outsize;
					bCompress = true;
		 
			  } 
			  else if (res == NO_NEED_COMPRESS)
			  {
					FinalBuff = buff;
					FinalSize = filesize;
			  }

			  //Base64 2編碼
			  CMailCodec MailCodec;
			  char *EncodeStr = MailCodec.Base64Encode2(FinalBuff,FinalSize);

			  string FinalSendStr ;
			  char LenStr[255];
			  itoa(filesize,LenStr, 10);

			  FinalSendStr = "OrgLen=" ;
			  FinalSendStr = FinalSendStr + LenStr;
			  FinalSendStr = FinalSendStr + "&Crash=";
			  FinalSendStr = FinalSendStr + EncodeStr;
			  FinalSendStr = FinalSendStr + "&DogId=";
			  FinalSendStr = FinalSendStr + DogId;

			  if (bCompress)
			  {
				FinalSendStr = FinalSendStr + "&cp=1";
			  }
			  else
			  {
			    FinalSendStr = FinalSendStr + "&cp=0";
			  }


			  CHTTPclient http; 

			  //#ifdef _LOCALLAN
			   HANDLE hand = http.IPPOST(ServerIp,"www.softworking.com","/ReportCrash.asp",(char *)FinalSendStr.c_str(),60 * 1000, 60 * 1000);
			  //#else
			  // HANDLE hand = http.IPPOST("61.62.70.233","www.softworking.com","/ReportCrash.asp",(char *)FinalSendStr.c_str(),60 * 1000, 60 * 1000);
		      //#endif
			
			  MailCodec.FreeBase64Encode(EncodeStr);
			  delete outbuff;


			  

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
			
				fclose(fp);

				DeleteFile(http.GetTempFilePath());	  
			  
			  delete buff;
		  }


#endif	 
	
	  AfxMessageBox("很抱歉 ! SpamDog 發生嚴重錯誤,\n您可能需要重新啟動 Outlook / Outlook Express , 或重新開機 !");
      LeaveCriticalSection(&m_CrashSec);
	  return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH ;//EXCEPTION_EXECUTE_HANDLER
   
}

BOOL CSpamDogApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	InitializeCriticalSection(&m_CrashSec);
	SetUnhandledExceptionFilter ( TheCrashHandlerFunction ) ;


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

	   int DogId = 0;
	   DogId = GetPrivateProfileInt("SysSetup","DogId",0,IniPath);
 
	   if (DogId <= 0)
	   {

			CBitmap bmHeader;	
			if(!bmHeader.LoadBitmap(IDB_BITMAP11))
			return FALSE;
	
			CWizSheet propSheet(NULL, 0, bmHeader, NULL, bmHeader);
			m_pMainWnd = &propSheet;
			int rtn = propSheet.DoModal();

			if (rtn == 999)
			{
				dlg = new CMainFrm();
				m_pMainWnd = dlg;
				dlg->Create(IDD_SPAMDOG_DIALOG); 
			}
	   }
	   else
	   {
	   			dlg = new CMainFrm();
				m_pMainWnd = dlg;
				dlg->Create(IDD_SPAMDOG_DIALOG); 
	   }

	//char * p = NULL ;
    //*p = 'p' ;
	//WinDBG command " .ecxr" for debug line

	 if (m_pMainWnd)
      return TRUE;
   else
      return FALSE;

    



	
	 //dlg->UpdateWindow(); 
	
     //dlg->ShowWindow(SW_HIDE); 
     //dlg->UpdateWindow(); 


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
//	return TRUE;
}

int CSpamDogApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (dlg != NULL)
		delete dlg;

	DeleteCriticalSection (&m_CrashSec);

	return CWinApp::ExitInstance();
}


void SetDefaultMailClient(int ClientCode)
{

	// 1 : Microsoft Outlook
	// 2 : Outlook Express

		HKEY RegKey;
		RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Clients\\Mail",&RegKey);
		 
		char ClientStr[50];
		if (ClientCode == 1)
		{
			strcpy(ClientStr,"Microsoft Outlook");
		} 
		else if (ClientCode == 2)
		{
			strcpy(ClientStr,"Outlook Express");
		}
		
		RegSetValueEx(RegKey,"",0,REG_SZ,(const unsigned char *)ClientStr,strlen(ClientStr));
	  
		RegCloseKey(RegKey);


}