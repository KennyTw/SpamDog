#include "Stdafx.h"
#include "OLapi.h"

/*

int gint = 0;

void  DLLfun2(LPCallBack lpback)
{
   lpback(gint);
};


void  DLLfun1(char** a , int b)
{
   //a[0] = '1';

	//char *tmpc =  (char *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS , 50); 
	*a = new char[50];
	memset(*a,0,50);
	*a[0] = 'a';

	gint = b;

	
	
	//*a = tmpc;

	OutputDebugString(*a);

	 
	
}; */

LPMAPISESSION lpMAPISession = NULL;
LPMDB         lpMDB  = NULL;

/////////// Outlook Api /////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

HRESULT OpenDefaultMessageStore()
{

   LPMAPITABLE pStoresTbl = NULL;
   LPSRowSet   pRow = NULL;
   static      SRestriction sres;
   SPropValue  spv;
   HRESULT     hRes = S_OK;
   //ULONG ulValues; 
   LPSPropValue pPropValues = NULL; 
   ULONG ul = 0;

   enum {EID, NAME, NUM_COLS};
   static SizedSPropTagArray(NUM_COLS,sptCols) = {NUM_COLS, PR_ENTRYID, PR_DISPLAY_NAME};
    

   //Get the table of all the message stores available
   hRes = lpMAPISession->GetMsgStoresTable(0, &pStoresTbl);
   if (FAILED(hRes)) goto quit;

   //Set up restriction for the default store
   sres.rt = RES_PROPERTY; //Comparing a property
   sres.res.resProperty.relop = RELOP_EQ; //Testing equality
   sres.res.resProperty.ulPropTag = PR_DEFAULT_STORE; //Tag to compare
   sres.res.resProperty.lpProp = &spv; //Prop tag and value to compare against

   spv.ulPropTag = PR_DEFAULT_STORE; //Tag type
   spv.Value.b   = TRUE; //Tag value

   //Convert the table to an array which can be stepped through
   //Only one message store should have PR_DEFAULT_STORE set to true, so only one will be returned
   hRes = HrQueryAllRows(
   pStoresTbl, //Table to query
   (LPSPropTagArray) &sptCols, //Which columns to get
   &sres, //Restriction to use
   NULL, //No sort order
   0, //Max number of rows (0 means no limit)
   &pRow); //Array to return
   if (FAILED(hRes))  goto quit;  

   //Open the first returned (default) message store
   hRes = lpMAPISession->OpenMsgStore(
      NULL,//Window handle for dialogs
      pRow->aRow[0].lpProps[EID].Value.bin.cb,//size and...
      (LPENTRYID)pRow->aRow[0].lpProps[EID].Value.bin.lpb,//value of entry to open
      NULL,//Use default interface (IMsgStore) to open store
      MAPI_BEST_ACCESS,//Flags
      &lpMDB);//Pointer to place the store in
   if (FAILED(hRes)) goto quit;

quit:
   FreeProws(pRow);
   UlRelease(pStoresTbl);
   return hRes;


}

HRESULT OlLogin(char* ProfileName)
{

	HRESULT       hRes = S_OK;
	hRes = MAPIInitialize(NULL);
	if (FAILED(hRes)) return hRes;

	hRes = MAPILogonEx(0,
      ProfileName,//profile name
      NULL,//password - This parameter should ALWAYS be NULL
      MAPI_LOGON_UI, //Allow a profile picker box to show if not logged in
      &lpMAPISession);//handle of session
	if (FAILED(hRes)) return hRes;

	hRes = OpenDefaultMessageStore(); //get lpMDB
	if (FAILED(hRes)) return hRes;

	//OutputDebugString(ProfileName);

	return hRes;

}




HRESULT OlLogout()
{

	HRESULT       hRes = S_OK;

	if (lpMDB != NULL)
	{
	 lpMDB->Release();
	 lpMDB = NULL;
	}

	if (lpMAPISession != NULL)
	{
		hRes = lpMAPISession->Logoff(0,MAPI_LOGOFF_UI,0); 
		MAPIUninitialize();
		lpMAPISession = NULL;

		OutputDebugString("Logout Succeed");
	}

	return hRes;

}

