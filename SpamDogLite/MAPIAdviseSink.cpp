// MAPIAdviseSink.cpp: implementation of the CMAPIAdviseSink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpamDogLite.h"
#include "MainFrm.h"
#include "MsgFrm.h"
#include "MAPIAdviseSink.h"

#include "../Swmatcher/BmMatch.h"
#include "../Swsocket/CoreClass.h"

#include "MapiEx.h"
#include <process.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

STDMETHODIMP CMAPIAdviseSink::QueryInterface (REFIID riid,
													LPVOID * ppvObj)
{
	*ppvObj = 0;
	if (riid == IID_IMAPIAdviseSink ||
		riid == IID_IUnknown)
	{
		*ppvObj = (LPVOID)this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CMAPIAdviseSink::AddRef() 
{
	//OutputDebugString("CFolderDlg::AddRef\n");
	LONG lCount = InterlockedIncrement(&m_cRef);
	//sprintf(szString,"CMAPITableAdviseSink.m_cRef increased to %d.\n",lCount);
	//OutputDebugString(szString);
	return lCount; 
}

STDMETHODIMP_(ULONG) CMAPIAdviseSink::Release() 
{
	//OutputDebugString("CFolderDlg::Release\n");
	LONG lCount = InterlockedDecrement(&m_cRef);	
	//sprintf(szString,"CMAPITableAdviseSink.m_cRef decreased to %d.\n",lCount);
	//OutputDebugString(szString);
	if (!lCount)  
	{
		delete this; 
	}
	return lCount;
}

CMAPIAdviseSink::~CMAPIAdviseSink () {
	
}

CMAPIAdviseSink::CMAPIAdviseSink (char Type ) 
{
	//mMapiNotify = MapiNotify;
	//mMainStatic = MainStatic;
	mtype = Type;
	//LastMailCount = 0;
	//bReload = false;

	//1 : InBoxFolder
	//2 : learn-ok
	//3 : learn-spam
	//4 : OutBox

		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

	
		HINSTANCE hInstance = GetModuleHandle(NULL);
		GetModuleFileName(hInstance, path_buffer, MAX_PATH);

		_splitpath( path_buffer, drive, dir, fname, ext );

		strcpy(WorkPath,drive);
		strcat(WorkPath,dir);	   


	 

}



STDMETHODIMP_(ULONG) CMAPIAdviseSink::OnNotify (ULONG cNotify,
													  LPNOTIFICATION lpNotifications)
{
  	HRESULT			hRes = S_OK;
	int				iPriorRow = 0;
	int				iCurrentRow = 0;
	ULONG			i = 0 ;
	int				iCurrentSelection = -1;
	int k = 0;
	bool bFound = false; 

	//LPSTR m_s  = NULL;
	//LPSTR text_s  = NULL;
	//LPSTR subject = NULL;
	//LPSTR sendername  = NULL;

	LPSRowSet		pRows = NULL;
	
	POSITION pos = NULL;
	POSITION mpos = NULL;
	
	MAPIMSGID msgid;
	//char TempMem[100];

	/*LPMESSAGE lpMessage = NULL;
	ULONG ulObjType = NULL;
	char			lpszSubject[512];
	LPSPropValue	lpval = NULL;
	SPropValue		spvSubjectProp = {0};*/


enum {      
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
  


	for (i=0;i<cNotify;i++)
	{
		 
		 
		if (fnevTableModified == lpNotifications[i].ulEventType)
		{
			switch(lpNotifications[i].info.tab.ulTableEvent)
			{
			case(TABLE_RESTRICT_DONE):
				OutputDebugString("TABLE_RESTRICT_DONE\n");
				break;
				//may not deal with this
			case(TABLE_SETCOL_DONE):
				//may not deal...reload?
				OutputDebugString("TABLE_SETCOL_DONE\n");
				break;
			case(TABLE_SORT_DONE):
				//may not deal...reload?
				OutputDebugString("TABLE_SORT_DONE\n");
				break;
			case(TABLE_ERROR):
				//probably should report the error, then reload the table
				OutputDebugString("TABLE_ERROR\n");
				break;
			case(TABLE_CHANGED):
				OutputDebugString("TABLE_CHANGED\n");
				//break;
			case(TABLE_RELOAD):
				OutputDebugString("TABLE_RELOAD\n");
				 

			   

				if (mtype == INBOX_F)
				{
						
					//�����ثe�ʼ�
					//unsigned long Nowcount = 0;
					//hRes = mMapiNotify->lpContentsTable->GetRowCount (
					//				0,
					//				&Nowcount);

					//�D�X�t��
					//int MailIdx = Nowcount - LastMailCount;
					//if (MailIdx > 0)
					//{

						hRes = g_MapiEx->lpContentsTable->SeekRow(
						BOOKMARK_BEGINNING,
						0,
						NULL);

						if (hRes != S_OK) 
						{
							break;						
						}

						for(;;)
						{
							hRes = g_MapiEx->lpContentsTable->QueryRows(
									1,
									NULL,
									&pRows);
							
							if (hRes != S_OK) 
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}
							if (!pRows->cRows)
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}

							for(int j = 0 ; j < pRows->cRows ; j++)
							{							
								//���T�w KeyTable
								bFound = false;
								mpos = g_MapiEx->MailKeyTable.GetHeadPosition();
								for ( k = 0 ; k < g_MapiEx->MailKeyTable.GetCount() ; k ++)
								{
					 				if (mpos == NULL) break;
									MAPIMSGID temp = g_MapiEx->MailKeyTable.GetNext(mpos);

													

									if (!memcmp(&temp, pRows->aRow[j].lpProps[ePR_INSTANCE_KEY].Value.bin.lpb,pRows->aRow[j].lpProps[ePR_INSTANCE_KEY].Value.bin.cb))
									{						
										bFound = true;
										OutputDebugString("TABLE_RELOAD : TABLE_FIND_INLIST\n");
									}			 

								}

								if (!bFound)
									ProcessMsg(&pRows->aRow[j]);

 
								
							}
		
							FreeProws(pRows);
							pRows = NULL;
							//i++;
						}
					//}
						
						g_MapiEx->InBoxTableReload();
				}
				else if (mtype == LEARNOKBOX_F)
				{	
						hRes = g_MapiEx->lpLearnOkContentsTable->SeekRow(
						BOOKMARK_BEGINNING,
						0,
						NULL);

						if (hRes != S_OK) 
						{
							break;						
						}

						for(;;)
						{
							hRes = g_MapiEx->lpLearnOkContentsTable->QueryRows(
									1,
									NULL,
									&pRows);
							
							if (hRes != S_OK) 
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}
							if (!pRows->cRows)
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}

							for(int j = 0 ; j < pRows->cRows ; j++)
							{							
								ProcessMsg(&pRows->aRow[j]);							
							}
		
							FreeProws(pRows);
							pRows = NULL;
							//i++;
						}
										
				}
				else if (mtype == LEARNSPAMBOX_F)
				{
						hRes = g_MapiEx->lpLearnSpamContentsTable->SeekRow(
						BOOKMARK_BEGINNING,
						0,
						NULL);

						if (hRes != S_OK) 
						{
							break;						
						}

						for(;;)
						{
							hRes = g_MapiEx->lpLearnSpamContentsTable->QueryRows(
									1,
									NULL,
									&pRows);
							
							if (hRes != S_OK) 
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}
							if (!pRows->cRows)
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}

							for(int j = 0 ; j < pRows->cRows ; j++)
							{							
								ProcessMsg(&pRows->aRow[j]);							
							}
		
							FreeProws(pRows);
							pRows = NULL;
							//i++;
						}				
										
				} 
				
				/*********************************************/
				/* HrqueryAllRow �|Ĳ�ʦ��ƥ� . in outlook 2000*/

				else if (mtype == LEARNWHITEBOX_F)
				{
						hRes = g_MapiEx->lpWhiteContentsTable->SeekRow(
						BOOKMARK_BEGINNING,
						0,
						NULL);

						if (hRes != S_OK) 
						{
							break;						
						}

						for(;;)
						{
							hRes = g_MapiEx->lpWhiteContentsTable->QueryRows(
									1,
									NULL,
									&pRows);
							
							if (hRes != S_OK) 
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}
							if (!pRows->cRows)
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}

							for(int j = 0 ; j < pRows->cRows ; j++)
							{							
								ProcessWhiteMsg(&pRows->aRow[j]);							
							}
		
							FreeProws(pRows);
							pRows = NULL;
							//i++;
						}				
										
				}
				
				
				/*else if (mtype == 4)
				{
				
						hRes = g_MapiEx->lpOutContentsTable->SeekRow(
						BOOKMARK_BEGINNING,
						0,
						NULL);

						if (hRes != S_OK) 
						{
							break;						
						}

						for(;;)
						{
							hRes = g_MapiEx->lpOutContentsTable->QueryRows(
									1,
									NULL,
									&pRows);
							
							if (hRes != S_OK) 
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}
							if (!pRows->cRows)
							{
								FreeProws(pRows);
								pRows = NULL;
								break;
							}

							for(int j = 0 ; j < pRows->cRows ; j++)
							{							
								//ProcessMsg(pRows->aRow);							
								ProcessOutMsg(pRows->aRow);							
							}
		
							FreeProws(pRows);
							pRows = NULL;
							//i++;
						}				
										
				
				}*/

			
				//m_lpMessageListCtrl->ClearMessages();
				//m_lpMessageListCtrl->LoadMessages();
				break;
			case(TABLE_ROW_ADDED):
				//figure out what row was added and add it in
				//bReload = false;
				
				OutputDebugString("TABLE_ROW_ADDED\n");	

				memset(&msgid,0,sizeof(msgid));
			    memcpy(msgid.MsgId,lpNotifications[i].info.tab.propIndex.Value.bin.lpb,lpNotifications[i].info.tab.propIndex.Value.bin.cb);			

			    g_MapiEx->MailKeyTable.AddHead(msgid);

				
				
				
				//memset(TempMem,0,100);
				//memcpy(TempMem,lpNotifications[i].info.tab.propIndex.Value.bin.lpb,lpNotifications[i].info.tab.propIndex.Value.bin.cb);			

				//g_MapiEx->MailKeyTable.AddHead(TempMem);
 

				//g_MapiEx->MailKeyTable.AddHead(&lpNotifications[i].info.tab.propIndex.Value.bin);

				//msgid = g_MapiEx->GetMsgId(&lpNotifications[i].info.tab.row);

				//if (msgid.GetLength() > 0)
				//{				
				//	g_MapiEx->MailKeyTable.AddHead(msgid);
				
				//}


				//if (PR_NULL == lpNotifications[i].info.tab.propPrior.ulPropTag)
				//{
				//	iPriorRow = -1;
				//}
				//else
				//{
				//mMapiNotify->MoveMsgById(&lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin,mMapiNotify->lpInboxFolder,mMapiNotify->lpUnknownFolder);

				/*
					hRes = mMapiNotify->lpMDB->OpenEntry(
					lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.cb,
					(LPENTRYID) lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.lpb,
					NULL,//default interface
					MAPI_BEST_ACCESS,
					&ulObjType,
					(LPUNKNOWN *) &lpMessage);
				
				 if (!FAILED(hRes))
				 {

					 // Get the Subject of the message	
					hRes = HrGetOneProp ((LPMAPIPROP)lpMessage, PR_SUBJECT, &lpval);
					if(FAILED(hRes)) break;
	
					// Copy the string into our buffer
					strcpy(lpszSubject, lpval->Value.LPSZ);

					if (strstr(lpszSubject,"[SPAM CHECKED]") == NULL)
					{	
						// Append our tag
						strcat(lpszSubject, " [SPAM CHECKED]");

						// Save the new subject out to the message
						spvSubjectProp.ulPropTag = PR_SUBJECT;
						spvSubjectProp.Value.lpszA = lpszSubject;

						// Set the news Subject to the message
						hRes = HrSetOneProp(lpMessage, &spvSubjectProp);	
						if(FAILED(hRes)) break;

						// Save the changes to the object
						hRes = lpMessage->SaveChanges(NULL);
						if(FAILED(hRes)) break;
					}
				 
				 }

*/
				
				if (mtype == INBOX_F || mtype == LEARNOKBOX_F || mtype == LEARNSPAMBOX_F)
				{
				  ProcessMsg(&lpNotifications[i].info.tab.row);
				}else if (mtype == OUTBOX_F)
				{
				  ProcessOutMsg(&lpNotifications[i].info.tab.row);
				}else if (mtype == LEARNWHITEBOX_F)
				{
				  ProcessWhiteMsg(&lpNotifications[i].info.tab.row);
				}



			 
				/*if (mtype == 1)
				{
				
					 
					hRes = mMapiNotify->lpContentsTable->GetRowCount (
									0,
									&LastMailCount);
									
					
				}*/
					 
				//}
				//hRes = m_lpMessageListCtrl->AddItemToListBox(
				//	iPriorRow+1,
				//	&lpNotifications[i].info.tab.row);
				//CHECKHRES(hRes);
				break;
			case(TABLE_ROW_DELETED):
				//figure out what row was deleted and cut it from our list
				OutputDebugString("TABLE_ROW_DELETED\n");
			
			    mpos = g_MapiEx->MailKeyTable.GetHeadPosition();
				for ( k = 0 ; k < g_MapiEx->MailKeyTable.GetCount() ; k ++)
				{
					if (mpos == NULL) break;
					MAPIMSGID temp = g_MapiEx->MailKeyTable.GetNext(mpos);

					if (!memcmp(&temp, lpNotifications[i].info.tab.propIndex.Value.bin.lpb, lpNotifications[i].info.tab.propIndex.Value.bin.cb))
					{						
						g_MapiEx->MailKeyTable.RemoveAt(g_MapiEx->MailKeyTable.FindIndex(k));
						OutputDebugString("TABLE_ROW_DELETED : TABLE_FIND_INLIST\n");
						break;
					}			 

				}

				//pos = g_MapiEx->MailKeyTable.Find(msgid);
				//if (pos != NULL)
						 //g_MapiEx->MailKeyTable.RemoveAt(pos);

			

				//msgid = g_MapiEx->GetMsgId(lpNotifications[i].info.tab.propIndex.Value.bin.lpb,lpNotifications[i].info.tab.propIndex.Value.bin.cb);

				//if (msgid.GetLength() > 0)
				//{				
					//POSITION pos = g_MapiEx->MailKeyTable.Find(msgid);
					//if (pos != NULL)
						 //g_MapiEx->MailKeyTable.RemoveAt(pos);
				
				//}

				//iCurrentSelection = m_lpMessageListCtrl->GetNextItem(-1,LVIS_SELECTED|LVIS_FOCUSED);
				//hRes = m_lpMessageListCtrl->GetIndexFromInstanceKey(
				//	&lpNotifications[i].info.tab.propIndex.Value.bin,
				//	&iCurrentRow);
				//CHECKHRES(hRes);
				//hRes = m_lpMessageListCtrl->DeleteItemFromListBox(
				//	iCurrentRow);
				//CHECKHRES(hRes);
				//if (iCurrentSelection == iCurrentRow)
				//	m_lpMessageListCtrl->SetSelectedItem(iCurrentSelection);

				/*if (mtype == 1)
				{
				
					 
					hRes = mMapiNotify->lpContentsTable->GetRowCount (
									0,
									&LastMailCount);
									
					
				}*/

				break;
			case(TABLE_ROW_MODIFIED):
				//figure out what row was modified and modify it
				OutputDebugString("TABLE_ROW_MODIFIED\n");
				//iCurrentSelection = m_lpMessageListCtrl->GetNextItem(-1,LVIS_SELECTED|LVIS_FOCUSED);
				//hRes = m_lpMessageListCtrl->GetIndexFromInstanceKey(
				//	&lpNotifications[i].info.tab.propIndex.Value.bin,
				//	&iCurrentRow);
				//CHECKHRES(hRes);
				//hRes = m_lpMessageListCtrl->DeleteItemFromListBox(
				//	iCurrentRow);
				//CHECKHRES(hRes);
				//hRes = m_lpMessageListCtrl->AddItemToListBox(
				//	iCurrentRow,
				//	&lpNotifications[i].info.tab.row);
				//CHECKHRES(hRes);
				//if (iCurrentSelection == iCurrentRow)
				//	m_lpMessageListCtrl->SetSelectedItem(iCurrentSelection);
				break;
			}
		}
		 
	}
	return 0;
}

