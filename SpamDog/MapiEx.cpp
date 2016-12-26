// MapiEx.cpp: implementation of the CMapiEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpamDog.h"
#include "MapiEx.h"
#include "../Swparser/MailParser.h"
#include "../Swvirus/Virus.h"

#include <initguid.h>
#include <mapiguid.h>

#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapiEx::CMapiEx()
{
	lpMAPISession = NULL;
	lpMDB = NULL ;
	lpInboxFolder = NULL;
   //pRows = NULL;
	//RowCursor = 0;
	lpContentsTable = NULL;
	lpLearnOkContentsTable = NULL;
	lpLearnSpamContentsTable = NULL;
	lpWhiteContentsTable = NULL;

	lpSpamFolder = NULL;
	lpNormalFolder = NULL;
	lpMAPIAdviseSink = NULL;
	lpLearnOkFolder = NULL;
	lpLearnSpamFolder = NULL;
	lpUnknownFolder = NULL;
	lpWhiteFolder = NULL;

	lpMAPIAdviseOkSink = NULL;
	lpMAPIAdviseSpamSink = NULL;
	lpMAPIAdviseWhiteSink = NULL;

	ulAdviseOutConnection = NULL;
	lpMAPIAdviseOutSink = NULL;
	lpOutboxFolder = NULL;
	ulAdviseWhiteConnection = NULL;

	//mFolderName = NULL;
	//mMainStatic = NULL;
	
    htmlsize = 0;
    bufsize = 10240;
	htmlbuf = new char[bufsize];
	
	bodysize = 0;
	bodybufsize = 10240;
	bodybuf = new char[bodybufsize];
    

	memset(SpamFolderName,0,sizeof(SpamFolderName));
	memset(OkFolderName,0,sizeof(OkFolderName));
	memset(UnknownFolderName,0,sizeof(UnknownFolderName));
	memset(LearnOkFolderName,0,sizeof(LearnOkFolderName));
	memset(LearnSpamFolderName,0,sizeof(LearnSpamFolderName)); 
	memset(LearnWhiteFolderName,0,sizeof(LearnWhiteFolderName)); 
	memset(SysMsgFolderName,0,sizeof(SysMsgFolderName)); 

	

	
	GetiniFolderSetup();
}

CMapiEx::~CMapiEx()
{

	
	NotificationOff(); 
	Logout();
	
	if (bufsize > 0) 
	{
		delete [] htmlbuf;
		bufsize = 0;
	}

	if (bodybufsize > 0) 
	{
		delete [] bodybuf;
		bodybufsize = 0;
	}
}

 

HRESULT CMapiEx::Login (char Type,char *profile)
{  
    //mFolderName = FolderName;
	HRESULT       hRes;
	LPSPropValue  tmp = NULL;
	//LPSTR  OutStr = "";
	char OutStr[50];

	

	hRes = MAPIInitialize(NULL);
	if (FAILED(hRes)) goto quit;

	

    hRes = MAPILogonEx(0,
      profile,//profile name
      NULL,//password - This parameter should ALWAYS be NULL
      MAPI_LOGON_UI, //Allow a profile picker box to show if not logged in
      &lpMAPISession);//handle of session
	if (FAILED(hRes)) goto quit;

	SetUnhandledExceptionFilter(TheCrashHandlerFunction);
	

   hRes = OpenDefaultMessageStore();
   if (FAILED(hRes)) goto quit;

   hRes = OpenInbox();
   if (FAILED(hRes)) goto quit;

   //Checking to see that we did get the Inbox
   hRes = HrGetOneProp(
      lpInboxFolder,
	 // lpMoveToFolder,
      PR_DISPLAY_NAME,
      &tmp);
   if (FAILED(hRes)) goto quit;
   
   
   strcpy(OutStr,"I managed to open the folder  ");
   strcat(OutStr,tmp->Value.lpszA);
   strcat(OutStr,"\n");

   OutputDebugString(OutStr);

   hRes = ListMessages(Type);
   
   

	return hRes;
quit:
 
   if (tmp) MAPIFreeBuffer(tmp);
   UlRelease(lpInboxFolder);
   UlRelease(lpMDB);
   UlRelease(lpMAPISession);

   return hRes;


}

HRESULT CMapiEx::Logout ()
{
    HRESULT       hRes;
	
	
	if (lpMAPIAdviseOkSink != NULL) 
	{

		if (ulAdviseOkConnection && lpLearnOkContentsTable)
		lpLearnOkContentsTable->Unadvise(ulAdviseOkConnection);

		ulAdviseOkConnection = NULL;
		//UlRelease(lpMAPIAdviseOkSink);
		//delete lpMAPIAdviseOkSink;
		lpMAPIAdviseOkSink = NULL;
	
	}

	if (lpMAPIAdviseSpamSink != NULL) 
	{

		if (ulAdviseSpamConnection && lpLearnSpamContentsTable)
		lpLearnSpamContentsTable->Unadvise(ulAdviseSpamConnection);

		ulAdviseSpamConnection = NULL;
		//UlRelease(lpMAPIAdviseSpamSink);
		//delete lpMAPIAdviseSpamSink;
		lpMAPIAdviseSpamSink = NULL;
	
	}

	if (lpMAPIAdviseOutSink != NULL) 
	{

		if (ulAdviseOutConnection && lpOutContentsTable)
		lpOutContentsTable->Unadvise(ulAdviseOutConnection);

		ulAdviseOutConnection = NULL;
		//UlRelease(lpMAPIAdviseOutSink);
		//delete lpMAPIAdviseOutSink;
		lpMAPIAdviseOutSink = NULL;
	
	}

	if (lpMAPIAdviseWhiteSink != NULL) 
	{

		if (ulAdviseWhiteConnection && lpWhiteContentsTable)
		lpWhiteContentsTable->Unadvise(ulAdviseWhiteConnection);

		ulAdviseWhiteConnection = NULL;
		//UlRelease(lpMAPIAdviseWhiteSink);
		//delete lpMAPIAdviseWhiteSink;
		lpMAPIAdviseWhiteSink = NULL;
	
	}
	
 
    if (lpContentsTable != NULL)	
	{
	  UlRelease(lpContentsTable);
	  lpContentsTable = NULL;
	} 

	if (lpOutContentsTable != NULL)	
	{
	  UlRelease(lpOutContentsTable);
	  lpOutContentsTable = NULL;
	} 

	if (lpLearnOkContentsTable != NULL)	
	{
	  UlRelease(lpLearnOkContentsTable);
	  lpLearnOkContentsTable = NULL;
	} 

	if (lpLearnSpamContentsTable != NULL)	
	{
	  UlRelease(lpLearnSpamContentsTable);
	  lpLearnSpamContentsTable = NULL;
	} 

	if (lpWhiteContentsTable != NULL)	
	{
	  UlRelease(lpWhiteContentsTable);
	  lpWhiteContentsTable = NULL;
	} 

	if (lpSysMsgContentsTable != NULL)	
	{
	  UlRelease(lpSysMsgContentsTable);
	  lpSysMsgContentsTable = NULL;
	} 

 
	if (lpNormalFolder != NULL && lpNormalFolder != lpInboxFolder)
	{
		lpNormalFolder->Release();
		lpNormalFolder = NULL;
	}

	if (lpInboxFolder != NULL)
	{
		lpInboxFolder->Release();
		lpInboxFolder = NULL;
	}

	if (lpSpamFolder != NULL)
	{
		lpSpamFolder->Release();
		lpSpamFolder = NULL;
	}
	



	if (lpLearnOkFolder != NULL)
	{
		lpLearnOkFolder->Release();
		lpLearnOkFolder = NULL;
	}

	if (lpLearnSpamFolder != NULL)
	{
		lpLearnSpamFolder->Release();
		lpLearnSpamFolder = NULL;
	}

	if (lpUnknownFolder != NULL)
	{
		lpUnknownFolder->Release();
		lpUnknownFolder = NULL;
	}

	if (lpOutboxFolder != NULL)
	{
		lpOutboxFolder->Release();
		lpOutboxFolder = NULL;
	}

	if (lpWhiteFolder != NULL)
	{
		lpWhiteFolder->Release();
		lpWhiteFolder = NULL;
	}

	if (lpSysMsgFolder != NULL)
	{
		lpSysMsgFolder->Release();
		lpSysMsgFolder = NULL;
	}

	if (lpMDB != NULL)
	{
	 lpMDB->Release();
	 lpMDB = NULL;
	}

	//if (pRows != NULL)
	//{
	//  FreeProws(pRows);
	//  pRows = NULL;

	//}



	if (lpMAPISession != NULL)
	{
	hRes = lpMAPISession->Logoff(0,MAPI_LOGOFF_UI,0); 
	MAPIUninitialize();

	lpMAPISession = NULL;
	}


	return hRes;

}