HRESULT OlOpenInBoxFolder(LPMAPIFOLDER *InBoxFolder)
{

   	ULONG        cbInbox;
    LPENTRYID    lpbInbox;
    ULONG        ulObjType;
	HRESULT       hRes = S_OK;

	hRes = lpMDB->GetReceiveFolder(
      NULL,      //Get default receive folder
      NULL,      //Flags
      &cbInbox,  //Size and ...
      &lpbInbox, //Value of the EntryID to be returned
      NULL);     //You don't care to see the class returned
   
	if (FAILED(hRes)) return hRes;
   
   
   hRes = lpMDB->OpenEntry(
      cbInbox,                      //Size and...
      lpbInbox,                     //Value of the Inbox's EntryID
      NULL,                         //We want the default interface    (IMAPIFolder)
      MAPI_BEST_ACCESS,             //Flags
      &ulObjType,                   //Object returned type
      (LPUNKNOWN *) InBoxFolder); //Returned folder
   if (FAILED(hRes)) return hRes;

   return hRes;


}

HRESULT OlOpenTrashFolder(LPMAPIFOLDER *TrashFolder)
{

   HRESULT       hRes = S_OK;

   ULONG ulValues; 
   LPSPropValue pPropValues = NULL; 

   enum {     
      ePR_IPM_WASTEBASKET_ENTRYID,	  
      NUM_COLS};
   //These tags represent the message information we would like to pick up

   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
      PR_IPM_WASTEBASKET_ENTRYID	  
   };


    hRes = lpMDB->GetProps((LPSPropTagArray) &sptCols, 
         0, 
         &ulValues, 
         &pPropValues); 

	if (hRes == S_OK)
	{
	
			ULONG ulObjType = 0; 
		 
			hRes = lpMDB->OpenEntry(pPropValues[0].Value.bin.cb, 
				(ENTRYID *) pPropValues[0].Value.bin.lpb, 
				NULL, 
				MAPI_BEST_ACCESS, 
				&ulObjType, 
				(LPUNKNOWN *) TrashFolder);
			
			MAPIFreeBuffer(pPropValues);
	
	}
     

	return hRes;

}


HRESULT OlGetFirstSubFolder(LPMAPIFOLDER ParentFolder , OlFOLDERPROPS *folderpros ,  OlFOLDERITOR *folderitor )
{
	HRESULT       hRes = S_OK;
	LPMAPITABLE lpContentsTable = NULL;
	LPSRowSet pRows ;

   enum {     
      ePR_ENTRYID,
	  ePR_DISPLAY_NAME,
	  //ePR_SUBFOLDERS,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
      PR_ENTRYID,
	  PR_DISPLAY_NAME
	  //PR_SUBFOLDERS
   };


		//取出下層
		hRes = ParentFolder->GetHierarchyTable(0,&lpContentsTable);
		if (FAILED(hRes)) return hRes;		
   
		//列出 query
		hRes = HrQueryAllRows(
			lpContentsTable,
			(LPSPropTagArray) &sptCols,
			NULL,//restriction...we're not using this parameter
			NULL,//sort order...we're not using this parameter
			0,
			&pRows);

		if (FAILED(hRes)) return hRes;	
		UlRelease(lpContentsTable);

		if (pRows->cRows > 0)
		{
 

		
			folderpros->FolderIdcb = pRows->aRow[0].lpProps[ePR_ENTRYID].Value.bin.cb;
			folderpros->FolderIdlpb = pRows->aRow[0].lpProps[ePR_ENTRYID].Value.bin.lpb;
			folderpros->szName = pRows->aRow[0].lpProps[ePR_DISPLAY_NAME].Value.lpszA;
			


			folderitor->itor = 0;
			folderitor->pRows = pRows;
			//folderitor->lpContentsTable = lpContentsTable;
					
		 
		}
		else
		{
			memset(folderitor,0,sizeof(OlFOLDERITOR));
			memset(folderpros ,0,sizeof(OlFOLDERPROPS));

			return -1;

			
		}		

	 


	return hRes;	

}

