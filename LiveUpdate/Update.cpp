// Update.cpp: implementation of the CUpdate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LiveUpdate.h"
#include "Update.h"

#include "../Swparser/MailParser.h"
#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"
#include "../Swzip/Compress.h"
#include "../Swspam/SpamDB.h"
//#include "../DBTool/DBdata.h"

#include "LiveUpdateDlg.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef CHTTPclient* (*HTTPclient)();  
typedef CSPAMDB* (*SPAMDB)(char*  , char*  , char*  , int );  
typedef void (*NULLFUNC)(); 
typedef CCompress* (*Compress)();
typedef void (*OneChar)(char *);
typedef void (*TwoChar)(char *,char *);

typedef void (*DllSendReport)(char *,char *,HWND);
typedef HRESULT (*DllParseKeyFile)(char *, HWND );



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpdate::CUpdate(CLiveUpdateDlg *parent)
{
	//CCoreSocket::Startup(); 
	m_progress = NULL;
	m_info = NULL;
	m_parent = parent;


}

CUpdate::~CUpdate()
{
	//CCoreSocket::Cleanup();

}
 
void CUpdate::SendDomainLogToServer()
{
      
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	 char IniPath[_MAX_PATH];
	

	   HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	  
	   strcat(IniPath,"SpamDog.ini");	  

	   char ServerIp[16];
	   GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",ServerIp,sizeof(ServerIp),IniPath);


	//�s�u�d��	 
	   HMODULE SpamMod;
	   SpamMod = LoadLibrary("Swspam.dll");  


	   OneChar createFunc;
	   createFunc = (OneChar) GetProcAddress(SpamMod, "DllSendDomainToServer");
	   
	   //#ifdef _LOCALLAN
	   createFunc(ServerIp);  
	   //#else
	   //NULLFUNC createFunc;
	   //createFunc = (NULLFUNC) GetProcAddress(SpamMod, "DllSendDomainToServer");
	   // createFunc("61.62.70.233");  
	   //#endif	   
	   
	   FreeLibrary(SpamMod);

}
 

unsigned int CUpdate::GetFileVersion(char* FileName)
{

	 unsigned int rc = 0;
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];	  

	 HINSTANCE hInstance = GetModuleHandle(NULL);
     GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	 _splitpath( path_buffer, drive, dir, fname, ext );   


	 char FileNamePath[_MAX_PATH];

	 strcpy(FileNamePath,drive);
	 strcat(FileNamePath,dir);
	 strcat(FileNamePath,FileName);

	 int VersionSize = GetFileVersionInfoSize(FileNamePath,NULL);
	 if (VersionSize > 0)
	 {
		char *Version = new char[VersionSize+1];
		
		if (GetFileVersionInfo(FileNamePath,NULL,VersionSize,Version)  > 0)
		{
			char * versiondetail=NULL;
			char * versionInfo=NULL;
			static char fileVersion[256];
			DWORD vLen=0,langD=0;

			VerQueryValue(Version,"\\VarFileInfo\\Translation",(void **)&versiondetail,(UINT *)&vLen);

			memcpy(&langD,versiondetail,4);            
    
			sprintf(fileVersion, "\\StringFileInfo\\%02X%02X%02X%02X\\FileVersion",
                        (langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, 
                        (langD & 0xff0000)>>16);  

			VerQueryValue((void *) Version, fileVersion,(void **) &versiondetail,(UINT *)&vLen);


			
			//�D�X�ഫ��
			char Ver[255];	
			memset(Ver,0,255);

			int commacount = 0;
			int idx = 0;
			for (unsigned int i = 0 ; i < vLen ; i ++)
			{
			
				if (isdigit(versiondetail[i]))
				{
					Ver[idx] = versiondetail[i];
					idx ++;
				}
				else if (versiondetail[i] == ',')
				{
				
						commacount ++;
					//	idx = 0;				
				}			
			}	

			if (commacount >= 3)
			{
				rc = atoi(Ver);
			}
		
		}

		delete Version;
	 }




	 return rc ;

}

