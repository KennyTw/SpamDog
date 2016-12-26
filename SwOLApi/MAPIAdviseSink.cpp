// MAPIAdviseSink.cpp: implementation of the CMAPIAdviseSink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MAPIAdviseSink.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <initguid.h>
#include <mapiguid.h>

STDMETHODIMP CMAPIAdviseSink2::QueryInterface (REFIID riid,
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

STDMETHODIMP_(ULONG) CMAPIAdviseSink2::AddRef() 
{
	
	LONG lCount = InterlockedIncrement(&m_cRef);
	return lCount; 
}

STDMETHODIMP_(ULONG) CMAPIAdviseSink2::Release() 
{

	LONG lCount = InterlockedDecrement(&m_cRef);
	if (!lCount)  
	{
		delete this; 
	}
	return lCount;
}

CMAPIAdviseSink2::~CMAPIAdviseSink2 () {
	
}

CMAPIAdviseSink2::CMAPIAdviseSink2 (LPVOID lpParam,  LPMDB   lpMDB ,OlREGISTERNOTIFY *regnotify, LPOlOnNotify onnotify ) 
{	 
	mlpParam = lpParam;
	//mlpContentsTable = lpContentsTable;
	mOnNotify = onnotify;
	mregnotify = regnotify;
	//mRegType = RegType;
	mlpMDB = lpMDB;

	//mMsgTable.clear();

	TableLoad();

	
	
	
	

	
}

void CMAPIAdviseSink2::ProcessReloadTableAdd(LPSRowSet pRows)
{
	
	if (mregnotify->lpContentsTable == NULL) return;
	enum {     
	  ePR_INSTANCE_KEY,	  
      ePR_ENTRYID,
      NUM_COLS};

	MsgTable::iterator itor; 
	LPMESSAGE lpMessage = NULL;
	ULONG ulObjType = NULL;

    //HRESULT hRes;

	
							//Table Add
							if ((mregnotify->RegType & TABLE_ROW_ADDED) == TABLE_ROW_ADDED) 
							{

									for(unsigned int j = 0 ; j < pRows->cRows ; j++)
									{ 

											bool bFound = false;
											for (itor =  mMsgTable.begin(); itor != mMsgTable.end() ; itor++)
											{
												OlMAPIMSGDATA MapiMsgData;
												memset(&MapiMsgData,0,sizeof(MapiMsgData));	
												MapiMsgData = *itor;						 

												if (!memcmp(MapiMsgData.InstanceKey, pRows->aRow[j].lpProps[ePR_INSTANCE_KEY].Value.bin.lpb,pRows->aRow[j].lpProps[ePR_INSTANCE_KEY].Value.bin.cb))								
												{						
													bFound = true;
													break;
												}			 

											}

											if (!bFound)
											{
													/*hRes = mlpMDB->OpenEntry(
														pRows->aRow[j].lpProps[ePR_ENTRYID].Value.bin.cb,
														(LPENTRYID) pRows->aRow[j].lpProps[ePR_ENTRYID].Value.bin.lpb,
														NULL,//default interface
														MAPI_BEST_ACCESS,
														&ulObjType,
														(LPUNKNOWN *) &lpMessage);

													if (!FAILED(hRes))
													{*/
													OlMAILPROPS mailpros;
													memset(&mailpros,0,sizeof(mailpros));

													mailpros.MailIdcb = pRows->aRow[j].lpProps[ePR_ENTRYID].Value.bin.cb;
													mailpros.MailIdlpb = pRows->aRow[j].lpProps[ePR_ENTRYID].Value.bin.lpb;
													//mailpros.szSubject = pRows->aRow[j].lpProps[ePR_SUBJECT].Value.lpszA;
			

													mOnNotify(mlpParam ,TABLE_ROW_ADDED, mregnotify,&mailpros);
													//}
								
													//lpMessage->Release();
											}
									}
				 
							
						}
}

void CMAPIAdviseSink2::ProcessReloadTableDel(LPSRowSet pRows)
{
	if (mregnotify->lpContentsTable == NULL) return;

	enum {     
	  ePR_INSTANCE_KEY,	  
      ePR_ENTRYID,
      NUM_COLS};

	MsgTable::iterator itor; 
	LPMESSAGE lpMessage = NULL;
	ULONG ulObjType = NULL;

    //HRESULT hRes;

	bool bFound = false;

					  //Table Delete
						if ((mregnotify->RegType & TABLE_ROW_DELETED) == TABLE_ROW_DELETED) 
						{

							
									
							for (itor =  mMsgTable.begin(); itor != mMsgTable.end() ; itor++)
							{
									bFound = false;

									OlMAPIMSGDATA MapiMsgData;
									memset(&MapiMsgData,0,sizeof(MapiMsgData));	
									MapiMsgData = *itor;
									
									//OutputDebugString("in table \n");

									for(unsigned int j = 0 ; j < pRows->cRows ; j++)
									{ 

										if (!memcmp(MapiMsgData.InstanceKey, pRows->aRow[j].lpProps[ePR_INSTANCE_KEY].Value.bin.lpb,pRows->aRow[j].lpProps[ePR_INSTANCE_KEY].Value.bin.cb))			
										{						
												bFound = true;

												//OutputDebugString("break\n");

												break;
										}

										//char a[10];
										//itoa(j,a,10);
										//OutputDebugString(a);
										//OutputDebugString("\n");
									}

									if (!bFound)
									{
											/*hRes = mlpMDB->OpenEntry(
											MapiMsgData.EntrySize,
											(LPENTRYID) MapiMsgData.EntryId,
											NULL,//default interface
											MAPI_BEST_ACCESS,
											&ulObjType,
											(LPUNKNOWN *) &lpMessage);

											if (!FAILED(hRes))
											{*/

											OlMAILPROPS mailpros;
											memset(&mailpros,0,sizeof(mailpros));

											mailpros.MailIdcb = MapiMsgData.EntrySize;
											mailpros.MailIdlpb = MapiMsgData.EntryId;
											//mailpros.szSubject = pRows->aRow[j].lpProps[ePR_SUBJECT].Value.lpszA;
			

											mOnNotify(mlpParam , TABLE_ROW_DELETED , mregnotify,&mailpros);
											//}
								
											//lpMessage->Release();
									}

							}

									 
						}



}

STDMETHODIMP_(ULONG) CMAPIAdviseSink2::OnNotify (ULONG cNotify,
													  LPNOTIFICATION lpNotifications)
{
	HRESULT			hRes = S_OK;
	//int				iPriorRow = 0;
	//int				iCurrentRow = 0;
	ULONG			i = 0;
	//int				iCurrentSelection = -1;
	LPSRowSet		pRows = NULL;

	LPMESSAGE lpMessage = NULL;
	ULONG ulObjType = NULL;

	bool bFound = false;


	enum {     
	  ePR_INSTANCE_KEY,	  
      ePR_ENTRYID,
      NUM_COLS};

	  MsgTable::iterator itor; 
  


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

				       if (mregnotify->lpContentsTable == NULL) break;

					   	hRes = HrQueryAllRows(
							mregnotify->lpContentsTable,
							//(LPSPropTagArray) &sptCols,
							NULL,
						  NULL,//restriction...we're not using this parameter
						  NULL,//sort order...we're not using this parameter
						  0,
						  &pRows);

							if (SUCCEEDED(hRes))
							{
								//Table Add
								ProcessReloadTableAdd(pRows);
							
					            //Table Del
						        ProcessReloadTableDel(pRows);
							    
							
								FreeProws(pRows);
								pRows = NULL;
							}

							TableLoad();




						/*
 
						hRes = mregnotify->lpContentsTable->SeekRow(
						BOOKMARK_BEGINNING,
						0,
						NULL);

						if (hRes != S_OK) 
						{
							
							break;						
						}

						for(;;)
						{
							hRes = mregnotify->lpContentsTable->QueryRows(
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
								//null Table
								ProcessReloadTableDel(pRows);

								FreeProws(pRows);
								pRows = NULL;
								break;
							}

						//Table Add
							ProcessReloadTableAdd(pRows);
							TableLoad();
					    //Table Del
						    ProcessReloadTableDel(pRows);
							TableLoad();

							FreeProws(pRows);
							pRows = NULL;
						}*/

						
					 
									  
				 
			
			
				break;
			case(TABLE_ROW_ADDED):
				
				OutputDebugString("TABLE_ROW_ADDED\n"); 

				if ((mregnotify->RegType & TABLE_ROW_ADDED) != TABLE_ROW_ADDED) break;
				if (mregnotify->lpContentsTable == NULL) break;
				//if (lpNotifications[i].info.tab.row.lpProps[ePR_SENDER_NAME].ulPropTag == PR_SENDER_NAME)
				//{					
					//OutputDebugString(lpNotifications[i].info.tab.row.lpProps[ePR_SENDER_NAME].Value.lpszA);
				//}
				
				OlMAPIMSGDATA MapiMsgData;
				memset(&MapiMsgData,0,sizeof(MapiMsgData));	
				
			    memcpy(MapiMsgData.InstanceKey,lpNotifications[i].info.tab.propIndex.Value.bin.lpb,lpNotifications[i].info.tab.propIndex.Value.bin.cb);			
				MapiMsgData.EntrySize = lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.cb;
				memcpy(MapiMsgData.EntryId,lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.lpb,MapiMsgData.EntrySize);
				
				mMsgTable.insert(mMsgTable.end(),MapiMsgData);
			 

				/*hRes = mlpMDB->OpenEntry(
					lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.cb,
					(LPENTRYID) lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.lpb,
					NULL,//default interface
					MAPI_BEST_ACCESS,
					&ulObjType,
					(LPUNKNOWN *) &lpMessage);

				if (!FAILED(hRes))
				{*/
				OlMAILPROPS mailpros;
				memset(&mailpros,0,sizeof(mailpros));

				mailpros.MailIdcb = lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.cb;
				mailpros.MailIdlpb = lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.lpb;
				//mailpros.szSubject = lpNotifications[i].info.tab.row.lpProps[ePR_SUBJECT].Value.lpszA;
			


				mOnNotify(mlpParam ,TABLE_ROW_ADDED, mregnotify,&mailpros);
				//}
				
				//lpMessage->Release();
 

				break;
			case(TABLE_ROW_DELETED):
				
				if ((mregnotify->RegType & TABLE_ROW_DELETED) != TABLE_ROW_DELETED) break;
				if (mregnotify->lpContentsTable == NULL) break;
				OutputDebugString("TABLE_ROW_DELETED\n"); 
							 
				for (itor =  mMsgTable.begin(); itor != mMsgTable.end() ; itor++)
				{
						OlMAPIMSGDATA MapiMsgData;
						memset(&MapiMsgData,0,sizeof(MapiMsgData));	
						MapiMsgData = *itor;

						 

						if (!memcmp(MapiMsgData.InstanceKey, lpNotifications[i].info.tab.propIndex.Value.bin.lpb, lpNotifications[i].info.tab.propIndex.Value.bin.cb))
						{						
							mMsgTable.erase(itor);
							OutputDebugString("TABLE_ROW_DELETED : TABLE_FIND_INLIST\n");

							OlMAILPROPS mailpros;
							memset(&mailpros,0,sizeof(mailpros));

							mailpros.MailIdcb = MapiMsgData.EntrySize;
							mailpros.MailIdlpb = MapiMsgData.EntryId;
											//mailpros.szSubject = pRows->aRow[j].lpProps[ePR_SUBJECT].Value.lpszA;
			

							mOnNotify(mlpParam , TABLE_ROW_DELETED , mregnotify,&mailpros);
											//}

							/*hRes = mlpMDB->OpenEntry(
							MapiMsgData.EntrySize,
							(LPENTRYID) MapiMsgData.EntryId,
							NULL,//default interface
							MAPI_BEST_ACCESS,
							&ulObjType,
							(LPUNKNOWN *) &lpMessage);

							if (!FAILED(hRes))
							{*/
							//	mOnNotify(mlpParam , TABLE_ROW_DELETED , lpMessage);
							//}
				
							//lpMessage->Release();

							break;
						}			 

				}

			
			
			 

				break;
			case(TABLE_ROW_MODIFIED):
			
				/*if ((mRegType & TABLE_ROW_MODIFIED) != TABLE_ROW_MODIFIED) break;
				OutputDebugString("TABLE_ROW_MODIFIED\n");

				hRes = mlpMDB->OpenEntry(
					lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.cb,
					(LPENTRYID) lpNotifications[i].info.tab.row.lpProps[ePR_ENTRYID].Value.bin.lpb,
					NULL,//default interface
					MAPI_BEST_ACCESS,
					&ulObjType,
					(LPUNKNOWN *) &lpMessage);

				if (!FAILED(hRes))
				{
					mOnNotify(mAdviseKey , lpMessage);
				}
				
				lpMessage->Release();*/
			
				break;
			}
		}
		 
	}
	return 0;
}