HRESULT OlGetNextSubFolder(OlFOLDERPROPS *folderpros ,  OlFOLDERITOR *folderitor )
{

	HRESULT       hRes = S_OK;
	 enum {     
      ePR_ENTRYID,
	  ePR_DISPLAY_NAME,
	  //ePR_SUBFOLDERS,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
      PR_ENTRYID,
	  PR_DISPLAY_NAME
	  //PR_SUBFOLDERS
   };


	folderitor->itor++;

	
	if (folderitor->itor < folderitor->pRows->cRows)
	{
	
			folderpros->FolderIdcb = folderitor->pRows->aRow[folderitor->itor].lpProps[ePR_ENTRYID].Value.bin.cb;
			folderpros->FolderIdlpb = folderitor->pRows->aRow[folderitor->itor].lpProps[ePR_ENTRYID].Value.bin.lpb;
			folderpros->szName = folderitor->pRows->aRow[folderitor->itor].lpProps[ePR_DISPLAY_NAME].Value.lpszA;

	}
	else
	{
	
		memset(folderitor,0,sizeof(OlFOLDERITOR));
		memset(folderpros ,0,sizeof(OlFOLDERPROPS));

		return -1;
	
	}

	return hRes;
}

HRESULT OlOpenFolder(OlFOLDERPROPS *folderpros , LPMAPIFOLDER *OpenFolder)
{
		
	enum {     
      ePR_ENTRYID,
	  ePR_DISPLAY_NAME,
	  //ePR_SUBFOLDERS,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
      PR_ENTRYID,
	  PR_DISPLAY_NAME
	  //PR_SUBFOLDERS
   };

   

	HRESULT       hRes = S_OK;		
	ULONG        ulObjType;

			hRes = lpMDB->OpenEntry(
					folderpros->FolderIdcb,                      //Size and...
					(LPENTRYID) folderpros->FolderIdlpb,                     //Value of the Inbox's EntryID
					NULL,                         //We want the default interface    (IMAPIFolder)
					MAPI_BEST_ACCESS,             //Flags
					&ulObjType,                   //Object returned type
					(LPUNKNOWN *) OpenFolder); //Returned folder


    return hRes;
}

void OlCloseFolder(LPMAPIFOLDER CloseFolder)
{

	if (CloseFolder != NULL)
	{
		CloseFolder->Release();
		//UlRelease(CloseFolder);
		//CloseFolder = NULL;
	}

}

void OlCloseFolderProps(OlFOLDERITOR* folderitor )
{

	if (folderitor != NULL)
	{	
		FreeProws(folderitor->pRows);
		//UlRelease(folderitor->lpContentsTable);
	}


}

void OlCloseMailProps(OlMAILITOR* mailitor )
{

	if (mailitor != NULL)
	{	
		FreeProws(mailitor->pRows);
		//UlRelease(folderitor->lpContentsTable);
	}


}

HRESULT OlCreateFolder(LPMAPIFOLDER ParentFolder , char* FolderName , char* FolderDesc , LPMAPIFOLDER* CreateFolder)
{

	HRESULT       hRes = -1;
	if (ParentFolder != NULL)
	{	
		hRes = ParentFolder->CreateFolder(FOLDER_GENERIC,FolderName,FolderDesc,NULL,OPEN_IF_EXISTS,CreateFolder);
	}

	return hRes;
}

HRESULT OlDeleteFolder(LPMAPIFOLDER ParentFolder ,OlFOLDERPROPS *folderpros)
{
	HRESULT       hRes = -1;
	if (ParentFolder != NULL)
	{
		hRes = ParentFolder->DeleteFolder(folderpros->FolderIdcb,(ENTRYID *) folderpros->FolderIdlpb,NULL,NULL,DEL_FOLDERS | DEL_MESSAGES );
	}

	return hRes;

}

HRESULT OlRegFolderNotify(LPVOID lpParam , OlREGISTERNOTIFY *regnotify ,LPOlOnNotify lpOnNotify)
{
	HRESULT       hRes = -1;

	enum {     
	  ePR_INSTANCE_KEY,	  
      ePR_ENTRYID,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,      
	  PR_INSTANCE_KEY,
      PR_ENTRYID	 
   };

	hRes = regnotify->RegisterFolder->GetContentsTable(
      0,
      &regnotify->lpContentsTable);

    if (FAILED(hRes)) return hRes;

	hRes = regnotify->lpContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);

	CMAPIAdviseSink2* lpMAPIAdviseSink = new CMAPIAdviseSink2(lpParam, lpMDB,regnotify,lpOnNotify);
	
	hRes = regnotify->lpContentsTable->Advise(
		fnevTableModified,	 
		(IMAPIAdviseSink *) lpMAPIAdviseSink,
		&regnotify->ulAdviseConnection);


	return hRes;
}

