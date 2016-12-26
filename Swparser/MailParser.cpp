#include "stdafx.h"
#include "MailParser.h"
 
#include <math.h>
#include <mbstring.h>


//#include <windows.h>

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

void CEmail::AddReceivedList(ReceivedList* list , char *ReceievedStr)
{

    // Find the end of the list
	ReceievedData *end = NULL;
    ReceievedData *ptr = list->m_ReceievedData;
    if (ptr)
    {
        while (ptr->next)
            ptr = ptr->next;
        end = ptr;
    }

    
	//create new obj
	ReceievedData *obj = new ReceievedData;
	obj->Receieved = new char[strlen(ReceievedStr) + 1];
	strcpy(obj->Receieved , ReceievedStr);	
	
	obj->next = NULL;
    //obj->prev = end;

    if (end == NULL)
    {
        // List is empty
        list->m_ReceievedData = obj;
    }
    else
    {
        // Put new object at the end 
        end->next = obj;
        //obj->prev = end;
    }

	list->size++;

}
void CEmail::FreeReceivedList(ReceivedList* list)
{
	ReceievedData *ptr = list->m_ReceievedData;
	while (ptr)
	{	  
	    
	    ReceievedData *nextptr = ptr->next;

		   delete ptr->Receieved;
		   delete ptr;
	 
		ptr = nextptr;		
	  
	}

	list->m_ReceievedData = NULL;
	list->size = 0;

}

CEmail::CEmail()
{
   /*FileSize = 0;
   FILE * fp;
   fp = fopen(MailPath,"rb");

   fseek(fp,0,SEEK_END);
	FileSize=ftell(fp);
   fseek(fp,0,SEEK_SET);

   mailContent = new char[FileSize+1];

   fread(mailContent,sizeof(char),FileSize,fp);   
   mailContent[FileSize] = 0;
    
   

   fclose(fp);*/

   
   

}

CEmail::~CEmail()
{
 
	//delete mailContent; 
}



void CEmail::TrimQuote(char *AStr, char Quote)
{
	int len = strlen(AStr);
	
	if (AStr[0] == Quote)
	{
		memmove(AStr,AStr+1,len-1);	
		AStr[len-1] = 0;
	}

	len = strlen(AStr);
	if (AStr[len-1] == Quote)
	{
	   AStr[len-1] = 0;
	}



}
 
void CEmail::Trim(char *Astr)
{

	int len = strlen(Astr);

	int lpos = -1;
	//處理 right trim
	for (int i = len - 1 ; i >=0  ; i --)
	{
		if (Astr[i] == char(13) ||
			Astr[i] == char(10) ||
			Astr[i] == char(9) ||
			Astr[i] == char(0x20))
		{
			lpos = i;
		}
		else
		{
			break;
		}
	
	}

	if (lpos != -1)
		Astr[lpos] = 0;

	//處理 left trim
	int rpos = -1;
	for(int j = 0 ; j < len ; j++)
	{
		if (Astr[j] == char(13) ||
			Astr[j] == char(10) ||
			Astr[j] == char(9) ||
			Astr[j] == char(0x20))
		{
			rpos = j+1;
		}
		else
		{
			break;
		}	
	}

	if (rpos != -1)
	{
		//Astr[lpos] = 0; 
		memmove(Astr,Astr+rpos,len-rpos);
		Astr[len-rpos] = 0;
	}

}
 

MailData* CEmail::GetMailData(char* MailContent)
{
	
   MailData *m_MailData = new MailData;
   
   unsigned int filesize = strlen(MailContent);
   int pos1 = pos(MailContent,"\r\n\r\n",0);   

   if (pos1 > -1)
   {
	   //pos1 不可大於原始長度
	   //if (pos1 > srcfilesize) 
	   //{	    
	   //   return m_MailData;
	  // }

	   m_MailData->mailHeader = new char[pos1 + 3];
	   memcpy(m_MailData->mailHeader,MailContent,pos1+2);
	   m_MailData->mailHeader[pos1+2] = 0;
	   

	   m_MailData->mailBody = new char[filesize - pos1 - 3];	   
	   memcpy(m_MailData->mailBody,MailContent + pos1 + 4,filesize - pos1 - 4);	   
	   m_MailData->mailBody[filesize-pos1-4] = 0;  	   
	   
   }
   else
   {
	//	throw;
	   delete m_MailData;
	   return NULL;
   }

   return m_MailData;
}

void CEmail::FreeMailData(MailData* AMailData)
{

	delete AMailData->mailBody;
	delete AMailData->mailHeader;
	delete AMailData;
}