void CMAPIAdviseSink::ProcessOutMsg(SRow *mSRow)
{
	CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();
	if (!dlg->Settings.MailTracer) return;
/*

enum {      
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};

	  //����H�H
	  g_MapiEx->lpMDB->AbortSubmit(mSRow->lpProps[ePR_ENTRYID].Value.bin.cb,
	   (LPENTRYID) mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb,0);

	  //g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpOutboxFolder,g_MapiEx->lpUnknownFolder);

	  

	  LPMESSAGE lpMessage = NULL;
	  ULONG ulObjType = NULL;

	  HRESULT hRes = NULL;

		hRes = g_MapiEx->lpMDB->OpenEntry(
		mSRow->lpProps[ePR_ENTRYID].Value.bin.cb,
		(LPENTRYID) mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb,
		NULL,//default interface
		MAPI_BEST_ACCESS,
		&ulObjType,
		(LPUNKNOWN *) &lpMessage);
		
	  if (!FAILED(hRes))
	  {
		  //g_MapiEx->lpMDB->SetLockState(lpMessage,MSG_LOCKED); 	
		  
		  SPropValue    spvAttach[2];

			spvAttach[0].ulPropTag = PR_SUBJECT;
			spvAttach[0].Value.lpszA = "�Q���!" ;

			//spvAttach[1].ulPropTag = PR_MESSAGE_FLAGS; 
		    //spvAttach[1].Value.l = MSGFLAG_UNSENT ; 


			
			hRes = lpMessage->SetProps(
					1,
					(LPSPropValue)&spvAttach,
						NULL);

			if (SUCCEEDED(hRes))
			{
				//hRes = lpMessage->SaveChanges(0);
				//lpMessage->SaveChanges(KEEP_OPEN_READWRITE);
				 
			 
			    //g_MapiEx->lpOutboxFolder->SaveChanges(0);
				
				
				
				hRes = lpMessage->SubmitMessage (0);
			
			}


		  
		lpMessage->Release();
			
	  }

  */


	  

// Original code 
enum {      
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
	  
	  //���D��
	  char *Subject=NULL;
	  char *Sender=NULL;


	  if (mSRow->lpProps[ePR_SUBJECT].ulPropTag == PR_SUBJECT)
	  {
		  Subject = mSRow->lpProps[ePR_SUBJECT].Value.lpszA;

		   if (strlen(Subject) == 0) return;

		  if (strnicmp(Subject,"RE: ",4) == 0 )
		  {
				strcpy(Subject,Subject+4);
		  } 
		  else if (strnicmp(Subject,"FW: ",4) == 0 )
		  {
				return;
		  }
	  }
	  //���H���

	  if (mSRow->lpProps[ePR_SENDER_NAME].ulPropTag == PR_SENDER_NAME)
	  {
		  Sender = mSRow->lpProps[ePR_SENDER_NAME].Value.lpszA;		 
		  if (strlen(Sender) == 0) return;
	  }

	  

	  	char RulePath[_MAX_PATH]; 
		strcpy(RulePath,WorkPath);		 
		strcat(RulePath,"Rule.dat");

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		bool FoundFile=false;

		hFind = FindFirstFile(RulePath,&FindFileData);
 		if (hFind != INVALID_HANDLE_VALUE) 
		{
			FoundFile = true;
		}

		FindClose(hFind);

		FILE *fp =  NULL;
		RuleHeader rulehead;
		memset(&rulehead,0,sizeof(RuleHeader));
		
		if (FoundFile)
		{
			fp = fopen(RulePath,"r+b"); 
			fread(&rulehead,1,sizeof(rulehead),fp);
		}
		else
		{
		    fp = fopen(RulePath,"w+b"); 
		}
 
			RuleFile m_RuleFile;
			memset(&m_RuleFile,0,sizeof(RuleFile));

			//strcpy(m_RuleFile.Rule,"RE: ");
			strcat(m_RuleFile.Rule,Subject);

			strcpy(m_RuleFile.Rule2,Sender); 
		 
		//�ˬd����
		RuleFile old_m_RuleFile;
		while(fread(&old_m_RuleFile,1,sizeof(RuleFile),fp))
		{
			if (strcmp(old_m_RuleFile.Rule,m_RuleFile.Rule) == 0)
			{
			   if (old_m_RuleFile.Used) //������
			   {
				//����
				fclose(fp); 
				return;
			   }


			}
		}

			
		
			unsigned int newidx = rulehead.LastId + 1;
			rulehead.LastId = newidx;

			//update ��sidx
			fseek(fp,0,SEEK_SET);
			fwrite(&rulehead,1,sizeof(rulehead),fp);
			fseek(fp,0,SEEK_END);

		 
		

			//strcpy(m_RuleFile.Rule,Subject);
			m_RuleFile.AlertType = 2;//�S�O
			m_RuleFile.CheckType = 2;//�̥D��
			m_RuleFile.ShowColor = 16744448;
			m_RuleFile.RuleType = 1; //�l�ܾ�
			m_RuleFile.RuleId = newidx;
			m_RuleFile.Used = true;

			//���Ĵ��� , �C��
			CTime t1((time_t) CTime::GetCurrentTime().GetTime());
			CTimeSpan ts1( dlg->Settings.TracerExpire, 0, 0, 0);    
			CTime t2(t1+ts1);

			m_RuleFile.ExpireTime = (long) t2.GetTime();
		

			//save to file
			fwrite(&m_RuleFile,1,sizeof(RuleFile),fp);

	

			fclose(fp);


		
		dlg->GetIniRule(); 
}

void CMAPIAdviseSink::ProcessWhiteMsg(SRow *mSRow)
{
enum {      
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};

	LPSTR sendername  = NULL;   
	if (mSRow->lpProps[ePR_SENDER_NAME].ulPropTag == PR_SENDER_NAME)
	{
		  sendername = mSRow->lpProps[ePR_SENDER_NAME].Value.lpszA;		  
	}

	CMainFrm *pobj = (CMainFrm *) AfxGetMainWnd();
		//�[�J rule
				char RulePath[_MAX_PATH]; 
				strcpy(RulePath,WorkPath);		 
				strcat(RulePath,"Rule.dat");

				WIN32_FIND_DATA FindFileData;
				HANDLE hFind;
				bool FoundFile=false;

				hFind = FindFirstFile(RulePath,&FindFileData);
 				if (hFind != INVALID_HANDLE_VALUE) 
				{
					FoundFile = true;
				}

				FindClose(hFind);

				FILE *fp =  NULL;
				RuleHeader rulehead;
				memset(&rulehead,0,sizeof(RuleHeader));
		
				if (FoundFile)
				{
					fp = fopen(RulePath,"r+b"); 
					fread(&rulehead,1,sizeof(rulehead),fp);
				}
				else
				{
					fp = fopen(RulePath,"w+b");
					fseek(fp,0,SEEK_SET);
					fwrite(&rulehead,1,sizeof(rulehead),fp);
					fseek(fp,0,SEEK_END);
				}

				RuleFile m_RuleFile;
				memset(&m_RuleFile,0,sizeof(RuleFile));			 
				strcat(m_RuleFile.Rule,sendername);	


				//�ˬd����
				bool bDoubleAdd = false;
				RuleFile old_m_RuleFile;
				while(fread(&old_m_RuleFile,1,sizeof(RuleFile),fp))
				{
					if (strcmp(old_m_RuleFile.Rule,m_RuleFile.Rule) == 0)
					{
						//����
						if (old_m_RuleFile.Used) //������
						{
							//fclose(fp);
							//imsg->Release();
							//delete TotalMailBuff;
							//�H�󲾦^
						    //MoveMsg(MsgId,m_pLearnWhiteBoxStoreFolder , m_pInBoxStoreFolder);

							//dlg->m_MainStatic.SetWindowText("Express �ʱ���..");
							//dlg->ChangeIcon(0);
							bDoubleAdd = true;
							break;

							//return;
						}
						
					}
				}
							
	 

					//unsigned int newidx = rulehead.LastId + 1;
					//rulehead.LastId = newidx;

					//update ��sidx
					//fseek(fp,0,SEEK_SET);
					//fwrite(&rulehead,1,sizeof(rulehead),fp);
						m_RuleFile.AlertType = 1;//���`
						m_RuleFile.CheckType = 1;//�H���
						m_RuleFile.ShowColor = g_MapiEx->SysOkMailColor;;
						m_RuleFile.RuleType = 2; //�զW��
						//m_RuleFile.RuleId = newidx;
						m_RuleFile.Used = true;
						


					

                   //�H�󲾰�		
					//AfxMessageBox("Arrange before");
					//return;

					//g_MapiEx->ArrangeMail(g_MapiEx->lpWhiteContentsTable,g_MapiEx->lpWhiteFolder , NULL , mSRow);
				
					

					if (!bDoubleAdd)
					{ 
						
						/*LPMAPIFOLDER lpArrangeFolder;
						if (g_MapiEx->FindFolder(g_MapiEx->lpInboxFolder,&lpArrangeFolder,pobj->Settings.ArrangeMailFolder) == S_OK)
						{
							LPMAPIFOLDER lpTargetFolder;
							if (g_MapiEx->FindFolder(lpArrangeFolder,&lpTargetFolder,sendername) == S_OK)
							{
								  strcpy(m_RuleFile.ListData.FolderName, sendername);
								  
								  LPSPropValue   lpPropss = NULL;
								  HRESULT hRes = HrGetOneProp(lpTargetFolder,PR_ENTRYID ,&lpPropss);

								  if (SUCCEEDED(hRes))
								  {
								  
									  
									  m_RuleFile.ListData.olFolder.FolderLen = lpPropss->Value.bin.cb;
									  memcpy(m_RuleFile.ListData.olFolder.FolderId,lpPropss->Value.bin.lpb,m_RuleFile.ListData.olFolder.FolderLen );
									  

									  fseek(fp,0,SEEK_END); 
									  //save to file
							          fwrite(&m_RuleFile,1,sizeof(RuleFile),fp); 

								  }
								  
								  
							}

						
						}*/
				
						fseek(fp,0,SEEK_END); 
									  //save to file
					    fwrite(&m_RuleFile,1,sizeof(RuleFile),fp);


					}
				 

					fclose(fp);

				    g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpWhiteFolder,g_MapiEx->lpInboxFolder);


					CString tmpstr = sendername;
					tmpstr += " �w�[�J�զW��!";											

					ShowMsg(m_RuleFile.ShowColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()),m_RuleFile.AlertType);					
					tmpstr.ReleaseBuffer();		
					
					
					pobj->GetIniRule();

					return;
}