HRESULT OlUnRegFolderNotify(OlREGISTERNOTIFY *regnotify)
{
	HRESULT       hRes = -1;

	hRes = regnotify->lpContentsTable->Unadvise(regnotify->ulAdviseConnection);
	

	UlRelease(regnotify->lpContentsTable);
	regnotify->lpContentsTable = NULL;

	  

	return hRes;
}

HRESULT OlMailGetSenderName(LPMESSAGE lpMessage, char **SenderName)
{
	HRESULT       hRes = S_OK;

	LPSPropValue   lpPropss = NULL;
	hRes = HrGetOneProp(lpMessage,PR_SENDER_NAME ,&lpPropss);

	if (SUCCEEDED(hRes))
	{	
		int len = strlen(lpPropss->Value.lpszA);
		char *AllocMem = NULL;
		MAPIAllocateBuffer(len+1,(void **) &AllocMem);
		strcpy(AllocMem,lpPropss->Value.lpszA);

		*SenderName = AllocMem;

		MAPIFreeBuffer(lpPropss);
	}
	else
	{
		hRes = -1;
	}
 
 

	return hRes;
}


HRESULT OlMailGetSubject(LPMESSAGE lpMessage , char **Subject)
{

	HRESULT       hRes = S_OK;
	LPSPropValue   lpPropss = NULL;

	hRes = HrGetOneProp(lpMessage,PR_SUBJECT ,&lpPropss);

	if (SUCCEEDED(hRes))
	{	
		int len = strlen(lpPropss->Value.lpszA);
		char *AllocMem = NULL;
		MAPIAllocateBuffer(len+1,(void **) &AllocMem);
		strcpy(AllocMem,lpPropss->Value.lpszA);

		*Subject = AllocMem;
		
		MAPIFreeBuffer(lpPropss);
	}
	else
	{
		hRes = -1;
	}	  

	

	return hRes;


}

HRESULT OlMailGetHeader(LPMESSAGE lpMessage, char **Header)
{
	HRESULT       hRes = S_OK;

	LPSPropValue   lpPropss = NULL;

	hRes = HrGetOneProp(lpMessage,PR_TRANSPORT_MESSAGE_HEADERS ,&lpPropss);

	if (SUCCEEDED(hRes))
	{	
		int len = strlen(lpPropss->Value.lpszA);
		char *AllocMem = NULL;
		MAPIAllocateBuffer(len+1,(void **) &AllocMem);
		strcpy(AllocMem,lpPropss->Value.lpszA);

		*Header = AllocMem;
		
		MAPIFreeBuffer(lpPropss);
	}
	else
	{
		hRes = -1;
	}	  

				


	return hRes;

}

void ParseRTF(char *buf,unsigned int *len, int *idx , int *parseidx ,int *bignoreidx)
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

void ParseRTFBuf(char *buf,unsigned int*len)
{

	int idx = 0 , idx2 = 0;
	int bignoreidx = 0;
	
	ParseRTF(buf,len,&idx,&idx2,&bignoreidx);

}