HRESULT  CMapiEx::OpenDefaultMessageStore()
{
   

   LPMAPITABLE pStoresTbl = NULL;
   LPSRowSet   pRow = NULL;
   static      SRestriction sres;
   SPropValue  spv;
   HRESULT     hRes;
   ULONG ulValues; 
   LPSPropValue pPropValues = NULL; 
   ULONG ul = 0;
  // LPMDB       lpTempMDB = NULL;
   

   enum {EID, NAME, NUM_COLS};
   static SizedSPropTagArray(NUM_COLS,sptCols) = {NUM_COLS, PR_ENTRYID, PR_DISPLAY_NAME};

   //*lpMDB = NULL;

   //Get the table of all the message stores available
   hRes = lpMAPISession -> GetMsgStoresTable(0, &pStoresTbl);
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
   if (FAILED(hRes)) goto quit;

   

   //Open the first returned (default) message store
   hRes = lpMAPISession->OpenMsgStore(
      NULL,//Window handle for dialogs
      pRow->aRow[0].lpProps[EID].Value.bin.cb,//size and...
      (LPENTRYID)pRow->aRow[0].lpProps[EID].Value.bin.lpb,//value of entry to open
      NULL,//Use default interface (IMsgStore) to open store
      MAPI_BEST_ACCESS,//Flags
      &lpMDB);//Pointer to place the store in
   if (FAILED(hRes)) goto quit;


   
    hRes = lpMDB->GetProps(NULL, 
         0, 
         &ulValues, 
         &pPropValues); 
    if (FAILED(hRes)) goto quit;

	for (ul = 0; ul < ulValues; ul++) 
    { 
        //if (PR_IPM_OUTBOX_ENTRYID == pPropValues[ul].ulPropTag) 

		if (PR_IPM_SENTMAIL_ENTRYID == pPropValues[ul].ulPropTag)
        { 
            //bOutboxFound = TRUE; 
			//OutputDebugString("OutBox Found");

			 // Try to open the Outbox 
			ULONG ulObjType = 0; 
			LPMAPIFOLDER pOutbox = NULL; 
			hRes = lpMDB->OpenEntry(pPropValues[ul].Value.bin.cb, 
				(ENTRYID *) pPropValues[ul].Value.bin.lpb, 
				NULL, 
				MAPI_BEST_ACCESS, 
				&ulObjType, 
				(LPUNKNOWN *) &lpOutboxFolder); 
			if (FAILED(hRes)) goto quit;
			// We have the Outbox opened... 

            break; 
        } 
    } 



   //Assign the out parameter
   //*lpMDB = lpTempMDB;

   //Always clean up your memory here!
quit:
   FreeProws(pRow);
   UlRelease(pStoresTbl);
   if (FAILED(hRes))
   {
      HRESULT hr;
      LPMAPIERROR lpError; 
      hr = lpMAPISession->GetLastError(hRes,0,&lpError);
      if (!hr)
      {
        // printf("%s\n%s\n",lpError->lpszError,lpError->lpszComponent);
         MAPIFreeBuffer(lpError);
      }
   }

   
   return hRes;
}

HRESULT CMapiEx::OpenInbox()
{
   ULONG        cbInbox;
   LPENTRYID    lpbInbox;
   ULONG        ulObjType;
   HRESULT      hRes = S_OK;
   LPSRowSet pRows ; 

   unsigned int i = 0;
   CString tmpStr = "";

    bool bSpamFolder=false;
	bool bFolder=false;
	bool bUnknownFolder=false;
	bool bLearnOkFolder=false;
	bool bLearnSpamFolder=false;
	bool bWhiteFolder=false;
	bool bSysMsgFolder=false;
   //LPMAPIFOLDER	lpTempFolder = NULL;
	
   //*lpInboxFolder = NULL;

   //The Inbox is usually the default receive folder for the message store
   //You call this function as a shortcut to get it's Entry ID
   
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

   hRes = lpMDB->GetReceiveFolder(
      NULL,      //Get default receive folder
      NULL,      //Flags
      &cbInbox,  //Size and ...
      &lpbInbox, //Value of the EntryID to be returned
      NULL);     //You don't care to see the class returned
   if (FAILED(hRes)) goto quit;
   
   
   hRes = lpMDB->OpenEntry(
      cbInbox,                      //Size and...
      lpbInbox,                     //Value of the Inbox's EntryID
      NULL,                         //We want the default interface    (IMAPIFolder)
      MAPI_BEST_ACCESS,             //Flags
      &ulObjType,                   //Object returned type
      (LPUNKNOWN *) &lpInboxFolder); //Returned folder
   if (FAILED(hRes)) goto quit;
   

  
		//取出下層
		hRes = lpInboxFolder->GetHierarchyTable(0,&lpContentsTable);
		if (FAILED(hRes)) goto quit;

		
   
		//列出 query
		hRes = HrQueryAllRows(
			lpContentsTable,
			(LPSPropTagArray) &sptCols,
			NULL,//restriction...we're not using this parameter
			NULL,//sort order...we're not using this parameter
			0,
			&pRows);

		if (FAILED(hRes)) goto quit;

   
		

		for (i = 0; i < pRows -> cRows; i++)
		{
			if (pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].ulPropTag == PR_DISPLAY_NAME)
			{
				/*
				FolderListData mListData;
				strcpy(mListData.FolderName,pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA);
				mListData.FolderLen = pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb;
				memcpy(mListData.FolderId,pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,mListData.FolderLen);
				
				m_FolderList.insert(m_FolderList.end(),mListData);*/

				tmpStr.Format("%s",pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA);	   
				//取消更換預設收信目錄的功能
				//if (mFolderName != NULL && *mFolderName != 0)
				//{
					/*if (strcmp(mFolderName,tmpStr) == 0)  
					{
					//OutputDebugString(tmpStr);

						lpInboxFolder->Release();

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpInboxFolder); //Returned folder

						
				
						if (FAILED(hRes)) goto quit;

					//break;
					} else */
				    if (strcmp(SpamFolderName,tmpStr) == 0)  
					{
					
						//lpInboxFolder->Release();

						bSpamFolder = true;
						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpSpamFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

						//取得 message table size
						LPMAPITABLE lpConTable;
						hRes = lpSpamFolder->GetContentsTable(
							0,
						  &lpConTable);

						if (SUCCEEDED(hRes))
						{
						

							LPSRowSet pRows ;
							
							hRes = HrQueryAllRows(
								lpConTable,
								(LPSPropTagArray) &sptCols,
								NULL,//restriction...we're not using this parameter
								NULL,//sort order...we're not using this parameter
								0,
								&pRows);

							if (pRows->cRows > 0 )
							{
							
								CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();
								dlg->ChangeTrashIcon(1);
							
							}

							FreeProws(pRows);
							UlRelease(lpConTable);


						}


					
					
					}else if (strcmp(OkFolderName,tmpStr) == 0 
						&& OkFolderName[0] != 0)  
					{
					
						//lpInboxFolder->Release();
						bFolder = true;

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpNormalFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

					
					
					}else if (strcmp(LearnOkFolderName,tmpStr) == 0)  
					{
					
						//lpInboxFolder->Release();
						bLearnOkFolder = true;

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpLearnOkFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

					
					
					}else if (strcmp(LearnSpamFolderName,tmpStr) == 0)  
					{
					
						//lpInboxFolder->Release();
						bLearnSpamFolder = true;

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpLearnSpamFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

					
					
					}else if (strcmp(UnknownFolderName,tmpStr) == 0)  
					{
					
						//lpInboxFolder->Release();
						bUnknownFolder=true;

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpUnknownFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

					
					
					}
					else if (strcmp(LearnWhiteFolderName,tmpStr) == 0)  
					{
					
						//lpInboxFolder->Release();
						bWhiteFolder=true;

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpWhiteFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

					
					
					}else if (strcmp(SysMsgFolderName,tmpStr) == 0)  
					{
					
						//lpInboxFolder->Release();
						bSysMsgFolder=true;

						hRes = lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &lpSysMsgFolder); //Returned folder

					 
				
						if (FAILED(hRes)) goto quit;

					
					
					}
				//}
				
			}

		}

		lpContentsTable->Release();
		FreeProws(pRows); 
		pRows = NULL;

		if (OkFolderName[0] == 0)
		{
		  lpNormalFolder = lpInboxFolder;
		}
		else if (!bFolder)
		{
		  //建立目錄
			lpInboxFolder->CreateFolder(FOLDER_GENERIC,OkFolderName,"正常的信件將放於此處",NULL,OPEN_IF_EXISTS,&lpNormalFolder);
		}
	 

		if (!bSpamFolder)
		{
		  lpInboxFolder->CreateFolder(FOLDER_GENERIC,SpamFolderName,"垃圾信件將放於此處",NULL,OPEN_IF_EXISTS,&lpSpamFolder);
		}

		if (!bUnknownFolder)
		{
		  lpInboxFolder->CreateFolder(FOLDER_GENERIC,UnknownFolderName,"無法辨識信件將放於此處,請拖拉到學習信件匣",NULL,OPEN_IF_EXISTS,&lpUnknownFolder);
		}

		if (!bLearnOkFolder)
		{
		  lpInboxFolder->CreateFolder(FOLDER_GENERIC,LearnOkFolderName,"拖拉信件至此,學習為正常信件",NULL,OPEN_IF_EXISTS,&lpLearnOkFolder);
		}

		if (!bLearnSpamFolder)
		{
		  lpInboxFolder->CreateFolder(FOLDER_GENERIC,LearnSpamFolderName,"拖拉信件至此,學習為垃圾信件",NULL,OPEN_IF_EXISTS,&lpLearnSpamFolder);
		}

		if (!bWhiteFolder)
		{
			lpInboxFolder->CreateFolder(FOLDER_GENERIC,LearnWhiteFolderName,"拖拉信件至此,可加入白名單",NULL,OPEN_IF_EXISTS,&lpWhiteFolder);
		}

		if (!bSysMsgFolder)
		{
			lpInboxFolder->CreateFolder(FOLDER_GENERIC,SysMsgFolderName,"系統訊息",NULL,OPEN_IF_EXISTS,&lpSysMsgFolder);
		}
   

  
   //Always clean up your memory here!
   quit:
   MAPIFreeBuffer(lpbInbox);
   //MAPIFreeBuffer(ipm_eid);



  return hRes;
}

