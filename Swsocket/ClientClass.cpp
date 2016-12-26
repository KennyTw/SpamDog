#include "stdafx.h"
#include "ClientClass.h"
 
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>


////////////////////////////////////////////////////////
int wpos(const wchar_t* InStr , const wchar_t* FindStr , int Offset)
{

	wchar_t *findres = NULL;
	
	int iResult = -1;
	
	findres = wcsstr(InStr + Offset,FindStr);
	if (findres != NULL)
	{
		iResult = (int) (findres - InStr) ;
	
	}
	return iResult;
}

int pos(const char* InStr , const char* FindStr , int Offset)
{

	char *findres = NULL;
	
	int iResult = -1;
	
	findres = strstr(InStr + Offset,FindStr);
	if (findres != NULL)
	{
		iResult = (int) (findres - InStr) ;
	
	}
	return iResult;
}

 

void SetNullString(char** AStr)
{
    *AStr = new char[1];	
	*AStr[0] = 0;
}

///
//// CDNSclient
//////////////////////////////////////////////////////////////////////////////

CDNSclient::CDNSclient()
{
  FID=FBitCode=FQDCount=0;
  m_QueryResult[0] = 0;
  
}

CDNSclient::~CDNSclient()
{}

HANDLE CDNSclient::Resolve(char* DnsServer , char *Domain , int ResType )
{
	
	m_Domain = Domain;
	m_ResType = ResType;
	//m_timeout = timeout;

	return   Connect(IPPROTO_UDP,DnsServer,53,0,1 * 1500);
	 
}

char* CDNSclient::GetQueryResult()
{

	return m_QueryResult;

}

void CDNSclient::ParseStr(char *DomainStr , int* index ,int DataLen, char* ResultStr)
{

    int len = 0; 
	int SaveIdx = 0;

	char tmpstr[255];
	ResultStr[0] = 0;
 
	do 
	{
		len = *(DomainStr + *index);

		//有壓縮的資料
		while ((len & 0xC0) == 0xC0 ) 
		{
		   if (SaveIdx == 0) SaveIdx = (*index)+1;

		   char tmpchar = char(len & 0x3F); 

		   WORD newidx = 0;
		   newidx = (tmpchar << 8) & 0xFF00 | *(DomainStr + *index + 1) & 0x00FF;
		   //newidx ++;

		   *index = newidx;		
		   len = *(DomainStr + *index);
		}

		if (len > 0)
		{
		    strncpy(tmpstr,DomainStr + *index + 1,len);
			tmpstr[len] = 0;

			*index = *index + len + 1;
		}

		strcat(ResultStr,tmpstr);
		strcat(ResultStr,".");
	
	} 
	while (*(DomainStr + *index) != 0 && *index < DataLen);

	int ResLen = strlen(ResultStr);
	if (ResultStr[ResLen-1] == '.')
		ResultStr[ResLen-1] = 0;

	if (SaveIdx > 0) *index = SaveIdx; // restore original Idx +1
    *index = *index + 1; // set to first char of next item in  the resource


	

}