MailHeader* CEmail::GetMailHeader(char* MailHeaderContent)
{
	unsigned int LineBegin = 0;
	char *tmpline=NULL;
	int HeaderPos = 0;

	MailHeader *m_MailHeader = new MailHeader;
	memset(m_MailHeader , 0 , sizeof(MailHeader));
	
	SetNullString(&m_MailHeader->Content_Transfer_Encoding);

	m_MailHeader->Content_Type = new ContentType;
	memset(m_MailHeader->Content_Type,0,sizeof(ContentType));

	SetNullString(&m_MailHeader->Content_Type->boundary);
	SetNullString(&m_MailHeader->Content_Type->charset);
	SetNullString(&m_MailHeader->Content_Type->name);
	SetNullString(&m_MailHeader->Content_Type->text);
	SetNullString(&m_MailHeader->Content_Type->type);	

	SetNullString(&m_MailHeader->From);
	SetNullString(&m_MailHeader->Sender);
	SetNullString(&m_MailHeader->Reply_To);	
	SetNullString(&m_MailHeader->Return_Path);
	SetNullString(&m_MailHeader->Subject);
	SetNullString(&m_MailHeader->To);
	SetNullString(&m_MailHeader->Cc);
	SetNullString(&m_MailHeader->Bcc);
	SetNullString(&m_MailHeader->Message_Id);
	SetNullString(&m_MailHeader->Date);
	//SetNullString(&m_MailHeader->Received);
	SetNullString(&m_MailHeader->Content_ID);
	SetNullString(&m_MailHeader->SpamDogMailFrom);
	SetNullString(&m_MailHeader->SpamDogRcptTo);
	SetNullString(&m_MailHeader->SpamDogRemoteIp);
	SetNullString(&m_MailHeader->SpamDogXMailer);
	

	m_MailHeader->IsAttachment = false;

	unsigned int mailsize = strlen(MailHeaderContent);
   

	for(unsigned int i = 0 ; i < mailsize ; i ++)
	{
	
		if (MailHeaderContent[i] == char(13) 
			 && MailHeaderContent[i+1] == char(10)
			 && MailHeaderContent[i+2] != char(0x20)
			 && MailHeaderContent[i+2] != char(0x09))
		{
			tmpline = new char[i+1-LineBegin];
			memcpy(tmpline,MailHeaderContent+LineBegin,i-LineBegin);
			tmpline[i-LineBegin] = 0;

			HeaderPos = pos(tmpline,":",0);
			if(HeaderPos > -1 )
			{

				tmpline[HeaderPos] = 0;
			
				if (_stricmp(tmpline,"From") == 0)
				{
					tmpline[HeaderPos] = ':';
					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->From;
						m_MailHeader->From = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->From,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->From,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						char *Savep = m_MailHeader->From;
						CMailCodec cm;
						m_MailHeader->From = cm.DeHeaderCode(m_MailHeader->From);

						Trim(m_MailHeader->From);
						ReplaceToSpace(m_MailHeader->From);

						delete Savep;
					}


					//printf("From:%s\n",header_From);
				} 
				else if (_stricmp(tmpline,"Sender") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Sender;
						m_MailHeader->Sender = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Sender,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Sender,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						char *Savep = m_MailHeader->Sender;
						CMailCodec cm;
						m_MailHeader->Sender = cm.DeHeaderCode(m_MailHeader->Sender);
						Trim(m_MailHeader->Sender);
						ReplaceToSpace(m_MailHeader->Sender);

						delete Savep;
					}


					//printf("From:%s\n",header_From);
				} 
				else if (_stricmp(tmpline,"Date") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Date;
						m_MailHeader->Date = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Date,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Date,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						Trim(m_MailHeader->Date);
						ReplaceToSpace(m_MailHeader->Date);

					}
					//printf("From:%s\n",header_From);
				} 
				else if (_stricmp(tmpline,"To") == 0)
				{
				    
						tmpline[HeaderPos] = ':';
						 

						if (strlen(tmpline)-HeaderPos-1 > 1)
						{
							delete m_MailHeader->To;
							m_MailHeader->To = new char[strlen(tmpline)-HeaderPos];
							memset(m_MailHeader->To,0,strlen(tmpline)-HeaderPos);
							memcpy(m_MailHeader->To,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
							//printf("To:%s\n",header_To);
							char *Savep = m_MailHeader->To;
							CMailCodec cm;
							m_MailHeader->To = cm.DeHeaderCode(m_MailHeader->To);
							
							Trim(m_MailHeader->To);
							ReplaceToSpace(m_MailHeader->To);
							
							delete Savep;
						}
					 

				}
				else if (_stricmp(tmpline,"Subject") == 0)
				{
				    tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Subject;
						m_MailHeader->Subject = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Subject,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Subject,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						
						printf("Subject:%s\n",m_MailHeader->Subject);
						
						char *Savep = m_MailHeader->Subject;
						CMailCodec cm;
						m_MailHeader->Subject = cm.DeHeaderCode(m_MailHeader->Subject);
						
						Trim(m_MailHeader->Subject);
						ReplaceToSpace(m_MailHeader->Subject);

						delete Savep;
					}
				}
				else if (_stricmp(tmpline,"Reply-To") == 0)
				{
				    tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Reply_To;
						m_MailHeader->Reply_To = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Reply_To,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Reply_To,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						//printf("Reply-To:%s\n",header_Reply_To);
						char *Savep = m_MailHeader->Reply_To;
						CMailCodec cm;
						m_MailHeader->Reply_To = cm.DeHeaderCode(m_MailHeader->Reply_To);
						
						Trim(m_MailHeader->Reply_To);
						ReplaceToSpace(m_MailHeader->Reply_To);
						
						delete Savep;
					}
				}
				else if (_stricmp(tmpline,"Cc") == 0)
				{
				    tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Cc;
						m_MailHeader->Cc = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Cc,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Cc,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						//printf("To:%s\n",header_To);
						char *Savep = m_MailHeader->Cc;
						CMailCodec cm;
						m_MailHeader->Cc = cm.DeHeaderCode(m_MailHeader->Cc);
						
						Trim(m_MailHeader->Cc);
						ReplaceToSpace(m_MailHeader->Cc);

						delete Savep;
					}

				}
				else if (_stricmp(tmpline,"Bcc") == 0)
				{
				    tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Bcc;
						m_MailHeader->Bcc = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Bcc,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Bcc,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						//printf("To:%s\n",header_To);
						char *Savep = m_MailHeader->Bcc;
						CMailCodec cm;
						m_MailHeader->Bcc = cm.DeHeaderCode(m_MailHeader->Bcc);
						
						Trim(m_MailHeader->Bcc);
						ReplaceToSpace(m_MailHeader->Bcc);

						delete Savep;
					}

				}
				else if (_stricmp(tmpline,"Message-ID") == 0)
				{
				    tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Message_Id;
						m_MailHeader->Message_Id = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Message_Id,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Message_Id,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						//printf("To:%s\n",header_To);

						Trim(m_MailHeader->Message_Id);
						ReplaceToSpace(m_MailHeader->Message_Id);
					}
				

				}
				else if (_stricmp(tmpline,"Content-Transfer-Encoding") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Content_Transfer_Encoding;
						m_MailHeader->Content_Transfer_Encoding = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Content_Transfer_Encoding,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Content_Transfer_Encoding,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						//printf("Content-Transfer-Encoding:%s\n",header_Content_Transfer_Encoding);

						Trim(m_MailHeader->Content_Transfer_Encoding);
						ReplaceToSpace(m_MailHeader->Content_Transfer_Encoding);
					}
				}
				else if (_stricmp(tmpline,"Content-Type") == 0)
				{
					tmpline[HeaderPos] = ':';
					unsigned int LineBegin2 = HeaderPos + 2;
					unsigned int tmpstrlen = strlen(tmpline);

					for (unsigned int j = HeaderPos +1 ; j <= tmpstrlen ; j++)
					{
					
						if (tmpline[j] == ';' || j == (tmpstrlen))
						{
						
							//Ltrim 
							while (*(tmpline + LineBegin2) == char(13)
								   || *(tmpline + LineBegin2) == char(10)
								   || *(tmpline + LineBegin2) == char(9)
								   || *(tmpline + LineBegin2) == char(0x20)
								    )
							{
								LineBegin2 ++;
								if (LineBegin2 >= tmpstrlen) break;
							}
							
							char* tmpline2 = new char[j+1-LineBegin2];
							memcpy(tmpline2,tmpline+LineBegin2,j-LineBegin2);
							tmpline2[j-LineBegin2] = 0;


							if ((int) LineBegin2 == HeaderPos + 2)
							{
								//第一個字段
								delete m_MailHeader->Content_Type->text;
								//+ 1 預留 /0 的空間
								m_MailHeader->Content_Type->text = new char[j+1-LineBegin2];
								strcpy(m_MailHeader->Content_Type->text,tmpline2);							
							}
							else if (_strnicmp(tmpline2,"boundary=",9) == 0)
							{
								delete m_MailHeader->Content_Type->boundary;

								//刪除前後的 " 符號
								//Trim(tmpline2+9);
								int leftoffset = 0;
								char *tmpp = tmpline2+9;
								int len = strlen(tmpp);

								for (int i = 0 ; i < len ; i++)
								{
									if (tmpp[i] == char(13) ||
										tmpp[i] == char(10) ||
										tmpp[i] == char(9) ||
										tmpp[i] == char(0x20))
									{
										leftoffset = i + 1;
									}
									else
									{
										break;
									}
								
								}



								int TrimLeft = 0;
								int TrimRight = 0;

								if (*(tmpline2+9+leftoffset) == '"') TrimLeft = 1;
								//if (*(tmpline2+j-LineBegin2-1) == '"') TrimRight = 1;
								if ( tmpline2[strlen(tmpline2)-1] == '"')TrimRight = 1;
								
								int newsize = j-LineBegin2 -9 - TrimLeft - TrimRight - leftoffset;

								
								//int newsize = strlen(tmpline2+9);

								if (newsize > 0)
								{
									m_MailHeader->Content_Type->boundary = new char[newsize+1];
									strncpy(m_MailHeader->Content_Type->boundary,tmpline2+9+TrimLeft+leftoffset,newsize);
									m_MailHeader->Content_Type->boundary[newsize] = 0;
								}
							
							}
							else if (_strnicmp(tmpline2,"charset=",8) == 0)
							{
								delete m_MailHeader->Content_Type->charset;

								if (j+1-LineBegin2-8 > 0)
								{
									m_MailHeader->Content_Type->charset = new char[j+1-LineBegin2-8];
									strncpy(m_MailHeader->Content_Type->charset,tmpline2+8,j+1-LineBegin2-8);
								}
							}
							else if (_strnicmp(tmpline2,"name=",5) == 0)
							{
							
								delete m_MailHeader->Content_Type->name;
								if (j+1-LineBegin2-5 > 0)
								{
									m_MailHeader->Content_Type->name = new char[j+1-LineBegin2-5];
									strncpy(m_MailHeader->Content_Type->name,tmpline2+5,j+1-LineBegin2-5);
								}
							}
							else if (_strnicmp(tmpline2,"type=",5) == 0)
							{
								delete m_MailHeader->Content_Type->type;
								if (j+1-LineBegin2-5 > 0)
								{
									m_MailHeader->Content_Type->type = new char[j+1-LineBegin2-5];
									strncpy(m_MailHeader->Content_Type->type,tmpline2+5,j+1-LineBegin2-5);
								}
							}

							delete tmpline2;
							LineBegin2 = j + 1;
						
						}

					
					}
					//delete m_MailHeader->Content_Type;

					/*
					delete m_MailHeader->Content_Type;
					m_MailHeader->Content_Type = new char[strlen(tmpline)-HeaderPos-1];
					memcpy(m_MailHeader->Content_Type,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
					*/
				}
				else if (_stricmp(tmpline,"Return-Path") == 0)
				{
					tmpline[HeaderPos] = ':';
					
					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Return_Path;
						m_MailHeader->Return_Path = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Return_Path,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Return_Path,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
						Trim(m_MailHeader->Return_Path);
						ReplaceToSpace(m_MailHeader->Return_Path);

						//printf("Return-Path:%s\n",header_Return_Path);
					}
				}
				else if (_stricmp(tmpline,"Received") == 0)
				{
					tmpline[HeaderPos] = ':';

					//加入 receieve 的字串結束以便 strcpy

					//char tmpchar = tmpline[strlen(tmpline)-HeaderPos-1] ;
					//tmpline[strlen(tmpline)-HeaderPos-1] = 0;
					AddReceivedList(&m_MailHeader->m_ReceievedList,tmpline+HeaderPos+2);
					//tmpline[strlen(tmpline)-HeaderPos-1] = tmpchar; //放回

					/*if (strcmp(m_MailHeader->Received,"") != 0)
					{
						//已經有資料
						int oldsize = strlen(m_MailHeader->Received);
						int newsize = strlen(tmpline)-HeaderPos-1 ; //含 /0

						char* newdata = new char[newsize + oldsize + 2];//加上 兩換行
						//放舊的						
						memcpy(newdata,m_MailHeader->Received,oldsize);
						//放入換行
						memcpy(newdata+oldsize,"\r\n",2);
						//放新的						
						memcpy(newdata+oldsize+2,tmpline+HeaderPos+2,newsize+2);
						newdata[newsize+oldsize+1] =0;

						delete m_MailHeader->Received;
						m_MailHeader->Received = newdata;
						
					}
					else
					{
						delete m_MailHeader->Received;
						m_MailHeader->Received = new char[strlen(tmpline)-HeaderPos-1];
						memcpy(m_MailHeader->Received,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 
					
					}*/


 
				}
				else if (_stricmp(tmpline,"Content-ID") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->Content_ID;
						m_MailHeader->Content_ID = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->Content_ID,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->Content_ID,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						Trim(m_MailHeader->Content_ID);
						ReplaceToSpace(m_MailHeader->Content_ID);
					}
				 
				}
				else if (_stricmp(tmpline,"Content-Disposition") == 0)
				{
					tmpline[HeaderPos] = ':';
					if (pos(_strlwr(tmpline),"attachment",0) > -1)
						m_MailHeader->IsAttachment = true;
					 
				}
				else if (_stricmp(tmpline,"SpamDog-MailFrom") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->SpamDogMailFrom;
						m_MailHeader->SpamDogMailFrom = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->SpamDogMailFrom,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->SpamDogMailFrom,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						Trim(m_MailHeader->SpamDogMailFrom);
						ReplaceToSpace(m_MailHeader->SpamDogMailFrom);

					}				
				} 
				else if (_stricmp(tmpline,"SpamDog-RcptTo") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->SpamDogRcptTo;
						m_MailHeader->SpamDogRcptTo = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->SpamDogRcptTo,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->SpamDogRcptTo,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						Trim(m_MailHeader->SpamDogRcptTo);
						ReplaceToSpace(m_MailHeader->SpamDogRcptTo);

					}				
				} 
				else if (_stricmp(tmpline,"SpamDog-RemoteIp") == 0)
				{
					tmpline[HeaderPos] = ':';

					if (strlen(tmpline)-HeaderPos-1 > 1)
					{
						delete m_MailHeader->SpamDogRemoteIp;
						m_MailHeader->SpamDogRemoteIp = new char[strlen(tmpline)-HeaderPos];
						memset(m_MailHeader->SpamDogRemoteIp,0,strlen(tmpline)-HeaderPos);
						memcpy(m_MailHeader->SpamDogRemoteIp,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-1); 

						Trim(m_MailHeader->SpamDogRemoteIp);
						ReplaceToSpace(m_MailHeader->SpamDogRemoteIp);

					}				
				} 
				else if (_stricmp(tmpline,"X-Mailer") == 0)
				{
					tmpline[HeaderPos] = ':';

					//再確認一次是否是 X-Mailer: SpamDog AntiSpam (IP)
					if (_strnicmp(tmpline,"X-Mailer: SpamDog AntiSpam",26) == 0)
					{
						HeaderPos += 18;

						if (strlen(tmpline)-HeaderPos-1 > 1)
						{
							delete m_MailHeader->SpamDogXMailer;
							m_MailHeader->SpamDogXMailer = new char[strlen(tmpline)-HeaderPos-2];
							memset(m_MailHeader->SpamDogXMailer,0,strlen(tmpline)-HeaderPos-2);
							memcpy(m_MailHeader->SpamDogXMailer,tmpline + HeaderPos+2 , strlen(tmpline)-HeaderPos-3); 

							Trim(m_MailHeader->SpamDogXMailer);
							ReplaceToSpace(m_MailHeader->SpamDogXMailer);

						}	
					}
				} 
			
			}

			

			delete tmpline;
			LineBegin = i + 2;
		
		}
	
	
	}

	return m_MailHeader;

}
void CEmail::FreeMailHeader(MailHeader* AMailHeader)
{

 
	//if (AMailHeader->Content_Transfer_Encoding != NULL)
	  delete AMailHeader->Content_Transfer_Encoding;

	//if (AMailHeader->Content_Type != NULL)
	  
	
	//if (AMailHeader->From != NULL)
	  delete AMailHeader->From;

	//if (AMailHeader->Reply_To != NULL)
	  delete AMailHeader->Reply_To;

	//if (AMailHeader->Return_Path != NULL)
	  delete AMailHeader->Return_Path;

	//if (AMailHeader->Subject != NULL)
	  delete AMailHeader->Subject;

	//if (AMailHeader->To != NULL)
	  delete AMailHeader->To;

	 // delete AMailHeader->Received;

	  FreeReceivedList(&AMailHeader->m_ReceievedList);

	  delete AMailHeader->Content_ID;

	  delete AMailHeader->Content_Type->boundary;
	  delete AMailHeader->Content_Type->charset;
	  delete AMailHeader->Content_Type->name;
	  delete AMailHeader->Content_Type->text;
	  delete AMailHeader->Content_Type->type;
	  delete AMailHeader->Content_Type;
	  delete AMailHeader->SpamDogMailFrom;
	  delete AMailHeader->SpamDogRcptTo;
	  delete AMailHeader->SpamDogRemoteIp;
	  delete AMailHeader->SpamDogXMailer;
	
	  delete AMailHeader;
}

