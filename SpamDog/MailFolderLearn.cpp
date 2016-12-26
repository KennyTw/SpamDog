// MailFolderLearn.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "MailFolderLearn.h"

#include "MainFrm.h"
#include "OEapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailFolderLearn property page

IMPLEMENT_DYNCREATE(CMailFolderLearn, CPropertyPage)

CMailFolderLearn::CMailFolderLearn() : CPropertyPage(CMailFolderLearn::IDD)
{
	//{{AFX_DATA_INIT(CMailFolderLearn)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMailFolderLearn::~CMailFolderLearn()
{
}

void CMailFolderLearn::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailFolderLearn)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailFolderLearn, CPropertyPage)
	//{{AFX_MSG_MAP(CMailFolderLearn)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailFolderLearn message handlers

BOOL CMailFolderLearn::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CPropertySheet *pSheet = (CPropertySheet *) this->GetParent();
	pSheet->SetWizardButtons(PSWIZB_DISABLEDFINISH);

	imglist.Create(IDB_FOLDERLIST,16,2,RGB(255,255,255));
	m_Tree.SetImageList(&imglist,TVSIL_NORMAL);

	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
	{
		GetMailBoxFolder();

	} else if (mfrm->Settings.APVersion == AP_OUTLOOK)
	{
	
		root_hItem = m_Tree.InsertItem("收件匣",0,1);

		ULONG        cbInbox;
		LPENTRYID    lpbInbox;

		enum {     
			ePR_ENTRYID,
			ePR_DISPLAY_NAME,		
			NUM_COLS};	
   
		static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
			PR_ENTRYID,
			PR_DISPLAY_NAME	  
			};

		HRESULT hRes = g_MapiEx->lpMDB->GetReceiveFolder(
			NULL,      //Get default receive folder
			NULL,      //Flags
			&cbInbox,  //Size and ...
			&lpbInbox, //Value of the EntryID to be returned
			NULL);     //You don't care to see the class returned

		
		if (SUCCEEDED(hRes))
		{
		
			strcpy(m_ListData.FolderName,"收件匣");
			m_ListData.olFolder.FolderLen = cbInbox;
			memcpy(m_ListData.olFolder.FolderId,lpbInbox,m_ListData.olFolder.FolderLen);

			m_FolderList.insert(m_FolderList.end(),m_ListData);
			m_Tree.SetItemData(root_hItem,m_FolderList.size()); //放入編號

		}

		ListOLFolder(root_hItem,g_MapiEx->lpInboxFolder);
		m_Tree.Expand(root_hItem,TVE_EXPAND);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMailFolderLearn::GetMailBoxFolder()
{

	
	STOREFOLDERID dwFolderId = FOLDERID_ROOT;
	FOLDERPROPS props;
	HENUMSTORE hEnum = NULL;
 
	
	props.cbSize = sizeof(FOLDERPROPS);
 

	HRESULT hr = g_OEapi->m_pStoreNamespace->GetFirstSubFolder(dwFolderId, &props, &hEnum);
	if(FAILED(hr)) 
	{		
		return ;
	}
 
	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) 
	{
		
		 	
	    if (props.sfType == FOLDER_INBOX)
		{			 
			
			root_hItem = m_Tree.InsertItem(props.szName,0,1);
			m_Tree.SetItemData(root_hItem,props.dwFolderId);

			ListFolder(root_hItem,props.dwFolderId);
			m_Tree.Expand(root_hItem,TVE_EXPAND);

			break;	
		}

		hr = g_OEapi->m_pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	if(hEnum) 
	{
		g_OEapi->m_pStoreNamespace->GetSubFolderClose(hEnum);
	}

}

void CMailFolderLearn::ListFolder(HTREEITEM phItem ,STOREFOLDERID pFolder)
{

	//STOREFOLDERID dwFolderId ;
	FOLDERPROPS props;
	HENUMSTORE hEnum = NULL;
 
	
	props.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = g_OEapi->m_pStoreNamespace->GetFirstSubFolder(pFolder, &props, &hEnum);
	if(FAILED(hr)) 
	{		
		return ;
	}
 
	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) 
	{
		
		 	
	    //if (props.sfType == FOLDER_INBOX)
		//{		
		
		if(strcmp(g_OEapi->LearnOkFolderName,props.szName) != 0 &&
		   strcmp(g_OEapi->LearnSpamFolderName,props.szName) != 0 )
		{
			
			HTREEITEM hItem = m_Tree.InsertItem(props.szName,0,1,phItem);
			m_Tree.SetItemData(hItem,props.dwFolderId);

			ListFolder(hItem,props.dwFolderId);
		}
		//	break;	
		//}

		hr = g_OEapi->m_pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	if(hEnum) 
	{
		g_OEapi->m_pStoreNamespace->GetSubFolderClose(hEnum);
	}
	
}