HRESULT CMapiEx::ListMessages(char Type)
{
   HRESULT hRes = S_OK;
   lpContentsTable = NULL;
   
   //LPSTREAM lpStream = NULL;
   //ULONG i;
   //CString  SaveStr = "" ;

   //You define a SPropTagArray array here using the SizedSPropTagArray Macro
   //This enum will allows you to access portions of the array by a name instead of a number.
   //If more tags are added to the array, appropriate constants need to be added to the enum.
   enum {
      //ePR_SENT_REPRESENTING_NAME,
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,
     // ePR_BODY,
      //ePR_PRIORITY,
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
      //PR_SENT_REPRESENTING_NAME,	  
	  PR_INSTANCE_KEY,
	  PR_SENDER_NAME,
      PR_SUBJECT,
	 // PR_BODY_HTML,      
      //PR_PRIORITY,
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };
   //PR_RTF_COMPRESSED,

   //PR_BODY_HTML,

   
   //GetContentsTable
   hRes = lpInboxFolder->GetContentsTable(
      0,
      &lpContentsTable);
   if (FAILED(hRes)) goto quit;

   hRes = lpLearnOkFolder->GetContentsTable(
      0,
      &lpLearnOkContentsTable);
   if (FAILED(hRes)) goto quit;

   hRes = lpLearnSpamFolder->GetContentsTable(
      0,
      &lpLearnSpamContentsTable);
   if (FAILED(hRes)) goto quit;

   hRes = lpOutboxFolder->GetContentsTable(
      0,
      &lpOutContentsTable);
   if (FAILED(hRes)) goto quit;

   hRes = lpWhiteFolder->GetContentsTable(
      0,
      &lpWhiteContentsTable);
   if (FAILED(hRes)) goto quit;

    hRes = lpSysMsgFolder->GetContentsTable(
      0,
      &lpSysMsgContentsTable);
   if (FAILED(hRes)) goto quit;

   hRes = lpOutContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags

   hRes = lpLearnOkContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags

   hRes = lpLearnSpamContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags

   hRes = lpWhiteContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags

   hRes = lpSysMsgContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags
   	 
   if (Type == 1)
   {
	   //取消 type = 1
   //取得所有的 Message rows
   /*hRes = HrQueryAllRows(
      lpContentsTable,
      (LPSPropTagArray) &sptCols,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &pRows);*/

   }
   else if (Type == 2)
   {
    hRes = lpContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags
   
   }
   if (FAILED(hRes)) goto quit;

   
  
	return hRes;
   


   quit:
   //FreeProws(pRows);
   //pRows = NULL;
   //潛在 Leak 	問題
   UlRelease(lpContentsTable);
   lpContentsTable = NULL;

   UlRelease(lpOutContentsTable);
   lpOutContentsTable = NULL;

   UlRelease(lpLearnOkContentsTable);
   lpLearnOkContentsTable = NULL;

   UlRelease(lpLearnSpamContentsTable);
   lpLearnSpamContentsTable = NULL;

   UlRelease(lpOutContentsTable);
   lpOutContentsTable = NULL;

   UlRelease(lpWhiteContentsTable);
   lpWhiteContentsTable = NULL;

   UlRelease(lpSysMsgContentsTable);
   lpSysMsgContentsTable = NULL;

   return hRes;

}