MailBoundary* CEmail::GetBoundary(char* MailContent , char* FirstBoundStr)
{
	MailBoundary* m_MailBoundary = new MailBoundary;

	//MailBoundary* m_MailBoundary;
	memset(m_MailBoundary,0,sizeof(MailBoundary));

    BoundaryParser(MailContent,FirstBoundStr,m_MailBoundary);


	return m_MailBoundary;

}

void CEmail::ReplaceToSpace(char *AStr)
{
	int len = strlen(AStr);

	for (int i = 0 ; i < len ; i ++)
	{
		if (AStr[i] == char(13) ||
			AStr[i] == char(10) ||
			AStr[i] == char(9) )
		{
			AStr[i] = char(0x20);
		}
	
	}

}

bool CEmail::CheckValid(char *Email)
{
	//檢查是否是正確的 email
	char* dupEmail = _strdup(Email);
	bool bOk = false;

	char* pos1 = (char *) _mbsstr((unsigned char *) dupEmail,(unsigned char *)"@");
	
	if (pos1 == NULL) //找不到 @
	{
	  bOk =  false;
	} 
	else if (*dupEmail == *pos1) //@ 左邊空白
	{
	  bOk =  false;
	}
	else if (*(dupEmail + 1) == 0) //@右邊空白
	{
	  bOk = false;
	}
	else
	{
	  bOk = true;
	}



	delete dupEmail;

	return bOk;

}

void CEmail::FreeBoundary(MailBoundary* AMailBoundary)
{
	MailBoundary *ptr = AMailBoundary;
	while (ptr)
	{	  
	    
	    MailBoundary *nextptr = ptr->next;
	 

		if (ptr->BoundContent != NULL)
			delete ptr->BoundContent;

		if (ptr->BoundMail != NULL)
			FreeMailData(ptr->BoundMail);

		if (ptr->BoundHeader != NULL)
			FreeMailHeader(ptr->BoundHeader);

		delete ptr;	    
		ptr = nextptr;		  
	}
}

void CEmail::SplitEmailAddress(char *AddressStr , char* PersonalName , char* EMail)
{

	Trim(AddressStr);

	//Find Space
	int CutOffset = 0;
	int CutOffset2 = 0;

	char* spac = strstr(AddressStr+1,"\" "); //always add one space to find , prevent From: " Toeic多益全修" <edm@hotmail.com>	
	if (spac == NULL)
	{
	    spac = strstr(AddressStr," <");
		if (spac != NULL)
		{
			CutOffset2 = -1;
		}
		else
		{
			spac = strstr(AddressStr,"<");
			CutOffset = -1;

			
		}
	}
	else
	{
		CutOffset = 1;
	}

	

	if (spac != NULL && AddressStr != spac)
	{
	   	strncpy(PersonalName,AddressStr,spac - AddressStr+CutOffset);
		PersonalName[spac - AddressStr+CutOffset] = 0;
		TrimQuote(PersonalName,'"');

		spac = spac+1+CutOffset+CutOffset2;

		Trim(spac);

		TrimQuote(spac,'<');
		TrimQuote(spac,'>');

		if (CheckValid(spac))
		{
		 strcpy(EMail,spac);	
		}


	}
	else if (spac != NULL && AddressStr == spac)
	{
	 
			TrimQuote(spac,'<');
			TrimQuote(spac,'>');

			if (CheckValid(spac))
			{
				strcpy(EMail,spac);	
			}

	}else
	{	 

		if (CheckValid(AddressStr))
		{
			strcpy(EMail,AddressStr);	
		}
	}
/*


	int len = strlen(AddressStr);
	int StartPos = -1;
	int EndPos = -1;
	for (int i = 0 ; i < len ; i++)
	{
	
		if (AddressStr[i] == '<')
		{		
			StartPos = i + 1;		
		}
		else if (StartPos > -1 && AddressStr[i] == '>') 
		{
			EndPos = i - 1;
		}
	
	}

	if (StartPos > -1 && EndPos > -1)
	{
		
		if (StartPos - 2 > 0)
		{
			strncpy(PersonalName,AddressStr,StartPos - 2);
			AddressStr[StartPos - 2] = 0;
		}

		strncpy(EMail,AddressStr + StartPos , EndPos - StartPos + 1);
		EMail[EndPos - StartPos + 1] = 0;
	
	}*/


}