HRESULT CUpdate::GetLastDB()
{

	 
	
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	 char IniPath[_MAX_PATH];
	 bool bCloseMainWnd=false;
	 bool bHasExecute = false;

	 HRESULT hr=S_OK;

	   HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	  
	   strcat(IniPath,"SpamDog.ini");

	   //int lastver = 0;
	   //lastver = GetPrivateProfileInt("LiveUpdate","CurrentVersion",0,IniPath);

	   char DogId[255];
	   GetPrivateProfileString("SysSetup","DogId","0",DogId,255,IniPath);

	   char ServerIp[16];
	   GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",ServerIp,sizeof(ServerIp),IniPath);


	   if (m_info != NULL)
			m_info->SetWindowText("�ˬd�̷s������...");

	   m_parent->AddLine("�ˬd�̷s������...");
	   //�s�u�d��	 
	   HMODULE SocketMod;
	   SocketMod = LoadLibrary("Swsocket.dll");
	   HTTPclient phttp;

	   phttp = (HTTPclient) GetProcAddress(SocketMod, "createHTTPObject");

	   NULLFUNC StartUp,CleanUp;
	   StartUp = (NULLFUNC) GetProcAddress(SocketMod, "SocketStartUp");
	   CleanUp = (NULLFUNC) GetProcAddress(SocketMod, "SocketCleanUp");  
	   
	   StartUp();
	   CHTTPclient *http = phttp();

//#ifdef _LOCALLAN 
	   //HttpSetDNS pSetDNS;
	   //pSetDNS = (HttpSetDNS) GetProcAddress(SocketMod, "SetDNS");
	   //pSetDNS ("192.168.1.2");
	   //http.SetDNS("192.168.1.2");
//	   http->SetDNS("192.168.1.2");
//#endif

	   
	   
//#ifdef _OUTLOOK
	   CString GetVerPath;
	   GetVerPath.Format("DogId=%s&SpamDog=%u&Swspam=%u&SwOLApi=%u&Swfiledb=%u&Swmatcher=%u&Swparser=%u&Swsocket=%u&Swvirus=%u&Swzip=%u&LiveUpdate=%u",
		   DogId,
		   GetFileVersion("SpamDog.exe"),
		   GetFileVersion("Swspam.dll"),
		   GetFileVersion("SwOLApi.dll"),
		   GetFileVersion("Swfiledb.dll"),
		   GetFileVersion("Swmatcher.dll"),
		   GetFileVersion("Swparser.dll"),
		   GetFileVersion("Swsocket.dll"),
		   GetFileVersion("Swvirus.dll"),
		   GetFileVersion("Swzip.dll"),
		   GetFileVersion("LiveUpdate.exe")
		   );
	   
///#ifdef _LOCALLAN
		HANDLE hand = http->IPPOST(ServerIp,"www.softworking.com","/GetVersion4.asp",GetVerPath.GetBuffer(GetVerPath.GetLength()),60 * 1000, 60 * 1000);
//	   #else
//		HANDLE hand = http->IPGET("61.62.70.233","www.softworking.com",GetVerPath.GetBuffer(GetVerPath.GetLength()),60 * 1000, 60 * 1000);
//	   #endif
	   GetVerPath.ReleaseBuffer();
//#endif

	  
	 //  HANDLE hand = pGet("www.microbean.com.tw","/GetVersion.asp",60 * 1000, 60 * 1000);
	   DWORD rtn = WaitForSingleObject(hand,5);
			
	   bool SaveTotal = false;
	   if (m_progress != NULL)
					m_progress->SetRange(0,0);

	    while (rtn != WAIT_OBJECT_0)
		{
				if (!SaveTotal && http->TotalTransfer > 0 && m_progress != NULL)							    
				{
					m_progress->SetRange(0,http->TotalTransfer);
					m_progress->SetPos(http->ByteTransfer);

					SaveTotal = true;
				}

				if (m_progress != NULL)
					m_progress->SetPos(http->ByteTransfer);

				//�B�z waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
		}

		//if (http.ByteTransfer > 0 && m_progress != NULL)
		if (http->ByteTransfer > 0 && m_progress != NULL)
		{
			m_progress->SetRange(0,http->TotalTransfer);
			m_progress->SetPos(http->ByteTransfer);
		}

		//�ˬd�ǿ鱡��
		
		if (!http->DoTransSucceed() )
		{
			if (m_info != NULL)
						m_info->SetWindowText("��s����...");

			m_parent->AddLine("��s����...");

			DeleteFile(http->GetTempFilePath());
			delete http;
	        CleanUp();
	        FreeLibrary(SocketMod);

			return S_FALSE;
		}

       char TempFilePath[_MAX_PATH];
	   strcpy(TempFilePath,http->GetTempFilePath());
 
	   
	   delete http;
	   CleanUp();
	   FreeLibrary(SocketMod);

	   //http �w��������

	   FILE *fp = fopen(TempFilePath,"rb");
	   if (fp != NULL)
	   {
		    //int UpdateNo=0;
			int UpdateItem=0;
			char itemstr[255];
			char FileName[255];

			//fscanf(fp,"%d\n",&UpdateNo); //����s�s��

			//if (ServerVer > lastver )
			//{
				fscanf(fp,"%d\n",&UpdateItem); //���ƥ�

				for (int i= 0 ; i < UpdateItem ; i++)
				{				
					fscanf(fp,"%s %s\n",itemstr,FileName); 

					CString tmpstr;
					tmpstr.Format("�ǳƤU���ɮ� %s",itemstr);

					m_parent->AddLine(tmpstr);
				

					CString srvpath;
					srvpath = "/update/" ;
					srvpath += itemstr;

				
					HMODULE SocketMod;
					SocketMod = LoadLibrary("Swsocket.dll");
					HTTPclient phttp;

					phttp = (HTTPclient) GetProcAddress(SocketMod, "createHTTPObject");

					NULLFUNC StartUp,CleanUp;
					StartUp = (NULLFUNC) GetProcAddress(SocketMod, "SocketStartUp");
					CleanUp = (NULLFUNC) GetProcAddress(SocketMod, "SocketCleanUp");  
	   
					StartUp();
					CHTTPclient *http = phttp();
					//CHTTPclient http;

//#ifdef _LOCALLAN 
//	   http->SetDNS("192.168.1.2");
//#endif

					if (m_info != NULL)
						m_info->SetWindowText("�U����s�ɮ�...");

					m_parent->AddLine("�U����s�ɮ�...");

					//#ifdef _LOCALLAN 
				    HANDLE hand = http->IPGET(ServerIp,"www.softworking.com",srvpath.GetBuffer(srvpath.GetLength()),60 * 1000, 60 * 1000);
					//#else
					//HANDLE hand = http->IPGET("61.62.70.233","www.softworking.com",srvpath.GetBuffer(srvpath.GetLength()),60 * 1000, 60 * 1000);
					//#endif

				    DWORD rtn = WaitForSingleObject(hand,5);			
					
					if (m_progress != NULL)				
						m_progress->SetRange(0,0);

					SaveTotal = false;
					
					while (rtn != WAIT_OBJECT_0)
					{
						if (!SaveTotal && http->TotalTransfer > 0 && m_progress != NULL)				
						{
							m_progress->SetRange(0,http->TotalTransfer);
							m_progress->SetPos(http->ByteTransfer);							 
						}

						if (m_progress != NULL)
							m_progress->SetPos(http->ByteTransfer);

						//�B�z waitting msg dump
						MSG msg;
						while (::PeekMessage(&msg, NULL,   // pump message until none
								NULL, NULL, PM_REMOVE))   // are left in the queue
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}

						rtn = WaitForSingleObject(hand,5);
					}

					if (http->ByteTransfer > 0 && m_progress != NULL)
					{
						m_progress->SetRange(0,http->TotalTransfer);
						m_progress->SetPos(http->ByteTransfer);
					}
					srvpath.ReleaseBuffer();

					//�ˬd�ǿ鱡��
					if (!http->DoTransSucceed())
					{
						if (m_info != NULL)
						  m_info->SetWindowText("��s����...");

						m_parent->AddLine("��s����...");

						DeleteFile(http->GetTempFilePath());
						delete http;
					    CleanUp();
					    FreeLibrary(SocketMod);

						fclose(fp);
						DeleteFile(TempFilePath);

						return S_FALSE;
					}

					//�� http
					 char TempFilePath2[_MAX_PATH];
					 strcpy(TempFilePath2,http->GetTempFilePath());
 
	   
					 delete http;
					 CleanUp();
					 FreeLibrary(SocketMod);


					if (srvpath.Find(".sdb") != -1)
					{
					
						
						//sdb update ����
						if (m_info != NULL)
						 m_info->SetWindowText("��s SpamDB ��...");
						 m_parent->AddLine("��s SpamDB ��...");

						 
						hr = ParseKeyFile(TempFilePath2);
						DeleteFile(TempFilePath2);

						if (!SUCCEEDED(hr))
							break;

					 
					
					}
					else
					{
						//�@���ɮ�
					   //�����Y
						bHasExecute = true;
						if (bCloseMainWnd == false)
						{
							
							//show main frm
							
							m_parent->OnNotifyIcon(IDR_MAINFRAME,WM_LBUTTONDOWN);
							m_parent->SetForegroundWindow();
						    m_parent->FlashWindow(true);
							
						

							if (MessageBox(m_parent->m_hWnd,"�t�ΧY�N�i���s , ���ɱN�|������ SpamDog ,\n��s��A�۰ʱҰ� SpamDog\n\n�z�O�_�P�N�{�b��s ?" ,"SpamDog ��s�q��",MB_YESNO) == IDNO)
							{
								
								if (m_info != NULL)
									m_info->SetWindowText("��s����...");

									m_parent->AddLine("��s����...");

									DeleteFile(TempFilePath2);
								
							

								fclose(fp);
								DeleteFile(TempFilePath);

								return S_FALSE;
							
							}
							//
							MSG msg;
							while (::PeekMessage(&msg, NULL,   // pump message until none
								NULL, NULL, PM_REMOVE))   // are left in the queue
							{
								TranslateMessage(&msg);
								DispatchMessage(&msg);
							}

							//���� main
							if (m_info != NULL)
								m_info->SetWindowText("�����D�{��...");

							m_parent->AddLine("�����D�{��...");

							CLiveUpdateDlg *dlg = (CLiveUpdateDlg *) AfxGetMainWnd();
							::SendMessage(dlg->MainWnd,WM_CLOSESPAMDOG,0,0);

							HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dlg->MainPID);
							WaitForSingleObject(phandle,INFINITE);

							CloseHandle(phandle);  

							
							while (::PeekMessage(&msg, NULL,   // pump message until none
								NULL, NULL, PM_REMOVE))   // are left in the queue
							{
								TranslateMessage(&msg);
								DispatchMessage(&msg);
							}
						
							bCloseMainWnd = true;
						}

						if (m_info != NULL)
						 m_info->SetWindowText("��s�ɮפ�...");
						m_parent->AddLine("��s�ɮפ�...");

					  char FilePath[_MAX_PATH];
					   strcpy(FilePath,drive);
					   strcat(FilePath,dir);	  
					   strcat(FilePath,FileName);


					  if (strncmp(itemstr,"Swzip.dll",9) == 0)
					  {
					    //�����Y�覡��s
						  CopyFile(TempFilePath2,FilePath,FALSE);
					  }
					  else
					  {
					   
						  	HMODULE ZipMod;
							ZipMod = LoadLibrary("Swzip.dll");
							Compress pzip;
							pzip = (Compress) GetProcAddress(ZipMod, "createZipObject");
						   
						    CCompress *cc = pzip();

							//�h�� .z
							/*if (strncmp(FilePath+strlen(FilePath)-2,".z",2) == 0)
							{
								FilePath[strlen(FilePath)-2] = 0;	
							}*/
							
							try
							{
						      cc->UnZipFile(TempFilePath2,FilePath);
							}
							catch(...)
							{
								if (m_progress != NULL)
								  m_progress->SetPos(0); 

								if (m_info != NULL)
									m_info->SetWindowText("�ɮפU������...");

								m_parent->AddLine("�ɮפU������...");
								

								DeleteFile(TempFilePath2);

								hr = -1;

							}


							delete cc;							 
							FreeLibrary(ZipMod);

					  }

					  DeleteFile(TempFilePath2);
				     // AfxMessageBox(http.GetTempFilePath());
					} 
				}

				 
				/* 
				
				if (SUCCEEDED(hr))
				{
					//CString tmpstr;
					//tmpstr.Format("%d",ServerVer);
					//WritePrivateProfileString("LiveUpdate","CurrentVersion",tmpstr,IniPath);

					//�q�� server ��s���\
					HMODULE SocketMod;
					SocketMod = LoadLibrary("Swsocket.dll");
					HTTPclient phttp;

					phttp = (HTTPclient) GetProcAddress(SocketMod, "createHTTPObject");

					NULLFUNC StartUp,CleanUp;
					StartUp = (NULLFUNC) GetProcAddress(SocketMod, "SocketStartUp");
					CleanUp = (NULLFUNC) GetProcAddress(SocketMod, "SocketCleanUp");  
	   
					StartUp();
					CHTTPclient *http = phttp();

					CString UpdatePath;
				    UpdatePath.Format("/FinishUpdate.asp?DogId=%s&UpdateNo=%d",DogId,UpdateNo);

					//#ifdef _LOCALLAN 
				    HANDLE hand = http->IPGET(ServerIp,"www.softworking.com",UpdatePath.GetBuffer(UpdatePath.GetLength()),60 * 1000, 60 * 1000);
					//#else
					//HANDLE hand = http->IPGET("61.62.70.233","www.softworking.com",UpdatePath.GetBuffer(UpdatePath.GetLength()),60 * 1000, 60 * 1000);
					//#endif

					DWORD rtn = WaitForSingleObject(hand,5);				
					while (rtn != WAIT_OBJECT_0)
					{
					 
						//�B�z waitting msg dump
						MSG msg;
						while (::PeekMessage(&msg, NULL,   // pump message until none
								NULL, NULL, PM_REMOVE))   // are left in the queue
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}

						rtn = WaitForSingleObject(hand,5);
 
					}

					UpdatePath.ReleaseBuffer();

					DeleteFile(http->GetTempFilePath());
					delete http;
					CleanUp();
					FreeLibrary(SocketMod);

				 
				}
				 

				*/
			
			 

			//}

			fclose(fp);
	   }

	   DeleteFile(TempFilePath);

	   
	   

	   if (bCloseMainWnd && bHasExecute)
	   {
		   //���� main
		 if (m_info != NULL)
					m_info->SetWindowText("�ҰʥD�{��...");

		   m_parent->AddLine("�ҰʥD�{��...");

		   char SpamDogPath[_MAX_PATH];

		   strcpy(SpamDogPath,drive);
	       strcat(SpamDogPath,dir);	  
//#ifdef _OUTLOOK
	       strcat(SpamDogPath,"SpamDog.exe");
//#endif
//#ifdef _OUTLOOKEX
//		   strcat(SpamDogPath,"SpamDogOE.exe");
//#endif
		   
		   ShellExecute(0,"open",SpamDogPath,"","",SW_SHOWMINNOACTIVE);
	   }

	   return hr;

}