/*
void CMAPIAdviseSink::ProcessLearnMsg(SRow *mSRow , char LearnType)
{
	LPSTR m_s  = NULL;
	LPSTR text_s  = NULL;
	LPSTR subject = NULL;
	LPSTR sendername  = NULL; 

enum {      
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};	   

	  char IP[16];
	  IP[0] = 0;
	  char ReceiveTime[255];
	  ReceiveTime[0] = 0;

	  //char* TotalMailBuff = NULL;
	 

	  char MsgId[25];

	  CChecker mc;

	  m_s = NULL;
	  subject = NULL;
	  sendername = NULL;
	  text_s = NULL;

	  CMainFrm *pobj;
				    
	  pobj = (CMainFrm *) AfxGetMainWnd();
	  CMsgFrm *msgobj = &pobj->MsgFrm;

	  int AttCount = 0;						
	  HRESULT hRes = g_MapiEx->GetRowMsgItem(mSRow,&m_s,&text_s,&subject,&sendername,&AttCount);


	  LPMESSAGE lpMessage = NULL;
	  ULONG ulObjType = NULL;

	  HRESULT hRes2 = g_MapiEx->lpMDB->OpenEntry(
				mSRow->lpProps[ePR_ENTRYID].Value.bin.cb,
				(LPENTRYID) mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb,
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

						CString msgidstr;
						msgidstr.Format("%02d%02d%02d%02d%02d%02d%04d",
										SystemCreate.wYear,
										SystemCreate.wMonth,
										SystemCreate.wDay,
										SystemCreate.wHour,
										SystemCreate.wMinute,
										SystemCreate.wSecond,
										SystemCreate.wMilliseconds); 

						strcpy(MsgId,msgidstr.GetBuffer(msgidstr.GetLength()));
						msgidstr.ReleaseBuffer();
					
					 
						LPSPropValue   lpProps = NULL;
						hRes2 = HrGetOneProp(lpMessage,PR_TRANSPORT_MESSAGE_HEADERS,&lpProps);

						if (SUCCEEDED(hRes2))
						{
							
							//Save Data to Buffer TotalMailBuff

							//int contentsize = strlen(m_s);
							//int headersize = strlen(lpProps->Value.lpszA);
							

							//TotalMailBuff = new char[headersize + contentsize + 1];
							//strcpy(TotalMailBuff,lpProps->Value.lpszA);
							//strcat(TotalMailBuff,m_s);
 
								char IniPath[_MAX_PATH];	   			   

								strcpy(IniPath,WorkPath);										   
								strcat(IniPath,"SpamDog.ini");

								if (pobj->Settings.IPident[0] == 0)
								{
									//mc.GetLastReceiveBy(buffer,dlg->Settings.IPident);
									mc.GetHeaderIP(lpProps->Value.lpszA,pobj->Settings.IPident,IP,ReceiveTime); 
								
									//Save To iniFile
									if (pobj->Settings.IPident[0] != 0)
									{
										 

										strcat(pobj->Settings.IPident,"|");
										WritePrivateProfileString("SysSetup","IPident",pobj->Settings.IPident,IniPath);

										//mc.GetHeaderIP(buffer,dlg->Settings.IPident,IP,ReceiveTime); //�A���@��
									}
									
								}
								else
								{
								    char TempIdent[255];
									strcpy(TempIdent, pobj->Settings.IPident);

									if (mc.GetHeaderIP(lpProps->Value.lpszA,TempIdent,IP,ReceiveTime) == 0) 
									{
									  //�S��� ident
										if (TempIdent[0] != 0)
										{
											AfxMessageBox(subject);
											AfxMessageBox(TempIdent);
											
											strcat(pobj->Settings.IPident,TempIdent);
											strcat(pobj->Settings.IPident,"|");

											WritePrivateProfileString("SysSetup","IPident",pobj->Settings.IPident,IniPath);

											
										}

									}
  
									
								}							

						}
						
						
		lpMessage->Release();

		mc.GetCheckKeys(m_s,text_s,sendername,subject,IP,pobj->Settings.SpamSubject);							
		if (mc.ResList.size() > 0 || mc.SecResList.size() > 0)
		{

			mc.GetSpamData();
			if (mc.DataList.size()  > 0 || mc.SecDataList.size()  > 0)
			{

				if (LearnType == 2) // learn ok 
				{				
					mc.UpdateSpamRate(1,0);
				} 
				else if (LearnType == 1) //learn spam
				{
				
					mc.UpdateSpamRate(0,1);
				}
					
			}

		}
	}

		 


}*/
void CMAPIAdviseSink::ProcessMsg(SRow *mSRow)
{
	LPSTR m_s  = NULL;
	LPSTR text_s  = NULL;
	LPSTR subject = NULL;
	LPSTR sendername  = NULL;
	
	bool LocalMail = false;
	bool SysCheckMail = false;
	bool IgCharsetMail = false;

										
    char sCharset[50];									


	MailPath m_MailPath;
	m_MailPath.NextWrite = -2;

enum {      
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};	   

	  char IP[16];
	  IP[0] = 0;
	  char ReceiveTime[255];
	  ReceiveTime[0] = 0;
	  long MsgSize = 0;

	  char* TotalMailBuff = NULL;
	 

	  

	  char MsgId[25];
	  MsgId[0] = 0;

	 // unsigned ThreadId;
	  

				    m_s = NULL;
					subject = NULL;
					sendername = NULL;
					text_s = NULL;

					CString tmpstr;
					CMainFrm *pobj;
				    
					pobj = (CMainFrm *) AfxGetMainWnd();
					CMsgFrm *msgobj = &pobj->MsgFrm;

					//EnterCriticalSection(&pobj->m_CritSec);
					CChecker *mc = new CChecker(&pobj->m_DBSec);

				 //if (!pobj->Settings.Licensed )
				//	pobj->Settings.ShowAD = true;
					//if (pobj->Settings.ADText != 0)
					//	pobj->ShowToolTip(pobj->Settings.ADTitle,pobj->Settings.ADText, 10 * 1000);

			

					//�B�z waitting msg dump
					MSG msg;
					while (::PeekMessage(&msg, pobj->m_hWnd,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					while (::PeekMessage(&msg, msgobj->m_hWnd,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					pobj->Settings.LastProcessTime = CTime::GetCurrentTime();

					 //update LastProcessTime , �� LiveUpdate �Ѧ�
					 //�p�G > 2 min �~��s

					pobj->ChangeIcon(1);

					
					pobj->m_MainStatic.SetWindowText("�H���ˬd��..");

					HRESULT hRes = S_FALSE;
					HRESULT HasCheched = 0;

					COLORREF OkMailColor = g_MapiEx->SysOkMailColor;
					COLORREF SpamMailColor = g_MapiEx->SpamMailColor;
					COLORREF UnknownMailColor = g_MapiEx->UnknownColor;
					COLORREF VirusMailColor = g_MapiEx->VirusColor;

					int AttCount = 0;						
					hRes = g_MapiEx->GetRowMsgItem(mSRow,&m_s,&text_s,&subject,&sendername,&AttCount);

					//���ݭn�٧U�~�i�H���� ipsource ident					
					if (!pobj->Settings.Licensed || pobj->Settings.IpSourceIdent || pobj->Settings.AuthEx )
					{

							LPMESSAGE lpMessage = NULL;
							ULONG ulObjType = NULL;

						

							HRESULT hRes2 = g_MapiEx->lpMDB->OpenEntry(
									mSRow->lpProps[ePR_ENTRYID].Value.bin.cb,
									(LPENTRYID) mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb,
									NULL,//default interface
									MAPI_BEST_ACCESS,
									&ulObjType,
									(LPUNKNOWN *) &lpMessage);

							if (SUCCEEDED(hRes2))
							{
								
								if ((!pobj->Settings.Licensed || (pobj->Settings.IpSourceIdent && pobj->Settings.NeedReport)) )
								{
								
									LPSPropValue   lpPropss = NULL;
									hRes2 = HrGetOneProp(lpMessage,PR_CREATION_TIME ,&lpPropss);

									SYSTEMTIME SystemCreate;
									FileTimeToSystemTime(&lpPropss->Value.ft,&SystemCreate);

									MAPIFreeBuffer(lpPropss);	

									LPSPropValue   lpPropss2 = NULL;
									hRes2 = HrGetOneProp(lpMessage,PR_MESSAGE_SIZE ,&lpPropss2);

									MsgSize = lpPropss2->Value.l;

									CString msgidstr;
									msgidstr.Format("%02d%02d%02d%02d%02d%02d%04d%4d",
													SystemCreate.wYear,
													SystemCreate.wMonth,
													SystemCreate.wDay,
													SystemCreate.wHour,
													SystemCreate.wMinute,
													SystemCreate.wSecond,
													SystemCreate.wMilliseconds,
													lpPropss2->Value.l); 

									strcpy(MsgId,msgidstr.GetBuffer(msgidstr.GetLength()));
									msgidstr.ReleaseBuffer();

									MAPIFreeBuffer(lpPropss2);	
								}

							
								
							
								LPSPropValue   lpProps = NULL;
								hRes2 = HrGetOneProp(lpMessage,PR_TRANSPORT_MESSAGE_HEADERS,&lpProps);

								if (SUCCEEDED(hRes2))
								{
								
								
									if (pobj->Settings.Licensed && pobj->Settings.AuthEx && !pobj->Settings.IpSourceIdent )
									{
										if (mc->CheckLocalMail(lpProps->Value.lpszA))
										   LocalMail = true;
									}

									if (pobj->Settings.RBLMaintain)
									{

												if (pobj->Settings.HeaderKeyStr[0] != 0)
												{
												
													mc->GetHeaderKeyStrIP(lpProps->Value.lpszA,pobj->Settings.HeaderKeyStr,IP);

												} else if (pobj->Settings.HeaderKeyPos > 0)
												{
												
													mc->GetKeyPosIP(lpProps->Value.lpszA,pobj->Settings.HeaderKeyPos,IP);
												
												}								
											
											
									}

									//Chatset Check								 

									
									if (mc->GetMailCharset(lpProps->Value.lpszA,sCharset) == true)
									{
										StrList::iterator itor; 
										std::string tmpstdstr;

										for (itor =  pobj->CharsetIgList.begin(); itor != pobj->CharsetIgList.end() ; itor++)
										{
											tmpstdstr = *itor;										

											if (stricmp(sCharset,tmpstdstr.c_str()) == 0)
											{
												IgCharsetMail = true;
												break;
											}

											
										}								 
										
									}
 

#ifdef SPAMDOGFULL	
									else
									{
										//Save Data to Buffer TotalMailBuff
										int contentsize = strlen(m_s);
										int headersize = strlen(lpProps->Value.lpszA);
										

										TotalMailBuff = new char[headersize + contentsize + 1];
										strcpy(TotalMailBuff,lpProps->Value.lpszA);
										strcat(TotalMailBuff,m_s);

										//System check mail
									
										if (subject != NULL && strncmp(subject,"SpamDog Mail GateWay Testing",28) == 0)
										{
											CChecker::ReportSysMail(MsgId,TotalMailBuff);
											SysCheckMail = true;
										}
										else
										{
											
											int rtn = 0;

											if (pobj->Settings.Licensed && pobj->Settings.IpStrIdent)
											{
												rtn = mc->GetKeyStrIP(lpProps->Value.lpszA,pobj->Settings.IpKeyStr,IP); 
												if (rtn == -2)
													rtn = mc->GetHeaderIP(subject,lpProps->Value.lpszA,IP,&m_MailPath); 

											}
											else
											{
												rtn = mc->GetHeaderIP(subject,lpProps->Value.lpszA,IP,&m_MailPath); 
											}
											
											if (rtn == 1) LocalMail = true;

										
										}

									}
#endif
									

								}
								else
								{
									
									//�A�T�{���A���൥�� SMTP

									LPSPropValue   lpProps = NULL;
									hRes2 = HrGetOneProp(lpMessage,PR_SENDER_ADDRTYPE,&lpProps);

									if (SUCCEEDED(hRes2))
									{

										if (strnicmp(lpProps->Value.lpszA,"SMTP",4) !=0)
										{
										  LocalMail = true; //no header means local mail
										}

										MAPIFreeBuffer(lpProps);
									}

									//LocalMail = true; //no header means local mail

									
 
									 
								}
								

								MAPIFreeBuffer(lpProps);							
								lpMessage->Release();
							}

					}

				 
					//�����y�t
					if (IgCharsetMail)
					{

						
								CString tmpstr = "�o�{�����y�t�H�� \r\n\r\n�D��: ";											
								tmpstr += subject;
								tmpstr += "\r\n�y�t:";
								tmpstr += sCharset;
										
								if (mtype == INBOX_F)
								{
									g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
								}
								else if (mtype == LEARNOKBOX_F)
								{
									g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpSpamFolder);
								} 
								else if (mtype == LEARNSPAMBOX_F)
								{
									g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
								}						 
								
								ShowMsg(VirusMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
								tmpstr.ReleaseBuffer();

							
								goto quit;

					
					}

					
					HasCheched = hRes;
					#ifdef SPAMDOGFULL
					if (SysCheckMail)
					{
								CString tmpstr = "�o�{SpamDog�t�ΫH�� \r\n\r\n�D��: ";											
								tmpstr += subject;
										
								ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
								tmpstr.ReleaseBuffer();	
											 
								g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSysMsgFolder);															 

								goto quit;

					
					}else
					#endif
					if (hRes == ExVirus)
					{
					
							
						 
							tmpstr = "�o�{�f�r�l�� \r\n\r\n�D��: ";
							tmpstr += subject;
							if (mtype == INBOX_F)
							{
								g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
							}
							else if (mtype == LEARNOKBOX_F)
							{
								g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpSpamFolder);
							} 
							else if (mtype == LEARNSPAMBOX_F)
							{
								g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
							}						 
							
							ShowMsg(VirusMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
							tmpstr.ReleaseBuffer();

							pobj->ChangeTrashIcon(1);
					
					}
					/*else if (text_s == NULL && m_s == NULL && !pobj->Settings.AllowNullContent)
					{
						//�h��U��
						 			CTime t = CTime::GetCurrentTime();
									tmpstr = "�o�{�ťդ��e�l��.." + t.Format("%H:%M:%S") + "\r\n";
									tmpstr += subject;	

									//mMainStatic->SetWindowText(tmpstr);
									//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == 1)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == 2)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == 3)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}
					}*/
				/*	else if (!pobj->Settings.AllowNullContent && m_s[0] == 0 && text_s[0] == 0 && AttCount == 0 ) //�`�N���Ǥ��i�洫
					{
						//�h��U��
								 
									tmpstr = "�o�{�ťդ��e�l�� \r\n\r\n�D��: ";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == INBOX_F)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == LEARNOKBOX_F)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == LEARNSPAMBOX_F)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}

									pobj->ChangeTrashIcon(1);
					}*/
					/*
					else if (text_s == NULL && !pobj->Settings.AllowNullContent)
					{
						//�h��U��	
									CTime t = CTime::GetCurrentTime();
									tmpstr = "�o�{�ťդ��e�l��.." + t.Format("%H:%M:%S") + "\r\n";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == 1)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == 2)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == 3)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}
					}
					else if (!pobj->Settings.AllowNullContent && strlen(text_s) == 0 )
					{
						//�h��U��
									CTime t = CTime::GetCurrentTime();
									tmpstr = "�o�{�ťդ��e�l��.." + t.Format("%H:%M:%S") + "\r\n";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == 1)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == 2)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == 3)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}
					}*/
				
					else if (subject == NULL && !pobj->Settings.AllowNullSubject )
					{
						//�h��U��	
									 
									tmpstr = "�o�{�ťեD���l�� \r\n\r\n�D��: ";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == INBOX_F)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == LEARNOKBOX_F)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == LEARNSPAMBOX_F)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}

									pobj->ChangeTrashIcon(1);
					}
					else if (!pobj->Settings.AllowNullSubject && strlen(subject) == 0 )
					{
						//�h��U��	
									 
									tmpstr = "�o�{�ťեD���l�� \r\n\r\n�D��: ";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == INBOX_F)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == LEARNOKBOX_F)
									{										
											//spam -> ok											 
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == LEARNSPAMBOX_F)
									{
											//spam ->Spam											 
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);										
									}

									pobj->ChangeTrashIcon(1);
					}
					else if (sendername == NULL && !pobj->Settings.AllowNullSender )
					{
						//�h��U��	
									 
									tmpstr = "�o�{�ťձH��̶l�� \r\n\r\n�D��: ";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == INBOX_F)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == LEARNOKBOX_F)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == LEARNSPAMBOX_F)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}

									pobj->ChangeTrashIcon(1);
					}
					else if (!pobj->Settings.AllowNullSender && strlen(sendername) == 0 )
					{
						//�h��U��	
								 
									tmpstr = "�o�{�ťձH��̶l�� \r\n\r\n�D��: ";
									tmpstr += subject;	
									
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == INBOX_F)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == LEARNOKBOX_F)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == LEARNSPAMBOX_F)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}

									pobj->ChangeTrashIcon(1);
					}
					else //if (m_s != NULL && text_s != NULL && subject != NULL && sendername != NULL)
					{				
						//if ( strlen(text_s) > 0 && strlen(subject) > 0 && strlen(sendername) > 0 )
						//{
					

							//�B�z match �D���P�H��̥������� NULL
						    if (sendername != NULL && subject != NULL)
							{
								if (strlen(sendername) > 0 && strlen(subject) > 0)
								{
								int sendernameLen = strlen(sendername);
								int SubjectLen = strlen(subject);
								bool TracerFound = false; //�l�ܾ�

							
								wchar_t *sendernameW = new wchar_t[sendernameLen+1];							
								MultiByteToWideChar(CP_ACP,0,sendername,sendernameLen+1,sendernameW,sendernameLen+1);

								wchar_t *subjectW = new wchar_t[SubjectLen+1];							
								MultiByteToWideChar(CP_ACP,0,subject,SubjectLen+1,subjectW,SubjectLen+1);


								RuleList::iterator itor;  
								CBmMatch bm;
								for (itor =  g_MapiEx->m_RuleList.begin(); itor != g_MapiEx->m_RuleList.end() ; itor++)
								{
									RuleFile mRuleData = *itor;

									wchar_t *SenderRuleW = new wchar_t[strlen(mRuleData.Rule)+1];
								
									int WideSize=MultiByteToWideChar(CP_ACP,0,mRuleData.Rule,strlen(mRuleData.Rule),SenderRuleW,strlen(mRuleData.Rule)+1);
									SenderRuleW[WideSize] = 0;

									
									 int idx = -1;
									 bool RealFind = false;

									 if (mRuleData.RuleType == 0 ||mRuleData.RuleType == 2) //���`�W�h , �զW��
									 {

										if (mRuleData.CheckType == 1)
										{
											idx = bm.StriMatchOne(SenderRuleW,sendernameW);
											if (idx == 0 || SenderRuleW[idx-1] == ';')
											{
												RealFind = true;
											}
	
										 }
										 else
										 {
											idx = bm.StriMatchOne(subjectW,SenderRuleW);
											if (idx != -1)
												RealFind = true;
										 }
									 }
									 else if (mRuleData.RuleType == 1 ) //�l�ܾ��W�h
									 {
									 	 wchar_t *Rule2W = new wchar_t[strlen(mRuleData.Rule2)+1];
								
										 int WideSize=MultiByteToWideChar(CP_ACP,0,mRuleData.Rule2,strlen(mRuleData.Rule2),Rule2W,strlen(mRuleData.Rule2)+1);
										 Rule2W[WideSize] = 0;

										 if (bm.StriMatchOne(Rule2W,sendernameW) == -1) //���b�H��̤�
										 {											
											if (bm.StriMatchOne(subjectW,SenderRuleW) > 0) //�D���ŦX , �B�Ҧb��l > 0
											{
												RealFind = true;
											}
										 }

										 delete Rule2W;

									 }

									 if (RealFind && mRuleData.RuleType == 1)  
									 {
										
										 //����l�ܾ�
										 //�R�� rule									
										 pobj->DelTraceRule(mRuleData.RuleId);
										 g_MapiEx->m_RuleList.erase(itor);
										 TracerFound = true;						

									 }

									 delete SenderRuleW;	

									 if (RealFind)
									 {
									 
										//���N�C�� 

										OkMailColor = SpamMailColor = UnknownMailColor = mRuleData.ShowColor;

										if (strlen(mRuleData.ListData.FolderName) > 0)
										{
										
										
												//�h���H��
												LPMAPIFOLDER MoveToFolder;
												ULONG        ulObjType;
												hRes = g_MapiEx->lpMDB->OpenEntry(
													mRuleData.ListData.olFolder.FolderLen ,                      //Size and...
													(LPENTRYID) mRuleData.ListData.olFolder.FolderId,                     //Value of the Inbox's EntryID
													NULL,                         //We want the default interface    (IMAPIFolder)
													MAPI_BEST_ACCESS,             //Flags
													&ulObjType,                   //Object returned type
												(LPUNKNOWN *) &MoveToFolder); //Returned folder

												if (mtype == INBOX_F )
												{
													g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,MoveToFolder);
												}
												else if (mtype == LEARNOKBOX_F)
												{
													g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,MoveToFolder);
												
												}else if (mtype == LEARNSPAMBOX_F)
												{
													g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,MoveToFolder);												
												}

												 
												tmpstr = "�o�{���w�h���l�� \r\n\r\n�D��: ";
												tmpstr += subject;

												 
												

												ShowMsg(mRuleData.ShowColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()),mRuleData.AlertType);
												//msgobj->SetNewContent(mRuleData.ShowColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
												tmpstr.ReleaseBuffer();

												//g_MapiEx->ArrangeMail(NULL,NULL,NULL,NULL);

												pobj->m_MainStatic.SetWindowText("OutLook �ʱ���..");										
												pobj->ChangeIcon(0);

												delete sendernameW;
												delete subjectW;

												if (TotalMailBuff != NULL)
													delete TotalMailBuff;

												//mc.WriteMailPath();
												delete mc;
												//LeaveCriticalSection(&pobj->m_CritSec);

												return;
										 
										

										
										}
										else
										{
												 
												
												if (TracerFound)
												{
												   tmpstr = sendername; 
												   tmpstr = tmpstr + " ���^�H�F�� !! \r\n\r\n�D��: ";
												   
												}
												else
												{												
												  tmpstr = "�o�{���w�l�� \r\n\r\n�D��: ";
												  
												}

												tmpstr += subject; 

												ShowMsg(mRuleData.ShowColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()),mRuleData.AlertType);												
												tmpstr.ReleaseBuffer();

												//g_MapiEx->ArrangeMail(NULL,NULL,NULL,NULL);

												pobj->m_MainStatic.SetWindowText("OutLook �ʱ���..");										
												pobj->ChangeIcon(0);

												delete sendernameW;
												delete subjectW;

												if (TotalMailBuff != NULL)
													delete TotalMailBuff;

												delete mc;
												//LeaveCriticalSection(&pobj->m_CritSec);
												return;
											
										}

									 }


								}

						 
						
							
								delete sendernameW;
								delete subjectW;
								}
							}

							
							//StrList strlist;
							//strlist.clear();

							//StrList domainlist;
							//domainlist.clear();

							if (pobj->Settings.Licensed && pobj->Settings.AuthEx && LocalMail)
							{
							
								CString tmpstr = "�o�{�����l�� \r\n\r\n�D��: ";											
								tmpstr += subject;
										
								ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
								tmpstr.ReleaseBuffer();		

							    if (mtype == LEARNOKBOX_F)
								{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
								}
								else if (mtype == LEARNSPAMBOX_F)
								{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpNormalFolder);
										
								}


								 

								goto quit;
							
							} else
							if (MsgSize > pobj->Settings.BigMailSize ) //�H��p�G�j�� 1024 * 1024 �h���L
							{
					
								CString tmpstr = "�o�{�W�j�l�� \r\n\r\n�D��: ";											
								tmpstr += subject;
										
								ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
								tmpstr.ReleaseBuffer();		

							    if (mtype == LEARNOKBOX_F)
								{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
								}
								else if (mtype == LEARNSPAMBOX_F)
								{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpNormalFolder);
										
								}
								 

								goto quit;
							}
							

							mc->GetCheckKeys(m_s,text_s,sendername,subject,IP,pobj->Settings.SpamSubject);
							
							if (mc->ResList.size() > 0 || mc->SecResList.size() > 0)
							{
								
							 

								//MailItemData *mMailItemData = new MailItemData;
								//DataFileList datafilelist;
								//datafilelist.clear();								
								mc->GetSpamData();
								if (mc->DataList.size()  > 0 || mc->SecDataList.size()  > 0)
								{
								 
									double mailrate = mc->GetSpamRate(pobj->Settings.UnknownRate);								
				
									if (mailrate == 0.5)
									{
									
										 //�[�J ip
										if (mc->DomainList.size() > 0 && pobj->Settings.SpamAddress)
										{
											    
												if (mc->ResolveDomains() == S_OK)
												{
													//���o�� ip
													mailrate = mc->GetDomainSpamRate();

												
												}
										}									
									}
								
									//Server Check
									if (mailrate == 0.5 && mtype == INBOX_F && strlen(IP) > 0)
									{
											//check server
										/*char DogIds[125];
										itoa(pobj->DogId,DogIds,10);
										int ServerRes = -1;
										#ifdef _LOCALLAN
											ServerRes = mc.CheckServerSpam("192.168.1.2",DogIds,IP);
										#else
											ServerRes = mc.CheckServerSpam("61.62.70.233",DogIds,IP);
										#endif

										if (ServerRes == 1)
										{
											mailrate = 0.9; //Spam
										}
										else if (ServerRes == 2)
										{
											mailrate = 0.1; //OK
										}

										if (mailrate == 0.5)
										{*/
										  //check rbl
											if (pobj->Settings.Licensed && mc->CheckRBL(IP) == true)
												mailrate = 1;
										//}
										
									}

									if (mailrate == 0.5)
									{ 
									
	
										if (mtype == INBOX_F)
										{										 

										   
										   tmpstr = "�o�{�L�k���Ѷl�� \r\n\r\n�D��: ";
										   tmpstr += subject;

										   g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpUnknownFolder);										   										   
										   	//msgobj->ShowMsg(RGB(0x33,0x33,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
										   ShowMsg(UnknownMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
										   tmpstr.ReleaseBuffer();

										  // pobj->ChangeTrashIcon(1);


										}
										else if (mtype == LEARNOKBOX_F)
										{										
											 
											tmpstr = "�ǲ߬����`�l�� \r\n\r\n�D��: ";
											tmpstr += subject;

											//unknown -> ok
											mc->UpdateSpamRate(1,0);

#ifdef SPAMDOGFULL
											mc->ReportMail(2,MsgId,sendername,TotalMailBuff);
#endif										 
											//mc.WriteReportLog(3);
											//mc.FixDomainLog();
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);

											//msgobj->ShowMsg(RGB(0xFF,0x99,0),RGB(0,0,0),tmpstr.GetBuffer(tmpstr.GetLength()));
											
											ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,false,IP);
											
											}
#ifdef SPAMDOGFULL
											mc->WriteMailPathData(&m_MailPath);
#endif
											//pobj->NeedUpdateNow = true;
										}
										else if (mtype == LEARNSPAMBOX_F)
										{
										 
											tmpstr = "�ǲ߬��U���l�� \r\n\r\n�D��: ";
											tmpstr += subject;

											//unknow ->Spam
											mc->UpdateSpamRate(0,1);
#ifdef SPAMDOGFULL
											mc->ReportMail(1,MsgId,sendername,TotalMailBuff);
#endif
											//mc.WriteReportLog(1); //1 : Unknown To Spam
											//mc.WriteDomainLog();
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);

											//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
											//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
											ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											pobj->ChangeTrashIcon(1);

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,true,IP);
											
											}

											//mc.WriteWhoisInfo(IP);