HRESULT OlMailGetRTFBody(LPMESSAGE lpMessage , char **Body)
{

	LPSTREAM lpStream = NULL;
	HRESULT       hRes = S_OK;	
	hRes = lpMessage->OpenProperty(                  
					PR_RTF_COMPRESSED,
					&IID_IStream,
					STGM_READ,
					NULL,
					(LPUNKNOWN *) &lpStream);

					if (hRes == S_OK)
					{
							LPSTREAM iunstream = NULL; // for the uncompressed stream
							//LPSTREAM iunstream2 = NULL; // for the uncompressed stream2
							//CreateStreamOnHGlobal(NULL, TRUE, &iunstream2);

							WrapCompressedRTFStream(lpStream,0 ,&iunstream);

							char *AllocMem = NULL;
							//char Buff[1024];
							unsigned int ReadSize = 0;

							MAPIAllocateBuffer(1024,(void **) &AllocMem);
							ULONG readbyte = 0;
							do
							{
								iunstream->Read(AllocMem + ReadSize ,1024,&readbyte);
								ReadSize += readbyte;

								if(readbyte >= 1024 )
								{
									
									char *AllocMemSave = AllocMem; //Save Old mem
									MAPIAllocateBuffer(ReadSize + 1024,(void **) &AllocMem);
									//copy old
									//if (ReadSize - 1024 > 0)
									memcpy(AllocMem,AllocMemSave,ReadSize);

									//memcpy(AllocMem + (ReadSize - 1024) ,Buff,readbyte);



									MAPIFreeBuffer(AllocMemSave);//Free Old 
								}

							
							} while (readbyte >= 1024 );

							AllocMem[ReadSize] = 0;
 
							
							ParseRTFBuf(AllocMem,&ReadSize);

							*Body = AllocMem;
							
							 

							iunstream->Release();						 
							lpStream->Release();
					}

					return hRes ;
}

HRESULT OlMailGetBody(LPMESSAGE lpMessage , char **Body)
{
	HRESULT       hRes = S_OK;

	SPropValue *svmask;
	LPSTREAM lpStream = NULL;
	
	bool IsSupportHtml=false;
            
	hRes = HrGetOneProp(
               lpMDB,
               PR_STORE_SUPPORT_MASK,			   
               &svmask);

	if (hRes == S_OK)
	{
			//if ( (svmask->Value.ul & STORE_RTF_OK) != 0 ) 
			//{
			//	mustsync=false;
			//}
			//else 
		    if ( (svmask->Value.ul & 0x00010000) != 0 ) //STORE_HTML_OK
			{					  
				IsSupportHtml = true;
			}
			//else if (mSRow->lpProps[ePR_RTF_IN_SYNC].ulPropTag == PR_RTF_IN_SYNC 
			//		         && mSRow->lpProps[ePR_RTF_IN_SYNC].Value.b !=0)
			//{			 
			//	   	   mustsync=false;				   
			//}

            MAPIFreeBuffer(svmask);
	}

	if (IsSupportHtml)
	{
	
			hRes = lpMessage->OpenProperty(                  
				PROP_TAG(PT_TSTRING,0x1013),
				&IID_IStream,
				STGM_READ,
				NULL,
				(LPUNKNOWN *) &lpStream);

			if (hRes == S_OK)
			{			
			
				STATSTG statstg;
				memset (&statstg, 0, sizeof(statstg));
				hRes = lpStream->Stat(&statstg, STATFLAG_NONAME);
				if (hRes == S_OK)
				{
					char *AllocMem = NULL;
					unsigned int len = statstg.cbSize.LowPart + sizeof(char);
					MAPIAllocateBuffer(len,(void **) &AllocMem);

					lpStream->Read((void*)AllocMem, statstg.cbSize.LowPart, NULL);

					AllocMem[statstg.cbSize.LowPart] = 0;

					*Body = AllocMem;

					lpStream->Release();


					
				}
			
			}
			else
			{
					hRes = OlMailGetRTFBody(lpMessage,Body);
			}
	
	}
	else
	{
	
		//BOOL isupdated; 
		//RTFSync(lpMessage,RTF_SYNC_BODY_CHANGED,&isupdated);
		//if (isupdated) lpMessage->SaveChanges(0);


		hRes = OlMailGetRTFBody(lpMessage,Body);
						
	
	}


	return hRes;


}