void CMapiEx::CreateSysMsg(char *MailHeader , char *MailContent , char *SenderEmail , char *Subject)
{

	
	
	LPMESSAGE  lpMessage = NULL;
	lpSysMsgFolder->CreateMessage(NULL,NULL,&lpMessage);

	SPropValue    spvAttach[12];

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

	spvAttach[11].ulPropTag =  PR_TRANSPORT_MESSAGE_HEADERS;
	spvAttach[11].Value.lpszA = MailHeader;

	

	
//	spvAttach[8].ulPropTag = PR_TRANSPORT_MESSAGE_HEADERS; 
	//spvAttach[8].Value.lpszA = "From: =?Big5?B?obSrS6l5sOq72rlxuNwgobQ=?= <aaaek.aaaek@casparcaspar.org>\r\n\r\n"; 



	lpMessage->SetProps(
					12,
					(LPSPropValue)&spvAttach,
						NULL);

	lpMessage->SaveChanges(KEEP_OPEN_READWRITE);

	lpMessage->Release();
	


}
HRESULT CMapiEx::DelSysFolderMail(int day)
{

	  enum {   
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,   
	  PR_INSTANCE_KEY,
	  PR_SENDER_NAME,
      PR_SUBJECT,	 
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };
   
	LPSRowSet pRows ;
			
		 HRESULT hRes = HrQueryAllRows(
				 lpSysMsgContentsTable,
				 (LPSPropTagArray) &sptCols,
				 NULL,//restriction...we're not using this parameter
				 NULL,//sort order...we're not using this parameter
				 0,
				 &pRows);

				 if (SUCCEEDED(hRes))
				 {
					
					 
					  int Count = 0;

					  
					  //SPropValue *sprop = new SPropValue[pRows->cRows];
					  SBinary *sbin = new SBinary[pRows->cRows];

					 for (int i = 0; i < pRows->cRows; i++)
					 {

						 	LPMESSAGE lpMessage = NULL;
							ULONG ulObjType = NULL;

							 

							HRESULT hRes2 = g_MapiEx->lpMDB->OpenEntry(
							pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,
							NULL,//default interface
							MAPI_BEST_ACCESS,
							&ulObjType,
							(LPUNKNOWN *) &lpMessage);

							if (SUCCEEDED(hRes2))
							{
								LPSPropValue   lpPropss = NULL;
								hRes2 = HrGetOneProp(lpMessage,PR_CREATION_TIME ,&lpPropss);

								SYSTEMTIME SystemCreate;
								FileTimeToSystemTime(&lpPropss->Value.ft,&SystemCreate);

								CTime t1(SystemCreate);
								CTime t2((time_t) CTime::GetCurrentTime().GetTime());

								CTimeSpan ts = t2-t1;

								// 大於 x 日才刪除
								if(ts.GetTotalHours() > 24 * day)
								{			 
									
									sbin[Count] = pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin;
									Count ++;


								}
							}
					 }


					  if (Count > 0)
					  {
						ENTRYLIST list; 
						list.cValues = Count; 	  
						list.lpbin = sbin;


						lpSysMsgFolder->DeleteMessages(&list,0,0,0);
					  }

					  delete sbin;

				 }


				 FreeProws(pRows);


		return hRes;

}

/*
HRESULT CMapiEx::LearnAllFolder(OLFolder* folder , char howlearn ,CProgressCtrl *progress)
{
	
	  enum {    
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,   
	  PR_SENDER_NAME,
      PR_SUBJECT,	 
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };
	
	
	LPMAPIFOLDER LearnFolder;
	ULONG        ulObjType;
	HRESULT hRes = lpMDB->OpenEntry(
			folder->FolderLen ,                      //Size and...
		(LPENTRYID) folder->FolderId,                     //Value of the Inbox's EntryID
			NULL,                         //We want the default interface    (IMAPIFolder)
			MAPI_BEST_ACCESS,             //Flags
			&ulObjType,                   //Object returned type
			(LPUNKNOWN *) &LearnFolder); //Returned folder

	if (SUCCEEDED(hRes))
	{
	
		LPMAPITABLE lpLearnContentsTable = NULL;

		hRes = LearnFolder->GetContentsTable(
		0,
		&lpLearnContentsTable);


		if (SUCCEEDED(hRes))
		{	
		 
				 LPSRowSet pRows ;
			
				 hRes = HrQueryAllRows(
				 lpLearnContentsTable,
				 (LPSPropTagArray) &sptCols,
				 NULL,//restriction...we're not using this parameter
				 NULL,//sort order...we're not using this parameter
				 0,
				 &pRows);

				 if (SUCCEEDED(hRes))
				 {
					 if (progress != NULL)
							progress->SetRange(0, pRows->cRows);

					 for (int i = 0; i < pRows -> cRows; i++)
					 {
						//if (pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].ulPropTag == PR_DISPLAY_NAME)
						//{				
				 
						lpMAPIAdviseSink->ProcessLearnMsg(&pRows->aRow[i],howlearn);
							
						if (progress != NULL)
							progress->SetPos(i+1);
						//}
					 }

					FreeProws(pRows); 
				 }

			
		
		}


		if (lpLearnContentsTable != NULL)
			lpLearnContentsTable->Release();
	
	}

	return S_OK;
}*/