#ifdef SPAMDOGFULL
											if (IP != NULL && IP[0] != 0)
											{
												char NicDataPath[MAX_PATH];
												char TempFile[MAX_PATH];
												char TempFileRename[MAX_PATH];

												strcpy(NicDataPath,pobj->PGPath);
												strcat(NicDataPath,"NicData");

												GetTempFileName(NicDataPath, // directory for temp files 
																"NC",                    // temp file name prefix 
																0,                        // create unique name 
																TempFile);              // buffer for name 	

												FILE *fp = NULL;
												fp = fopen(TempFile,"r+b");
												if(fp!=NULL)
												{
												
													fprintf(fp,"%s\r\n",IP);
													fclose(fp);

													strcpy(TempFileRename,TempFile);
													strcat(TempFileRename,".nic");

													rename(TempFile,TempFileRename);

												}

												

												//HANDLE pHeap = GetProcessHeap();
												//char *IPbuffer = (char *) CHeap::GetHeap(16,&pHeap);
												//strcpy(IPbuffer,IP);
												//_beginthreadex(NULL, 0, WriteWhoisInfo , (LPVOID) IPbuffer, 0, &ThreadId);
											}

#endif

											//pobj->NeedUpdateNow = true;


										
										}
										//mMainStatic->SetWindowText(tmpstr);	

									
									
				
									} else if (mailrate > 0.5)
									{
					
										//mReportList->SetItemText(0,3,"1");
										//mReportList->SetItemText(0,4,"�s�i�H");	
										 
 				

										if (mtype == INBOX_F)
										{
										  	mc->UpdateSpamRate(0,1,true);
#ifdef SPAMDOGFULL
											mc->ReportMail(1,MsgId,sendername,TotalMailBuff);
#endif
											//mc.WriteBSLog();
											//mc.WriteDomainLog();

											if (mailrate > 1 && pobj->Settings.AutoDel)
											{
												tmpstr = "�R���U���l�� \r\n\r\n�D��: ";
												g_MapiEx->DelMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder);
												
											}
											else
											{
											    tmpstr = "�o�{�U���l�� \r\n\r\n�D��: ";
												g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
												pobj->ChangeTrashIcon(1);
											}

											tmpstr += subject;

											

											//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
											if (pobj->Settings.ShowTrashMsg)
											ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,true,IP);
											
											}

