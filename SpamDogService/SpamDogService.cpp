// SpamDogService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SpamDogService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../Swparser/MailParser.h"
#include "../Swzip/Compress.h"
#include "../Swspam/Checker.h"

#include "../../NSM/Swsocket2/CoreClass.h"
//#include "../../NSM/Swsocket2/ClientClass.h"

#include "../../NSM/Swsocket2/SMTPClient.h"
#include "Dbghelp.h"
//#include "Checker2.h"


#pragma warning(disable:4146) 
#import "C:\Program Files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "EndOfFile")
#include <afxdb.h>
#include <conio.h>
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

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
            MiniDumpWithFullMemory,
            &eInfo,
            NULL,
            NULL);


	  CloseHandle(hMiniDumpFile);




	 
	
	  //AfxMessageBox("很抱歉 ! SpamDog Server 發生嚴重錯誤");
      return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH ;//EXCEPTION_EXECUTE_HANDLER
   
}

void WriteQueryLog(char *SQL)
{

	/*FILE *fp=NULL;
	fp = fopen("c:\\sql.txt","r+b");
	if (fp == NULL)
	{
		fp = fopen("C:\\sql.txt","w+b");
	}

	fseek(fp,0,SEEK_END);

	fprintf(fp,"%s\r\n",SQL);

	fclose(fp);*/
	cout << SQL << endl;
	


}


void SendReportFile(char *IP,CString Contact , CString SpamMailId)
{
		FILE *fp1 = fopen("c:\\ReportMail.eml","r+b");

		CString ReadContent;
		CString MailContent;

		char buff[1025];
		while ( !feof( fp1 ))
		{
			int readsize = fread(buff,1,1024,fp1);
			buff[readsize] = 0;
			ReadContent += buff;
		}

		fclose(fp1);

		int pos1 = ReadContent.Find("<!--Replace1-->");
		int pos2 = ReadContent.Find("<!--Replace2-->");
		int pos3 = ReadContent.Find("<!--Replace3-->");
		int pos4 = ReadContent.Find("<!--Replace4-->");
		int pos5 = ReadContent.Find("<!--Replace5-->");

 
		FILE *fp = fopen("c:\\ProcessMail.eml","r+b");
		fseek(fp,0,SEEK_END);
		int size = ftell(fp);

		char *MailBuf = new char[size+1];
		fseek(fp,0,SEEK_SET);
		fread(MailBuf ,1,size,fp);
		fclose(fp);
		MailBuf[size] = 0;


		MailContent = ReadContent.Mid(0,pos1);
		MailContent += IP;
		MailContent += ReadContent.Mid(pos1+15,pos2-pos1-15);
		MailContent += Contact;
		MailContent += ReadContent.Mid(pos2+15,pos3-pos2-15);
		MailContent += IP;
		MailContent += ReadContent.Mid(pos3+15,pos4-pos3-15);
		MailContent += SpamMailId;
		MailContent += ReadContent.Mid(pos4+15,pos5-pos4-15);
		MailContent += MailBuf;
		MailContent += ReadContent.Mid(pos5+15);

	

		delete MailBuf;

		char TempDataPath[_MAX_PATH];
		char TempMailPath[_MAX_PATH];

		SpamMailId.TrimLeft();
		SpamMailId.TrimRight();

		strcpy(TempDataPath,"d:\\ReportISP\\");
		strcat(TempDataPath,SpamMailId);
		strcat(TempDataPath,".dat");

		
				
	//	GetTempFileName("c:\\ReportISP", // directory for temp files 
	//				"Mail",                    // temp file name prefix 
	//				0,                        // create unique name 
	//				TempDataPath);              // buffer for name 		
				
		FILE *fpdata = fopen(TempDataPath,"w+b");
		fprintf(fpdata,"spamreport@softworking.com\r\n","");
		fprintf(fpdata,"%s\r\n",Contact);
		fclose(fpdata);

		strcpy(TempMailPath,"d:\\ReportISP\\");
		strcat(TempMailPath,SpamMailId);
		strcat(TempMailPath,".eml");

	
	 
		FILE *fpmail = fopen(TempMailPath,"w+b");

		if (fpmail != NULL)
		{
		
		    
			fwrite(MailContent.GetBuffer(MailContent.GetLength()),1,MailContent.GetLength(),fpmail);
			fclose(fpmail);

			MailContent.ReleaseBuffer();
		}


		//char RenameTempDataPath[_MAX_PATH];
		//strcpy(RenameTempDataPath,TempDataPath);
		//strcat(RenameTempDataPath,".dat");
		//rename(TempDataPath,RenameTempDataPath);

		/*
		CString MailDomain;
		int FindPos = Contact.Find('@');
		MailDomain = Contact.Mid(FindPos+1);

		CDnsClient dns;
		char MailIP[20];
		MailIP[0] = 0;

		HANDLE ch = dns.Resolve("192.168.1.3",(char *)MailDomain.GetBuffer(MailDomain.GetLength()),qtMX,MailIP);	 
		WaitForSingleObject(ch,INFINITE);	
		//strcpy(MailIP,dns.GetQueryResult()); 

		if (MailIP[0] != 0)
		{

			char SendResult[255];
			SendResult[0] = 0;

			CSMTPClient smtpclient;
			HANDLE smtphd = smtpclient.Send(MailIP,"softworking.com",TempMailPath,RenameTempDataPath,SendResult);
			if (WaitForSingleObject(smtphd,5 * 60 * 1000) == WAIT_OBJECT_0)
			{
						
							if (SendResult[0] == 0)
							{
								//成功
								//DeleteFile(TempMailPath);
								//DeleteFile(RenameTempDataPath);
								char RenameToPath[_MAX_PATH];
								strcpy(RenameToPath,TempMailPath);
								strcat(RenameToPath,".bak");

								rename(TempMailPath,RenameToPath);
								
								strcpy(RenameToPath,RenameTempDataPath);
								strcat(RenameToPath,".bak");

								rename(RenameTempDataPath,RenameToPath);
								

								printf("Send OK %s\r\n",TempMailPath);

							}
							else
							{
								char RenameToPath[_MAX_PATH];
								strcpy(RenameToPath,TempMailPath);
								strcat(RenameToPath,".err");

								rename(TempMailPath,RenameToPath);
								
								strcpy(RenameToPath,RenameTempDataPath);
								strcat(RenameToPath,".err");

								char ErrorMsgPath[_MAX_PATH];
								strcpy(ErrorMsgPath,TempMailPath);
								strcat(ErrorMsgPath,".msg");

								FILE *fp = fopen(ErrorMsgPath,"w+b");
								if (fp != NULL)
								{
									fwrite(SendResult,1,strlen(SendResult),fp);
									fclose(fp);
								}

								rename(RenameTempDataPath,RenameToPath);
								
								printf("Send ERROR %s\r\n",SendResult);
							}

			}
		}*/


}