void CDNSclient::ParseDNS(char *DomainStr , int* index , int *NSType, int* Prefer, char* RRDomain, int DataLen, char* ResultStr)
{

	char RRName[255];
	char tmpstr[1024];
	RRDomain[0] = 0; 

	ParseStr(DomainStr,index,DataLen,RRName);

	WORD RR_Type =  (*(DomainStr + *index) << 8) & 0xFF00 | *(DomainStr + *index + 1) & 0x00FF;
	WORD RR_Class = (*(DomainStr + *index + 2) << 8) & 0xFF00 | *(DomainStr + *index + 3) & 0x00FF;
	int RR_TTL =  (*(DomainStr + *index + 4) << 24) & 0xFF000000 |
		          (*(DomainStr + *index + 5) << 16) & 0x00FF0000 |
				  (*(DomainStr + *index + 6) << 8) & 0x0000FF00 |
				  (*(DomainStr + *index + 7) ) & 0x000000FF;

	WORD RD_Length = (*(DomainStr + *index + 8) << 8) & 0xFF00 | *(DomainStr + *index + 9) & 0x00FF;

	*NSType = RR_Type;
 

	if (RR_Type == qtMX)
	{
	 
		int tmplen = *index + 9  + RD_Length + 1 ;
		memcpy(tmpstr,DomainStr,tmplen);

		WORD Preference = (*(DomainStr + *index + 10) << 8) & 0xFF00 | *(DomainStr + *index + 11) & 0x00FF;
 
		int i = *index + 12;
		ParseStr(tmpstr,&i,tmplen,ResultStr);

		*Prefer = Preference;	
	
	} else if (RR_Type == qtA)
	{
	   if (RD_Length > 0)
	   {
		   WORD ip1 = (*(DomainStr + *index + 10) & 0xFF00) << 8 | *(DomainStr + *index + 10) & 0x00FF;
		   WORD ip2 = (*(DomainStr + *index + 11) & 0xFF00) << 8 | *(DomainStr + *index + 11) & 0x00FF;
		   WORD ip3 = (*(DomainStr + *index + 12) & 0xFF00) << 8 | *(DomainStr + *index + 12) & 0x00FF;
		   WORD ip4 = (*(DomainStr + *index + 13) & 0xFF00) << 8 | *(DomainStr + *index + 13) & 0x00FF;
		   
		   sprintf(ResultStr,"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
		                                   
	   }
            
        
        strcpy(RRDomain,RRName);

	} else if (RR_Type == qtNS || RR_Type == qtPTR)
	{
		int tmplen = *index + 9  + RD_Length + 1 ;
		memcpy(tmpstr,DomainStr,tmplen);

		int i = *index + 10;
		ParseStr(tmpstr,&i,tmplen,ResultStr);
	
	} else if (RR_Type == qtSOA)
	{
	    int tmplen = *index + 9  + RD_Length + 1 ;
		memcpy(tmpstr,DomainStr,tmplen);

		int i = *index + 10;

		char FMNAME[255];
		ParseStr(tmpstr,&i,tmplen,FMNAME);

		*index = i;
		char FRNAME[255];
		ParseStr(tmpstr,index,tmplen,FRNAME);

		int FSerial =  (*(tmpstr + *index) << 24) & 0xFF000000 |
					   (*(tmpstr + *index + 1) << 16) & 0x00FF0000 |
				       (*(tmpstr + *index + 2) << 8) & 0x0000FF00 |
				       (*(tmpstr + *index + 3) ) & 0x000000FF;


		*index = *index + 4;
		int FRefresh =  (*(tmpstr + *index) << 24) & 0xFF000000 |
					   (*(tmpstr + *index + 1) << 16) & 0x00FF0000 |
				       (*(tmpstr + *index + 2) << 8) & 0x0000FF00 |
				       (*(tmpstr + *index + 3) ) & 0x000000FF;

		*index = *index + 4;
		int FRetry =  (*(tmpstr + *index) << 24) & 0xFF000000 |
					   (*(tmpstr + *index + 1) << 16) & 0x00FF0000 |
				       (*(tmpstr + *index + 2) << 8) & 0x0000FF00 |
				       (*(tmpstr + *index + 3) ) & 0x000000FF;


		*index = *index + 4;
		int FExpire =  (*(tmpstr + *index) << 24) & 0xFF000000 |
					   (*(tmpstr + *index + 1) << 16) & 0x00FF0000 |
				       (*(tmpstr + *index + 2) << 8) & 0x0000FF00 |
				       (*(tmpstr + *index + 3) ) & 0x000000FF;

		*index = *index + 4;
		int FMinimumTTL =  (*(tmpstr + *index) << 24) & 0xFF000000 |
					   (*(tmpstr + *index + 1) << 16) & 0x00FF0000 |
				       (*(tmpstr + *index + 2) << 8) & 0x0000FF00 |
				       (*(tmpstr + *index + 3) ) & 0x000000FF;


	}

	*index = *index + RD_Length + 10;
}

void CDNSclient::ConvertDomainStr(char *Domain)
{

    char tmpchar[255] ;

	int len=strlen(Domain);
	int lencount=0;
	int reppos = 0;

	ZeroMemory(tmpchar,255);

     

	for(int i= 0 ; i < len ; i++)
	{
		if ( *(Domain+i) == '.')
		{
		     
			 //*(Domain+i) = lencount;
			 tmpchar[reppos] = lencount;
			 strncpy(tmpchar+reppos+1,Domain+reppos,lencount);

			 reppos = i+1;

			 //strnset((Domain+i),lencount,1);
			 lencount = 0;
		}
		else
		{
		    lencount ++;
		}
	}

	//補最後
	if (reppos > 0)
	{
	 tmpchar[reppos] = lencount;
	 strncpy(tmpchar+reppos+1,Domain+reppos,lencount);
	}

	//tmpchar[len+1] = '/0';



	strcpy(Domain,tmpchar);

	//return tmpchar;
 
}

int CDNSclient::OnConnected(Socket_Obj *sock)
{

	char SendQuery[255];

	//SetQr(0);
    //SetOpCode(0); //標準查詢
	//SetRd(true);
	FID = 0;
	FBitCode = 256;
	FQDCount = 1; //一次查一個 domain

	char DomainStr[255];

		if (m_ResType == qtPTR)
	{
	    //反轉 + arpr

		int len = strlen(m_Domain);
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
			if (m_Domain[i] == '.')
			{
			
				if (ZoneIdx == 0)
				{					
					if (i - SavePos > 0)
					strncpy(Zone4,m_Domain + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 1)
				{
					if (i - SavePos > 0)
					strncpy(Zone3,m_Domain + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 2)
				{
					if (i - SavePos > 0)
					strncpy(Zone2,m_Domain + SavePos, i - SavePos);
					if (len-i > 0)
					strncpy(Zone1,m_Domain + i + 1, len-i);
					 
					break;
				}
			 

				ZoneIdx ++;
				SavePos = i + 1;

			}
		}

		strcpy(DomainStr,Zone1);
		strcat(DomainStr,".");
		strcat(DomainStr,Zone2);
		strcat(DomainStr,".");
		strcat(DomainStr,Zone3);
		strcat(DomainStr,".");
		strcat(DomainStr,Zone4);
		strcat(DomainStr,".");	  
		strcat(DomainStr,"in-addr.arpa");
	}
	else
	{
		strcpy(DomainStr,m_Domain);
	}

	
	ConvertDomainStr(DomainStr);

	char FillValue = '0';
	int FillLen = 0;
	

	FillValue = (FID & 0xFF00) >> 8;	 
	memcpy(SendQuery,&FillValue,1);
	FillLen++;

	FillValue = (FID & 0x00FF) ;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen++;

	FillValue = (FBitCode & 0xFF00) >> 8;	 
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen++;

	FillValue = (FBitCode & 0x00FF)  ;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen++;

	FillValue = (FQDCount & 0xFF00) >> 8;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen++;

	FillValue = (FQDCount & 0x00FF)  ;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen++;

	for (int i = 0 ; i < 6 ; i ++)
	{
		FillValue = 0;	
		memcpy(SendQuery+FillLen,&FillValue,1);
		FillLen++;
	}
	 	
	memcpy(SendQuery+FillLen,DomainStr,strlen(DomainStr));
	FillLen = FillLen + strlen(DomainStr);

	FillValue = 0;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen = FillLen + 1;

	FillValue = (m_ResType & 0xFF00) >> 8;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen = FillLen + 1;

	FillValue = (m_ResType & 0x00FF)  ;	
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen = FillLen + 1;

	FillValue = 0;
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen = FillLen + 1;

	FillValue = 1;
	memcpy(SendQuery+FillLen,&FillValue,1);
	FillLen = FillLen + 1; 


	 
	SendBuffer(sock,SendQuery,FillLen);
	
	return NO_ERROR;
}

int CDNSclient::OnDataRead(Buffer_Obj *bobj)
{

 
//	OutputDebugString("Data Read\n");

	int APos = 12;
	char Result[1024]; 

	int NsType = 0;
	int Pref = 0;

	int LowestPref = 65535;
	char SaveDomain[255];

	char LastMxIp[255];
	LastMxIp[0] = 0;

	
	int DataLen = bobj->bytes;
	ParseStr(bobj->buf,&APos,DataLen,Result);
	APos = APos + 4;

	char RRDomain[255];

	

	while (APos < DataLen)
	{
		ParseDNS(bobj->buf,&APos,&NsType,&Pref,RRDomain,DataLen,Result);

		//printf("%d->%s %s\n",NsType,RRDomain,Result);

		if (m_ResType == qtMX)
		{
		   if (NsType == qtMX)
		   {
			   if (Pref < LowestPref )
			   {
				   LowestPref = Pref;
				   strcpy(SaveDomain,Result);
			   }

			  //printf("%d\n",Pref);
		   } else if (NsType == qtA)
		   {
		   
			   if (strcmp(RRDomain,SaveDomain) == 0)
			   {
			     //printf("\nLowest: %d %s %s\n\n",LowestPref,SaveDomain,Result);
				 strcpy(m_QueryResult,Result);
			   }

			   strcpy(LastMxIp,Result);
		   
		   }
		
		}
		else if (m_ResType == qtA)
		{
		
		 	if (NsType == qtA)
			{     

				//if (strcmp(RRDomain,m_Domain) == 0)
				//{
				  //printf("\n%s\n\n",Result);
				  strcpy(m_QueryResult,Result);
				  break;
				  //直接取第一筆
				//}
			}

		}
		else if (m_ResType == qtPTR)
		{
		
		 	if (NsType == qtPTR)
			{     

			
				  strcpy(m_QueryResult,Result);
				  break;
				  
				
			}

		}
		
	
	}


	if (m_QueryResult[0] == 0 && m_ResType ==  qtMX)
	{
		strcpy(m_QueryResult,LastMxIp);
	}

	
	//printf("%d\n",bobj->bytes);

	closesocket(bobj->Socket->s);

	return IO_NOTPOSTRECV;

}

//// CSMTPclient
//////////////////////////////////////////////////////////////////////////////
CSMTPclient::CSMTPclient()
{
   //PrevCmd[0] = 0;
   m_Mail_Obj = NULL;
}

void CSMTPclient::GetDomain(char* email , char*Domain)
{

	int tokenpos = pos(email,"@",0);

	if (tokenpos > -1)
	{
	
		strncpy(Domain,email+tokenpos+1,strlen(email));
	
	}
	

}


/*
void CSMTPclient::PutStr(char* mstr)
{

	strcpy(PrevCmd,
}*/

int CSMTPclient::OnDataRead(Buffer_Obj *bobj)
{

	CBaseClient::OnDataRead(bobj);

	

	//printf("%s \n",bobj->buf);
	//OutputDebugString(bobj->buf);
	

	if (m_Mail_Obj == NULL)
	{
	  return IO_ERROR;
	}

	

	char RtnCode[3];
	//char CmdStr[255];
	char ParmStr[255];
	char SendCmd[1024];

	SendCmd[0] = 0;

	

	strncpy(RtnCode,bobj->buf,3);
	RtnCode[3] = 0;
	 
	if (bobj->bytes - 4 > 0)
	{
		strncpy(ParmStr,bobj->buf+4, bobj->bytes - 4);
		ParmStr[bobj->bytes - 4] = 0;
	}


	

	if (bobj->Socket->lastcmd == 0)
	{

		//strcpy(PrevCmd,"HELO");
		//strcat(PrevCmd,"HELO");

		bobj->Socket->lastcmd ++;		
	    sprintf(SendCmd,"HELO %s\r\n",m_Mail_Obj->HELO);
		//sprintf(SendCmd,"HELO %s%c%c","mail.microbean.com.tw",13,10);
		SendLn(bobj->Socket,SendCmd);				  

	
	}
	else 
	if (bobj->Socket->lastcmd  == 1)
	{
		bobj->Socket->lastcmd ++;
	
		//strcpy(PrevCmd,"MAIL");
		sprintf(SendCmd,"MAIL FROM:<%s>\r\n",m_Mail_Obj->MailFrom);
		//sprintf(SendCmd,"MAIL FROM:<%s>%c%c","kenny@microbean.com.tw",13,10);
		SendLn(bobj->Socket,SendCmd);	
		 

	}
	else 
	if (bobj->Socket->lastcmd == 2)
	{
	
		bobj->Socket->lastcmd ++;
		 
		sprintf(SendCmd,"RCPT TO:<%s>\r\n",m_Mail_Obj->RcptTo);
		//sprintf(SendCmd,"RCPT TO:<%s>%c%c","microbean@gmail.com",13,10);
		SendLn(bobj->Socket,SendCmd);

	}
	else 
	if (bobj->Socket->lastcmd == 3)
	{
		bobj->Socket->lastcmd ++;
	
		//strcpy(PrevCmd,"DATA");
	 
		sprintf(SendCmd,"%s%c%c","DATA",13,10);
		SendLn(bobj->Socket,SendCmd);

	}
	else 
	if (bobj->Socket->lastcmd == 4)
	{
	
		bobj->Socket->lastcmd ++;
		//strcpy(PrevCmd,"DATS");
		//strcpy(PrevCmd,"QUIT");

		/*sprintf(SendCmd,"From:%s<%s>\nSubject: %s\n\n%s%c%c.%c%c",
				"Mr.Kenny",
				"kenny@microbean.com.tw",
				"This is my Test",
				"Hello ? \n second line\n"
				,13,10,13,10);*/

		sprintf(SendCmd,"From:%s<%s>\nSubject: %s\n\n%s\r\n.\r\n",
				m_Mail_Obj->Sender,
				m_Mail_Obj->MailFrom,
				m_Mail_Obj->Subject,
				m_Mail_Obj->Body);

		SendLn(bobj->Socket,SendCmd);

	}
	else
	if (bobj->Socket->lastcmd == 5)
	{
		bobj->Socket->lastcmd ++; 

		sprintf(SendCmd,"%s\r\n","QUIT");
		SendLn(bobj->Socket,SendCmd);

	}

	return IO_OK;
}

HANDLE CSMTPclient::SendMail(Mail_Obj* mailobj ,int timeout,int eventtimeout)
{

	
	char Domain[255];
	Domain[0] = 0;

	m_Mail_Obj = mailobj;

	if (m_Mail_Obj != NULL)
	{
		if (m_Mail_Obj->RcptTo != NULL)
		{		
			GetDomain(m_Mail_Obj->RcptTo,Domain);		
		}
	
	}
	
	
	char QueryResult[255];
	if (m_Mail_Obj->SmtpIp == NULL)
	{
		CDNSclient *mxdns = new CDNSclient;
		//CDNSclient mxdns;
		HANDLE ch = mxdns->Resolve("168.95.1.1",Domain,qtMX);
	 
		WaitForSingleObject(ch,INFINITE);
	
		strcpy(QueryResult,mxdns->GetQueryResult());

		delete mxdns;
	}
	else
	{	
		strcpy(QueryResult,m_Mail_Obj->SmtpIp);
	}

	//char *QueryResult = "64.233.171.27";
	//char *QueryResult = "192.168.1.3";
	if (QueryResult != "")
	{
		return   CBaseClient::Connect(IPPROTO_TCP,QueryResult,25,timeout,eventtimeout);
	}
	else
	{
	   return NULL;
	}

	return NULL;
}


//// CPOPclient
//////////////////////////////////////////////////////////////////////////////

CPOPclient::CPOPclient()
{

	m_LoginId = NULL;
	m_Password = NULL;

	m_TotalMail = 0;
	m_index = 0;

}

HANDLE CPOPclient::CheckMail(char* HostIp, char* LoginId,char* Password ,int timeout,int eventtimeout)
{
	m_LoginId = LoginId;
	m_Password = Password;
	
	return   CBaseClient::Connect(IPPROTO_TCP,HostIp,110,timeout,eventtimeout);
}


int  CPOPclient::OnDataRead(Buffer_Obj *bobj)
{

    CBaseClient::OnDataRead(bobj);

	#ifdef _CONSOLEDBG
	_cprintf("%s \n",bobj->buf);
	#endif
	

	char RtnCode[3];	
	char ParmStr[255];
	char SendCmd[1024];

	int pos1 = 0;

	SendCmd[0] = 0;



	
	if ((bobj->Socket->lastcmd != P_RETR) && (bobj->Socket->lastcmd != P_FIRSTRETR))
	{

		strncpy(RtnCode,bobj->buf,3);
		RtnCode[3] = 0;
	 
		if (bobj->bytes - 4 <= sizeof(ParmStr) && bobj->bytes - 4 > 0)
		{
			strncpy(ParmStr,bobj->buf+4, bobj->bytes - 4);
			ParmStr[bobj->bytes - 4] = 0;
		}
		else
		{		
			OutputDebugString("POP size error\n");
			//printf("%s \n",bobj->buf);

			throw "size Error";
		}


		if (strcmp(RtnCode,"+OK") == 0)
		{
			switch (bobj->Socket->lastcmd)
			{
				case NONE :
					bobj->Socket->lastcmd = P_USER;		
					sprintf(SendCmd,"USER %s\r\n",m_LoginId);		
					SendLn(bobj->Socket,SendCmd);	
					break;
				
				case P_USER :
					bobj->Socket->lastcmd = P_PASS;	
					sprintf(SendCmd,"PASS %s\r\n",m_Password);		
					SendLn(bobj->Socket,SendCmd);	
					break;

				case P_PASS:
					bobj->Socket->lastcmd = P_STAT;		
					sprintf(SendCmd,"STAT \r\n");		
					SendLn(bobj->Socket,SendCmd);	
					break;  
				
				case P_STAT:

					//取得信件數
					pos1 = pos(ParmStr," ",0);
					if (pos1 >= 0)
					{					
						ParmStr[pos1] = 0;
						m_TotalMail = atoi(ParmStr);
					}

					if (m_index < m_TotalMail && m_TotalMail > 0)
					{

						m_index ++;
						bobj->Socket->lastcmd = P_LIST;
						sprintf(SendCmd,"LIST %d \r\n",m_index);		
						SendLn(bobj->Socket,SendCmd);						
					}
					else
					{
						bobj->Socket->lastcmd = P_QUIT;	
						sprintf(SendCmd,"QUIT \r\n");		
						SendLn(bobj->Socket,SendCmd);
					
					}

				
					break;  

				case P_LIST:

					if (m_index <= m_TotalMail && m_TotalMail > 0)
					{
						 
						bobj->Socket->lastcmd = P_UIDL;	
						sprintf(SendCmd,"UIDL %d\r\n",m_index);		
						SendLn(bobj->Socket,SendCmd);				
				

					}
					else
					{
						bobj->Socket->lastcmd = P_QUIT;	
						sprintf(SendCmd,"QUIT \r\n");		
						SendLn(bobj->Socket,SendCmd);
					
					}

					break;

				case P_UIDL:

					if (m_index <= m_TotalMail && m_TotalMail > 0)
					{
						
						//取得UIDL						
						pos1 = pos(ParmStr," ",0);
						if (pos1 >= 0 && strlen(ParmStr)-2-pos1-1 > 0)
						{					
							strncpy(m_UIDL,ParmStr+pos1+1,strlen(ParmStr)-2-pos1-1);
							m_UIDL[strlen(ParmStr)-2-pos1-1] = 0;
						}
												
						bobj->Socket->lastcmd = P_FIRSTRETR;
						sprintf(SendCmd,"RETR %d \r\n",m_index);		
						SendLn(bobj->Socket,SendCmd);
						
						system("cls");

						char tmp[255];
						sprintf(tmp,"c:\\%s.eml",m_UIDL);

						fp = fopen(tmp,"wb");

					}
					else
					{
						bobj->Socket->lastcmd = P_QUIT;	
						sprintf(SendCmd,"QUIT \r\n");		
						SendLn(bobj->Socket,SendCmd);
					
					}

					break;


				case P_RETR:
					break;
				
				case P_QUIT:
					break;

				default:
			 		bobj->Socket->lastcmd ++;		
					sprintf(SendCmd,"QUIT \r\n");		
					SendLn(bobj->Socket,SendCmd);	
					break;  
			}
		
		}
		else
		{
				//指令異常			
				bobj->Socket->lastcmd = P_QUIT;	
				sprintf(SendCmd,"QUIT \r\n");		
				SendLn(bobj->Socket,SendCmd);

			
					
		}

	}
	else 
	{
	 
	 
		 int offset = 0;
		 if (bobj->Socket->lastcmd == P_FIRSTRETR)
		 {
		 

			 	strncpy(RtnCode,bobj->buf,3);
				RtnCode[3] = 0;
	 
				if (strcmp(RtnCode,"+OK") == 0)
				{

					pos1 = pos(bobj->buf,"\r\n",0);
					if (pos1 >= 0)
					{					
						offset = pos1 + 2;
					}
				}

				bobj->Socket->lastcmd = P_RETR;
		 
		 }
		
		 
		 if (//*(bobj->buf + bobj->bytes - 5) == char(13) &&
		     //*(bobj->buf + bobj->bytes - 4) == char(10) &&
		     *(bobj->buf + bobj->bytes - 3) == '.' &&
		     *(bobj->buf + bobj->bytes - 2) == char(13) &&
			 *(bobj->buf + bobj->bytes - 1) == char(10))
		  { 

		       bobj->buf[bobj->bytes-5] = 0;
			   fputs(bobj->buf + offset,fp);
			   
			   fclose(fp);

			   if (m_index < m_TotalMail && m_TotalMail > 0)
			   {
								        
				        m_index ++;
						bobj->Socket->lastcmd = P_LIST;
						sprintf(SendCmd,"LIST %d \r\n",m_index);		
						SendLn(bobj->Socket,SendCmd);						
				}
				else
				{
						bobj->Socket->lastcmd = P_QUIT;	
						sprintf(SendCmd,"QUIT \r\n");		
						SendLn(bobj->Socket,SendCmd);
					
				}
				
		  }
		 else
		 {		 
				fputs(bobj->buf + offset,fp);		 
		 }

		  
	}
 
	 


	




	return IO_OK;
}



//// CHTTPclient
//////////////////////////////////////////////////////////////////////////////

CHTTPclient::CHTTPclient()
{ 
   //memset(ResStr,0,2048);
   memset(Header,0,sizeof(Header));
   memset(m_DNSServer,0,17);

   ByteTransfer = 0;
   TotalTransfer = 0 ;
   ByteSend = 0;
   SendCmd = NULL;
}

int CHTTPclient::OnConnected(Socket_Obj *sock)
{
	 

	return SendLn(sock,SendCmd);
	
	//return NO_ERROR;
}

int  CHTTPclient::OnDataRead(Buffer_Obj *bobj)
{

	CBaseClient::OnDataRead(bobj);

	//strcpy(ResStr,bobj->buf);

    //printf("%s",bobj->buf);

	ByteTransfer+= bobj->bytes;

	int OffSet = 0;
	if (bobj->Socket->lastcmd == 0)
	{
		//取得 header;
		OffSet = GetHeader(bobj->buf,bobj->bytes);
		
		if (OffSet > 0)
		{
			bobj->Socket->lastcmd ++;
			ByteTransfer -= OffSet;
			fwrite(bobj->buf + OffSet,sizeof(char),bobj->bytes - OffSet,fp);
		}
		else
		{
			ByteTransfer = 0;
		}
	}
	else
	{
		 fwrite(bobj->buf + OffSet,sizeof(char),bobj->bytes - OffSet,fp);
	}

	
	

	return IO_OK;

}

int  CHTTPclient::OnDataWrite(Buffer_Obj *bobj)
{

	CBaseClient::OnDataWrite(bobj);
	ByteSend+= bobj->bytes;

	return IO_OK;
}

HANDLE CHTTPclient::GET(char* Host, char* PathFile ,int timeout,int eventtimeout)
{
 
	SendCmd = new char[1024];
	strcpy(SendCmd,"GET");
	strcat(SendCmd," ");
	strcat(SendCmd,PathFile);
	strcat(SendCmd," HTTP/1.1\r\n");
	//strcat(SendCmd,"Accept: */*\r\n");
	//strcat(SendCmd,"Accept-Language: zh-tw\r\n");
	//strcat(SendCmd,"Accept-Encoding: gzip, deflate\r\n");
	//strcat(SendCmd,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n");
	strcat(SendCmd,"Host: ");
	strcat(SendCmd,Host);
	strcat(SendCmd,"\r\n"); 
	strcat(SendCmd,"Connection: Close\r\n");	
	//strcat(SendCmd,"Connection: Keep-Alive\r\n");	
	strcat(SendCmd,"\r\n\r\n"); 

	char QueryResult[255];
	if (m_DNSServer[0] == 0)
	{
		strcpy(m_DNSServer,"168.95.1.1");
	}

	

	//retry 
	for (int i=0 ; i < 3 ; i ++)
	{
 
	CDNSclient *dns = new CDNSclient();		
	HANDLE ch = dns->Resolve(m_DNSServer,Host,qtA);
	
	#ifdef _WINDOWS
			DWORD rtn = WaitForSingleObject(ch,100);
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

				rtn = WaitForSingleObject(ch,100);
			}		
	#endif

	#ifndef _WINDOWS
		WaitForSingleObject(ch,INFINITE);	
	#endif

	strcpy(QueryResult,dns->GetQueryResult()); 

	if (strlen(QueryResult) > 0 ) 
	{
		delete dns;
		break;
	}
	else
	{
		OutputDebugString("HTTP DNS Retry");
	}
	 
	delete dns;

	}
 



	char TempPath[MAX_PATH];									
	GetTempPath(MAX_PATH,TempPath);

	GetTempFileName(TempPath, // directory for temp files 
        "SF",                    // temp file name prefix 
        0,                        // create unique name 
        TempFileName);              // buffer for name 


	//strcat(TempPath,"SpamDB.txt");
	fp = fopen(TempFileName,"w+b");

	//fp = fopen("c:\\sockdata.txt","wb");

	return   CBaseClient::Connect(IPPROTO_TCP,QueryResult,80,timeout,eventtimeout);
}

HANDLE CHTTPclient::IPGET(char* IP,char* Host, char* PathFile ,int timeout,int eventtimeout)
{
 
	SendCmd = new char[1024];
	strcpy(SendCmd,"GET");
	strcat(SendCmd," ");
	strcat(SendCmd,PathFile);
	strcat(SendCmd," HTTP/1.1\r\n");
	//strcat(SendCmd,"Accept: */*\r\n");
	//strcat(SendCmd,"Accept-Language: zh-tw\r\n");
	//strcat(SendCmd,"Accept-Encoding: gzip, deflate\r\n");
	//strcat(SendCmd,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n");
	strcat(SendCmd,"Host: ");
	strcat(SendCmd,Host);
	strcat(SendCmd,"\r\n"); 
	strcat(SendCmd,"Connection: Close\r\n");	
	//strcat(SendCmd,"Connection: Keep-Alive\r\n");	
	strcat(SendCmd,"\r\n\r\n"); 

	#ifdef _CONSOLEDBG
	_cprintf(SendCmd);
	#endif

	//char QueryResult[255];
	//if (m_DNSServer[0] == 0)
	//{
	//	strcpy(m_DNSServer,"168.95.1.1");
	//}

	

	//retry 
	/*
	for (int i=0 ; i < 3 ; i ++)
	{
 
	CDNSclient *dns = new CDNSclient();		
	HANDLE ch = dns->Resolve(m_DNSServer,Host,qtA);
	
	#ifdef _WINDOWS
			DWORD rtn = WaitForSingleObject(ch,100);
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

				rtn = WaitForSingleObject(ch,100);
			}		
	#endif

	#ifndef _WINDOWS
		WaitForSingleObject(ch,INFINITE);	
	#endif

	strcpy(QueryResult,dns->GetQueryResult()); 

	if (strlen(QueryResult) > 0 ) 
	{
		delete dns;
		break;
	}
	else
	{
		OutputDebugString("Retry");
	}
	 
	delete dns;

	}
 
*/


	char TempPath[MAX_PATH];									
	GetTempPath(MAX_PATH,TempPath);

	GetTempFileName(TempPath, // directory for temp files 
        "SF",                    // temp file name prefix 
        0,                        // create unique name 
        TempFileName);              // buffer for name 


	//strcat(TempPath,"SpamDB.txt");
	fp = fopen(TempFileName,"w+b");

	//fp = fopen("c:\\sockdata.txt","wb");

	return   CBaseClient::Connect(IPPROTO_TCP,IP,80,timeout,eventtimeout);
}

HANDLE CHTTPclient::IPPOST(char *IP,char* Host, char* PathFile ,char* PostStr,int timeout,int eventtimeout)
{
 
	int Strlen = strlen(PostStr);
	SendCmd = new char[1024+Strlen];

	strcpy(SendCmd,"POST");
	strcat(SendCmd," ");
	strcat(SendCmd,PathFile);
	strcat(SendCmd," HTTP/1.1\r\n");
	strcat(SendCmd,"Accept: */*\r\n");
	//strcat(SendCmd,"Accept-Language: zh-tw\r\n");
	//strcat(SendCmd,"Accept-Encoding: gzip, deflate\r\n");
	//strcat(SendCmd,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n");
	strcat(SendCmd,"Host: ");
	strcat(SendCmd,Host);
	strcat(SendCmd,"\r\n"); 
	strcat(SendCmd,"Connection: Close\r\n"); 
	strcat(SendCmd,"Content-Type: application/x-www-form-urlencoded\r\n"); 
	strcat(SendCmd,"Content-Length: ");

	
	char intstr[50];
	itoa(Strlen,intstr,10);
	strcat(SendCmd,intstr);
	strcat(SendCmd,"\r\n\r\n");
	//strcat(SendCmd,"Connection: Keep-Alive\r\n");	
	strcat(SendCmd,PostStr);
	strcat(SendCmd,"\r\n\r\n");

 
	char TempPath[MAX_PATH];
								    
									
	GetTempPath(MAX_PATH,TempPath);

	GetTempFileName(TempPath, // directory for temp files 
        "SF",                    // temp file name prefix 
        0,                        // create unique name 
        TempFileName);              // buffer for name 


	//strcat(TempPath,"SpamDB.txt");
	fp = fopen(TempFileName,"w+b");



 
	//char QueryResult[255];

	//if (m_DNSServer[0] == 0)
	//{
	//	strcpy(m_DNSServer,"168.95.1.1");
	//}
 
	
	//CDNSclient *dns = new CDNSclient();		
	//HANDLE ch = dns->Resolve(m_DNSServer,Host,qtA);
	
	//#ifdef _WINDOWS
		//DWORD rtn = WaitForSingleObject(ch,100);
			//while (rtn != WAIT_OBJECT_0)
			//{
				//處理 waitting msg dump
				//MSG msg;
			//	while (::PeekMessage(&msg, NULL,   // pump message until none
			//			NULL, NULL, PM_REMOVE))   // are left in the queue
			//	{
			//		TranslateMessage(&msg);
			//		DispatchMessage(&msg);
			//	}

			//	rtn = WaitForSingleObject(ch,100);
			//}		
	//#endif

	//#ifndef _WINDOWS
		//WaitForSingleObject(ch,INFINITE);	
	//#endif
	
	//strcpy(QueryResult,dns->GetQueryResult()); 
	//delete dns; 

 

 

	return   CBaseClient::Connect(IPPROTO_TCP,IP,80,timeout,eventtimeout);
}

HANDLE CHTTPclient::POST(char* Host, char* PathFile ,char* PostStr,int timeout,int eventtimeout)
{
 
	int Strlen = strlen(PostStr);
	SendCmd = new char[1024+Strlen];

	strcpy(SendCmd,"POST");
	strcat(SendCmd," ");
	strcat(SendCmd,PathFile);
	strcat(SendCmd," HTTP/1.1\r\n");
	strcat(SendCmd,"Accept: */*\r\n");
	//strcat(SendCmd,"Accept-Language: zh-tw\r\n");
	//strcat(SendCmd,"Accept-Encoding: gzip, deflate\r\n");
	//strcat(SendCmd,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n");
	strcat(SendCmd,"Host: ");
	strcat(SendCmd,Host);
	strcat(SendCmd,"\r\n"); 
	strcat(SendCmd,"Connection: Close\r\n"); 
	strcat(SendCmd,"Content-Type: application/x-www-form-urlencoded\r\n"); 
	strcat(SendCmd,"Content-Length: ");

	
	char intstr[50];
	itoa(Strlen,intstr,10);
	strcat(SendCmd,intstr);
	strcat(SendCmd,"\r\n\r\n");
	//strcat(SendCmd,"Connection: Keep-Alive\r\n");	
	strcat(SendCmd,PostStr);
	strcat(SendCmd,"\r\n\r\n");

 
	char TempPath[MAX_PATH];
								    
									
	GetTempPath(MAX_PATH,TempPath);

	GetTempFileName(TempPath, // directory for temp files 
        "SF",                    // temp file name prefix 
        0,                        // create unique name 
        TempFileName);              // buffer for name 


	//strcat(TempPath,"SpamDB.txt");
	fp = fopen(TempFileName,"w+b");



 
	char QueryResult[255];

	if (m_DNSServer[0] == 0)
	{
		strcpy(m_DNSServer,"168.95.1.1");
	}
 
	
	CDNSclient *dns = new CDNSclient();		
	HANDLE ch = dns->Resolve(m_DNSServer,Host,qtA);
	
	#ifdef _WINDOWS
			DWORD rtn = WaitForSingleObject(ch,100);
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

				rtn = WaitForSingleObject(ch,100);
			}		
	#endif

	#ifndef _WINDOWS
		WaitForSingleObject(ch,INFINITE);	
	#endif
	
	strcpy(QueryResult,dns->GetQueryResult()); 
	delete dns; 

 

 

	return   CBaseClient::Connect(IPPROTO_TCP,QueryResult,80,timeout,eventtimeout);
}