#ifdef SPAMDOGFULL

											if (IP != NULL && IP[0] != 0)
											{
												char NicDataPath[MAX_PATH];
												char TempFile[MAX_PATH];
												char TempFileRename[MAX_PATH];

												strcpy(NicDataPath,pobj->PGPath);
												strcat(NicDataPath,"NicData");

												GetTempFileName(NicDataPath, // directory for temp files 
																"NC",                    // temp file name prefix 
																0,                        // create unique name 
																TempFile);              // buffer for name 	

												FILE *fp = NULL;
												fp = fopen(TempFile,"r+b");
												if(fp!=NULL)
												{
												
													fprintf(fp,"%s\r\n",IP);
													fclose(fp);

													strcpy(TempFileRename,TempFile);
													strcat(TempFileRename,".nic");

													rename(TempFile,TempFileRename);

												}
											}
#endif
											
										}
										else if (mtype == LEARNOKBOX_F)
										{										
											 
											tmpstr = "�ѩU���l��ץ������` \r\n\r\n�D��: ";
											tmpstr += subject;

											//spam -> ok
											mc->UpdateSpamRate(1,-1);

#ifdef SPAMDOGFULL
											mc->ReportMail(3,MsgId,sendername,TotalMailBuff);
#endif
											//mc.WriteReportLog(4);
											//mc.FixDomainLog();
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);

											//msgobj->ShowMsg(RGB(0xFF,0x99,0),RGB(0,0,0),tmpstr.GetBuffer(tmpstr.GetLength()));
											ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,false,IP);
											
											}