HRESULT OlGetFirstMail(LPMAPIFOLDER MailFolder , OlMAILPROPS *mailpros , OlMAILITOR *mailitor )
{
	HRESULT       hRes = S_OK;

   enum {  	          
      ePR_ENTRYID,
	  ePR_SUBJECT,
      NUM_COLS};
   
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS, 
       PR_ENTRYID ,
	   PR_SUBJECT
   };

    LPMAPITABLE lpContentsTable;

	hRes = MailFolder->GetContentsTable(
      0,
      &lpContentsTable);

    if (FAILED(hRes)) return hRes;

	hRes = lpContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);

	if (FAILED(hRes)) return hRes;

	LPSRowSet pRows ;
	

    hRes = HrQueryAllRows(
      lpContentsTable,
      (LPSPropTagArray) &sptCols,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &pRows);
	
	UlRelease(lpContentsTable);
	

	if (SUCCEEDED(hRes))
	{
		if (pRows->cRows > 0)
		{
		
		   /*hRes = lpMDB->OpenEntry(
            mSRow->aRow[0].lpProps[ePR_ENTRYID].Value.bin.cb,
            (LPENTRYID) mSRow->aRow[0].lpProps[ePR_ENTRYID].Value.bin.lpb,
            NULL,//default interface
            MAPI_BEST_ACCESS,
            &ulObjType,
            (LPUNKNOWN *) lpMessage);
			
		   if (!FAILED(hRes))
		   {
				mailitor->itor = 0;
				mailitor->pRows = mSRow;
		
		   }*/

			mailpros->MailIdcb = pRows->aRow[0].lpProps[ePR_ENTRYID].Value.bin.cb;
			mailpros->MailIdlpb = pRows->aRow[0].lpProps[ePR_ENTRYID].Value.bin.lpb;
			mailpros->szSubject = pRows->aRow[0].lpProps[ePR_SUBJECT].Value.lpszA;
			


				mailitor->itor = 0;
				mailitor->pRows = pRows;
		}
		else
		{
			memset(mailitor,0,sizeof(OlMAILITOR));
			memset(mailpros ,0,sizeof(OlMAILPROPS));

			return -1;
		}
	}




	return hRes;
}

HRESULT OlGetNextMail(OlMAILPROPS *mailpros ,  OlMAILITOR *mailitor )
{
	HRESULT       hRes = S_OK;

	enum {  	          
      ePR_ENTRYID,
	  ePR_SUBJECT,
      NUM_COLS};
   
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS, 
       PR_ENTRYID ,
	   PR_SUBJECT
   };


	mailitor->itor++;

	
	if (mailitor->itor < mailitor->pRows->cRows)
	{
	
		
			mailpros->MailIdcb = mailitor->pRows->aRow[mailitor->itor].lpProps[ePR_ENTRYID].Value.bin.cb;
			mailpros->MailIdlpb = mailitor->pRows->aRow[mailitor->itor].lpProps[ePR_ENTRYID].Value.bin.lpb;
			mailpros->szSubject = mailitor->pRows->aRow[mailitor->itor].lpProps[ePR_SUBJECT].Value.lpszA;

	}
	else
	{
	
		memset(mailitor,0,sizeof(OlMAILITOR));
		memset(mailpros ,0,sizeof(OlMAILPROPS));

		return -1;
	
	}


	return hRes;
}

HRESULT OlOpenMail(OlMAILPROPS *mailpros , LPMESSAGE *lpMessage)
{

	HRESULT       hRes = S_OK;
	ULONG ulObjType = NULL;

	
	
	
	hRes = lpMDB->OpenEntry(
            mailpros->MailIdcb,
            (LPENTRYID) mailpros->MailIdlpb,
            NULL,//default interface
            MAPI_BEST_ACCESS,
            &ulObjType,
            (LPUNKNOWN *) lpMessage);		
		   

    return hRes;
}

void OlCloseMail(LPMESSAGE lpMessage)
{
	lpMessage->Release();
}


HRESULT OlFreeMemory(char *mem)
{
	return	MAPIFreeBuffer(mem);
}

HRESULT OlMoveMail(OlMAILPROPS *mailpros , LPMAPIFOLDER FromFolder ,LPMAPIFOLDER ToFolder )
{

	HRESULT       hRes = S_OK;
	
	 SBinary *sbin = new SBinary[1];
	 sbin->cb = mailpros->MailIdcb;
	 sbin->lpb = mailpros->MailIdlpb;
	 
	  ENTRYLIST list; 
      list.cValues = 1; 	  
      list.lpbin = sbin; 
	 
	  hRes = FromFolder->CopyMessages(&list,NULL,ToFolder,NULL,NULL,MESSAGE_MOVE );
	  
	  delete sbin;

	  return hRes;

}