HRESULT CMapiEx::GetRowMsgItem(SRow *mSRow , LPSTR* MsgContent ,LPSTR* TextContent, LPSTR* Subject , LPSTR* SenderName, int *AttCount)
{
      LPMESSAGE lpMessage = NULL;
      ULONG ulObjType = NULL;
      //LPSPropValue lpProp = NULL;
	  HRESULT hRes = S_OK;
	  LPSTREAM lpStream = NULL;
	  //char *htmlbuf=0;
	  //unsigned int htmlsize=0;
	  //unsigned int bufsize=10240; 
	  htmlsize = 0;
	  bodysize = 0;


	  bodybuf[0] = 0;
	  htmlbuf[0] = 0;

	  *TextContent = bodybuf;
	  *MsgContent = htmlbuf;

	  /*enum { 
	  ePR_SENDER_NAME,
	  ePR_SUBJECT,
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
      NUM_COLS};*/

	  //SRow *mSRow;
	  //mSRow = &pRows->aRow[msgindex];

	  //取 sender string
	  if (mSRow->lpProps[ePR_SENDER_NAME].ulPropTag == PR_SENDER_NAME)
	  {
		  *SenderName = mSRow->lpProps[ePR_SENDER_NAME].Value.lpszA;		  
	  }

	  //取主旨
	  if (mSRow->lpProps[ePR_SUBJECT].ulPropTag == PR_SUBJECT)
	  {
		  *Subject = mSRow->lpProps[ePR_SUBJECT].Value.lpszA;
	  }
	  
	 

      
         hRes = lpMDB->OpenEntry(
            mSRow->lpProps[ePR_ENTRYID].Value.bin.cb,
            (LPENTRYID) mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb,
            NULL,//default interface
            MAPI_BEST_ACCESS,
            &ulObjType,
            (LPUNKNOWN *) &lpMessage);
         if (!FAILED(hRes))
         {
			  
			   //取 body
			   //if (mSRow->lpProps[ePR_BODY].ulPropTag == PR_BODY)
			   //{
					if (MAPI_E_NOT_FOUND != mSRow->lpProps[ePR_BODY].Value.l)
					{
						LPSPropValue lpProp = NULL;
						LPSTREAM lpBodyStream = NULL;

						hRes = HrGetOneProp(
										lpMessage,
										PR_BODY,
										&lpProp);

						if (hRes == MAPI_E_NOT_ENOUGH_MEMORY)
						{
						   //不處理過大信件	
						   /*hRes = lpMessage->OpenProperty(
									PR_BODY,
									&IID_IStream,
									STGM_READ,
									NULL,
									(LPUNKNOWN *) &lpBodyStream);

							ReadBodyStream(lpBodyStream);							 

							bodybuf[bodysize] = 0;
							*TextContent = bodybuf;*/

						}
						else
						{
						
							if (strlen(lpProp->Value.lpszA) < bodybufsize)
							{
							
								bodysize = strlen(lpProp->Value.lpszA) ;
								memcpy(bodybuf,lpProp->Value.lpszA,bodysize + 1);
								*TextContent = bodybuf;

							}
						}

						MAPIFreeBuffer(lpProp);

					}
				 
				//}
			 
			  SPropValue *svmask;
			  bool mustsync=true;
			  bool IsSupportHtml=false;
			  //bool 

			
            
			  hRes = HrGetOneProp(
               lpMDB,
               PR_STORE_SUPPORT_MASK,
			   //PR_BODY_HTML,
               &svmask);

			   if (hRes == S_OK)
			   {
				   if ( (svmask->Value.ul & STORE_RTF_OK) != 0 ) 
				   {
					   mustsync=false;
				   }
				   else if ( (svmask->Value.ul & 0x00010000) != 0 ) //STORE_HTML_OK
				   {					  
					   IsSupportHtml = true;
				   }

				   else if (mSRow->lpProps[ePR_RTF_IN_SYNC].ulPropTag == PR_RTF_IN_SYNC 
					         && mSRow->lpProps[ePR_RTF_IN_SYNC].Value.b !=0)
				   {			 
				   	   mustsync=false;				   
				   }

                   MAPIFreeBuffer(svmask);
			   }

			   if (IsSupportHtml)
			   {
			   
				   //PR_BODY_HTML
				   	hRes = lpMessage->OpenProperty(                  
					PROP_TAG(PT_TSTRING,0x1013),
					&IID_IStream,
					STGM_READ,
					NULL,
					(LPUNKNOWN *) &lpStream);

					if (hRes == S_OK)
					{ 
				
						//IStream *iunstream; // for the uncompressed stream					 
						ReadStream(lpStream);
						
						//iunstream->Release();
						lpStream->Release();

						if (*TextContent == NULL)
						{
							//有 html 但是不給 text , 必須自己解
							 if (bodybufsize < htmlsize)
							 {							 
								 //bodybuf 不足
								 delete bodybuf;

								 bodysize = htmlsize;
								 bodybufsize = htmlsize;
								 bodybuf = new char[htmlsize];							 
							 }


							 CMailCodec cm;					  
							 cm.TrimHTML(htmlbuf,bodybuf);
							 *TextContent = bodybuf;


						
						}

						
						//
					}


			   }
			   else
			   {

					if (mustsync)
					{ 
						BOOL isupdated; 
						RTFSync(lpMessage,RTF_SYNC_BODY_CHANGED,&isupdated);
						if (isupdated) lpMessage->SaveChanges(0);
					}

					hRes = lpMessage->OpenProperty(                  
					PR_RTF_COMPRESSED,
					&IID_IStream,
					STGM_READ,
					NULL,
					(LPUNKNOWN *) &lpStream);

					if (hRes == S_OK)
					{ 
				
						IStream *iunstream; // for the uncompressed stream
						WrapCompressedRTFStream(lpStream,0,&iunstream);
						//htmlsize = ReadStream(iunstream,&htmlbuf,&bufsize);
						ReadStream(iunstream);


						CMailCodec cm;					  
						cm.ParseRTFBuf(htmlbuf,&htmlsize);
					

						iunstream->Release();
						lpStream->Release();					 
					}
			   }
				 

			 //* 處理附件
             IMAPITable *atable;
			 hRes = lpMessage->GetAttachmentTable(0,&atable);
			 if (hRes == S_OK)
			 {
			 
				 enum { //ePR_ATTACH_SIZE,
					    ePR_ATTACH_NUM,
						//ePR_ATTACH_METHOD ,						 
						NUM_COLS};

				 SizedSPropTagArray(NUM_COLS,sptCols) = {NUM_COLS, {PR_ATTACH_NUM  }};					 
				 SRowSet *arows;
				  
				
				 hRes = HrQueryAllRows(atable,(SPropTagArray*)&sptCols,NULL,NULL,0,&arows);
				 if (hRes==S_OK)
				 { 
					//if (arows->cRows>0) AvailableOffline = true;
					if (arows->cRows > 0 )
					{
					    *AttCount = arows->cRows;
						//CString tmpStr = "";

						for (ULONG j= 0 ; j < arows->cRows ; j ++)
						{ 
						
								LPATTACH lpAttach = NULL;
								if (SUCCEEDED(hRes = lpMessage->OpenAttach (
											arows->aRow[j].lpProps[ePR_ATTACH_NUM].Value.l,
											NULL, MAPI_BEST_ACCESS, &lpAttach)))
								{
											 
									SizedSPropTagArray(1,sptCols) = {1, {PR_ATTACH_METHOD}};
									//SizedSPropTagArray(1,sptCols) = {1, {PR_ATTACH_METHOD}};
								 
									LPSPropValue pProps = NULL;
									ULONG cVals = 0;
									
									if (SUCCEEDED(hRes = lpAttach->GetProps((SPropTagArray*)&sptCols,NULL,&cVals,&pProps)))
									{
										if (pProps[0].ulPropTag == PR_ATTACH_METHOD)
										{
												if (pProps[0].Value.l == ATTACH_EMBEDDED_MSG)
												{
													LPMESSAGE AttlpMessage = NULL;

													hRes = lpAttach->OpenProperty(                  
													PR_ATTACH_DATA_OBJ,
													&IID_IMessage ,
													0L,
													0,
													(LPUNKNOWN *) &AttlpMessage );


													if (hRes == S_OK)
													{ 
												 

														LPSPropValue lpProp = NULL;
														hRes = HrGetOneProp(
																AttlpMessage,
																PR_BODY,
																&lpProp);

														if (hRes == MAPI_E_NOT_ENOUGH_MEMORY)
														{
															//不處理過大信件
															/*
															LPSTREAM lpBodyStream = NULL;
															hRes = AttlpMessage->OpenProperty(
															PR_BODY,
															&IID_IStream,
															STGM_READ,
															NULL,
															(LPUNKNOWN *) &lpBodyStream);

															ReadBodyStream(lpBodyStream);							 

															bodybuf[bodysize] = 0;
															*TextContent = bodybuf;

															lpBodyStream->Release();*/

														}else if (SUCCEEDED(hRes) && strlen(lpProp->Value.lpszA)+bodysize < bodybufsize)
														{
							
															if (bodysize > 0)
															{
																
																memcpy(bodybuf+bodysize,lpProp->Value.lpszA,strlen(lpProp->Value.lpszA) + 1);
																bodysize = bodysize + strlen(lpProp->Value.lpszA);
																//*TextContent = bodybuf;
															}
															else
															{
																memcpy(bodybuf,lpProp->Value.lpszA,strlen(lpProp->Value.lpszA) + 1);
																*TextContent = bodybuf;														
															}

														}
														else
														{
															//改用 PR_BODY_HTML 讀

															hRes = HrGetOneProp(
																AttlpMessage,
																PROP_TAG(PT_TSTRING,0x1013),
																&lpProp);

															if (SUCCEEDED(hRes)  && strlen(lpProp->Value.lpszA)+bodysize < bodybufsize)
															{
															
																if (bodysize > 0)
																{
																
																	memcpy(bodybuf+bodysize,lpProp->Value.lpszA,strlen(lpProp->Value.lpszA) + 1);
																	bodysize = bodysize + strlen(lpProp->Value.lpszA);
																	//*TextContent = bodybuf;
																}
																else
																{
																	memcpy(bodybuf,lpProp->Value.lpszA,strlen(lpProp->Value.lpszA) + 1);
																	*TextContent = bodybuf;														
																}

															
															}




														
														}

														MAPIFreeBuffer(lpProp);										
											
													}

													AttlpMessage->Release();


												}
												else 
												{
													SizedSPropTagArray(1,sptCols) = {1, {PR_ATTACH_FILENAME}};
													LPSPropValue pProps = NULL;
													ULONG cVals = 0;


													if (SUCCEEDED(hRes = lpAttach->GetProps((SPropTagArray*)&sptCols,NULL,&cVals,&pProps)))
													{
									

														if (pProps[0].ulPropTag == PR_ATTACH_FILENAME)
														{
										
															CString FileName = "";
															FileName = pProps[0].Value.lpszA;										

															CVirus cv;
															
															if (FileName.Find(".gif",0) >= 0 || FileName.Find(".jpg",0) >= 0 ||FileName.Find(".jpeg",0) >= 0 || FileName.Find(".png",0) >= 0 || FileName.Find(".bmp",0) >= 0)
															{
																continue;
															}
															if (cv.FileNameCheck(pProps[0].Value.lpszA))
															{
																hRes = ExVirus; 
																break;
															} 
															else 
															{
															
																//做 副檔名 check
																	IStream *m_istream = NULL;

																	hRes = lpAttach->OpenProperty(                  
																		PR_ATTACH_DATA_BIN ,
																		&IID_IStream,
																		STGM_READ,
																		NULL,
																		(LPUNKNOWN *) &m_istream);

																	if (hRes == S_OK)
																	{ 
												 
																		
																		char *buff = new char[cv.GetBlockSize()];

																		ULONG byteread = 0;
																		m_istream->Read(buff,cv.GetBlockSize(),&byteread);

																		//
																		if (cv.ZipHeaderCheck(buff,byteread))
																		{
								
																				hRes = ExVirus; 
																				m_istream->Release();
																				delete buff;

																				break;
																		}

																		delete buff;
																		m_istream->Release();											
											
																	}			
																											
															
															}
														
										
															#ifdef _ATTDBG
															if (FileName.Find("SpamDB.txt",0) >= 0)
															{
																	hRes = ExHasChecked;
																	break;
															}
															#endif

															#ifdef _ATTDBG
															  else if ((FileName.Find(".htm",0) > 0  || FileName.Find(".txt",0) > 0 )&& FileName.Find("SpamDB.txt",0) == -1)
															#endif

															#ifndef _ATTDBG
															 if ((FileName.Find(".htm",0) > 0  || FileName.Find(".txt",0) > 0 )&& FileName.Find("SpamDB.txt",0) == -1)
															#endif
															{
															//txt ,html 取內容
																	hRes = lpAttach->OpenProperty(                  
																		PR_ATTACH_DATA_BIN ,
																		&IID_IStream,
																		STGM_READ,
																		NULL,
																		(LPUNKNOWN *) &lpStream);

																	if (hRes == S_OK)
																	{ 
												 
																		ReadStream(lpStream); 
																		lpStream->Release();											
											
																	}
											
															}

														}

													}
												}

										}
									}
								}
								lpAttach->Release();
			
						}

						
						
						
					}
					FreeProws(arows);
				 } 
				  atable->Release();

			 }




         }
     // }
      //MAPIFreeBuffer(lpProp);	  
      UlRelease(lpMessage);
      //hRes = S_OK; 

   	  htmlbuf[htmlsize] = 0;
	  *MsgContent = htmlbuf;
   
   return hRes;
}

       
HRESULT CMapiEx::EmptySysFolder()
{

	HRESULT       hRes = NULL ;

	if (lpSpamFolder != NULL)
	{
	
		hRes = lpSysMsgFolder->EmptyFolder(NULL,NULL,DEL_ASSOCIATED);
	
	}

	return hRes;

}
HRESULT CMapiEx::EmptySpamFolder()
{

	HRESULT       hRes = NULL ;

	if (lpSpamFolder != NULL)
	{
	
		hRes = lpSpamFolder->EmptyFolder(NULL,NULL,DEL_ASSOCIATED);
	
	}

	return hRes;

	

}

