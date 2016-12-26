// SMTPservice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SMTPservice.h"

#include "Dbghelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "../../NSM/Swsocket2/SMTPClient.h"
#include <afxdb.h>
#include <conio.h>


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
	   strcat(DumpPath,"SpamDogSmtpServerCrash.dmp");
	  
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
            MiniDumpWithFullMemory,
            &eInfo,
            NULL,
            NULL);


	  CloseHandle(hMiniDumpFile);



	  //Beep(2000,1000 * 60);
	 
	
	  //AfxMessageBox("很抱歉 ! SpamDog Server 發生嚴重錯誤");
      return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH ;//EXCEPTION_EXECUTE_HANDLER
   
}

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


struct MultiSmtpObj
{
  	char SendResult[255];
	char DataFile[_MAX_PATH];	
	char EmailFile[_MAX_PATH];	
	int ReturnCode;	
};

 

void SendMail()
{
		
	 

		HANDLE smtphd[20];
		int ThreadCount = 0;

		MultiSmtpObj SmtpObj[20]={0};

		//char SendResult[4][255];
		//char DataFile[4][_MAX_PATH];	
		//char EmailFile[4][_MAX_PATH];	
		//int ReturnCode[4];	
		

		bool StopSendNext = false;

		char RelayPath[MAX_PATH];
		strcpy(RelayPath,"d:\\ReportISP\\");

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		char DirSpec[MAX_PATH + 1];  // directory specification

		strcpy(DirSpec,RelayPath);
		strcat(DirSpec,"*.eml");	 

		CSMTPClient smtpclient;
		smtpclient.SocketTimeout = 60;

		CDatabase db;
		//db.SetLoginTimeout(0);
		//db.SetQueryTimeout(0);

		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

	//while(1)
	//{

		hFind = FindFirstFile(DirSpec, &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{	
				if (ThreadCount >= 20)  break;
				
				
			
				strcpy(SmtpObj[ThreadCount].DataFile,RelayPath);				 
							
				strncat(SmtpObj[ThreadCount].DataFile,FindFileData.cFileName,strlen(FindFileData.cFileName)-3);			
				strcat(SmtpObj[ThreadCount].DataFile,"dat");

				strcpy(SmtpObj[ThreadCount].EmailFile,RelayPath);			 		
				strcat(SmtpObj[ThreadCount].EmailFile,FindFileData.cFileName);
				
				printf("SendMail..Email:%s , Data:%s\r\n",SmtpObj[ThreadCount].EmailFile,SmtpObj[ThreadCount].DataFile);

				//move to sending

								char drive[_MAX_DRIVE];
								char dir[_MAX_DIR];
								char fname[_MAX_FNAME];
								char ext[_MAX_EXT];
							 
								char RenameToPath[_MAX_PATH];
								strcpy(RenameToPath,SmtpObj[ThreadCount].EmailFile);
								//strcat(RenameToPath,".bak");

								_splitpath(RenameToPath, drive, dir, fname, ext );

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Sending//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);

								
								MoveFile(SmtpObj[ThreadCount].EmailFile,RenameToPath);
								strcpy(SmtpObj[ThreadCount].EmailFile,RenameToPath );

								
								//strcat(RenameToPath,".bak");
								strcpy(RenameToPath,SmtpObj[ThreadCount].DataFile);
								_splitpath(RenameToPath, drive, dir, fname, ext );								

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Sending//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);						 

								MoveFile(SmtpObj[ThreadCount].DataFile,RenameToPath);

								strcpy(SmtpObj[ThreadCount].DataFile ,RenameToPath );



				//讀出 rcpt to 	
			
					SmtpObj[ThreadCount].SendResult[0] = 0;
					SmtpObj[ThreadCount].ReturnCode = NO_ERROR;
				 
					//smtphd[ThreadCount] = smtpclient.ThreadDnsSend("192.168.1.3","softworking.com",SmtpObj[ThreadCount].EmailFile,SmtpObj[ThreadCount].DataFile,SmtpObj[ThreadCount].SendResult,&SmtpObj[ThreadCount].ReturnCode);
					
					smtphd[ThreadCount] = smtpclient.Send("192.168.1.3","softworking.com",SmtpObj[ThreadCount].EmailFile,SmtpObj[ThreadCount].DataFile,SmtpObj[ThreadCount].SendResult,&SmtpObj[ThreadCount].ReturnCode);
					ThreadCount++;
				
			


				 

			} while (FindNextFile(hFind, &FindFileData) != 0) ;
    
      
			FindClose(hFind);		
		
		}

		if (ThreadCount < 20) StopSendNext = true;

		//Beep(1000,500);	
		//printf("Thread Count %d\r\n\r\n",ThreadCount);
		if (ThreadCount > 0)
		{
				while(1)
				{
				
					int rc = WaitForMultipleObjects(ThreadCount,smtphd,FALSE,1000 * 60 * 10) ;
					if (rc == WAIT_TIMEOUT)
					{
						printf("Time Out\r\n");
						//getch();
						break;
					}else
					if (rc >= WAIT_OBJECT_0  && rc < WAIT_OBJECT_0 + ThreadCount)
					{
						    //Beep((500 * rc - WAIT_OBJECT_0)+500,500);
						    
							CloseHandle (smtphd[rc - WAIT_OBJECT_0] );
							if (SmtpObj[rc - WAIT_OBJECT_0].ReturnCode == NO_ERROR && SmtpObj[rc - WAIT_OBJECT_0].SendResult[0] == 0)
							{
								//成功
								char drive[_MAX_DRIVE];
								char dir[_MAX_DIR];
								char fname[_MAX_FNAME];
								char ext[_MAX_EXT];
							 
								char RenameToPath[_MAX_PATH];

								
								strcpy(RenameToPath,SmtpObj[rc - WAIT_OBJECT_0].EmailFile);
								//strcat(RenameToPath,".bak");

								_splitpath(RenameToPath, drive, dir, fname, ext );

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Ready//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);

								if (MoveFile(SmtpObj[rc - WAIT_OBJECT_0].EmailFile,RenameToPath) == FALSE)
								{
									printf("Move File Error %s \n",SmtpObj[rc - WAIT_OBJECT_0].EmailFile);
									//getch();
								}

								strcpy(RenameToPath,SmtpObj[rc - WAIT_OBJECT_0].DataFile);
								//strcat(RenameToPath,".bak");
								
								_splitpath(RenameToPath, drive, dir, fname, ext );								

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Ready//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);						 

								if (MoveFile(SmtpObj[rc - WAIT_OBJECT_0].DataFile,RenameToPath) == FALSE)
								{
									printf("Move File Error %s \n",SmtpObj[rc - WAIT_OBJECT_0].DataFile);
									//getch();
								}
								

								printf("Send OK %s\r\n",SmtpObj[rc - WAIT_OBJECT_0].EmailFile);

								CString sql;
								sql.Format("update ReportIsp set ReportTime=getdate() , Status = 1 where SpammailId = %s",fname);
								printf("%s\r\n",sql.GetBuffer(sql.GetLength()));
								db.ExecuteSQL(sql);

								sql.ReleaseBuffer();

							}
							else
							{
								char drive[_MAX_DRIVE];
								char dir[_MAX_DIR];
								char fname[_MAX_FNAME];
								char ext[_MAX_EXT];
								
								char RenameToPath[_MAX_PATH];
								
								strcpy(RenameToPath,SmtpObj[rc - WAIT_OBJECT_0].EmailFile);
								//strcat(RenameToPath,".err");

								_splitpath(RenameToPath, drive, dir, fname, ext );

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Error//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);


								if (MoveFile(SmtpObj[rc - WAIT_OBJECT_0].EmailFile,RenameToPath) == FALSE)
								{
								    printf("Move File Error %s \n",SmtpObj[rc - WAIT_OBJECT_0].EmailFile);
									//getch();
								}
							 
								

								strcpy(RenameToPath,SmtpObj[rc - WAIT_OBJECT_0].DataFile);
								//strcat(RenameToPath,".bak");
								
								_splitpath(RenameToPath, drive, dir, fname, ext );		

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Error//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);			
								
								if (MoveFile(SmtpObj[rc - WAIT_OBJECT_0].DataFile,RenameToPath) == FALSE)
								{
									printf("Move File Error %s \n",SmtpObj[rc - WAIT_OBJECT_0].DataFile);
									//getch();
								}

 
							
								
								printf("Send ERROR %s\r\n",SmtpObj[rc - WAIT_OBJECT_0].SendResult);

								CString ResStr(SmtpObj[rc - WAIT_OBJECT_0].SendResult);
								ResStr.Replace("\'","");


								CString sql;
								sql.Format("update ReportIsp set ReportTime=getdate() , Status = 2,ReturnErr='%s' where SpammailId = %s",ResStr,fname);
								printf("%s\r\n",sql.GetBuffer(sql.GetLength()));
							
								db.ExecuteSQL(sql);

								sql.ReleaseBuffer();

								sql.Format("Delete from nic where nicid = (select nicid from ReportIsp where SpammailId = %s)",fname);
								printf("%s\r\n",sql.GetBuffer(sql.GetLength()));
							
								db.ExecuteSQL(sql);

								sql.ReleaseBuffer();
							}


							//準備送下一封
							hFind = INVALID_HANDLE_VALUE;
							if (!StopSendNext) 
							{
								hFind = FindFirstFile(DirSpec, &FindFileData);
							}

							if ( hFind != INVALID_HANDLE_VALUE ) 
							//if (1 == 2)
							{
								strcpy(SmtpObj[rc - WAIT_OBJECT_0].DataFile,RelayPath);				 
							
								strncat(SmtpObj[rc - WAIT_OBJECT_0].DataFile,FindFileData.cFileName,strlen(FindFileData.cFileName)-3);			
								strcat(SmtpObj[rc - WAIT_OBJECT_0].DataFile,"dat");

								strcpy(SmtpObj[rc - WAIT_OBJECT_0].EmailFile,RelayPath);			 		
								strcat(SmtpObj[rc - WAIT_OBJECT_0].EmailFile,FindFileData.cFileName);
				
								printf("Next.....SendMail ..Email:%s , Data:%s\r\n",SmtpObj[rc - WAIT_OBJECT_0].EmailFile,SmtpObj[rc - WAIT_OBJECT_0].DataFile);

								//move to sending

								char drive[_MAX_DRIVE];
								char dir[_MAX_DIR];
								char fname[_MAX_FNAME];
								char ext[_MAX_EXT];
							 
								char RenameToPath[_MAX_PATH];
								strcpy(RenameToPath,SmtpObj[rc - WAIT_OBJECT_0].EmailFile);
								//strcat(RenameToPath,".bak");

								_splitpath(RenameToPath, drive, dir, fname, ext );

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Sending//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);

								if (MoveFile(SmtpObj[rc - WAIT_OBJECT_0].EmailFile,RenameToPath) == FALSE)
								{
									printf("Move File Error %s \n",SmtpObj[rc - WAIT_OBJECT_0].EmailFile);
									//getch();
								}

								strcpy(SmtpObj[rc - WAIT_OBJECT_0].EmailFile ,RenameToPath );

								
								//strcat(RenameToPath,".bak");
								strcpy(RenameToPath,SmtpObj[rc - WAIT_OBJECT_0].DataFile);
								_splitpath(RenameToPath, drive, dir, fname, ext );								

								strcpy(RenameToPath,drive);
								strcat(RenameToPath,dir);
								strcat(RenameToPath,"Sending//");
								strcat(RenameToPath,fname);
								strcat(RenameToPath,ext);						 

								if (MoveFile(SmtpObj[rc - WAIT_OBJECT_0].DataFile,RenameToPath) == FALSE)
								{
									printf("Move File Error %s \n",SmtpObj[rc - WAIT_OBJECT_0].DataFile);
									//getch();
								}

								strcpy(SmtpObj[rc - WAIT_OBJECT_0].DataFile ,RenameToPath );


								SmtpObj[rc - WAIT_OBJECT_0].SendResult[0] = 0;
								SmtpObj[rc - WAIT_OBJECT_0].ReturnCode = NO_ERROR;

							
								smtphd[rc - WAIT_OBJECT_0] = smtpclient.ThreadDnsSend("192.168.1.3","softworking.com",SmtpObj[rc - WAIT_OBJECT_0].EmailFile,SmtpObj[rc - WAIT_OBJECT_0].DataFile,SmtpObj[rc - WAIT_OBJECT_0].SendResult,&SmtpObj[rc - WAIT_OBJECT_0].ReturnCode);
				 
								FindClose(hFind);	

							}
							else
							{
								//printf("Find Next Error\r\n");
								//RemoveEvent(smtphd,rc - WAIT_OBJECT_0);

								if (ThreadCount > 1 )
								{
									int size = sizeof(smtphd) / sizeof(smtphd[0]);	

									for (int i = rc - WAIT_OBJECT_0 ; i < ThreadCount ; i ++)
									{
										smtphd[i] = smtphd[i+1];
										SmtpObj[i] = SmtpObj[i+1];
									}
								}

								ThreadCount--;
								StopSendNext = true;

							}

				 
					 
					}
					else
					{
						printf("Wait Error! ThreadCount:%d \r\n\r\n",ThreadCount);
						//if (ThreadCount != 0) getch();
						
						break;
					}
				}
		}
		//else
		//{
		//	printf("No Thread!! \r\n");
		//	getch();
			//break;
		//}

	
		db.Close();
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	
	SetUnhandledExceptionFilter ( TheCrashHandlerFunction ) ;

	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.

	
		//while(1)
		//{
		    //WinExec("c:\\MoveMail.bat",SW_SHOW);

			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = "c:\\MoveMail.bat";		
			ShExecInfo.lpParameters = "";	
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;	
			ShellExecuteEx(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
			
			SendMail();
			//Sleep(1000 * 60);
		//}

		/*
			CDnsClient dns;	 
			char Domain[255];
			strcpy(Domain,"strato.de");

			char hostIP[20];
			hostIP[0] = 0 ;


			HANDLE ch = dns.Resolve("192.168.1.3",(char *)Domain,qtMX,hostIP);	 
			WaitForSingleObject(ch,INFINITE);
			
			printf("Dns for Resolve : %s  , IP: %s\r\n",Domain,hostIP);
			*/
	}

	return nRetCode;
}