void SendToIsp(char *IP , CString SpamMailId)
{
	//由 db 查出 report mail 
	//如果沒有則起動 nic query
 
	if (IP == NULL || IP[0] == 0) return ;

	CSpamContact sc;
	sc.SetIp(IP);

	unsigned long IPvalue = sc.GetIPValue();

	CString sql;
	sql.Format("select top 1 contact,nicid from nic where IPs <= %lu and IPe >= %lu order by IPs desc",IPvalue,IPvalue);
	WriteQueryLog(sql.GetBuffer(sql.GetLength()));

 
	CDatabase db;
	db.SetLoginTimeout(0); 
    db.SetQueryTimeout(0);

	db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

	CDatabase db2;

	db2.SetLoginTimeout(0); 
    db2.SetQueryTimeout(0);

	db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

	CRecordset rec(&db);
	
	rec.Open(CRecordset::forwardOnly,sql);
	//while(!rec.IsEOF())
	//{
	//	rec.MoveNext();	
	//}

	if (!rec.IsEOF())
	{
		CString Contact;
		CString nicid;

		rec.GetFieldValue((int)0,Contact);
		rec.GetFieldValue((int)1,nicid);

		// load temp file and replace tag
		// write forward email
		// mail from /r/n SpamReport@softworking.com
		// rcpt to /r/n contact

		Contact.TrimLeft();
		Contact.TrimRight();

		if (Contact.GetLength() > 0)
		{

			sql.Format("insert into ReportIsp (SpammailId,NicId,Ip) values (%s,%s,'%s')",SpamMailId,nicid,IP);
			WriteQueryLog(sql.GetBuffer(sql.GetLength()));
			db2.ExecuteSQL(sql);


			//WriteQueryLog("SendReportFile !");
			//SendReportFile(IP,Contact, SpamMailId);
		}
		else
		{
		
			sql.Format("delete from nic where nicid = %s",nicid);
			WriteQueryLog(sql.GetBuffer(sql.GetLength()));
			db2.ExecuteSQL(sql);

		}
	 

		
	
	}
	else
	{
	
	/*	db沒資料則不寄
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

		WriteQueryLog("Start GetContact !");

		sc.GetContact(InetNumS,InetNumE,netname,country,contact); 
	
		
		if ( InetNumS[0] != 0 && InetNumE[0] != 0)
		{
			//query to db


				CSpamContact m_sc1;	
				m_sc1.SetIp(InetNumS);

				CSpamContact m_sc2;	
				m_sc2.SetIp(InetNumE);

				unsigned int IPs = m_sc1.GetIPValue();
				unsigned int IPe = m_sc2.GetIPValue();


			sql.Format("insert into Nic (IPs,IPe,NetName,Country,Contact) values (%lu,%lu,'%s','%s','%s')",IPs,IPe,netname,country ,contact);
			WriteQueryLog(sql.GetBuffer(sql.GetLength()));

			db.ExecuteSQL(sql);

			sql.Format("select nicid from Nic where IPs = %lu and IPe = %lu ",IPs,IPe);
			WriteQueryLog(sql.GetBuffer(sql.GetLength()));

			CRecordset rec2(&db2);
	
			rec2.Open(CRecordset::forwardOnly,sql);
			if (!rec2.IsEOF())
			{
				CString nicid;
				
				rec2.GetFieldValue((int)0,nicid);	
				rec2.Close();

				sql.Format("insert into ReportIsp (SpammailId,NicId,Ip) values (%s,%s,'%s')",SpamMailId,nicid,IP);
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				
				db.ExecuteSQL(sql);


			}



			if (contact[0] != 0)
			{
				CString cContact(contact);
				SendReportFile(IP,cContact, SpamMailId);
			 
			}


		}
		else
		{
			WriteQueryLog("NO Contact !");
		}*/
	}

	db.Close();
	db2.Close();


   

}