void CMapiEx::GetiniFolderSetup()
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


	  
	GetPrivateProfileString("FolderSetup","SpamFolderName","[垃圾信件匣]",SpamFolderName,sizeof(SpamFolderName),IniPath);
	GetPrivateProfileString("FolderSetup","OkFolderName","",OkFolderName,sizeof(OkFolderName),IniPath);
	GetPrivateProfileString("FolderSetup","UnknownFolderName","[無法辨識信件匣]",UnknownFolderName,sizeof(UnknownFolderName),IniPath);
	GetPrivateProfileString("FolderSetup","LearnOkFolderName","[學習正常信件匣]",LearnOkFolderName,sizeof(LearnOkFolderName),IniPath);
	GetPrivateProfileString("FolderSetup","LearnSpamFolderName","[學習垃圾信件匣]",LearnSpamFolderName,sizeof(LearnSpamFolderName),IniPath);
	GetPrivateProfileString("FolderSetup","LearnWhiteFolderName","[學習白名單信件匣]",LearnWhiteFolderName,sizeof(LearnWhiteFolderName),IniPath);
	GetPrivateProfileString("FolderSetup","SysMsgFolderName","[系統訊息]",SysMsgFolderName,sizeof(SysMsgFolderName),IniPath);

}

HRESULT CMapiEx::DelMsgById(SBinary *sbin,LPMAPIFOLDER  lpFromFolder)
{
  HRESULT hRes = S_FALSE;
  if (sbin != NULL)
  {
  
	  

	  ENTRYLIST list; 
      list.cValues = 1; 	  
      list.lpbin = sbin;

	 
	  hRes = lpFromFolder->DeleteMessages(&list,NULL,NULL,NULL);	  
 	  //if (hRes != S_OK)
	  //{ 
		  //OutputDebugString("DeleteMessages Error \n"); 
	  //}

	  

	   
  }

  return hRes;
}

HRESULT CMapiEx::MoveMsgById(SBinary *sbin,LPMAPIFOLDER  lpFromFolder,LPMAPIFOLDER  lpMoveToFolder)
{
  HRESULT hRes = S_FALSE;
  if (sbin != NULL)
  {
  
	  

	  ENTRYLIST list; 
      list.cValues = 1; 	  
      list.lpbin = sbin;

	 
	  hRes = lpFromFolder->CopyMessages(&list,NULL,lpMoveToFolder,NULL,NULL,MESSAGE_MOVE );
	  //hRes = lpFromFolder->CopyMessages(&list,NULL,lpMoveToFolder,NULL,NULL,NULL);
 	  if (hRes != S_OK)
	  { 
		  OutputDebugString("CopyMessages Error \n"); 
	  }

	  

	   
  }

  return hRes;

}
HRESULT CMapiEx::NotificationOn ()
{
	HRESULT       hRes = NULL ;

	if (!lpContentsTable) goto quit;

	
	//處理 Table Sync
	InBoxTableReload();



	lpMAPIAdviseSink = new CMAPIAdviseSink(INBOX_F);
	// lpContentsTable
	hRes = lpContentsTable->Advise(
		fnevTableModified,
		//fnevNewMail,
		//fnevObjectCopied,
		(IMAPIAdviseSink *) lpMAPIAdviseSink,
		&ulAdviseConnection);

	 

	if (MAPI_E_NO_SUPPORT == hRes)//Some tables don't support this!
	{		
		UlRelease(lpMAPIAdviseSink);
		lpMAPIAdviseSink = NULL;
		//WARNHRESMSG(hRes,"This table doesn't support notifications");
		hRes = S_OK;//mask the error
		goto quit;
	}	
	
	if (!lpLearnOkContentsTable) goto quit;

	lpMAPIAdviseOkSink = new CMAPIAdviseSink(LEARNOKBOX_F);
	
	hRes = lpLearnOkContentsTable->Advise(
		fnevTableModified,
		(IMAPIAdviseSink *) lpMAPIAdviseOkSink,
		&ulAdviseOkConnection);

	if (MAPI_E_NO_SUPPORT == hRes)//Some tables don't support this!
	{		
		UlRelease(lpMAPIAdviseOkSink);
		lpMAPIAdviseOkSink = NULL;
		//WARNHRESMSG(hRes,"This table doesn't support notifications");
		hRes = S_OK;//mask the error
		goto quit;
	}		

	if (!lpLearnSpamContentsTable) goto quit;

	lpMAPIAdviseSpamSink = new CMAPIAdviseSink(LEARNSPAMBOX_F);
	
	hRes = lpLearnSpamContentsTable->Advise(
		fnevTableModified,
		(IMAPIAdviseSink *) lpMAPIAdviseSpamSink,
		&ulAdviseSpamConnection);

	if (MAPI_E_NO_SUPPORT == hRes)//Some tables don't support this!
	{		
		UlRelease(lpMAPIAdviseSpamSink);
		lpMAPIAdviseSpamSink = NULL;
		//WARNHRESMSG(hRes,"This table doesn't support notifications");
		hRes = S_OK;//mask the error
		goto quit;
	}	


	lpMAPIAdviseOutSink = new CMAPIAdviseSink(OUTBOX_F);
	
	hRes = lpOutContentsTable->Advise(
		fnevTableModified,
		(IMAPIAdviseSink *) lpMAPIAdviseOutSink,
		&ulAdviseOutConnection);

	if (MAPI_E_NO_SUPPORT == hRes)//Some tables don't support this!
	{		
		UlRelease(lpMAPIAdviseOutSink);
		lpMAPIAdviseOutSink = NULL;
		//WARNHRESMSG(hRes,"This table doesn't support notifications");
		hRes = S_OK;//mask the error
		goto quit;
	}	
   
	lpMAPIAdviseWhiteSink = new CMAPIAdviseSink(LEARNWHITEBOX_F);
	
	hRes = lpWhiteContentsTable->Advise(
		fnevTableModified,
		(IMAPIAdviseSink *) lpMAPIAdviseWhiteSink,
		&ulAdviseWhiteConnection);

	if (MAPI_E_NO_SUPPORT == hRes)//Some tables don't support this!
	{		
		UlRelease(lpMAPIAdviseWhiteSink);
		lpMAPIAdviseWhiteSink = NULL;
		//WARNHRESMSG(hRes,"This table doesn't support notifications");
		hRes = S_OK;//mask the error
		goto quit;
	}	
	//HrThisThreadAdviseSink(lpMAPIAdviseOutSink,(IMAPIAdviseSink **)  &lpMAPIAdviseOutSink);
	
	//if (!FAILED(hRes)) goto Cleanup;



quit:
	//MAPIFreeBuffer(pProps);
	return hRes;
	

}