void CMailFolderLearn::ListOLFolder(HTREEITEM phItem , LPMAPIFOLDER  pFolder)
{

		LPSRowSet pRows ;
		LPMAPITABLE lpContentsTable;

		enum {     
				ePR_ENTRYID,
				ePR_DISPLAY_NAME,	  
				NUM_COLS};
   
		static SizedSPropTagArray(NUM_COLS,sptCols) = { NUM_COLS,
			PR_ENTRYID,
			PR_DISPLAY_NAME,	  
		};

		HRESULT hRes = pFolder->GetHierarchyTable(0,&lpContentsTable);
		if (FAILED(hRes)) return;
   
		//列出 query
		hRes = HrQueryAllRows(
			lpContentsTable,
			(LPSPropTagArray) &sptCols,
			NULL,//restriction...we're not using this parameter
			NULL,//sort order...we're not using this parameter
			0,
			&pRows);

		if (FAILED(hRes)) 
		{
			FreeProws(pRows);
			UlRelease(lpContentsTable);
			return;  
		}
		

		for (int i = 0; i < pRows ->cRows; i++)
		{
 
			if (pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].ulPropTag == PR_DISPLAY_NAME)
			{
			    if(strcmp(g_MapiEx->LearnOkFolderName,pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA) != 0 &&
		        strcmp(g_MapiEx->LearnSpamFolderName,pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA) != 0 )
				{
				
				    HTREEITEM hItem = m_Tree.InsertItem(pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA,0,1,phItem);					

					strcpy(m_ListData.FolderName,pRows->aRow[i].lpProps[ePR_DISPLAY_NAME].Value.lpszA);
					m_ListData.olFolder.FolderLen = pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb;
					memcpy(m_ListData.olFolder.FolderId,pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,m_ListData.olFolder.FolderLen);

					m_FolderList.insert(m_FolderList.end(),m_ListData);
					m_Tree.SetItemData(hItem,m_FolderList.size()); //放入編號


					ULONG        ulObjType;
					LPMAPIFOLDER pFolder = NULL;

					hRes = g_MapiEx->lpMDB->OpenEntry(
						pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,                      //Size and...
							(LPENTRYID) pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,                     //Value of the Inbox's EntryID
							NULL,                         //We want the default interface    (IMAPIFolder)
							MAPI_BEST_ACCESS,             //Flags
							&ulObjType,                   //Object returned type
							(LPUNKNOWN *) &pFolder); //Returned folder

					if (SUCCEEDED(hRes))
					{
						ListOLFolder(hItem,pFolder);
						pFolder->Release();
					}
			   }

			}

		}

		FreeProws(pRows);
		UlRelease(lpContentsTable);



}

/*
void CMailFolderLearn::LearnMail(char Howto)
{
	HTREEITEM selecteditem = m_Tree.GetSelectedItem();
	if (selecteditem != NULL)
	{
		CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

		if (mfrm->Settings.APVersion == AP_OUTLOOK)
		{
			FolderList::iterator itor; 
			itor = m_FolderList.begin();

			int ItemIdx = m_Tree.GetItemData(selecteditem);
		
			for (int i = 1; i < ItemIdx; i++)
			{
				itor++;
			}			 

			if (ItemIdx > 0 )
			{		
				m_ListData = (FolderListData) *itor;				 
				g_MapiEx->LearnAllFolder(&m_ListData.olFolder , Howto , &m_progress1);
			}	
		

		} else if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
		{
		
			 STOREFOLDERID m_FolderId = m_Tree.GetItemData(selecteditem);
			 IStoreFolder *m_pStoreFolder = NULL;

			 HRESULT hr =  g_OEapi->m_pStoreNamespace->OpenFolder(m_FolderId,0,&m_pStoreFolder);

			 if (SUCCEEDED(hr))
			 {
				 	//取出信件數目
					FOLDERPROPS fprops;
					fprops.cbSize = sizeof(FOLDERPROPS);
					HRESULT hr = m_pStoreFolder->GetFolderProps(0,&fprops);

					int TotalMail = 0;
					if (SUCCEEDED(hr))
					{
						TotalMail = fprops.cMessage;
					}

					m_progress1.SetRange(0, TotalMail);


					if (TotalMail > 0)
					{
						//列出所有信件
						//MESSAGEIDLIST MsgIdList;
						//DWORD *MessageIdArray = new DWORD[TotalMail];

						//MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
						//MsgIdList.cMsgs = TotalMail;
		

						MESSAGEPROPS msgProps;
						HENUMSTORE hEnumMsg; 
	 

						hEnumMsg = NULL;
						msgProps.cbSize = sizeof(MESSAGEPROPS);

 
						int i = 0;
						hr = m_pStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
						
						while(SUCCEEDED(hr) && hr != S_FALSE) 
						{
			 
							//MessageIdArray[i] =  msgProps.dwMessageId;
							g_OEapi->ProcessLearnMsg(m_pStoreFolder,msgProps.dwMessageId,Howto);

							m_pStoreFolder->FreeMessageProps(&msgProps);							
							hr = m_pStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);

							m_progress1.SetPos(i+1);

							i++;
						}

						// close the enum
						if(hEnumMsg) 
						{
							m_pStoreFolder->GetMessageClose(hEnumMsg);
						}


						//MsgIdList.prgdwMsgId = MessageIdArray;
		
						//m_pStoreFolder->DeleteMessages(&MsgIdList, 0, 0);
		
						//delete MessageIdArray;

					}
			 }
				 
	
		}

		

	}

}*/

void CMailFolderLearn::OnButton1() 
{
	// TODO: Add your control notification handler code here
	//learn spam
	//LearnMail(1);
	AfxMessageBox("學習完畢!");

}

void CMailFolderLearn::OnButton2() 
{
	// TODO: Add your control notification handler code here
	//learn ok

	//LearnMail(2);
	AfxMessageBox("學習完畢!");
	
}