MailBoundary* CEmail::BoundaryParser(char* MailContent, char* BoundStr , MailBoundary* AMailBoundary)
{

	

	MailBoundary *RtnMailBoundary = AMailBoundary;
	MailBoundary *SaveMailBoundary = AMailBoundary;

	//MailBoundary* rtnBound = AMailBoundary;
	
	//MailContent 為已經解掉 header 的資料
	unsigned int BoundStrLen = strlen(BoundStr);

	char *tmpBoundStr = new char[BoundStrLen + 5];
	strcpy(tmpBoundStr,"--");
	strcat(tmpBoundStr,BoundStr);
	strcat(tmpBoundStr,"\r\n");

	char *tmpBoundStrEnd = new char[BoundStrLen + 3 + 3];
	strcpy(tmpBoundStrEnd,"--");
	strcat(tmpBoundStrEnd,BoundStr);	
	strcat(tmpBoundStrEnd,"--");
	

	BoundStrLen += 2; // -- 符號

	int startpos = pos(MailContent,tmpBoundStr,0);
	if (startpos > -1)
	{
		/*

		if (*(MailContent + startpos + BoundStrLen ) == '-' &&
			*(MailContent + startpos + BoundStrLen + 1) == '-')
		{		
			//在尾端
			AMailBoundary->prev->BoundLen = MailContent + startpos - 
				                            AMailBoundary->prev->BoundPos;

		
			MailBoundary *tmpMailBoundary = AMailBoundary->prev;
			AMailBoundary->prev->next = NULL;

			delete AMailBoundary;

		 
		
		}
		else
		{
			AMailBoundary->BoundPos = MailContent + startpos + BoundStrLen + 2;//跳過 #13#10
			AMailBoundary->BoundLen = 0;

				
			

			if (AMailBoundary->prev != NULL)
			{				
				//已知上一包完成
				AMailBoundary->prev->BoundLen = MailContent + startpos - 
					                            AMailBoundary->prev->BoundPos;


			
			}
			 

			AMailBoundary->next = new MailBoundary;
			AMailBoundary->next->prev = AMailBoundary;

			BoundaryParser(MailContent + startpos + BoundStrLen + 2,BoundStr,AMailBoundary->next);		
		}
*/

		int srclevel = AMailBoundary->level;
		int endpos = pos(MailContent,tmpBoundStrEnd,startpos+1);

		if (endpos > -1 )
		{
			
				AMailBoundary->BoundContent = new char[endpos - startpos - BoundStrLen - 1];
				memcpy(AMailBoundary->BoundContent,MailContent + startpos + BoundStrLen + 2,endpos - BoundStrLen - startpos - 2);
				AMailBoundary->BoundContent[endpos - BoundStrLen - startpos - 2 ] = 0;

			
				MailData *tmpMail = GetMailData(AMailBoundary->BoundContent);
				MailHeader *tmpHeader = GetMailHeader(tmpMail->mailHeader);

				int endpos2 = -1;
				int offset = 0;
				//bool HasEndPart = false;
				MailBoundary *LastMailBoundary = NULL;
				int Lastendpos2 = -1;

				while ((endpos2 = pos(AMailBoundary->BoundContent ,tmpBoundStr,offset)) > -1)
				{
					//向下
					        //檢查非最後
							if (strncmp(AMailBoundary->BoundContent + endpos2 + BoundStrLen + 2 , "--",2) == 0)
								 break;
					
							Lastendpos2 = endpos2 + BoundStrLen + 2;

					 	  	MailBoundary *NewMailBoundary = new MailBoundary;
							memset(NewMailBoundary,0,sizeof(MailBoundary));
							NewMailBoundary->level = srclevel ;
							LastMailBoundary = NewMailBoundary;
							RtnMailBoundary = LastMailBoundary;
							//AMailBoundary->child = NewMailBoundary;
							SaveMailBoundary->next = NewMailBoundary;
							SaveMailBoundary = NewMailBoundary; 

							NewMailBoundary->BoundContent = new char[endpos2 - offset  + 1];
							memcpy(NewMailBoundary->BoundContent,AMailBoundary->BoundContent + offset ,endpos2 - offset);
							NewMailBoundary->BoundContent[endpos2 - offset] = 0;

							MailData *tmpMail = GetMailData(NewMailBoundary->BoundContent);
							MailHeader *tmpHeader = GetMailHeader(tmpMail->mailHeader);

							if (strcmp(tmpHeader->Content_Type->boundary,"") != 0)
							{
								MailBoundary *NewMailBoundary2 = new MailBoundary;
								memset(NewMailBoundary2,0,sizeof(MailBoundary));
								NewMailBoundary2->level = srclevel + 2;								
								NewMailBoundary->next = NewMailBoundary2;

								
								//再找下一個
								LastMailBoundary = BoundaryParser(tmpMail->mailBody,tmpHeader->Content_Type->boundary,NewMailBoundary2);

								RtnMailBoundary = LastMailBoundary;
								SaveMailBoundary = LastMailBoundary;

								FreeMailHeader(tmpHeader);
								FreeMailData(tmpMail);

							}
							else
							{
								NewMailBoundary->IsRealData = true;
								NewMailBoundary->BoundMail = tmpMail;
								NewMailBoundary->BoundHeader = tmpHeader;
							}

						


							offset = endpos2 + BoundStrLen + 1;	
						
				
				}

				if (LastMailBoundary != NULL)
				{
					//End part
					        FreeMailHeader(tmpHeader);
						    FreeMailData(tmpMail);

							MailBoundary *NewMailBoundary = new MailBoundary;
							memset(NewMailBoundary,0,sizeof(MailBoundary));
							NewMailBoundary->level = srclevel ;
							
							//AMailBoundary->child = NewMailBoundary;
							LastMailBoundary->next = NewMailBoundary;
							RtnMailBoundary = NewMailBoundary;

							NewMailBoundary->BoundContent = new char[endpos - Lastendpos2  + 1];
							memcpy(NewMailBoundary->BoundContent,AMailBoundary->BoundContent + Lastendpos2 ,endpos - Lastendpos2);
							NewMailBoundary->BoundContent[endpos - Lastendpos2] = 0;

							MailData *tmpMail = GetMailData(NewMailBoundary->BoundContent);
							MailHeader *tmpHeader = GetMailHeader(tmpMail->mailHeader);

							if (strcmp(tmpHeader->Content_Type->boundary,"") != 0)
							{
								MailBoundary *NewMailBoundary2 = new MailBoundary;
								memset(NewMailBoundary2,0,sizeof(MailBoundary));
								NewMailBoundary2->level = srclevel + 2;								
								NewMailBoundary->next = NewMailBoundary2;
						
								//再找下一個
								BoundaryParser(tmpMail->mailBody,tmpHeader->Content_Type->boundary,NewMailBoundary2);

								RtnMailBoundary = NewMailBoundary2;

								FreeMailHeader(tmpHeader);
								FreeMailData(tmpMail);

							}
							else
							{
								NewMailBoundary->IsRealData = true;
								NewMailBoundary->BoundMail = tmpMail;
								NewMailBoundary->BoundHeader = tmpHeader;
							}

				}
				else
				{
							if (strcmp(tmpHeader->Content_Type->boundary,"") != 0)
							{
								MailBoundary *NewMailBoundary2 = new MailBoundary;
								memset(NewMailBoundary2,0,sizeof(MailBoundary));
								NewMailBoundary2->level = srclevel + 2;								
								SaveMailBoundary->next = NewMailBoundary2;
						
								//再找下一個
								BoundaryParser(tmpMail->mailBody,tmpHeader->Content_Type->boundary,NewMailBoundary2);

								RtnMailBoundary = NewMailBoundary2;

								FreeMailHeader(tmpHeader);
								FreeMailData(tmpMail);

							}
							else
							{
								SaveMailBoundary->IsRealData = true;
								SaveMailBoundary->BoundMail = tmpMail;
								SaveMailBoundary->BoundHeader = tmpHeader;
							}
				
				
				
				}






/*
				if (endpos2 > -1)
				{
						  //仍有就需要再切割
						  	MailBoundary *NewMailBoundary = new MailBoundary;
							memset(NewMailBoundary,0,sizeof(MailBoundary));
							NewMailBoundary->level = srclevel ;
							
							//AMailBoundary->child = NewMailBoundary;
							AMailBoundary->next = NewMailBoundary;

							NewMailBoundary->BoundContent = new char[endpos2  - 1];
							memcpy(NewMailBoundary->BoundContent,AMailBoundary->BoundContent ,endpos2  - 2);
							NewMailBoundary->BoundContent[endpos2  - 2 ] = 0;

							MailData *tmpMail = GetMailData(NewMailBoundary->BoundContent);
							MailHeader *tmpHeader = GetMailHeader(tmpMail->mailHeader);

							if (strcmp(tmpHeader->Content_Type->boundary,"") != 0)
							{
								MailBoundary *NewMailBoundary2 = new MailBoundary;
								memset(NewMailBoundary2,0,sizeof(MailBoundary));
								NewMailBoundary2->level = srclevel + 2;								
								NewMailBoundary->next = NewMailBoundary2;
						
								//再找下一個
								AMailBoundary = BoundaryParser(tmpMail->mailBody,tmpHeader->Content_Type->boundary,NewMailBoundary2);

								FreeMailHeader(tmpHeader);
								FreeMailData(tmpMail);

							}
							else
							{
								NewMailBoundary->IsRealData = true;
								NewMailBoundary->BoundMail = tmpMail;
								NewMailBoundary->BoundHeader = tmpHeader;
							}


							MailBoundary *NewMailBoundary2 = new MailBoundary;
							memset(NewMailBoundary2,0,sizeof(MailBoundary));
							NewMailBoundary2->level = srclevel ;							
							NewMailBoundary->next = NewMailBoundary2;							

							AMailBoundary = BoundaryParser(AMailBoundary->BoundContent + endpos2 - 1 ,BoundStr,NewMailBoundary->next);				  
						
					  
				}
				else
				{
						AMailBoundary->IsRealData = true;
						AMailBoundary->BoundMail = tmpMail;
						AMailBoundary->BoundHeader = tmpHeader;
				}*/

				
			

			//確認是否在尾端
		/*	if (*(MailContent + endpos + BoundStrLen ) == '-' &&
				*(MailContent + endpos + BoundStrLen + 1) == '-')
			{			
				
				//已在尾端
				//AMailBoundary->BoundPos =  MailContent + startpos + BoundStrLen + 2 ;//跳過 #13#10
				//AMailBoundary->BoundLen =  endpos - startpos +1;
				//AMailBoundary->BoundContent = new char[endpos - startpos +2];
				//memcpy(AMailBoundary->BoundContent,MailContent + startpos + BoundStrLen + 2,endpos - startpos +1);
				//AMailBoundary->BoundContent[endpos - startpos +1] = 0;
				AMailBoundary->next = NULL;

			}
			else
			{
				//下面還有平行的 boundary
				//AMailBoundary->BoundPos =  MailContent + startpos + BoundStrLen + 2 ;//跳過 #13#10
				//AMailBoundary->BoundLen =  endpos - startpos +1;			

				AMailBoundary->next = new MailBoundary;	
				memset(AMailBoundary->next,0,sizeof(MailBoundary));

				AMailBoundary->next->level = srclevel;
				
				//再找下一個
				AMailBoundary = BoundaryParser(MailContent + endpos ,BoundStr,AMailBoundary->next);

			}*/

			
			
		}
	}
		/*else
		{
			
			//已經到尾端了 沒有結尾符號的狀況
				AMailBoundary->BoundContent = new char[strlen(MailContent) - startpos - BoundStrLen - 1];
				memcpy(AMailBoundary->BoundContent,MailContent + startpos + BoundStrLen + 2,strlen(MailContent) - BoundStrLen - startpos - 2);
				AMailBoundary->BoundContent[strlen(MailContent) - BoundStrLen - startpos - 2] = 0;


					//往深度
				MailData *tmpMail = GetMailData(AMailBoundary->BoundContent);
				MailHeader *tmpHeader = GetMailHeader(tmpMail->mailHeader);

				  if (strcmp(tmpHeader->Content_Type->boundary,"") != 0)
				  {
				  
					  	MailBoundary *NewMailBoundary = new MailBoundary;
						memset(NewMailBoundary,0,sizeof(MailBoundary));
						NewMailBoundary->level = srclevel + 1;
						//AMailBoundary->child = NewMailBoundary;
						AMailBoundary->next = NewMailBoundary;
						//再找下一個
						AMailBoundary = BoundaryParser(tmpMail->mailBody,tmpHeader->Content_Type->boundary,NewMailBoundary);

						FreeMailHeader(tmpHeader);
						FreeMailData(tmpMail);

				  }
				  else
				  {
					  //實體資料
					  AMailBoundary->IsRealData = true;
					  
					  AMailBoundary->BoundMail = tmpMail;
					  AMailBoundary->BoundHeader = tmpHeader;
				  
				  }

				

			//AMailBoundary->BoundPos =  MailContent + startpos + BoundStrLen + 2 ;//跳過 #13#10
			//AMailBoundary->BoundLen =  BoundStrLen - startpos +1;
			AMailBoundary->next = NULL;
		}
		

	}
	else
	{
			//完全找不到 boundary 就回傳 null		 
			//AMailBoundary->BoundPos = MailContent;
			//AMailBoundary->BoundLen = BoundStrLen;
		    AMailBoundary->BoundContent = NULL;
			AMailBoundary->next = NULL;
			//AMailBoundary->prev = NULL;

			

	}*/

	delete tmpBoundStr;	
	delete tmpBoundStrEnd;

	return RtnMailBoundary; //皆是回傳最後產生的 boundary

 	 
}

/////////////////////////////////////////////////////////////////////////////
// CMailCodec