HRESULT CMapiEx::NotificationOff ()
{

	
	
	if (ulAdviseConnection && lpContentsTable)
		lpContentsTable->Unadvise(ulAdviseConnection);


	if (lpMAPIAdviseSink)
	{	
		ulAdviseConnection = NULL;
		
		//UlRelease(lpMAPIAdviseSink);
		//delete lpMAPIAdviseSink;
		lpMAPIAdviseSink = NULL;
	
	}	
	
	

	if (ulAdviseOkConnection && lpLearnOkContentsTable)
		lpLearnOkContentsTable->Unadvise(ulAdviseOkConnection);

	if (lpMAPIAdviseOkSink)
	{	
		ulAdviseOkConnection = NULL;
		//UlRelease(lpMAPIAdviseOkSink);
		//delete lpMAPIAdviseOkSink;
		lpMAPIAdviseOkSink = NULL;
	
	}

 //-->
	if (ulAdviseSpamConnection && lpLearnSpamContentsTable)
		lpLearnSpamContentsTable->Unadvise(ulAdviseSpamConnection);

	if (lpMAPIAdviseSpamSink)
	{	
		ulAdviseSpamConnection = NULL;
		//UlRelease(lpMAPIAdviseSpamSink);
		//delete lpMAPIAdviseSpamSink;
		lpMAPIAdviseSpamSink = NULL;
	
	}

	if (ulAdviseOutConnection && lpOutContentsTable)
		lpOutContentsTable->Unadvise(ulAdviseOutConnection);

	if (lpMAPIAdviseOutSink)
	{	
		ulAdviseOutConnection = NULL;
		//UlRelease(lpMAPIAdviseOutSink);
		//delete lpMAPIAdviseOutSink;
		lpMAPIAdviseOutSink = NULL;
	
	}

	if (ulAdviseWhiteConnection && lpWhiteContentsTable)
		lpWhiteContentsTable->Unadvise(ulAdviseWhiteConnection);

	if (lpMAPIAdviseWhiteSink)
	{	
		ulAdviseWhiteConnection = NULL;
		//UlRelease(lpMAPIAdviseWhiteSink);
		//delete lpMAPIAdviseWhiteSink;
		lpMAPIAdviseWhiteSink = NULL;
	
	}

	
	return S_OK;

}
void CMapiEx::ScanAllBox()
{
    LPSRowSet MsgRows ;
	
 

	enum {   
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,    
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,     
	  PR_INSTANCE_KEY,
	  PR_SENDER_NAME,
      PR_SUBJECT,	
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };

	HRESULT hRes = HrQueryAllRows(
      lpContentsTable,
      (LPSPropTagArray) &sptCols,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &MsgRows);

	if (SUCCEEDED(hRes))
	{
	
		for (int i=0 ; i < MsgRows->cRows; i ++)
		{
		
			lpMAPIAdviseSink->ProcessMsg(&MsgRows->aRow[i]); 
			
		}

		FreeProws(MsgRows);
	
	}
}

void CMapiEx::ScanUnknownBox()
{
    LPSRowSet MsgRows ; 

	enum {   
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,    
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,     
	  PR_INSTANCE_KEY,
	  PR_SENDER_NAME,
      PR_SUBJECT,	
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };

   
	LPMAPITABLE lpUnknownContentsTable;
	HRESULT	hRes = lpUnknownFolder->GetContentsTable(
      0,
      &lpUnknownContentsTable);
    if (FAILED(hRes)) return;

	hRes = lpUnknownContentsTable->SetColumns(
			(LPSPropTagArray) &sptCols,
			NULL);//Flags

	 hRes = HrQueryAllRows(
      lpUnknownContentsTable,
      (LPSPropTagArray) &sptCols,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &MsgRows);

	if (SUCCEEDED(hRes))
	{
	
		for (int i=0 ; i < MsgRows->cRows; i ++)
		{
		
			//lpMAPIAdviseSink->ProcessMsg(&MsgRows->aRow[i]); 
			MoveMsgById(&MsgRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin,lpUnknownFolder,lpInboxFolder);
			
		}

		FreeProws(MsgRows);
	
	}

	UlRelease(lpUnknownContentsTable);
}

HRESULT CMapiEx::FindFolder(LPMAPIFOLDER ParentFolder ,LPMAPIFOLDER* ResultFolder , LPSTR FolderName)
{

	LPSRowSet pRows ; 	
	int i=0;

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
	    LPMAPITABLE mlpContentsTable ;
		HRESULT hRes = ParentFolder->GetHierarchyTable(0,&mlpContentsTable);
		if (FAILED(hRes)) return S_FALSE;
   
		//列出 query
		hRes = HrQueryAllRows(
			mlpContentsTable,
			(LPSPropTagArray) &sptCols,
			NULL,//restriction...we're not using this parameter
			NULL,//sort order...we're not using this parameter
			0,
			&pRows);

		if (FAILED(hRes))
		{
			
			mlpContentsTable->Release();
			return S_FALSE;		
		}
		

		hRes = S_FALSE;
		for ( i = 0; i < pRows -> cRows; i++)
		{
			if (pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].ulPropTag == PR_DISPLAY_NAME)
			{
			 

					//tmpStr.Format("%s",pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA);	   
				 
				    if (strcmp(pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA,FolderName) == 0)  
					{
						  LPMAPIFOLDER FindFolder; 
						  enum {     
							   ePR_ENTRYID,
							   ePR_DISPLAY_NAME,			
							  NUM_COLS};
		
							static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
								PR_ENTRYID,
								PR_DISPLAY_NAME	   
							};

							ULONG        ulObjType;
							lpMDB->OpenEntry(
							pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &FindFolder); //Returned folder

							*ResultFolder = FindFolder;
						//*mSRow = pRows->aRow[i];
						hRes = S_OK;
						break;

					}
					
			}
		}

	FreeProws(pRows); 
	mlpContentsTable->Release();

	return hRes;

}

  