void SendFixMail(CString DogId , CString Ipid)
{

	CDatabase db,db2;

	db.SetLoginTimeout(0); 
	db.SetQueryTimeout(0);

	db2.SetLoginTimeout(0); 
	db.SetQueryTimeout(0);

	CRecordset rec(&db); 

	db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 
	db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
	
	CRecordset rec2(&db2);

	CString sql;

	CString SubjectContent;
	CString MailContent;
	CString ReplaceContent;

	sql.Format("insert into errfixlog (DogId,IpId) values (%s,%s)",DogId,Ipid);
	db.ExecuteSQL(sql);	

	//sql.Format("SELECT top 50 c.Subject, c.type ,  convert(char(19),b.AddTime,20) , Sender FROM iptbl AS a, spammail AS b, subtbl AS c WHERE c.orgsid=b.spammailid And c.ip=a.ip and a.ipid=%s and Dogid = %s order by b.AddTime desc",Ipid,DogId);
	sql.Format("SELECT top 10 Subject, type ,  convert(char(19),AddTime,20) , Sender , IpId FROM subtbl (nolock)  WHERE ipid=%s and Dogid=%s and AddTime >= DATEADD(day, -14, getdate()) order by subid desc",Ipid,DogId);

	BOOL QueryOk = false;
	//try
	//{
	WriteQueryLog(sql.GetBuffer(sql.GetLength()));
	sql.ReleaseBuffer();

	   QueryOk = rec.Open(CRecordset::forwardOnly,sql);
	//}
	//catch(...)
	//{
	  // AfxMessageBox(sql);
	//}

	//if (QueryOk)
	//{

			CString Subject;
			CString MailType;
			CString AddTime;
			CString Sender;
			CString IpId;

			while(!rec.IsEOF())
			{
			 
				int BadRate = 0;
				int OkRate = 0;
				
				rec.GetFieldValue((int)0,Subject);
				rec.GetFieldValue((int)1,MailType);
				rec.GetFieldValue((int)2,AddTime);
				rec.GetFieldValue((int)3,Sender);
				rec.GetFieldValue((int)4,IpId);
				

				CString TypeStr;

				if (MailType.Find("0") == 0)
				{
					TypeStr = "垃圾郵件";
					BadRate = 1;
				}
				else
				{
					TypeStr = "正常郵件";
					OkRate = 1;
				}

				CString tmpReplaceContent;
				tmpReplaceContent.Format("<tr><td width=138><p align=center>&nbsp;%s</td><td>%s</td><td>%s</td><td width=100><p align=center>&nbsp;%s</td></tr>",AddTime,Sender,Subject,TypeStr);

				ReplaceContent += tmpReplaceContent ;

				//刪除 iptbl 的記錄
				//sql.Format("update iptbl set Bad = Bad - %d , Ok = Ok - %d , lastusedtime = getdate() where Ipid = %s",BadRate,OkRate,IpId);
				//db2.ExecuteSQL(sql);


				rec.MoveNext();				
			}


	//}

	rec.Close();
	

	sql.Format("select RateLogId , Ok ,Bad  from ratelog (nolock) where Ipid = %s and DogId= %s  and AtVersion = 0",Ipid,DogId);
	WriteQueryLog(sql.GetBuffer(sql.GetLength()));
	sql.ReleaseBuffer();

	if (rec.Open(CRecordset::forwardOnly,sql))
	{

			CString RateLogId;

			CDBVariant Ok;
			CDBVariant Bad;
			 

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,RateLogId);
				rec.GetFieldValue((int)1,Ok,SQL_C_SLONG);
				rec.GetFieldValue((int)2,Bad,SQL_C_SLONG);			 
				

				 
				//刪除 iptbl 的記錄
				sql.Format("update iptbl set Bad = Bad - %d , Ok = Ok - %d , lastusedtime = getdate() where Bad - %d >= 0 and ok - %d >= 0 and Ipid = %s",Bad.m_iVal,Ok.m_iVal,Bad.m_iVal,Ok.m_iVal,Ipid);
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();
				//try
				//{
				 db2.ExecuteSQL(sql);
				//}
				//catch(...)
				//{
				  //AfxMessageBox("SendFixMail 1");
				//}

				sql.Format("update ratelog set AtVersion = 1 where RateLogId = %s",RateLogId);
				//try
				//{
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();

			    db2.ExecuteSQL(sql);
				//}
				//catch(...)
				//{
				   //AfxMessageBox("SendFixMail 2");
				//}


				rec.MoveNext();				
			}


	}



	if (ReplaceContent.GetLength() > 0)
	{

		FILE *fp1=NULL;

		fp1 = fopen("c:\\DbConfirm.htm","rb");

		CString ReadContent;
		char buff[1025];
		while ( !feof( fp1 ))
		{
			int readsize = fread(buff,1,1024,fp1);
			buff[readsize] = 0;
			ReadContent += buff;
		}

		fclose(fp1);

		int pos1 = ReadContent.Find("<!--Replace1-->");
		int pos2 = ReadContent.Find("<!--Replace2-->");
		int pos3 = ReadContent.Find("<!--Replace3-->");

 

		MailContent = ReadContent.Mid(0,pos1);
		MailContent += ReplaceContent;
		MailContent += ReadContent.Mid(pos1+15,pos2-pos1-15);
		MailContent += Ipid;
		MailContent += ReadContent.Mid(pos2+15,pos3-pos2-15);
		MailContent += DogId;
		MailContent += ReadContent.Mid(pos3+15);

		FILE *fp2=NULL;
		fp2 = fopen("c:\\out.htm","w+b");
		fwrite(MailContent.GetBuffer(MailContent.GetLength()),1,MailContent.GetLength(),fp2);
		fclose(fp2);

		CString MailHeader;
		MailHeader = "from: 軟體工廠垃圾郵件中心 <service@softworking.com>\r\n" ;
 		MailHeader += "Subject: 信件誤判警告通知 !!\r\n";
 		MailHeader += "Content-Type: text/html\r\n\r\n";
 		 
		MailContent = MailHeader + MailContent;

	}
	

	if (MailContent.GetLength() > 0 )
	{
	

		CCompress cc;

		unsigned int outsize = cc.EstimateOutBuffSize(MailContent.GetLength());
		char *outbuff = new char [outsize];
		memset(outbuff,0,outsize);

	 
	
		HRESULT res = cc.DoCompress((unsigned char*)MailContent.GetBuffer(MailContent.GetLength()),MailContent.GetLength(),(unsigned char *) outbuff,&outsize);
		if (res == S_OK || NO_NEED_COMPRESS)	
		{
			CMailCodec MailCodec;
			char *EncodeStr = MailCodec.Base64Encode2(outbuff,outsize);

			CString sql;
			sql.Format("insert into mail (IsCompress,OrgLen,ThisLen,MailContent) values (1,%d,%d,'%s')",MailContent.GetLength(),strlen(EncodeStr),EncodeStr);
			WriteQueryLog(sql.GetBuffer(sql.GetLength()));
			sql.ReleaseBuffer();

			db2.ExecuteSQL(sql);

			sql = "select Max(MailId)  from mail";

			WriteQueryLog(sql.GetBuffer(sql.GetLength()));
			sql.ReleaseBuffer();

			if (rec2.Open(CRecordset::forwardOnly ,sql))
			{
				if (!rec2.IsEOF())
				{
					CString MailId;
					rec2.GetFieldValue((int)0,MailId);

					//try
					//{
					CString sql;
					sql.Format("insert into pmail (MailId , ToId) values (%s,1)",MailId);					
					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();

					db2.ExecuteSQL(sql);

					sql.Format("insert into pmail (MailId , ToId) values (%s,2)",MailId);

					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();
					db2.ExecuteSQL(sql);

					sql.Format("insert into pmail (MailId , ToId) values (%s,%s)",MailId,DogId);
					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();
					db2.ExecuteSQL(sql);
					//}
					//catch(...)
					//{
						//AfxMessageBox("SendFixMail 3");
					//}
				}

			}
		 
     		
     	 

			MailCodec.FreeBase64Encode(EncodeStr); 
		}
 


 
		delete [] outbuff;

	
	}


	MailContent.ReleaseBuffer();

		db.Close();
		db2.Close();


}