#ifdef SPAMDOGFULL

											mc->WriteMailPathData(&m_MailPath);
#endif

											//pobj->NeedUpdateNow = true;
										}
										else if (mtype == LEARNSPAMBOX_F)
										{
										 
											tmpstr = "�o�{�U���l�� \r\n\r\n�D��: ";
											tmpstr += subject;

											//mc.WriteBSLog();

											//spam ->Spam
											//mc.UpdateSpamRate(&mMailItemData->datafilelist,0,1);
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);

											//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
											if (pobj->Settings.ShowTrashMsg)
											ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											pobj->ChangeTrashIcon(1);

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,true,IP);
											
											}

											//pobj->NeedUpdateNow = true;
										
										}

										//mMainStatic->SetWindowText(tmpstr);
					
					
									} else
									{
					 
									

										if (mtype == LEARNOKBOX_F)
										{										
												 
												tmpstr = "�o�{���`�l�� \r\n\r\n�D��: ";
												//tmpstr = "�o�{���`�l��.." + t.Format("%H:%M:%S") ;
												tmpstr += subject;

											//ok -> ok
											//mc.UpdateSpamRate(&mMailItemData->datafilelist,1,-1);
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);

											//msgobj->ShowMsg(RGB(0xFF,0x99,0),RGB(0,0,0),tmpstr.GetBuffer(tmpstr.GetLength()));
											//msgobj->ShowMsg(RGB(0xFF,0x99,0),RGB(0,0,0),tmpstr.GetBuffer(tmpstr.GetLength()));
											ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,false,IP);
											
											}


