// LiveUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LiveUpdate.h"
#include "LiveUpdateDlg.h"
#include "Dbghelp.h"

#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"

#include "../Swzip/Compress.h"
#include "../Swparser/MailParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <string>
#pragma warning(disable: 4251)
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdateApp

BEGIN_MESSAGE_MAP(CLiveUpdateApp, CWinApp)
	//{{AFX_MSG_MAP(CLiveUpdateApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdateApp construction

CLiveUpdateApp::CLiveUpdateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLiveUpdateApp object

CLiveUpdateApp theApp;
CLiveUpdateDlg *dlg;

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdateApp initialization

LONG __stdcall TheCrashHandlerFunction ( EXCEPTION_POINTERS * pExPtrs )
{

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
	   strcat(DumpPath,"SpamDogCrash.dmp");
	  
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
            MiniDumpNormal,
            &eInfo,
            NULL,
            NULL);


	  CloseHandle(hMiniDumpFile);

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


	 
	
	  AfxMessageBox("很抱歉 ! SpamDog LiveUpdate 發生嚴重錯誤,\n\n此錯誤己回報軟體工廠");
      return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH ;//EXCEPTION_EXECUTE_HANDLER
   
}

BOOL CLiveUpdateApp::InitInstance()
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

	SetUnhandledExceptionFilter ( TheCrashHandlerFunction ) ;

	dlg = new CLiveUpdateDlg();
	m_pMainWnd = dlg;


	dlg->Create(IDD_LIVEUPDATE_DIALOG);

	



   if (m_pMainWnd)
      return TRUE;
   else
      return FALSE;

/*	if (!dlg->Create(IDD_LIVEUPDATE_DIALOG)) 
     return FALSE; 
	
	 dlg->UpdateWindow();	
     dlg->ShowWindow(SW_HIDE); 
     dlg->UpdateWindow(); */

	 


	/*
	
	CLiveUpdateDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	//return TRUE;
}

int CLiveUpdateApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (dlg != NULL)
		delete dlg;

	return CWinApp::ExitInstance();
}