void CUpdate::UpdateIni()
{

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	 char IniPath[_MAX_PATH];

	   HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	  
	   strcat(IniPath,"SpamDog.ini");

	   CTime ct;
	   ct = CTime::GetCurrentTime();
	   long tvalue = (long) ct.GetTime();

	   CString tmpstr;
	   tmpstr.Format("%d",tvalue);

	   WritePrivateProfileString("LiveUpdate","LastUpdate",tmpstr,IniPath);

}

void CUpdate::SetProgress(CProgressCtrl *progress)
{
	m_progress = progress;

}

void CUpdate::SetStatic(CStatic	*info)
{
	m_info = info;
}


HRESULT CUpdate::ParseKeyFile(char *FileName)
{
	   HMODULE SpamMod;
	   SpamMod = LoadLibrary("Swspam.dll");  


	   DllParseKeyFile createFunc;
	   createFunc = (DllParseKeyFile) GetProcAddress(SpamMod, "DllParseKeyFile");
	    
	   HRESULT hr = createFunc(FileName,m_parent->m_Progress.m_hWnd);     
	   
	   FreeLibrary(SpamMod);
		 //CSPAMDB *spdb = pspamdb();
	
	 
	//��s last version
	return hr;
}

/*
#include "DbgHelp.h"
static IMAGEHLP_LINE g_stLine ;

LPCTSTR ConvertSimpleException ( DWORD dwExcept )
{
    switch ( dwExcept )
    {
        case EXCEPTION_ACCESS_VIOLATION         :
            return ( _T ( "EXCEPTION_ACCESS_VIOLATION" ) ) ;
        break ;

        case EXCEPTION_DATATYPE_MISALIGNMENT    :
            return ( _T ( "EXCEPTION_DATATYPE_MISALIGNMENT" ) ) ;
        break ;

        case EXCEPTION_BREAKPOINT               :
            return ( _T ( "EXCEPTION_BREAKPOINT" ) ) ;
        break ;

        case EXCEPTION_SINGLE_STEP              :
            return ( _T ( "EXCEPTION_SINGLE_STEP" ) ) ;
        break ;

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED    :
            return ( _T ( "EXCEPTION_ARRAY_BOUNDS_EXCEEDED" ) ) ;
        break ;

        case EXCEPTION_FLT_DENORMAL_OPERAND     :
            return ( _T ( "EXCEPTION_FLT_DENORMAL_OPERAND" ) ) ;
        break ;

        case EXCEPTION_FLT_DIVIDE_BY_ZERO       :
            return ( _T ( "EXCEPTION_FLT_DIVIDE_BY_ZERO" ) ) ;
        break ;

        case EXCEPTION_FLT_INEXACT_RESULT       :
            return ( _T ( "EXCEPTION_FLT_INEXACT_RESULT" ) ) ;
        break ;

        case EXCEPTION_FLT_INVALID_OPERATION    :
            return ( _T ( "EXCEPTION_FLT_INVALID_OPERATION" ) ) ;
        break ;

        case EXCEPTION_FLT_OVERFLOW             :
            return ( _T ( "EXCEPTION_FLT_OVERFLOW" ) ) ;
        break ;

        case EXCEPTION_FLT_STACK_CHECK          :
            return ( _T ( "EXCEPTION_FLT_STACK_CHECK" ) ) ;
        break ;

        case EXCEPTION_FLT_UNDERFLOW            :
            return ( _T ( "EXCEPTION_FLT_UNDERFLOW" ) ) ;
        break ;

        case EXCEPTION_INT_DIVIDE_BY_ZERO       :
            return ( _T ( "EXCEPTION_INT_DIVIDE_BY_ZERO" ) ) ;
        break ;

        case EXCEPTION_INT_OVERFLOW             :
            return ( _T ( "EXCEPTION_INT_OVERFLOW" ) ) ;
        break ;

        case EXCEPTION_PRIV_INSTRUCTION         :
            return ( _T ( "EXCEPTION_PRIV_INSTRUCTION" ) ) ;
        break ;

        case EXCEPTION_IN_PAGE_ERROR            :
            return ( _T ( "EXCEPTION_IN_PAGE_ERROR" ) ) ;
        break ;

        case EXCEPTION_ILLEGAL_INSTRUCTION      :
            return ( _T ( "EXCEPTION_ILLEGAL_INSTRUCTION" ) ) ;
        break ;

        case EXCEPTION_NONCONTINUABLE_EXCEPTION :
            return ( _T ( "EXCEPTION_NONCONTINUABLE_EXCEPTION" ) ) ;
        break ;

        case EXCEPTION_STACK_OVERFLOW           :
            return ( _T ( "EXCEPTION_STACK_OVERFLOW" ) ) ;
        break ;

        case EXCEPTION_INVALID_DISPOSITION      :
            return ( _T ( "EXCEPTION_INVALID_DISPOSITION" ) ) ;
        break ;

        case EXCEPTION_GUARD_PAGE               :
            return ( _T ( "EXCEPTION_GUARD_PAGE" ) ) ;
        break ;

        case EXCEPTION_INVALID_HANDLE           :
            return ( _T ( "EXCEPTION_INVALID_HANDLE" ) ) ;
        break ;

        case 0xE06D7363                         :
            return ( _T ( "Microsoft C++ Exception" ) ) ;
        break ;

        default :
            return ( NULL ) ;
        break ;
    }
}

DWORD ParseException(EXCEPTION_POINTERS* pExPtrs)
{


	
	 BOOL bRet = SymInitialize ( (HANDLE)GetCurrentProcessId ( )   ,
                                    NULL               ,
                                    TRUE                     ) ;

	ASSERT ( TRUE == bRet ) ;


	
	DWORD dwDisp ;

	 

	
	ZeroMemory ( &g_stLine , sizeof ( IMAGEHLP_LINE ) ) ;
    g_stLine.SizeOfStruct = sizeof ( IMAGEHLP_LINE ) ;

 

    if (TRUE == SymGetLineFromAddr((HANDLE) GetCurrentProcessId () ,
						(DWORD)pExPtrs->ExceptionRecord->ExceptionAddress,
						&dwDisp,
						&g_stLine))
	{

		CString outdebug;
		outdebug.Format("%s , LineNo : %d , %s",g_stLine.FileName,g_stLine.LineNumber,ConvertSimpleException(pExPtrs->ExceptionRecord->ExceptionCode));  

		AfxMessageBox(outdebug);
	}
	else
	{
		
	    CString  errs;
		errs.Format(_T("SymGetLineFromAddr64 returned error : %d "), GetLastError());
		AfxMessageBox(errs);

	}

	CString tmp;
	tmp.Format("%x",pExPtrs->ExceptionRecord->ExceptionAddress);
	AfxMessageBox(tmp);

	
	VERIFY ( SymCleanup ( (HANDLE)GetCurrentProcessId () ) ) ;

	return EXCEPTION_EXECUTE_HANDLER;
}
*/
void CUpdate::SendReport()
{

	 

	
		
	   char DogId[255];

	   char path_buffer[_MAX_PATH];
	   char drive[_MAX_DRIVE];
	   char dir[_MAX_DIR];
	   char fname[_MAX_FNAME];
	   char ext[_MAX_EXT];

	   char IniPath[_MAX_PATH];	    

	   HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	  
	   strcat(IniPath,"SpamDog.ini");

	    
	   
	   GetPrivateProfileString("SysSetup","DogId","0",DogId,255,IniPath);

	   char ServerIp[16];
	   GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",ServerIp,sizeof(ServerIp),IniPath);

	  

	   
	/*__try
	{*/
		
	   
	   

	   HMODULE SpamMod;
	   SpamMod = LoadLibrary("Swspam.dll");    
	   

	   DllSendReport createFunc;
	   createFunc = (DllSendReport) GetProcAddress(SpamMod, "DllSendReport");
	   
      //#ifdef _LOCALLAN    

	    

	    createFunc(ServerIp,DogId,m_parent->m_Progress.m_hWnd);  
	   //#else
	   //NULLFUNC createFunc;
	   //createFunc = (NULLFUNC) GetProcAddress(SpamMod, "DllSendDomainToServer");
	    //createFunc("61.62.70.233",DogId,m_parent->m_Progress.m_hWnd);  
	   //#endif	   
	   
	   FreeLibrary(SpamMod);

//	}
//	__except (ParseException(GetExceptionInformation()))
////	{
		

		

	
	//}


}