void  CMapiEx::ReadBodyStream(LPSTREAM lpStream)
{
	HRESULT       hRes;	
	bool done=false;
	//unsigned int contentsize=0;
	//htmlsize = 0;
	//htmlsize 含 /0 的空間 , 當大小大於 0 時 , 需先減去 /0 ,
	//以便接入字串
    if (bodysize > 0) 
	{
		bodysize --;	
	}

					 
		while (!done)
		{ 
				ULONG readsize; 
				hRes = lpStream->Read(bodybuf+bodysize, bodybufsize-bodysize, &readsize);						  
						  
				if (hRes!=S_OK) 
				{
							 // *(*buf+htmlsize)=0;
							 // htmlbuf[htmlsize] = 0;
							  done=true;
				}
				else
				{ 
							  bodysize+=readsize; 
							  done =  (readsize < bodybufsize-bodysize);						

						      if (!done)
							  { 
								  unsigned int newsize=2*bodysize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,bodybuf,bodysize); 
								  //delete[] htmlbuf;
								  delete [] bodybuf;

								  bodybuf=newbuf; 
								  bodybufsize=newsize;
							  }
				 }
		}

				//	*(*buf+contentsize)=0;
				//	htmlbuf[htmlsize] = 0;


  //return contentsize;
}

void  CMapiEx::ReadStream(LPSTREAM lpStream)
{
	HRESULT       hRes;	
	bool done=false;
	//unsigned int contentsize=0;
	//htmlsize = 0;
	//htmlsize 含 /0 的空間 , 當大小大於 0 時 , 需先減去 /0 ,
	//以便接入字串
    if (htmlsize > 0) 
	{
		htmlsize --;	
	}

		//FILE *fp;
		////fp = fopen("c:\\1.txt","w+b");
		while (!done)
		{ 
				ULONG readsize; 
				hRes = lpStream->Read(htmlbuf+htmlsize, bufsize-htmlsize, &readsize);						  
						  
				if (hRes!=S_OK) 
				{
							 // *(*buf+htmlsize)=0;
							 // htmlbuf[htmlsize] = 0;
							  done=true;
				}
				else
				{ 
							  //fwrite(htmlbuf+htmlsize,1,readsize,fp);

							  htmlsize+=readsize; 
							  done =  (readsize < bufsize-htmlsize);						  

						      if (!done)
							  { 
								  unsigned int newsize=2*htmlsize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,htmlbuf,htmlsize); 
								  //delete[] htmlbuf;
								  delete [] htmlbuf;
								  htmlbuf=newbuf; 
								  bufsize=newsize;
							  }
				 }
		}

				//	*(*buf+contentsize)=0;
		htmlbuf[htmlsize] = 0;


		//fclose(fp);


		//OutputDebugString("test");
  //return contentsize;
}


void CMapiEx::ArrangeMail(LPMAPITABLE lpTable , LPMAPIFOLDER  lpFolder,CProgressCtrl *progress , SRow *mSRow)
{

	//check backup mail folder

	
  
	if (lpTable == NULL)
	{
		lpTable = lpContentsTable;
	
	}

	if (lpFolder == NULL)
	{
		lpFolder = lpInboxFolder;
	
	}

	LPSRowSet MsgRows ;
	//SRow mSRow;

	LPMAPIFOLDER  lpArrangeFolder = NULL;
	
	CMainFrm *frm = (CMainFrm *) AfxGetMainWnd();

	//AfxMessageBox("AfxGetMainWnd"); 
	

	if (FindFolder(lpInboxFolder,&lpArrangeFolder,frm->Settings.ArrangeMailFolder) == S_OK)
	{
		//有找到

		 /*enum {     
		   ePR_ENTRYID,
		   ePR_DISPLAY_NAME,			
		  NUM_COLS};
		
		static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
			PR_ENTRYID,
			PR_DISPLAY_NAME	   
		};

		ULONG        ulObjType;
		HRESULT hRes = lpMDB->OpenEntry(
				mSRow.lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
					(LPENTRYID) mSRow.lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
					NULL,                         //We want the default interface    (IMAPIFolder)
					MAPI_BEST_ACCESS,             //Flags
					&ulObjType,                   //Object returned type
					(LPUNKNOWN *) &lpArrangeFolder); //Returned folder*/
		
	}
	else
	{		//建目錄
		lpInboxFolder->CreateFolder(FOLDER_GENERIC,frm->Settings.ArrangeMailFolder,"我的整理郵件",NULL,OPEN_IF_EXISTS,&lpArrangeFolder);
	}
 
	//AfxMessageBox("FindFolder"); 

	enum {   
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,    
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,  
	  PR_INSTANCE_KEY, 
	  PR_SENDER_NAME,
      PR_SUBJECT,	
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };

  // AfxMessageBox("Before HrQueryAllRows"); 

	HRESULT hRes = HrQueryAllRows(
      lpTable,
      (LPSPropTagArray) &sptCols,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &MsgRows);

//	AfxMessageBox("HrQueryAllRows"); 

	if (SUCCEEDED(hRes))
	{
	
		//AfxMessageBox("HrQueryAllRows ok"); 

		if (progress != NULL)
			progress->SetRange(0, MsgRows->cRows);

		for (int i=0 ; i < MsgRows->cRows; i ++)
		{
		
			//lpMAPIAdviseSink->ProcessMsg(&MsgRows->aRow[i]);
			//取 sender string

			if ( (mSRow != NULL && 
				  mSRow->lpProps[ePR_ENTRYID].Value.bin.cb == MsgRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb &&
                  memcmp(mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb ,MsgRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,mSRow->lpProps[ePR_ENTRYID].Value.bin.cb) == 0) 
				  || (mSRow == NULL))
			{

				if (MsgRows->aRow[i].lpProps[ePR_SENDER_NAME].ulPropTag == PR_SENDER_NAME)
				{
					char *SenderName = MsgRows->aRow[i].lpProps[ePR_SENDER_NAME].Value.lpszA;		  
					//SRow mSRow;
					LPMAPIFOLDER  lpMoveFolder = NULL;

					if (FindFolder(lpArrangeFolder,&lpMoveFolder,SenderName) == S_OK)
					{
					//目錄存在
				  	//移動信件
					
						//AfxMessageBox("before move");

						MoveMsgById(&MsgRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin,lpFolder,lpMoveFolder);					 
						lpMoveFolder->Release();

						//AfxMessageBox("after move");

					

					}
					else
					{
						//建目錄
						LPMAPIFOLDER  lpMoveFolder = NULL;
						lpArrangeFolder->CreateFolder(FOLDER_GENERIC,SenderName,SenderName,NULL,OPEN_IF_EXISTS,&lpMoveFolder);

						if (lpMoveFolder != NULL)
						{
						//移動信件
							//AfxMessageBox("before move");
							MoveMsgById(&MsgRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin,lpFolder,lpMoveFolder);
							//AfxMessageBox("after move");
						}

						lpMoveFolder->Release();
					}
				}
			}
			if (progress != NULL)
				progress->SetPos(i+1);
			
		}

		FreeProws(MsgRows);
	
	}

	if (lpArrangeFolder != NULL)
		lpArrangeFolder->Release();

}



void CMapiEx::InBoxTableReload()
{
  
    enum {     
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,      
	  PR_INSTANCE_KEY,
	  PR_SENDER_NAME,
      PR_SUBJECT,	 
      PR_ENTRYID,
	  PR_RTF_IN_SYNC,
	  PR_BODY
   };

	HRESULT hRes = S_OK;

	MailKeyTable.RemoveAll();

	//列出 All Mail In InBox

	LPSRowSet mpRows ; 

	hRes = HrQueryAllRows(
      lpContentsTable,
      //(LPSPropTagArray) &sptCols,
	  NULL,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &mpRows);


	if (SUCCEEDED(hRes))
	{  

	    for (int i=0 ; i < mpRows->cRows; i ++)
		{
			
			MAPIMSGID msgid;			
			memset(&msgid,0,sizeof(msgid));
			memcpy(msgid.MsgId,mpRows->aRow[i].lpProps[ePR_INSTANCE_KEY].Value.bin.lpb,mpRows->aRow[i].lpProps[ePR_INSTANCE_KEY].Value.bin.cb);			

			MailKeyTable.AddHead(msgid);
				
		}


		 FreeProws(mpRows);

	}

	
    
	


}