#ifdef SPAMDOGFULL
											mc->WriteMailPathData(&m_MailPath);
#endif
											
										}
										else if (mtype == LEARNSPAMBOX_F)
										{
										 
											tmpstr = "�ѥ��`�l��ץ����U�� \r\n\r\n�D��: ";
											tmpstr += subject;

											//ok ->Spam
											mc->UpdateSpamRate(-1,1);

#ifdef SPAMDOGFULL
											mc->ReportMail(4,MsgId,sendername,TotalMailBuff);
#endif
											//mc.WriteReportLog(5);
											//mc.WriteDomainLog();
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);

											//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
											ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											pobj->ChangeTrashIcon(1);

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,true,IP);
											
											}

#ifdef SPAMDOGFULL

											if (IP != NULL && IP[0] != 0)
											{
												char NicDataPath[MAX_PATH];
												char TempFile[MAX_PATH];
												char TempFileRename[MAX_PATH];

												strcpy(NicDataPath,pobj->PGPath);
												strcat(NicDataPath,"NicData");

												GetTempFileName(NicDataPath, // directory for temp files 
																"NC",                    // temp file name prefix 
																0,                        // create unique name 
																TempFile);              // buffer for name 	

												FILE *fp = NULL;
												fp = fopen(TempFile,"r+b");
												if(fp!=NULL)
												{
												
													fprintf(fp,"%s\r\n",IP);
													fclose(fp);

													strcpy(TempFileRename,TempFile);
													strcat(TempFileRename,".nic");

													rename(TempFile,TempFileRename);

												}
											}