HRESULT OlDeleteMail(OlMAILPROPS *mailpros , LPMAPIFOLDER MailFolder )
{

	 HRESULT       hRes = S_OK;
	
	 SBinary *sbin = new SBinary[1];
	 sbin->cb = mailpros->MailIdcb;
	 sbin->lpb = mailpros->MailIdlpb;
	 
	  ENTRYLIST list; 
      list.cValues = 1; 	  
      list.lpbin = sbin; 
	 
	  hRes = MailFolder->DeleteMessages(&list,NULL,NULL,NULL);
	  
	  delete sbin;

	  return hRes;

}

HRESULT OlCreateMail(LPMAPIFOLDER MailFolder, char *SenderEmail , char *Subject , char *MailContent )
{

	HRESULT       hRes = S_OK;

	LPMESSAGE  lpMessage = NULL;
	hRes = MailFolder->CreateMessage(NULL,NULL,&lpMessage);
	if (hRes != S_OK) return hRes;

	SPropValue    spvAttach[11];

	spvAttach[0].ulPropTag = PROP_TAG(PT_TSTRING,0x1013);
	spvAttach[0].Value.lpszA = MailContent;

	//spvAttach[1].ulPropTag = PR_TRANSPORT_MESSAGE_HEADERS;
	//spvAttach[1].Value.lpszA = "Subject: hello\r\n\r\n"; 
	spvAttach[1].ulPropTag = PR_SUBJECT;
	spvAttach[1].Value.lpszA = Subject;

	
	//spvAttach[2].ulPropTag =  PR_SENT_REPRESENTING_NAME  ;


	spvAttach[2].ulPropTag = PR_MESSAGE_CLASS;
	spvAttach[2].Value.lpszA = "IPM.Note";
	
	spvAttach[3].ulPropTag = PR_MESSAGE_FLAGS;
	spvAttach[3].Value.ul =  MSGFLAG_SUBMIT ; 

	spvAttach[4].ulPropTag = PR_MESSAGE_DELIVERY_TIME; 
	GetSystemTimeAsFileTime(&spvAttach[4].Value.ft);
	
	//spvAttach[6].ulPropTag =  PR_SENT_REPRESENTING_EMAIL_ADDRESS;
	//spvAttach[6].Value.lpszA = "kenny@ezfly.com";

	//spvAttach[7].ulPropTag = PR_SENDER_NAME;
	//spvAttach[7].Value.lpszA = "kenny";

 	spvAttach[5].ulPropTag =  PR_SENT_REPRESENTING_NAME  ;
	spvAttach[5].Value.lpszA = SenderEmail;

	spvAttach[6].ulPropTag = PR_SENDER_NAME;
	spvAttach[6].Value.lpszA = SenderEmail;

	spvAttach[7].ulPropTag =  PR_SENDER_EMAIL_ADDRESS;
	spvAttach[7].Value.lpszA = SenderEmail;

    spvAttach[8].ulPropTag =  PR_SENT_REPRESENTING_EMAIL_ADDRESS;
	spvAttach[8].Value.lpszA = SenderEmail;

	spvAttach[9].ulPropTag =  PR_SENDER_ADDRTYPE;
	spvAttach[9].Value.lpszA = "SMTP";

	spvAttach[10].ulPropTag =  PR_SENT_REPRESENTING_ADDRTYPE;
	spvAttach[10].Value.lpszA = "SMTP";

	//spvAttach[11].ulPropTag =  PR_DISPLAY_TO;
	//spvAttach[11].Value.lpszA = "service@softworking.com";

	

	

	
//	spvAttach[8].ulPropTag = PR_TRANSPORT_MESSAGE_HEADERS; 
	//spvAttach[8].Value.lpszA = "From: =?Big5?B?obSrS6l5sOq72rlxuNwgobQ=?= <aaaek.aaaek@casparcaspar.org>\r\n\r\n"; 



	lpMessage->SetProps(
					11,
					(LPSPropValue)&spvAttach,
						NULL);

	hRes = lpMessage->SaveChanges(KEEP_OPEN_READWRITE);

	lpMessage->Release();
	
	return hRes;

}