void CMAPIAdviseSink2::TableLoad()
{
	if (mregnotify->lpContentsTable == NULL) return;

	enum {     
	  ePR_INSTANCE_KEY,	  
      ePR_ENTRYID,
      NUM_COLS};
   //These tags represent the message information we would like to pick up
   
  static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,      
	  PR_INSTANCE_KEY,
      PR_ENTRYID	 
   };

	HRESULT hRes = S_OK;

	mMsgTable.clear();

	//¦C¥X All Mail In InBox
	LPSRowSet mpRows ; 

	hRes = HrQueryAllRows(
      mregnotify->lpContentsTable,
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
			
		 

			OlMAPIMSGDATA MapiMsgData;
			memset(&MapiMsgData,0,sizeof(MapiMsgData));	
				
			memcpy(MapiMsgData.InstanceKey,mpRows->aRow[i].lpProps[ePR_INSTANCE_KEY].Value.bin.lpb,mpRows->aRow[i].lpProps[ePR_INSTANCE_KEY].Value.bin.cb);			
			MapiMsgData.EntrySize = mpRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb;
			memcpy(MapiMsgData.EntryId,mpRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,MapiMsgData.EntrySize);
				
			mMsgTable.insert(mMsgTable.end(),MapiMsgData);
				
		}


		 FreeProws(mpRows);

	}

	
    


}