CMailCodec::CMailCodec()
{
  /*BaseBuffer = NULL;
  QPBuffer = NULL;
  UniBuffer = NULL;
  HeaderBuffer = NULL;*/

  //Base64EnTable =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}

CMailCodec::~CMailCodec()
{
	/*if (BaseBuffer != NULL)
		delete BaseBuffer;

	if (QPBuffer != NULL)
		delete QPBuffer;

	if (UniBuffer != NULL)
		delete UniBuffer;

	if(HeaderBuffer != NULL)
		delete HeaderBuffer;*/

}


char CMailCodec::Base64Table[] = 
{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,   0,   0,  63, 
 52,  53,  54,  55,  56,  57,  58,  59,  60,  61,   0,   0,   0,   0,   0,   0, 
  0,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14, 
 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,   0,   0,   0,   0,   0, 
  0,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40, 
 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,   0,   0,   0,   0,   0 
};

DLLDIR char CMailCodec::Base64Table2[] = 
{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   62,  0,   0,   0,   0,  63, 
 52,  53,  54,  55,  56,  57,  58,  59,  60,  61,   0,   0,   0,   0,   0,   0, 
  0,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14, 
 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,   0,   0,   0,   0,   0, 
  0,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40, 
 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,   0,   0,   0,   0,   0 
};

char CMailCodec::Base64TableM[] = 
{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   63,  0,   0,   0, 
 52,  53,  54,  55,  56,  57,  58,  59,  60,  61,   0,   0,   0,   0,   0,   0, 
  0,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14, 
 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,   0,   0,   0,   0,   0, 
  0,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40, 
 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,   0,   0,   0,   0,   0 
};

int CMailCodec::ImapFolderTable[] = 
{ -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
   1,   1,   1,   1,   1,   1,  -1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  -1  
};
char CMailCodec::Base64EnTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char CMailCodec::Base64EnTable2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789*/"; // + => *
char CMailCodec::Base64EnTableM[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";
 
int CMailCodec::QPTable[]=
{ -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   0,  1,   2,   3,   4,   5,   6,   7,   8,   9,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, 10,  11,  12,  13,  14,  15,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, 10,  11,  12,  13,  14,  15,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
};

int CMailCodec::IntTable[]=
{ -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   0,  1,   2,   3,   4,   5,   6,   7,   8,   9,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
};

int CMailCodec::StrToInt(char* Astr)
{

	register unsigned int length, i, value, shift;
	
    
	if (QPTable[(unsigned int) Astr[0]] == QPTable[(unsigned int) 'x'])
	{
		length = strlen(Astr) - 1;

		//for (length = 0; length < 9; length++) if (!hexstr[length]) break;

		shift = (length - 1) * 4;
		for (i = value = 0; i < length; i++, shift -= 4)
			value += QPTable[(unsigned int)Astr[i+1] & 127] << shift;
	}
	else
	{
		length = strlen(Astr) ;		

		shift = (length - 1);
		for (i = value = 0; i < length; i++, shift --)
			value += IntTable[(unsigned int)Astr[i] & 127] * (int)pow(10,shift);
	
	}

    return value;

}
char* CMailCodec::Base64Decode(char* InStr)
{
	char *BaseBuffer = NULL;

	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;

	BaseBuffer = new char[len+1];
	memset(BaseBuffer,0,len);

	while (i < len)
    {
                while (i < len && (InStr[i] == 13 || InStr[i] == 10))
                        i++;

                if (i < len)
                {
                        char b1 = (char)((Base64Table[(int)InStr[i] & 127] << 2 & 0xfc) +
                                        (Base64Table[(int)InStr[i + 1] & 127] >> 4 & 0x03));
                        //output += b1;
						memcpy(BaseBuffer + parsecount ,&b1,1);
						parsecount++;

                        if (InStr[i + 2] != '=')
                        {
                                char b2 = (char)((Base64Table[(int)InStr[i + 1] & 127] << 4 & 0xf0) +
                                                (Base64Table[(int)InStr[i + 2] & 127] >> 2 & 0x0f));
                                //output += b2;
								memcpy(BaseBuffer + parsecount ,&b2,1);
								parsecount++;
                        }
                        if (InStr[i + 3] != '=')
                        {
                                char b3 = (char)((Base64Table[(int)InStr[i + 2] & 127] << 6 & 0xc0) +
                                                Base64Table[(int)InStr[i + 3] & 127]);
                                //output += b3;
								memcpy(BaseBuffer + parsecount ,&b3,1);
								parsecount++;
                        }

                        i += 4;
                }
    }


	BaseBuffer[parsecount] = 0;
	return BaseBuffer;
}

void CMailCodec::Base64Decode(char* InStr,char** OutBuff,unsigned int *OutSize)
{
	char *BaseBuffer = NULL;

	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;

	BaseBuffer = new char[len+1];
	memset(BaseBuffer,0,len);

	while (i < len)
    {
                while (i < len && (InStr[i] == 13 || InStr[i] == 10))
                        i++;

                if (i < len)
                {
                        char b1 = (char)((Base64Table[(int)InStr[i] & 127] << 2 & 0xfc) +
                                        (Base64Table[(int)InStr[i + 1] & 127] >> 4 & 0x03));
                        //output += b1;
						memcpy(BaseBuffer + parsecount ,&b1,1);
						parsecount++;

                        if (InStr[i + 2] != '=')
                        {
                                char b2 = (char)((Base64Table[(int)InStr[i + 1] & 127] << 4 & 0xf0) +
                                                (Base64Table[(int)InStr[i + 2] & 127] >> 2 & 0x0f));
                                //output += b2;
								memcpy(BaseBuffer + parsecount ,&b2,1);
								parsecount++;
                        }
                        if (InStr[i + 3] != '=')
                        {
                                char b3 = (char)((Base64Table[(int)InStr[i + 2] & 127] << 6 & 0xc0) +
                                                Base64Table[(int)InStr[i + 3] & 127]);
                                //output += b3;
								memcpy(BaseBuffer + parsecount ,&b3,1);
								parsecount++;
                        }

                        i += 4;
                }
    }


	BaseBuffer[parsecount] = 0;

	*OutBuff = BaseBuffer;
	*OutSize  = parsecount;
	 
}

void CMailCodec::ImapUTF7Decode(char* InStr,char** OutBuff,unsigned int *OutSize)
{

	char *BaseBuffer = NULL;

	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	int StartCode = -1;
	

	BaseBuffer = new char[len+1];
	memset(BaseBuffer,0,len);

	for (unsigned int i = 0 ; i < len ; i ++)
	{
	
		if (StartCode == -1 && (InStr[i] == '&' && InStr[i+1] == '-'))
		{
			BaseBuffer[parsecount] = '&';
			parsecount ++ ;		
		}
		else if (StartCode == -1 && InStr[i] == '&')
		{		
			StartCode = i + 1 ;
		}
		else if (StartCode != -1 && InStr[i] == '-')
		{
		
			//放入 MBase64
			char *tmpBuf = NULL;
			unsigned int tmpCount = 0 ;
			DeImapBase64M(InStr + StartCode , i - StartCode ,&tmpBuf,&tmpCount);
			memcpy(BaseBuffer + parsecount , tmpBuf , tmpCount);
			delete tmpBuf;

			parsecount += tmpCount;

			StartCode = -1;

		
		} 
		else if (StartCode == -1)
		{
		
			BaseBuffer[parsecount] = InStr[i] ;
			parsecount ++ ;
		}

	
	
	}


	*OutBuff = BaseBuffer;
	*OutSize  = parsecount;

	


}


void CMailCodec::Base64Decode2(char* InStr,char** OutBuff,unsigned int *OutSize)
{
	char *BaseBuffer = NULL;

	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;

	BaseBuffer = new char[len+1];
	memset(BaseBuffer,0,len);

	while (i < len)
    {
                while (i < len && (InStr[i] == 13 || InStr[i] == 10))
                        i++;

                if (i < len)
                {
                        char b1 = (char)((Base64Table2[(int)InStr[i] & 127] << 2 & 0xfc) +
                                        (Base64Table2[(int)InStr[i + 1] & 127] >> 4 & 0x03));
                        //output += b1;
						memcpy(BaseBuffer + parsecount ,&b1,1);
						parsecount++;

                        if (InStr[i + 2] != '=')
                        {
                                char b2 = (char)((Base64Table2[(int)InStr[i + 1] & 127] << 4 & 0xf0) +
                                                (Base64Table2[(int)InStr[i + 2] & 127] >> 2 & 0x0f));
                                //output += b2;
								memcpy(BaseBuffer + parsecount ,&b2,1);
								parsecount++;
                        }
                        if (InStr[i + 3] != '=')
                        {
                                char b3 = (char)((Base64Table2[(int)InStr[i + 2] & 127] << 6 & 0xc0) +
                                                Base64Table2[(int)InStr[i + 3] & 127]);
                                //output += b3;
								memcpy(BaseBuffer + parsecount ,&b3,1);
								parsecount++;
                        }

                        i += 4;
                }
    }


	BaseBuffer[parsecount] = 0;

	*OutBuff = BaseBuffer;
	*OutSize  = parsecount;
	 
}

char* CMailCodec::Base64Encode(char* InStr, unsigned int size)
{


    char *BaseBuffer = NULL;

	unsigned int len = size;
	unsigned int parsecount = 0;
	 

	BaseBuffer = new char[len*4];
	memset(BaseBuffer,0,len*4);

  size_t i = 0;   
  while (i < len)
  {
          size_t remain = len - i;
          
          switch (remain)
          {
          case 1:
                  BaseBuffer[parsecount] = Base64EnTable[ ((InStr[i] >> 2) & 0x3f) ];
                  BaseBuffer[parsecount+1] = Base64EnTable[ ((InStr[i] << 4) & 0x30) ];
                  BaseBuffer[parsecount+2] = '=';
				  BaseBuffer[parsecount+3] = '=';

				  parsecount += 4;
                  break;
          case 2:
                  BaseBuffer[parsecount] = Base64EnTable[ ((InStr[i] >> 2) & 0x3f) ];
                  BaseBuffer[parsecount+1] = Base64EnTable[ ((InStr[i] << 4) & 0x30) + ((InStr[i + 1] >> 4) & 0x0f) ];
                  BaseBuffer[parsecount+2] = Base64EnTable[ ((InStr[i + 1] << 2) & 0x3c) ];
                  BaseBuffer[parsecount+3] = '=';

				  parsecount += 4;
                  break;
          default:
                  BaseBuffer[parsecount] = Base64EnTable[ ((InStr[i] >> 2) & 0x3f) ];
                  BaseBuffer[parsecount+1] = Base64EnTable[ ((InStr[i] << 4) & 0x30) + ((InStr[i + 1] >> 4) & 0x0f) ];
                  BaseBuffer[parsecount+2] = Base64EnTable[ ((InStr[i + 1] << 2) & 0x3c) + ((InStr[i + 2] >> 6) & 0x03) ];
                  BaseBuffer[parsecount+3] = Base64EnTable[ (InStr[i + 2] & 0x3f) ];

				  parsecount += 4;
          }
          
          i += 3;
  }

  BaseBuffer[parsecount] = 0;
  return BaseBuffer;

}

void CMailCodec::DeImapBase64M(char* InStr,unsigned int len,char** OutBuff,unsigned int *OutSize)
{
	char *BaseBuffer = NULL;

	char *InStrBuf = new char[len+3]; //為了 Base64 Code 使用
	memset(InStrBuf , 0 , len + 3);
	memcpy(InStrBuf,InStr,len);

	//unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;
	 

	BaseBuffer = new char[len+1];
	memset(BaseBuffer,0,len);

	//為了要讓 WideCharToMultiByte 能直接轉 要做 byte swap

	while (i < len)
    {
                while (i < len && (InStrBuf[i] == 13 || InStrBuf[i] == 10))
                        i++;

                if (i < len)
                {
                        char b1 = (char)((Base64TableM[(int)InStrBuf[i] & 127] << 2 & 0xfc) +
                                        (Base64TableM[(int)InStrBuf[i + 1] & 127] >> 4 & 0x03));
                        
						
						memcpy(BaseBuffer + parsecount  ,&b1,1);						
						parsecount++;

                        
                        char b2 = (char)((Base64TableM[(int)InStrBuf[i + 1] & 127] << 4 & 0xf0) +
                                                (Base64TableM[(int)InStrBuf[i + 2] & 127] >> 2 & 0x0f));
                        

						
						memcpy(BaseBuffer + parsecount  ,&b2,1);						
						parsecount ++;						
						
                        
                        char b3 = (char)((Base64TableM[(int)InStrBuf[i + 2] & 127] << 6 & 0xc0) +
                                                Base64TableM[(int)InStrBuf[i + 3] & 127]);
                                
						memcpy(BaseBuffer + parsecount,&b3,1);						
						parsecount++;
                        

                        i += 4;
                }
    }

	
	BaseBuffer[parsecount] = 0;

	delete InStrBuf;

	//做 Byte Swap

	for (unsigned int j = 0 ; j < parsecount ; j += 2)
	{

		char TmpChar = BaseBuffer[j];
		BaseBuffer[j] = BaseBuffer[j+1];
		BaseBuffer[j+1] = TmpChar;

	
	}

	//轉為 Current CodePage
	wchar_t *wchr = new  wchar_t[parsecount ];
	memset(wchr,0,parsecount );
	memcpy(wchr,BaseBuffer,parsecount);
	memset(BaseBuffer,0,parsecount);
	WideCharToMultiByte(CP_ACP,0,wchr,-1,BaseBuffer,parsecount,0,0);

	parsecount = strlen(BaseBuffer);

	delete wchr;

	*OutBuff = BaseBuffer;
	*OutSize  = parsecount;




}

char* CMailCodec::ImapBase64M(wchar_t* InStrW , unsigned int size)
{

	//wchar_t *InStrW = new wchar_t[size+1];							
	//MultiByteToWideChar(CP_ACP,0,InStr,size+1,InStrW,size+1);

 
	char *BaseBuffer = NULL;

	unsigned int len = size;// wcslen(InStrW);
	unsigned int parsecount = 0;
	 

	BaseBuffer = new char[len*4];
	memset(BaseBuffer,0,len*4);

	  size_t i = 0;   
	  while (i < len)
	  {
      
		  size_t remain = len - i;  

					switch (remain)
					{

						case 1:
							BaseBuffer[parsecount] = Base64EnTableM[ (( ((InStrW[i] >> 8 ) &0xFF ) >> 2) & 0x3f) ];
							BaseBuffer[parsecount+1] = Base64EnTableM[ ((((InStrW[i] >> 8  ) &0xFF ) << 4) & 0x30) + ((((InStrW[i] ) & 0xFF) >> 4) & 0x0f) ];
							BaseBuffer[parsecount+2] = Base64EnTableM[ (( ((InStrW[i]) & 0xFF) << 2) & 0x3c) ];

							parsecount += 3;
							i += 1;

							break;

						case 2:

							BaseBuffer[parsecount] = Base64EnTableM[ (( ((InStrW[i] >> 8 ) &0xFF ) >> 2) & 0x3f) ];
							BaseBuffer[parsecount+1] = Base64EnTableM[ ((((InStrW[i] >> 8  ) &0xFF ) << 4) & 0x30) + ((((InStrW[i] ) & 0xFF) >> 4) & 0x0f) ];
							BaseBuffer[parsecount+2] = Base64EnTableM[ (( ((InStrW[i]) & 0xFF) << 2) & 0x3c) + (( ((InStrW[i+1] >> 8 ) & 0xFF)  >> 6) & 0x03) ];
							BaseBuffer[parsecount+3] = Base64EnTableM[ ( ((InStrW[i+1] >> 8) & 0xFF) & 0x3f) ];

							BaseBuffer[parsecount+4] = Base64EnTableM[ (( ((InStrW[i+1]) &0xFF ) >> 2) & 0x3f) ];	
							BaseBuffer[parsecount+5] = Base64EnTableM[ ((((InStrW[i+1] ) &0xFF ) << 4) & 0x30) ];

							parsecount += 6;
							i += 2;

							break;

						default:

							//在此做 Byte Swap
							BaseBuffer[parsecount] = Base64EnTableM[ (( ((InStrW[i] >> 8 ) &0xFF ) >> 2) & 0x3f) ];
							BaseBuffer[parsecount+1] = Base64EnTableM[ ((((InStrW[i] >> 8  ) &0xFF ) << 4) & 0x30) + ((((InStrW[i] ) & 0xFF) >> 4) & 0x0f) ];
							BaseBuffer[parsecount+2] = Base64EnTableM[ (( ((InStrW[i]) & 0xFF) << 2) & 0x3c) + (( ((InStrW[i+1] >> 8 ) & 0xFF)  >> 6) & 0x03) ];
							BaseBuffer[parsecount+3] = Base64EnTableM[ ( ((InStrW[i+1] >> 8) & 0xFF) & 0x3f) ];

							BaseBuffer[parsecount+4] = Base64EnTableM[ (( ((InStrW[i+1]) &0xFF ) >> 2) & 0x3f) ];	
							BaseBuffer[parsecount+5] = Base64EnTableM[ ((((InStrW[i+1] ) &0xFF ) << 4) & 0x30) + ((((InStrW[i+2] >> 8) & 0xFF) >> 4) & 0x0f) ];
							BaseBuffer[parsecount+6] = Base64EnTableM[ (( ((InStrW[i+2] >> 8) & 0xFF) << 2) & 0x3c) + (( ((InStrW[i+2]) & 0xFF)  >> 6) & 0x03) ];
							BaseBuffer[parsecount+7] = Base64EnTableM[ ( ((InStrW[i+2]) & 0xFF) & 0x3f) ];
							
							
							parsecount += 8;
							i += 3;

							break;

					}
		
	  }

  BaseBuffer[parsecount] = 0;
  return BaseBuffer;


}

char* CMailCodec::ImapUTF7Encode(char* InStr , unsigned int size)
{
   //先確定哪些要編碼,確定再送 Base64  

	wchar_t *InStrW = new wchar_t[size+1];							
	MultiByteToWideChar(CP_ACP,0,InStr,size+1,InStrW,size+1);

	char *CodeBuffer = new char[size * 4];
	memset(CodeBuffer,0,size * 4);
	
	int StartCode = -1;
	int ParserCount = 0;
	unsigned int len = wcslen(InStrW);

	for(unsigned int i = 0 ; i < len ; i++)
	{
	
		if (StartCode == -1 && (InStrW[i] > 128 || ImapFolderTable[InStrW[i]] != 1))
		{		
			if (InStrW[i] == '&')
			{
				CodeBuffer[ParserCount] = '&';
				ParserCount ++;
				CodeBuffer[ParserCount] = '-';
				ParserCount ++;
			
			}
			else
			{
				//需要編碼 , 再找出結束位置
				StartCode = i;		

				CodeBuffer[ParserCount] = '&';
				ParserCount ++;
			}
			
		}
		else if (StartCode != -1 && ((InStrW[i] <= 128 && ImapFolderTable[InStrW[i]] == 1) || InStrW[i] == '&'))
		{	
			
			//放入 ImapBase64M
			char *UTF7 = ImapBase64M(InStrW + StartCode ,  i - StartCode);
            int len = strlen(UTF7);

			memcpy(CodeBuffer + ParserCount,UTF7,len);
			ParserCount += len;

			delete UTF7;

			StartCode = -1; //reset

			CodeBuffer[ParserCount] = '-';
			ParserCount ++;



			if (InStrW[i] == '&')
			{
				CodeBuffer[ParserCount] = '&';
				ParserCount ++;
				CodeBuffer[ParserCount] = '-';
				ParserCount ++;
			
			}
			else
			{
		        CodeBuffer[ParserCount] = InStrW[i] & 0xFF ;
			    ParserCount ++;
			}

			

		}
		else if (StartCode == -1)
		{		
			//直街放入 buffer
			CodeBuffer[ParserCount] = InStrW[i]  & 0xFF;
			ParserCount ++;


		
		}

	
	}

	if (StartCode != -1)
	{
	
		//編到最後
		char *UTF7 = ImapBase64M(InStrW + StartCode ,  len - StartCode);
        int len = strlen(UTF7);

		memcpy(CodeBuffer + ParserCount,UTF7,len);
		ParserCount += len;

		delete UTF7;

		CodeBuffer[ParserCount] = '-';
		ParserCount ++;

	
	}


	delete InStrW;
	return CodeBuffer;						
	


}

void CMailCodec::Free(char *Buffer)
{

	delete Buffer;

}

char* CMailCodec::Base64Encode2(char* InStr, unsigned int size)
{


    char *BaseBuffer = NULL;

	unsigned int len = size;
	unsigned int parsecount = 0;
	 

	BaseBuffer = new char[len*4];
	memset(BaseBuffer,0,len*4);

	  size_t i = 0;   
	  while (i < len)
	  {
			  size_t remain = len - i;
          
			  switch (remain)
			  {
				  case 1:
						  BaseBuffer[parsecount] = Base64EnTable2[ ((InStr[i] >> 2) & 0x3f) ];
						  BaseBuffer[parsecount+1] = Base64EnTable2[ ((InStr[i] << 4) & 0x30) ];
						  BaseBuffer[parsecount+2] = '=';
						  BaseBuffer[parsecount+3] = '=';

						  parsecount += 4;
						  break;
				  case 2:
						  BaseBuffer[parsecount] = Base64EnTable2[ ((InStr[i] >> 2) & 0x3f) ];
						  BaseBuffer[parsecount+1] = Base64EnTable2[ ((InStr[i] << 4) & 0x30) + ((InStr[i + 1] >> 4) & 0x0f) ];
						  BaseBuffer[parsecount+2] = Base64EnTable2[ ((InStr[i + 1] << 2) & 0x3c) ];
						  BaseBuffer[parsecount+3] = '=';

						  parsecount += 4;
						  break;
				  default:
						  BaseBuffer[parsecount] = Base64EnTable2[ ((InStr[i] >> 2) & 0x3f) ];
						  BaseBuffer[parsecount+1] = Base64EnTable2[ ((InStr[i] << 4) & 0x30) + ((InStr[i + 1] >> 4) & 0x0f) ];
						  BaseBuffer[parsecount+2] = Base64EnTable2[ ((InStr[i + 1] << 2) & 0x3c) + ((InStr[i + 2] >> 6) & 0x03) ];
						  BaseBuffer[parsecount+3] = Base64EnTable2[ (InStr[i + 2] & 0x3f) ];

						  parsecount += 4;
			  }
          
			  i += 3;
	  }

  BaseBuffer[parsecount] = 0;
  return BaseBuffer;

}

char* CMailCodec::QPDecode(char* InStr,char* StyleCode)
{
	char *QPBuffer = NULL;
	
	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;

	QPBuffer = new char[len+1];
	memset(QPBuffer,0,len);


	while (i < len)
    {

       if ((int) InStr[i] ==  *StyleCode) 
       {

            if ((i + 2) <  len)
            {
              if  (QPTable[(int)InStr[i+1] & 127] >= 0 && QPTable[(int)InStr[i+2] & 127] >= 0 )
              {
                 //Result := Result + Char(StrToInt('$' + mInStr[i+1] + mInStr[i+2]));
				  char tmpchar = (char) (QPTable[(int)InStr[i+1] & 127] * 16 + QPTable[(int)InStr[i+2] & 127]);
				  memcpy(QPBuffer + parsecount ,&tmpchar,1);
				  parsecount++;
              }
              else if ( InStr[i+1] != char(13)   ||  InStr[i+2] != char(10)) 
              {
               
                 //Result := Result + styleCode + InStr[i+1] + InStr[i+2];
				  memcpy(QPBuffer + parsecount ,StyleCode,1);
				  parsecount++;

				  memcpy(QPBuffer + parsecount ,InStr+i+1,2);
				  parsecount+=2;
              }
             
            }

            i += 3; 

       }
       else
       {

          //Result := Result + mInStr[i];
		  memcpy(QPBuffer + parsecount ,InStr+i,1);
		  parsecount++;
          i++;

       }



    }



	QPBuffer[parsecount] = 0;
	return QPBuffer;
}
  
char* CMailCodec::UniDecode(char* InStr)
{
	char *UniBuffer = NULL;
	

	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;
	char tmpchar[10]; //unicode 最常長度限制為 10
	unsigned int uCount = 0;
	wchar_t utmpchar[1] = {0};


	UniBuffer = new char[len+1];	
	memset(UniBuffer,0,len);


    
	while (i < len)
    {

       if ((int) InStr[i] == '&' && (int) InStr[i+1] == '#') 
       {
		   //走到 ;
		   uCount = 0;
		   while ( uCount < 10)
		   {		   
			   if ((int) InStr[i+2]  == ';' )
			   {
			       i += 3;
				   tmpchar[uCount] = 0;

				   break;
			   } 
			   else if (InStr[i+2] == '&')
			   {
				   i += 2;
				   tmpchar[uCount] = 0;

				   break;
			   }
			   else if (InStr[i+2] == 'x' || 
						(InStr[i+2] >= '0' && InStr[i+2] <= '9') ||
						(InStr[i+2] >= 'a' && InStr[i+2] <= 'z') ||
						(InStr[i+2] >= 'A' && InStr[i+2] <= 'Z') )
			   {
                 
				 tmpchar[uCount] = InStr[i+2];
				 uCount ++;
			   }
			   else
			   {
					i += 2;
					break;
			   }
			   
			   i++;

			   if (i+2 > len)
			   {
			   
				   i = len;
				   break;
			   
			   }
		   }

		   if (uCount > 0)
		   {
		     //轉碼			   
			  //wsprintfW(utmpchar,L"%c",StrToInt(tmpchar));
			  utmpchar[0] = StrToInt(tmpchar);

			  //判斷轉為 2 byte or 1 byte
			  BYTE ByteCount = 0;
			  if ((utmpchar[0] & 0xFF) > 128)
			  {
				  ByteCount = 2;
			  }
			  else
			  {
				  ByteCount = 1;
			  }				  

			  WideCharToMultiByte(CP_ACP, 0, utmpchar , -1,
				  UniBuffer + parsecount, ByteCount, NULL, NULL );
			  

			  parsecount+=ByteCount;	
			  uCount = 0;
		   }            

       }
       else
       {          
		  memcpy(UniBuffer + parsecount ,InStr+i,1);
		  parsecount++;
          i++;

       }
    }



	UniBuffer[parsecount] = 0;
	return UniBuffer;

}

char* CMailCodec::HeaderDecode(char* InStr)
{

	char *HeaderBuffer = NULL;
	

	
	unsigned int len = strlen(InStr);
	unsigned int parsecount = 0;
	unsigned int i = 0;
	unsigned int saveidx = 0;
	unsigned int BeginPos = 0 , EndPos = 0;

	char CodePage[10] = {0};
	char Encode[10] = {0};

	HeaderBuffer = new char[len+1];	
	memset(HeaderBuffer,0,len);


	//未在區間碼仍要顯示
	while (i < len)    
	{
	    if (InStr[i] == '=' && InStr[i+1] == '?')
		{

			BeginPos = parsecount;
			i += 2;
			//取 codepage
			saveidx = 0;
			while( InStr[i] != '?')
			{
				memcpy(CodePage+saveidx,InStr+i,1);
				saveidx++;
				i++;
			}
			saveidx = 0;
			i ++;
			//取編碼			
			while( InStr[i] != '?')
			{
				memcpy(Encode+saveidx,InStr+i,1);
				saveidx++;
				i++;
			}
			saveidx = 0;
			i++;
			//取內容 , 
			while( !(InStr[i] == '?' && InStr[i+1] == '=') && i < len )
			{
				memcpy(HeaderBuffer+parsecount,InStr+i,1);
				parsecount++;
				i++;
			}
			
		 
			i+=2;
			HeaderBuffer[parsecount] = 0;

			if (strlen(HeaderBuffer+BeginPos) > 0)
			{
	
				if(QPTable[(int)Encode[0] & 127] == QPTable[(int) 'b'])
				{
					//base64
					char *tmpres = Base64Decode(HeaderBuffer+BeginPos);
					//memset(HeaderBuffer,0,strlen(HeaderBuffer));
					//strcpy(HeaderBuffer,tmpres);			
					memcpy(HeaderBuffer+BeginPos,tmpres,strlen(tmpres));
					parsecount += strlen(tmpres)-strlen(HeaderBuffer+BeginPos);

					FreeBase64Decode(tmpres); 


				}
				else if (QPTable[(int)Encode[0] & 127] == QPTable[(int) 'q'])
				{
					//qp
					char *tmpres = QPDecode(HeaderBuffer+BeginPos);
					memcpy(HeaderBuffer+BeginPos,tmpres,strlen(tmpres));
					parsecount += strlen(tmpres) - strlen(HeaderBuffer+BeginPos);			
					FreeQPDecode(tmpres); 
		
				}
	
			}

			

		}
	

		memcpy(HeaderBuffer+parsecount,InStr+i,1);
		parsecount++;
		i++;
	}
	


   HeaderBuffer[parsecount] =0;

	 

	return HeaderBuffer;

}

 

void CMailCodec::FreeBase64Decode(char* Buffer)
{
  delete Buffer;
}

void CMailCodec::FreeBase64Encode(char* Buffer)
{
  delete Buffer;
}

void CMailCodec::FreeQPDecode(char* Buffer)
{
  delete Buffer;
}

void CMailCodec::FreeUniDecode(char* Buffer)
{
  delete Buffer;
}
void CMailCodec::FreeHeaderDecode(char* Buffer)
{
  delete Buffer;
}

void CMailCodec::TrimHTML(char* InStr, char* OutBuffer)
{
	int BodyLen = strlen(InStr); 
	int WritePos = 0;	 
	bool bIgnore = false;	 
	bool bIgnoretag = false;
	bool bContinue = false;
	
	for (int i = 0 ; i < BodyLen ; i++)
	{
		if (InStr[i] == '<')
		{
			bIgnore = true;
			if (strncmp(InStr+i+1,"!--",3) == 0)
			{
				bIgnoretag = true;
			}
		}
		else if (InStr[i] == '>')
		{
			if (bIgnoretag)
			{
				if (strncmp(InStr+i-2,"--",2) == 0)
				{
					bIgnoretag = false;
					bIgnore = false;
				}
			}
			else
			{
				bIgnore = false;
			}
		} 
		else if (!bIgnore)
		{
			 
				//中文
				OutBuffer[WritePos] = InStr[i];
				WritePos ++;
		}	
	}

	//防止 overflow
	if (BodyLen < WritePos)
	{
	  OutBuffer[WritePos] = 0;
	}
	else
	{
	  OutBuffer[BodyLen-1] = 0;
	}


}

void CMailCodec::ParseRTFBuf(char *buf,unsigned int*len)
{

	int idx = 0 , idx2 = 0;
	int bignoreidx = 0;


	
	ParseRTF(buf,len,&idx,&idx2,&bignoreidx);

}

void CMailCodec::ParseRTF(char *buf,unsigned int *len, int *idx , int *parseidx ,int *bignoreidx)
{

	char szKeyword[30];
	int KeyLen = 0;
	bool CheckSlash = false;

    while (*idx < (int) *len)
	{
		if (buf[*idx] == '{')
		{
			  (*idx) ++;			  
			  if (buf[(*idx)+1] == '*') //ignore /* 後 到 }
			  {				 
					  if (strncmp(buf+(*idx+3),"htmltag",6) == 0)  //support html
					  {
						  if (*bignoreidx > 0) (*bignoreidx) ++;
						  ParseRTF(buf, len, idx,parseidx,bignoreidx);
					  }
					  else
					  { 

						  int EnterCount = 1;

						  while (*idx < (int) *len)
						  {
							 if (buf[*idx] == '{') //計算再進入 , 也要省略
							 {
								EnterCount ++;
							 } 
							 else if (buf[*idx] == '}')
							 {
								EnterCount -- ;							 
								if (EnterCount <= 0) 
								{
									(*idx) ++;
									break;
								}
							 }
							  (*idx) ++;

							  
						  }
					  }
			  }
			  else
			  {

					if (*bignoreidx > 0) (*bignoreidx) ++;
					ParseRTF(buf, len, idx,parseidx,bignoreidx);
			  }
			 

			 
			 
			 //bPass = false;
		} 
		  else if (buf[*idx] == '}')
		{
			  ///bPass = false;
			  if (*bignoreidx > 0) (*bignoreidx) --;
			   (*idx) ++;
			  break;
		}	
		else if (buf[*idx] == '\\')
		{
			CheckSlash = true;
			(*idx) ++;
			if (!isalpha(buf[*idx]))
			{
			   if (	buf[*idx] == '\'')
			   {
			   
				   int HexVal = 0;
				   
				   //此部份並沒有轉換 codepage 如不同的 codepage 會產生亂碼

				   if (isdigit(buf[*idx + 1]))
                        HexVal += (char) buf[*idx + 1] - '0';
                   else
                    {
                        if (islower(buf[*idx + 1]))
                        {
                            HexVal += (char) buf[*idx + 1] - 'a';
							HexVal += 10;
                        }
                        else
                        {
                            HexVal += (char) buf[*idx + 1] - 'A';
							HexVal += 10;
                        }
                    }

				   HexVal = HexVal << 4;

				   if (isdigit(buf[*idx + 2]))
                        HexVal += (char) buf[*idx + 2] - '0';
                   else
                    {
                        if (islower(buf[*idx + 2]))
                        {
                            HexVal += (char) buf[*idx + 2] - 'a';
							HexVal += 10;
                        }
                        else
                        {
                            HexVal += (char) buf[*idx + 2] - 'A';
							HexVal += 10;
                        }
                    }

				   if (*bignoreidx == 0)
				   {
						if (HexVal != 0xa0) //過濾莫名字元
						{
							*(buf + *parseidx) = HexVal;				    
							*parseidx  = *parseidx + 1;
						}
				   }
				   
				   *idx = *idx + 3;
			   } 
			   else
			   {
				 
				
				 (*idx) ++;
				
			   }
			}
			else
			{
			
				KeyLen = 0;
				// - 符號 算在指令內
				while ((isalpha(buf[*idx]) || buf[*idx] == '-' ) && *idx < (int) len)
				{
				   szKeyword[KeyLen] = buf[*idx] ;
				   KeyLen++;
				   (*idx) ++;
				}

				szKeyword[KeyLen] = 0;
				while (isdigit(buf[*idx]) && *idx < (int) len)
				{
				   szKeyword[KeyLen] = buf[*idx] ;
				   KeyLen++;
				   (*idx) ++;
				}

				szKeyword[KeyLen] = 0;
				if (strcmp(szKeyword, "par") == 0)
				{
				
					*(buf + *parseidx) = '\r';
					*(buf + *parseidx + 1) = '\n';
					*parseidx  = *parseidx + 2;	
				} 
				else if (strcmp(szKeyword, "fonttbl") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}
				else if (strcmp(szKeyword, "colortbl") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}else if (strcmp(szKeyword, "stylesheet") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}else if (strcmp(szKeyword, "fldinst") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}else if (strcmp(szKeyword, "background") == 0)
				{	//乎略內容
					(*bignoreidx) ++;				
				}

				

				//如果 key 後 是 spac 則省略
				if (buf[*idx] == ' ') (*idx) ++;

				

			
			}
			
		}
		else 
		{
			    if (*bignoreidx == 0 && CheckSlash)
				{
					if (buf[*idx] != 13 && buf[*idx] != 10)
					{
					
						*(buf + *parseidx) = buf[*idx];
						*parseidx  = *parseidx + 1;				
					}				
				}

				 (*idx) ++;

				
		}

			 
		

		//*idx = *idx + 1;
	}  

	if (*idx == (int) *len)
	{
	  buf[*parseidx] = 0;
	  *len = *parseidx;
	}

}

void CMailCodec::FreeDeHeaderCode(char *AStr)
{

	delete [] AStr;

}

char * CMailCodec::DeHeaderCode(char *AStr)
{
	int len = strlen(AStr);
	int StartPos = -1;
	int EndPos = -1;
	char Language[20]={0};
	char Code[2]={0};
	char *RtnStr = NULL;

	for (int i = 0 ; i < len ; i++)
	{
		if (StartPos == -1 && AStr[i] == '=' && AStr[i+1] == '?')
		{
			//get language code
			for (int j = i+2; j < len ; j++)
			{
			
				if (AStr[j] == '?')
				{
					strncpy(Language,AStr+i+2,j-i-2);
					
					if (strnicmp(AStr+j+1,"B?",2) == 0)
					{
						Code[0] = 'B';
					}
					else if (strnicmp(AStr+j+1,"Q?",2) == 0)
					{
						Code[0] = 'Q';
					}

					i = j + 2;
					StartPos = i+1;
					break;

				}
			
			}

					
		}
		else if (StartPos == -1)
		{
				    if (RtnStr != NULL)
					{
						char *newbuf = new char[1+strlen(RtnStr)+1];
						memset(newbuf,0,1+strlen(RtnStr)+1);

						strcpy(newbuf,RtnStr);
						strncat(newbuf,AStr+i,1);

						delete [] RtnStr;

						RtnStr = newbuf;
					
					}
					else
					{
						char *newbuf = new char[2];
						memset(newbuf,0,2);
						strncpy(newbuf,AStr+i,1);

						RtnStr = newbuf;

					} 
		
		}
		else if (StartPos != -1 && AStr[i] == '?' && AStr[i+1] == '=')
		{
			EndPos = i;

			int CodeLen = EndPos - StartPos;
			char* tmpstr = new char[CodeLen+1];
			memset(tmpstr,0,CodeLen + 1);
			strncpy(tmpstr,AStr+StartPos,CodeLen);

			//Decode
			if (Code[0] == 'B')
			{
					//CMailCodec cm;
					char *out = NULL;
					unsigned int size=0;
								
					

					Base64Decode(tmpstr,&out,&size);

					if (RtnStr != NULL)
					{
						char *newbuf = new char[size+strlen(RtnStr)+1];
						memset(newbuf,0,size+strlen(RtnStr)+1);

						strcpy(newbuf,RtnStr);
						strcat(newbuf,out);

						delete [] out;
						delete [] RtnStr;

						RtnStr = newbuf;
					
					}
					else
					{
						RtnStr = out;
					}
 
			
			
			}else if (Code[0] == 'Q')
			{
			
					//CMailCodec cm;
					char *out = QPDecode(tmpstr);
					
					if (RtnStr != NULL)
					{
						char *newbuf = new char[strlen(out)+strlen(RtnStr)+1];
						memset(newbuf,0,strlen(out)+strlen(RtnStr)+1);

						strcpy(newbuf,RtnStr);
						strcat(newbuf,out);

						delete [] out;
						delete [] RtnStr;

						RtnStr = newbuf;
					
					}
					else
					{
						RtnStr = out;
					}

				 
			}

			delete tmpstr;

			StartPos = -1;
			Code[0] = 0;
			i = i + 1;

		
		}
	}



	return RtnStr;
}



/*

void CMailCodec::ParseRTF(char *buf,unsigned int *len, int *idx , int *parseidx ,int *bignoreidx)
{

	char szKeyword[30];
	int KeyLen = 0;

    while (*idx < (int) *len)
	{
		if (buf[*idx] == '{')
		{
			  (*idx) ++;
			 if (*bignoreidx > 0) (*bignoreidx) ++;
			 ParseRTF(buf, len, idx,parseidx,bignoreidx);
			 
			 //bPass = false;
		} 
		  else if (buf[*idx] == '}')
		{
			  ///bPass = false;
			  if (*bignoreidx > 0) (*bignoreidx) --;
			   (*idx) ++;
			  break;
		}	
		else if (buf[*idx] == '\\')
		{
			(*idx) ++;
			if (!isalpha(buf[*idx]))
			{
			   if (	buf[*idx] == '\'')
			   {
			   
				   int HexVal = 0;		   

				   if (isdigit(buf[*idx + 1]))
                        HexVal += (char) buf[*idx + 1] - '0';
                   else
                    {
                        if (islower(buf[*idx + 1]))
                        {
                            HexVal += (char) buf[*idx + 1] - 'a';
							HexVal += 10;
                        }
                        else
                        {
                            HexVal += (char) buf[*idx + 1] - 'A';
							HexVal += 10;
                        }
                    }

				   HexVal = HexVal << 4;

				   if (isdigit(buf[*idx + 2]))
                        HexVal += (char) buf[*idx + 2] - '0';
                   else
                    {
                        if (islower(buf[*idx + 2]))
                        {
                            HexVal += (char) buf[*idx + 2] - 'a';
							HexVal += 10;
                        }
                        else
                        {
                            HexVal += (char) buf[*idx + 2] - 'A';
							HexVal += 10;
                        }
                    }

				   if (*bignoreidx == 0)
				   {
						if (HexVal != 0xa0) //過濾莫名字元
						{
							*(buf + *parseidx) = HexVal;				    
							*parseidx  = *parseidx + 1;
						}
				   }
				   
				   *idx = *idx + 3;
			   }
			   else
			   {
				   (*idx) ++;
			   }
			}
			else
			{
			
				KeyLen = 0;
				// - 符號 算在指令內
				while ((isalpha(buf[*idx]) || buf[*idx] == '-' ) && *idx < (int) len)
				{
				   szKeyword[KeyLen] = buf[*idx] ;
				   KeyLen++;
				   (*idx) ++;
				}

				szKeyword[KeyLen] = 0;

				while (isdigit(buf[*idx]) && *idx < (int) len)
				{
				   szKeyword[KeyLen] = buf[*idx] ;
				   KeyLen++;
				   (*idx) ++;
				}

				szKeyword[KeyLen] = 0;
				if (strcmp(szKeyword, "par") == 0)
				{
				
					*(buf + *parseidx) = '\r';
					*(buf + *parseidx + 1) = '\n';
					*parseidx  = *parseidx + 2;	
				} 
				else if (strcmp(szKeyword, "fonttbl") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}
				else if (strcmp(szKeyword, "colortbl") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}else if (strcmp(szKeyword, "stylesheet") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}else if (strcmp(szKeyword, "fldinst") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}else if (strcmp(szKeyword, "background") == 0)
				{
					//乎略內容
					(*bignoreidx) ++;
				}

				

				//如果 key 後 是 spac 則省略
				if (buf[*idx] == ' ') (*idx) ++;

				

			
			}
			
		}
		else 
		{
			    if (*bignoreidx == 0)
				{
					if (buf[*idx] != 13 && buf[*idx] != 10)
					{
					
						*(buf + *parseidx) = buf[*idx];
						*parseidx  = *parseidx + 1;				
					}				
				}

				 (*idx) ++;

				
		}

			 
		

		//*idx = *idx + 1;
	}  

	if (*idx == (int) *len)
	{
	  buf[*parseidx] = 0;
	  *len = *parseidx;
	}

}*/