void CHTTPclient::OnDataFinish()
{
	 delete SendCmd;
	 SendCmd = NULL;
	 fclose(fp);
    // _cprintf("%s\n",Header);

}



char *CHTTPclient::GetTempFilePath()
{
	return TempFileName;
}

int CHTTPclient::GetHeader(char* buff,int size)
{

	int SavePos = 0;
	for (int i= 0 ; i < size ; i ++)
	{
	
		if (strnicmp(buff+i,"100 Continue",12) == 0)
		{
				break;
		}else
		if (strnicmp(buff+i,"/1.1 404 ",9) == 0)
		{
		
			TotalTransfer = 0;
			break;
		
		} else if (strnicmp(buff+i,"Content-Length:",15) == 0)
		{						
			SavePos = i + 16;

		}else if (SavePos > 0 && buff[i] == char(13) && buff[i+1] == char(10))
		{
			char tmpbuf[255];
			tmpbuf[0] = 0;

			if (i - SavePos > 0)
			{
				strncpy(tmpbuf,buff + SavePos,i - SavePos);
				tmpbuf[i - SavePos] = 0;
			}

			SavePos = 0;

			TotalTransfer =  atoi(tmpbuf);

			//OutputDebugString(tmpbuf);
			//OutputDebugString("\r\n");

		}
		if (buff[i] == char(13) && buff[i+1] == char(10) &&
			buff[i+2] == char(13) && buff[i+3] == char(10))
		{
		
			if (i-1 < sizeof(Header) && i > 0)			
			  strncpy(Header,buff,i);
			return i+4;
		}
	
	}

	return 0;

}

void CHTTPclient::SetDNS(char* DNSServer)
{
	strcpy(m_DNSServer,DNSServer);
}

int CHTTPclient::GetByteTransfer()
{
	return ByteTransfer;
	
}
int CHTTPclient::GetTotalTransfer()
{
    return TotalTransfer;
}

bool CHTTPclient::DoTransSucceed()
{

	if (TotalTransfer > 0 && TotalTransfer == ByteTransfer)
	{
		return true;
	}
	else
	{
		return false;
	}
}


__declspec(dllexport) CHTTPclient* createHTTPObject()
{
	return new CHTTPclient();
}


__declspec(dllexport) void  SocketStartUp()
{
	CCoreSocket::Startup();
}

__declspec(dllexport) void  SocketCleanUp()
{
	CCoreSocket::Cleanup();
}