void TrimCntrl(char* InStr)
{

	int len = strlen(InStr);
	int Count = 0;
 

	for(int i = 0 ; i < len ; i++)
	{
	
		if ((!iscntrl(InStr[i]) || InStr[i] > 127 || InStr[i] < 0) && InStr[i] != '\'')
		{
				InStr[Count] = InStr[i];
				Count++;		
		}
	
	}


	InStr[Count] = 0;

}


void IpReserve(char *IP)
{

		int len = strlen(IP);
		char Zone1[4];
		char Zone2[4];
		char Zone3[4];
		char Zone4[4];

		memset(Zone1,0,4);
		memset(Zone2,0,4);
		memset(Zone3,0,4);
		memset(Zone4,0,4);

		int ZoneIdx = 0;
		int SavePos = 0;

		for(int i= 0 ; i < len ; i++)
		{
			if (IP[i] == '.')
			{
			
				if (ZoneIdx == 0)
				{					
					strncpy(Zone4,IP + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 1)
				{
					strncpy(Zone3,IP + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 2)
				{
					strncpy(Zone2,IP + SavePos, i - SavePos);
					strncpy(Zone1,IP + i + 1, len-i);
					 
					break;
				}
			 

				ZoneIdx ++;
				SavePos = i + 1;

			}
		}

		strcpy(IP,Zone1);
		strcat(IP,".");
		strcat(IP,Zone2);
		strcat(IP,".");
		strcat(IP,Zone3);
		strcat(IP,".");
		strcat(IP,Zone4);
	 

}



void ConvertMail()
{

  int RBL_BASE = 3;

  CoInitialize(NULL);

  CDatabase db,db2,db3,db4,db5;

  db.SetLoginTimeout(0);
  db2.SetLoginTimeout(0);
  db3.SetLoginTimeout(0);
  db4.SetLoginTimeout(0);
  db5.SetLoginTimeout(0);

  db.SetQueryTimeout(0);
  db2.SetQueryTimeout(0);
  db3.SetQueryTimeout(0);
  db4.SetQueryTimeout(0);
  db5.SetQueryTimeout(0);


  db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  //db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

  db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  db3.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  db4.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  db5.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

  CRecordset rec(&db);
  CRecordset rec2(&db5);

  


  	_ConnectionPtr mConnection ; 
	_CommandPtr mCommand ;

	  SHELLEXECUTEINFO sei; 
	  ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO)); 
	  sei.cbSize = sizeof(SHELLEXECUTEINFO ); 

	  sei.lpFile = "c://dnscmd.exe"; 
	 
	
	  sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	  sei.nShow = SW_SHOWNORMAL; 

	

	

	char AdoConnStr[255];
	strcpy(AdoConnStr,"Provider=MSDASQL.1;Password=dog1234;Persist Security Info=True;User ID=dog;Data Source=SpamDog2;Initial Catalog=SpamDog2");



	HRESULT hr;
	hr = mConnection.CreateInstance(__uuidof(Connection));	
	if(SUCCEEDED(hr))
	{
		
		mConnection->ConnectionTimeout = 0;
		mConnection->CommandTimeout = 0;

		hr = mConnection->Open(AdoConnStr,"","",adModeUnknown);
		if(SUCCEEDED(hr))
		{
			mCommand.CreateInstance(__uuidof(Command));
			mCommand->ActiveConnection = mConnection;
			 
		}
	}


   BOOL QueryOk = false;
   //try
   //{
      CString sql;
	  sql.Format("select SpamMailId,MailContent,OrgLen,isCompress,addtime,DogId from spammail (nolock) where  OrgLen > 0 and AtVersion = 0");
	  WriteQueryLog(sql.GetBuffer(sql.GetLength()));
	  sql.ReleaseBuffer();

	  QueryOk = rec.Open(CRecordset::forwardOnly,sql.GetBuffer(sql.GetLength()));
	  sql.ReleaseBuffer();
   //}
   //catch(...)
   //{
	 // AfxMessageBox("Big Query Error");
   //}

  // if (QueryOk)
   //{
	   while (!rec.IsEOF())
	   {
	   
		   char *AllMailContent = NULL;
		   bool NeedSendFixMail = false;
		   CString SpamMailId;
		   CString MailContent;
		   CString orglenstr;
		   CString isCompress;
		   CString AddTime;
		   CString DogId;
		   CString gIpId;

		   CString SaveIpId;

		   rec.GetFieldValue((int)0,SpamMailId);
		   rec.GetFieldValue((int)1,MailContent);
		   rec.GetFieldValue((int)2,orglenstr);
		   rec.GetFieldValue((int)3,isCompress);
		   rec.GetFieldValue((int)4,AddTime);
		   rec.GetFieldValue((int)5,DogId);



		   CString ReportMail;
		   ReportMail = "";

		   CMailCodec MailCodec;

		   char *debase = NULL;
		   unsigned int sourcelen;
		   char *sourcestr = MailContent.GetBuffer(MailContent.GetLength());		 
	 
		   MailCodec.Base64Decode2(sourcestr,&debase,&sourcelen);

		   ReportMailData maildata;
		   memset(&maildata,0,sizeof(maildata));

				if (isCompress == "1")
				{
			
					unsigned int outsize = atoi(orglenstr);
					char *outbuf = new char[outsize +1];
					memset(outbuf,0,outsize +1);
			
			
					CCompress cc;
					cc.DeCompress((unsigned char*) debase,sourcelen,(unsigned char*) outbuf,&outsize);
		

					
					memcpy(&maildata,outbuf,sizeof(maildata));

					AllMailContent =  new char[outsize - sizeof(maildata) + 1];
					AllMailContent[outsize - sizeof(maildata)] = 0;
					memcpy(AllMailContent , outbuf + sizeof(maildata) , outsize - sizeof(maildata));


					
					//ParseStr(&m_list,outbuf);
				 
					FILE *fp=NULL;

					fp = fopen("c:\\ProcessMail.eml","w+b");
					fwrite(outbuf + sizeof(maildata) ,1,outsize - sizeof(maildata),fp);
					fclose(fp);

					delete outbuf;

				}
				else
				{
			
					//ParseStr(&m_list,debase);

					//ReportMailData maildata;
					//memset(&maildata,0,sizeof(maildata));
					memcpy(&maildata,debase,sizeof(maildata));

					
				 
			
				}


			if (maildata.Type == 1)
			{
				//處理 sys report mail , update Atversion = 2
				CString query;
				query.Format("update spammail set AtVersion = 2 where SpamMailId=%s",SpamMailId);
				WriteQueryLog(query.GetBuffer(query.GetLength()));
				query.ReleaseBuffer();

				mConnection->Execute(query.AllocSysString(),NULL,NULL);
			} 
			//else if (maildata.Type == 2)
			//{//ok mail
			
			//}
			else
			if (strlen(maildata.Sender) > 0 && strlen(maildata.Subject) > 0 && strlen(maildata.IP) > 0 && (maildata.Type == 0  || maildata.Type == 2))
			{
				if (mCommand->Parameters->Count > 0)
				{
					mCommand->Parameters->Delete("Subject");
					mCommand->Parameters->Delete("Sender");
				}

				TrimCntrl(maildata.Subject);
				TrimCntrl(maildata.Sender);

				//if (maildata.Type == 2) 

					//處理 ip

					int OkRate = 0;
					int BadRate = 0;

					if (maildata.Type == 0)
					{
					    BadRate = 1;
					}
					else
					{
						
						OkRate = 1;
					}

					CString sql;
					CRecordset rec2(&db2);
					sql.Format("select IpId,Ok,Bad,ReportMail from iptbl (nolock) where Ip = '%s'",maildata.IP);

					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();

					if (rec2.Open(CRecordset::forwardOnly,sql))
					{
						  if(!rec2.IsEOF())
						  {
							    //確認誤判
							    
							    CString IpId;
								CString sql;
								

								rec2.GetFieldValue((int)0,IpId);
								rec2.GetFieldValue((int)3,ReportMail);

								SaveIpId = IpId;

								CRecordset rec4(&db4);
								sql.Format("select top 1 RateLogId from ratelog (nolock) where Ipid = %s and DogId= %s and (ok > %d or bad > %d) and AtVersion = 0",IpId,DogId,OkRate,BadRate);

								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								if (rec4.Open(CRecordset::forwardOnly,sql))
								{
									if(!rec4.IsEOF())
									{
										//可能有誤判
										CString RateLogId;
										rec4.GetFieldValue((int)0,RateLogId);

										sql.Format("select SubId from subtbl (nolock)  where DogId=%s and ipid = %s and subject = ? and sender = ?  and  AddTime >= DATEADD(day, -7, getdate())",DogId,IpId);

										WriteQueryLog(sql.GetBuffer(sql.GetLength()));
										sql.ReleaseBuffer();

										mCommand->CommandText  = sql.AllocSysString();
										mCommand->CommandType = adCmdText;
					
										_ParameterPtr p1 = mCommand->CreateParameter("Subject",adChar ,adParamInput,strlen(maildata.Subject),maildata.Subject);	
										mCommand->Parameters->Append(p1);

										_ParameterPtr p2 = mCommand->CreateParameter("Sender",adChar ,adParamInput,strlen(maildata.Sender),maildata.Sender);	
										mCommand->Parameters->Append(p2);
										
										_RecordsetPtr mRecordset;

										//try
										//{										
											mRecordset = mCommand->Execute(NULL,NULL,adCmdText);
										//}
										//catch(...)
										//{
											//AfxMessageBox(adCmdText);
										//}

										    if (!mRecordset->EndOfFile)
											{

										//CRecordset rec5(&db3);
										//sql.Format("select SubId from subtbl (nolock)  where DogId=%s and ipid = %s and sender = '%s' and subject = '%s' and  AddTime > DATEADD(day, -7, getdate())",DogId,IpId,maildata.Sender,maildata.Subject);
									
										//if (rec5.Open(CRecordset::forwardOnly,sql))
										//{
											//if(!rec5.IsEOF())
											//{
												//誤判
												CString SubId;
												SubId = (char *) (_bstr_t) mRecordset->Fields->GetItem("SubId")->Value;
												//rec5.GetFieldValue((int)0,SubId);
												//SubId = mRecordset->Fields->GetItem("ident")->Value.SetString(; 

												//刪除 ratelog
												sql.Format("update ratelog set AtVersion = 1 where RateLogId = %s",RateLogId);												
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												
												db5.ExecuteSQL(sql);
												sql.ReleaseBuffer();
												//刪除 subtbl
												sql.Format("delete from subtbl where SubId = %s",SubId);
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												
												db5.ExecuteSQL(sql);
												sql.ReleaseBuffer();
												//更正 iptbl
												sql.Format("update iptbl set Bad = Bad - %d , Ok = Ok - %d , lastusedtime = getdate() where Bad - %d >=0 and Ok - %d >= 0 and Ipid = %s",BadRate,OkRate,BadRate,OkRate,IpId);
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												
												db5.ExecuteSQL(sql);
												sql.ReleaseBuffer();

												
												
												


											}
											else
											{
												NeedSendFixMail = true;
												gIpId = IpId;
												
											}
											//else
											//{											
												//沒誤判偏修
												CString sql2;
												sql2.Format("insert into ratelog (Ok,Bad,DogId,SpamMailId,Ipid) values (%d,%d,%s,%s,%s)",OkRate,BadRate,DogId,SpamMailId,IpId);
												
												WriteQueryLog(sql2.GetBuffer(sql2.GetLength()));
												sql2.ReleaseBuffer();

												db5.ExecuteSQL(sql2);

												sql.Format("update iptbl set Bad = Bad + %d , Ok = Ok + %d , lastusedtime = getdate() where Ipid = %s",BadRate,OkRate,IpId);
												
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												sql.ReleaseBuffer();

												db5.ExecuteSQL(sql);

												
											//}
										//}

												if (mCommand->Parameters->Count > 0)
												{
													mCommand->Parameters->Delete("Subject");
													mCommand->Parameters->Delete("Sender");
												}
									}
									else
									{
									
										CString sql2;
										sql2.Format("insert into ratelog (Ok,Bad,DogId,SpamMailId,Ipid) values (%d,%d,%s,%s,%s)",OkRate,BadRate,DogId,SpamMailId,IpId);
										WriteQueryLog(sql2.GetBuffer(sql2.GetLength()));
										sql2.ReleaseBuffer();

										db5.ExecuteSQL(sql2);

										sql.Format("update iptbl set Bad = Bad + %d , Ok = Ok + %d , lastusedtime = getdate() where Ipid = %s",BadRate,OkRate,IpId);
										WriteQueryLog(sql.GetBuffer(sql.GetLength()));
										sql.ReleaseBuffer();

										db5.ExecuteSQL(sql);

									
									}


								}
							
  
						  }
						  else
						  {
						  
							    //找不到 ip
								CRecordset rec3(&db3);

							    CString sql;
								sql.Format("insert into iptbl (Ip,Ok,Bad,lastusedtime) values ('%s',%d,%d,getdate())",maildata.IP,OkRate,BadRate);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								db5.ExecuteSQL(sql);

								

								sql.Format("select ipid from iptbl where ip = '%s'",maildata.IP);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								if (rec3.Open(CRecordset::forwardOnly,sql))
								{
									if(!rec3.IsEOF())
									{

										CString IpId;
										rec3.GetFieldValue((int)0,IpId);
										SaveIpId = IpId;

										CString sql2;
										sql2.Format("insert into ratelog (Ok,Bad,DogId,SpamMailId,Ipid) values (%d,%d,%s,%s,%s)",OkRate,BadRate,DogId,SpamMailId,IpId);
										WriteQueryLog(sql2.GetBuffer(sql2.GetLength()));
										sql2.ReleaseBuffer();

										db5.ExecuteSQL(sql2);



									}
								}

								rec3.Close();

							 

						  
						  }

						  rec2.Close();
					}



		
				//CString Sql;
				sql.Format("insert into subtbl (Subject,IP,Sender,orgsid,ReportTime,Type,DogId , IpId) values (?,'%s',?,%s,'%s',%d,%s,%s)",maildata.IP,SpamMailId,AddTime,maildata.Type,DogId,SaveIpId);
				//mCommand->CommandText = "insert into subtbl (Subject,IP,TimeStr) values (?,?,?)";

				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();
			
		
			

				if (strlen(maildata.Subject) > 0 && strlen(maildata.Sender) > 0)
				{
					mCommand->CommandText  = sql.AllocSysString();
					mCommand->CommandType = adCmdText;
					
					_ParameterPtr p1 = mCommand->CreateParameter("Subject",adChar ,adParamInput,strlen(maildata.Subject),maildata.Subject);	
					mCommand->Parameters->Append(p1);

					_ParameterPtr p2 = mCommand->CreateParameter("Sender",adChar ,adParamInput,strlen(maildata.Sender),maildata.Sender);	
					mCommand->Parameters->Append(p2);

				
			 
				
					//try
					//{
					  mCommand->Execute(NULL,NULL,adCmdText);
					//}
					//catch(...)
					//{
					//	AfxMessageBox(Sql);
					//}
				}

				CString query;
				query.Format("update spammail set AtVersion = 1 where SpamMailId=%s",SpamMailId);
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();
				//try
				//{
				  mConnection->Execute(query.AllocSysString(),NULL,NULL);
				//}
				//catch(...)
				//{
					//AfxMessageBox(query);
				
				//}
			

			}
			else
			{
				CString query;
				query.Format("update spammail set AtVersion = 1 where SpamMailId=%s",SpamMailId);
				WriteQueryLog(query.GetBuffer(query.GetLength()));
				query.ReleaseBuffer();
				//try
				//{
				  mConnection->Execute(query.AllocSysString(),NULL,NULL);
				//}
				//catch(...)
				//{
					//AfxMessageBox(query);
				//}
			
			
			}

			
				//db2.ExecuteSQL(query);
			
				MailContent.ReleaseBuffer(); 
				MailCodec.FreeBase64Decode(debase);

				if (NeedSendFixMail)
				{
					//try
					//{
					 SendFixMail(DogId,gIpId);
					//}
					//catch(...)
					//{
						//AfxMessageBox("SendFixMail");					
					//}
				}

				if (!NeedSendFixMail && strlen(maildata.Sender) > 0 && strlen(maildata.IP) > 0 && strlen(maildata.Subject) > 0 && (maildata.Type == 0  || maildata.Type == 2))
				{

						char Rip[20];
						strcpy(Rip,maildata.IP);				
						IpReserve(Rip);

						//確認是否加入 RBL
						CString sql;
						CRecordset rec2(&db2);

						sql.Format("select ipid from iptbl where (Bad - Ok >= %d) and ipid = %s ",RBL_BASE,SaveIpId);
						WriteQueryLog(sql.GetBuffer(sql.GetLength()));
						sql.ReleaseBuffer();

						if (rec2.Open(CRecordset::forwardOnly,sql))
						{
							if (!rec2.IsEOF())
							{
								//要加入 rbl
								//dnscmd localhost /RecordAdd test.com www.my A 10.22.22.22

								char Cmd[255];
								strcpy(Cmd,"192.168.1.3 /RecordAdd rbl.softworking.com ");
								strcat(Cmd,Rip);
								strcat(Cmd," A 127.0.0.5");
								//WinExec(Cmd,SW_SHOW);
								//ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW);
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

								WriteQueryLog(Cmd);

								strcpy(Cmd,"192.168.1.3 /RecordAdd rbl.tw ");
								strcat(Cmd,Rip);
								strcat(Cmd," A 127.0.0.5");
								//WinExec(Cmd,SW_SHOW);
								//ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW);
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

								WriteQueryLog(Cmd);

								sql.Format("update iptbl set RblStatus = 1 where ipid = %s",SaveIpId);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();
								db5.ExecuteSQL(sql);

							}
							rec2.Close();
						}

						
						sql.Format("select ipid from iptbl where (Bad - Ok < %d) and ipid = %s ",RBL_BASE,SaveIpId);
						WriteQueryLog(sql.GetBuffer(sql.GetLength()));
						sql.ReleaseBuffer();

						if (rec2.Open(CRecordset::forwardOnly,sql))
						{
							if (!rec2.IsEOF())
							{
								//要拉出 rbl
								//dnscmd localhost /RecordDelete test.com www.my A 

								char Cmd[255];
								strcpy(Cmd,"192.168.1.3 /RecordDelete rbl.softworking.com ");
								strcat(Cmd,Rip);
								strcat(Cmd," A /f");
								//WinExec(Cmd,SW_SHOW);
								//ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW);
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

								WriteQueryLog(Cmd);

								strcpy(Cmd,"192.168.1.3 /RecordDelete rbl.tw ");
								strcat(Cmd,Rip);
								strcat(Cmd," A /f");
								//WinExec(Cmd,SW_SHOW);
								//ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW);
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

								WriteQueryLog(Cmd);

								sql.Format("update iptbl set RblStatus = 0 where ipid = %s",SaveIpId);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								db5.ExecuteSQL(sql);

							}

							rec2.Close();
						}

							//寫入 db
						/*if (AllMailContent != NULL)
						{
							CChecker2 cc;
							cc.GetSpamRate(maildata.Sender,maildata.Subject,AllMailContent,maildata.IP);
							if (maildata.Type == 0)
							{
								cc.UpdateSpamRate(0,1);
							}
							else
							{								
								cc.UpdateSpamRate(1,0);
							}
							
						}*/


					


				}


			 


			//確認 Report ISP
			// if ((DogId == "2" || DogId == "1" ) && maildata.Type == 0)
			 //{
				  //rec2.Close();  
				  //sql.Format("select IP from iptbl (nolock) where (Bad - Ok >= 2) and ip ='%s'",maildata.IP);
				  //WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				  //sql.ReleaseBuffer();

				  //if (rec2.Open(CRecordset::forwardOnly,sql))
				  //{
					//   if (!rec2.IsEOF())
					  // {   
						  
						//   CString IP;				 

						  // rec2.GetFieldValue((int)0,IP);	

				if (ReportMail != "" && !NeedSendFixMail && maildata.Type == 0)
				{
						//Direct Send
					sql.Format("insert into ReportIsp (SpammailId,Ip) values (%s,'%s')",SpamMailId,maildata.IP);
					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					db5.ExecuteSQL(sql);

					//WriteQueryLog("SendReportFile !");
					//SendReportFile(maildata.IP,ReportMail, SpamMailId);

				} else
				if (!NeedSendFixMail && maildata.Type == 0)
				{
						  
					rec2.Close();  
				    sql.Format("select IP from iptbl (nolock) where (Bad - Ok >= %d) and ip ='%s'",RBL_BASE,maildata.IP);
				    WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				    sql.ReleaseBuffer();

				    if (rec2.Open(CRecordset::forwardOnly,sql))
					{
					    if (!rec2.IsEOF())
						{   
							SendToIsp(maildata.IP,SpamMailId);						   
						}

						rec2.Close();
					}

				}

				
			

						   //IP.ReleaseBuffer();
 
					   //}

					   //rec2.Close();  
					  
				  //}
			 //}



	       if (AllMailContent != NULL)
		   {
			   delete [] AllMailContent;
			   AllMailContent = NULL;
		   }

		   rec.MoveNext();

	   }

   //}

	
//掃全部的 block ip

  
  rec.Close();
  //CString sql;
  sql.Format("select IP,ipid from iptbl (nolock) where (Bad - Ok >= %d)  and RblStatus = 0",RBL_BASE);

  WriteQueryLog(sql.GetBuffer(sql.GetLength()));
  sql.ReleaseBuffer();

  if (rec.Open(CRecordset::forwardOnly,sql))
  {
	   while (!rec.IsEOF())
	   {   
		  
		   CString IP;		   
		   CString IPid;

		   rec.GetFieldValue((int)0,IP);
		   rec.GetFieldValue((int)1,IPid);

		   char Rip[20];
		   strcpy(Rip,IP);				
		   IpReserve(Rip);

		   char Cmd[255];
		   strcpy(Cmd,"192.168.1.3 /RecordAdd rbl.softworking.com ");
		   strcat(Cmd,Rip);
		   strcat(Cmd," A 127.0.0.5");
		   //WinExec(Cmd,SW_SHOW);
		   //ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW);
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

		   WriteQueryLog(Cmd);

		   strcpy(Cmd,"192.168.1.3 /RecordAdd rbl.tw ");
		   strcat(Cmd,Rip);
		   strcat(Cmd," A 127.0.0.5");
		   //WinExec(Cmd,SW_SHOW);
		   //ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW);
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

		   WriteQueryLog(Cmd);

		   sql.Format("update iptbl set RblStatus = 1 where ipid = %s",IPid);
		   WriteQueryLog(sql.GetBuffer(sql.GetLength()));
		   sql.ReleaseBuffer();

		   db2.ExecuteSQL(sql);

		rec.MoveNext();


	   }
	  
  }


  rec.Close();
  //CString sql;
  sql.Format("select IP,ipid from iptbl (nolock) where (Bad - Ok < %d)  and RblStatus = 1",RBL_BASE);

  WriteQueryLog(sql.GetBuffer(sql.GetLength()));
  sql.ReleaseBuffer();

  if (rec.Open(CRecordset::forwardOnly,sql))
  {
	   while (!rec.IsEOF())
	   {   
		  
		   CString IP;		   
		   CString IPid;

		   rec.GetFieldValue((int)0,IP);
		   rec.GetFieldValue((int)1,IPid);

		   char Rip[20];
		   strcpy(Rip,IP);				
		   IpReserve(Rip);

		   char Cmd[255];
		   strcpy(Cmd,"192.168.1.3 /RecordDelete rbl.softworking.com ");
								strcat(Cmd,Rip);
								strcat(Cmd," A /f");
								//WinExec(Cmd,SW_SHOW);
		   //ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW); 
								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

		   WriteQueryLog(Cmd);

		   strcpy(Cmd,"192.168.1.3 /RecordDelete rbl.tw ");
								strcat(Cmd,Rip);
								strcat(Cmd," A /f");
								//WinExec(Cmd,SW_SHOW);
		   //ShellExecute(NULL,"open","c://dnscmd.exe",Cmd,"",SW_SHOW); 

								sei.lpParameters = Cmd;
								if(ShellExecuteEx(&sei)) 
								{ 
									WaitForSingleObject(sei.hProcess, INFINITE); 
								}				   
								CloseHandle(sei.hProcess);

		   WriteQueryLog(Cmd);


		   sql.Format("update iptbl set RblStatus = 0 where ipid = %s",IPid);
		   WriteQueryLog(sql.GetBuffer(sql.GetLength()));
		   sql.ReleaseBuffer();

		   db2.ExecuteSQL(sql);

		rec.MoveNext();


	   }
	  
  }
  

     

  db.Close();
  
  mConnection->Close();

  db2.Close();
  db3.Close();
  db4.Close();
  db5.Close();

  //沒問題就清 log
  //DeleteFile("c:\\sql.txt");
  /*FILE *fp;
  fp=fopen("c:\\sql.txt","w+b");
  fclose(fp);*/

  CoUninitialize();

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
		//ConvertMail();

		/*if (argc == 2)
		{
			int SpamMailId = atoi(argv[1]);


			CString str;
			str.Format("Process..SpamMailId : %d \n",SpamMailId);
		
		
			//cout << str.GetBuffer(str.GetLength()) << endl;
			//str.ReleaseBuffer();

			WriteQueryLog(str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();

			ConvertMail();
				


			
		}*/

		//while(1)
		//{
			//try
			//{
				//ConvertMail();


				  /* SHELLEXECUTEINFO sei; 
				   ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO)); 
				   sei.cbSize = sizeof(SHELLEXECUTEINFO ); 

				   sei.lpFile = "c://SMTPservice.exe"; 
				   // attention ce flag est important ici !
				   sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
				  // sei.lpVerb = TEXT("print"); 
				   sei.nShow = SW_SHOWNORMAL; 
				   if(ShellExecuteEx(&sei)) 
				   { 
					  WaitForSingleObject(sei.hProcess, INFINITE); 
				   }
				   
				   CloseHandle(sei.hProcess);*/


			 

				//ShellExecute(NULL,"open","c://SMTPservice.exe","","",SW_SHOW);


			//}
			//catch (CDBException* pEx)
			//{
			//	TCHAR buff[80];
				//pEx->GetErrorMessage(buff,80);
				//AfxMessageBox(buff);
				//pEx->Delete();
			//}

		//	Sleep(1000 * 60);
		//}
	 
	 
		//getch();


	   FILE * fp;
		   fp = fopen("c:\\MaiAAC9.tmp","rb");

		   fseek(fp,0,SEEK_END);
		   int FileSize=ftell(fp);
		   fseek(fp,0,SEEK_SET);

		   char* mailContent = new char[FileSize+1];
		   char* ParseContent = new char[FileSize+1];
		   char From[255];
		   char Subject[255];

		   ParseContent[0] = 0;
			

		   fread(mailContent,sizeof(char),FileSize,fp);   
		   mailContent[FileSize] = 0;

		   CEmail mailp;
		   MailData *mdata = mailp.GetMailData(mailContent);

		   MailHeader *mheader = mailp.GetMailHeader(mdata->mailHeader);

		   strcpy(From,mheader->From);
		   strcpy(Subject,mheader->Subject);

		   if (mheader->Content_Type->boundary[0] != 0)
		   {
			//代表有 boundary

			   MailBoundary* m_MailBoundary = 
			   mailp.GetBoundary(mdata->mailBody,mheader->Content_Type->boundary);
			   
			   MailBoundary* ptr =  m_MailBoundary;

				char tmpc[500];
				int BoundCount = 0;
				while (ptr)
				{
					if (ptr->IsRealData)
					{
						BoundCount ++ ;	

						 
					
						strncpy(tmpc,ptr->BoundContent,50);

						if (stricmp(ptr->BoundHeader->Content_Transfer_Encoding,"base64") == 0)
						{
						
							CMailCodec cm;
							char *out = NULL;
							unsigned int size=0;
							
							cm.Base64Decode(ptr->BoundMail->mailBody,&out,&size);

							printf("%s\n",out);
							strcat(ParseContent,out);

							cm.FreeBase64Decode(out);
						
						}
						else if (stricmp(ptr->BoundHeader->Content_Transfer_Encoding,"quoted-printable") == 0)
						{
						
								CMailCodec cm;
								char *out = cm.QPDecode(ptr->BoundMail->mailBody);

								printf("%s\n",out);
								strcat(ParseContent,out);

								cm.FreeQPDecode(out);
						
						}

						tmpc[499] = 0;
						printf("%s\n",tmpc);					
						printf("level : %d %s=>%s+++++++++++++++++++++++++++++++++++++++++\n\n\n",ptr->level,ptr->BoundHeader->Content_Type->text,ptr->BoundHeader->Content_Transfer_Encoding);


					}
 
					ptr = ptr->next;	
				}

				mailp.FreeBoundary(m_MailBoundary);


		   
		   }

		   

		   mailp.FreeMailHeader(mheader);
		   mailp.FreeMailData(mdata);
    
   

		   fclose(fp);

		  
		  
		   delete mailContent;


		   //printf("%s",ParseContent);

		   //CChecker2 cc;

		   //double rate = cc.GetSpamRate(From,Subject,ParseContent,NULL);		   
		   //cc.UpdateSpamRate(0,1);

		   //printf("%f\n",rate);





		   delete ParseContent;

	  

	
	}

	getch();

	return nRetCode;
}