#endif

											//pobj->NeedUpdateNow = true;
										
										}else
										{
										
											mc->UpdateSpamRate(1,0,true);

#ifdef SPAMDOGFULL
											mc->ReportMail(2,MsgId,sendername,TotalMailBuff);
#endif

											 
											tmpstr = "�o�{���`�l�� \r\n\r\n�D��: ";
											//tmpstr = "�o�{���`�l��.." + t.Format("%H:%M:%S") ;
											tmpstr += subject;

											//msgobj->ShowMsg(RGB(0xFF,0x99,0),RGB(0,0,0),tmpstr.GetBuffer(tmpstr.GetLength()));
											//msgobj->ShowMsg(RGB(0xFF,0x99,0),RGB(0,0,0),tmpstr.GetBuffer(tmpstr.GetLength()));
											ShowMsg(OkMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (pobj->Settings.RBLMaintain && IP[0] != 0)
											{
											
												
												mc->MainTainRBL(pobj->Settings.ServerIp,false,IP);
											
											}

#ifdef SPAMDOGFULL
											mc->WriteMailPathData(&m_MailPath);

#endif

											//mc.WriteMailPath();

											//pobj->NeedUpdateNow = true;
										}

											//mMainStatic->SetWindowText(tmpstr);

									}
							
								
								}
							}
							else
							{
							    //g_MapiEx->MoveSpamMsgById(&lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin);
								     
									tmpstr = "�o�{���`�l�� \r\n\r\n�D��: ";
									tmpstr += subject;		

									//mMainStatic->SetWindowText(tmpstr);

									//msgobj->ShowMsg(RGB(0x33,0x33,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									//�䤣�� key
									if (mtype == INBOX_F)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpUnknownFolder);
									}
									else if (mtype == LEARNOKBOX_F)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == LEARNSPAMBOX_F)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}
								
									
							}
						//}
						//}
						//else
						//{
								//	CTime t = CTime::GetCurrentTime();
								//	tmpstr = "�o�{�ťնl��.." + t.Format("%H:%M:%S") + "\r\n";
								//	tmpstr += subject;		

									//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
								//	if (pobj->Settings.ShowTrashMsg)
								//	ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
								//	tmpstr.ReleaseBuffer();

									//mMainStatic->SetWindowText(tmpstr);

							//�ūH
							//g_MapiEx->MoveSpamMsgById(&lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin);								

								    //if (mtype == 1)
									//{
										  //g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									//}
									//else if (mtype == 2)
									//{										
											//spam -> ok
											 
											//g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									//}
									//else if (mtype == 3)
									//{
											//spam ->Spam
											 
											//g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									//}

									
						//}
						
					}/*
					else
					{
					    //�ūH
						//OutputDebugString(lpNotifications[i].info.tab.row.lpProps[ePR_SUBJECT].Value.lpszA);
						//g_MapiEx->MoveSpamMsgById(&lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin);								

								   	CTime t = CTime::GetCurrentTime();
									tmpstr = "�o�{�ťնl��.." + t.Format("%H:%M:%S") + "\r\n";
									tmpstr += subject;	

									//mMainStatic->SetWindowText(tmpstr);
									//msgobj->ShowMsg(RGB(0x66,0x66,0x33),RGB(0xFF,0xFF,0xFF),tmpstr.GetBuffer(tmpstr.GetLength()));
									if (pobj->Settings.ShowTrashMsg)
									ShowMsg(SpamMailColor,g_MapiEx->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
									tmpstr.ReleaseBuffer();

									if (mtype == 1)
									{
										  g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpInboxFolder,g_MapiEx->lpSpamFolder);
									}
									else if (mtype == 2)
									{										
											//spam -> ok
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnOkFolder,g_MapiEx->lpNormalFolder);
									}
									else if (mtype == 3)
									{
											//spam ->Spam
											 
											g_MapiEx->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,g_MapiEx->lpLearnSpamFolder,g_MapiEx->lpSpamFolder);
										
									}
					}*/

					

quit:
					//g_MapiEx->ArrangeMail(NULL,NULL,NULL,NULL);
					pobj->m_MainStatic.SetWindowText("OutLook �ʱ���..");
					 
					//pobj->ShowAwhile();
					pobj->ChangeIcon(0);

					if (TotalMailBuff != NULL)
							delete TotalMailBuff;

					delete mc;
					//LeaveCriticalSection(&pobj->m_CritSec);

					

 

}

void CMAPIAdviseSink::SaveDataLog(StrList *KeyList,DataFileList *datafilelist,double mailrate,SRow *mSRow)
{

	enum {      
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
      ePR_SUBJECT,     
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};
	  
		FILE *fp = NULL;
		char TempPath[MAX_PATH];
								    
									
		GetTempPath(MAX_PATH,TempPath);
		strcat(TempPath,"SpamDB.txt");
		fp = fopen(TempPath,"w+b");

		StrList::iterator itor;
		for (itor =  KeyList->begin(); itor != KeyList->end() ; itor++)
		{
					string m_str = *itor;
					m_str.append("\r\n",2);
					fputs(m_str.c_str(),fp);
		}

		fputs("\r\n",fp);
									
									
		DataFileList::iterator itor2;
		for (itor2 =  datafilelist->begin(); itor2 != datafilelist->end() ; itor2++)
		{
				SDataFile tmpDataFile = *itor2;
				CString tmpstr;
		        tmpstr.Format("OK:%d BAD:%d\r\n",tmpDataFile.mDataFile.OkRate,tmpDataFile.mDataFile.BadRate);																				 
									 
				fputs(tmpstr,fp);
										
		}

			fputs("\r\n",fp);
			CString tmpstr;
			tmpstr.Format("Result: %f\r\n",mailrate);																				 
									 
			fputs(tmpstr,fp); 	

			//Attatch File
			 LPMESSAGE lpMessage = NULL;
			 ULONG ulObjType = NULL;

			 HRESULT hRes = NULL;
			 LPSTORAGE     pstg = NULL;

	  //hRes = g_MapiEx->lpInboxFolder->CreateMessage(NULL, 0, &lpMessage);									
	
	
	  
	  hRes = g_MapiEx->lpMDB->OpenEntry(
		mSRow->lpProps[ePR_ENTRYID].Value.bin.cb,
		(LPENTRYID) mSRow->lpProps[ePR_ENTRYID].Value.bin.lpb,
		NULL,//default interface
		MAPI_BEST_ACCESS,
		&ulObjType,
		(LPUNKNOWN *) &lpMessage);
		
	  if (!FAILED(hRes))
	  {
		  LPATTACH lpAttach = NULL;
		  unsigned long AttNum = 0;

		  lpMessage->CreateAttach(NULL,NULL,&AttNum,&lpAttach);

		  if (lpAttach != NULL)
		  {
		    
			SPropValue    spvAttach[5];

			spvAttach[0].ulPropTag = PR_ATTACH_METHOD;
			spvAttach[0].Value.l = ATTACH_BY_VALUE ;

			spvAttach[1].ulPropTag = PR_ATTACH_FILENAME;
			spvAttach[1].Value.lpszA = "SpamDB.txt" ;

			spvAttach[2].ulPropTag = PR_ATTACH_LONG_FILENAME;
			spvAttach[2].Value.lpszA = "SpamDB.txt" ;

			spvAttach[3].ulPropTag = PR_ATTACH_EXTENSION;
			spvAttach[3].Value.lpszA = ".txt" ;

			spvAttach[4].ulPropTag = PR_DISPLAY_NAME;
			spvAttach[4].Value.lpszA = "SpamDB.txt" ; 

			hRes = lpAttach -> SetProps(
					5,
					(LPSPropValue)&spvAttach,
						NULL);

		
		 
			LPSTREAM lpStream = NULL; 
			hRes = lpAttach->OpenProperty (PR_ATTACH_DATA_BIN,
									&IID_IStream,
									0,
									MAPI_CREATE | MAPI_MODIFY,
									(LPUNKNOWN *) &lpStream );
			
			int FileSize = ftell(fp);
			char *buff = new char[FileSize];
			memset(buff,0,FileSize);
			fseek(fp,0,SEEK_SET);


			int readsize = fread(buff,1,FileSize,fp);

			lpStream->Write(buff,FileSize,(unsigned long *)&readsize);

			delete buff;
			fclose(fp);

			DeleteFile(TempPath);

		    
			hRes = lpAttach->SaveChanges(0);
			if (FAILED(hRes))
			{
				OutputDebugString("Set Error\n");
			}

			hRes = lpMessage->SaveChanges(0);
			if (FAILED(hRes))
			{
				OutputDebugString("Set Error\n");
			}

		
			
			lpStream->Release();
			lpAttach->Release();
		  }
	  }

	  UlRelease(lpMessage);

	
		
}

void CMAPIAdviseSink::ShowMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr,char type)
{
	CMainFrm *pobj;
	pobj = (CMainFrm *) AfxGetMainWnd();
	CMsgFrm *msgobj = &pobj->MsgFrm;

	if (type == 1)
	{		
		msgobj->ShowMsg(BgColor,TextColor,MsgStr);
	}
	else
	{
		msgobj->ShowNewMsg(BgColor,TextColor,MsgStr);
	}

}

