// MapiFolder.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDog.h"
#include "MapiFolder.h"
#include "MainFrm.h"
#include "OEapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapiFolder dialog


CMapiFolder::CMapiFolder(CWnd* pParent /*=NULL*/)
	: CDialog(CMapiFolder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapiFolder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(&m_ListData,0,sizeof(FolderListData));
}


void CMapiFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapiFolder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapiFolder, CDialog)
	//{{AFX_MSG_MAP(CMapiFolder)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapiFolder message handlers

BOOL CMapiFolder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	imglist.Create(IDB_FOLDERLIST,16,2,RGB(255,255,255));
	m_Tree.SetImageList(&imglist,TVSIL_NORMAL);

	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
	{
		GetMailBoxFolder();

	} else if (mfrm->Settings.APVersion == AP_OUTLOOK)
	{
	
		root_hItem = m_Tree.InsertItem("¦¬¥ó§X",0,1);
		ListOLFolder(root_hItem,g_MapiEx->lpInboxFolder);
		m_Tree.Expand(root_hItem,TVE_EXPAND);
	}

 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//#ifdef _OUTLOOKEX
void CMapiFolder::GetMailBoxFolder()
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
//#endif

//#ifdef _OUTLOOKEX
void CMapiFolder::ListFolder(HTREEITEM phItem ,STOREFOLDERID pFolder)
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
//#endif
//#ifdef _OUTLOOK
void CMapiFolder::ListOLFolder(HTREEITEM phItem , LPMAPIFOLDER  pFolder)
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
   
		//¦C¥X query
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
					m_Tree.SetItemData(hItem,m_FolderList.size());


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
//#endif

void CMapiFolder::OnButton1() 
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

			if (ItemIdx == 0)
			{
				memset(&m_ListData,0,sizeof(FolderListData));
				EndDialog(1);
				return;

			}

			if (ItemIdx > 0 )
			{
		
				m_ListData = (FolderListData) *itor;		
				EndDialog(1);
				return;
			}

		} 
		else if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
		{

		 
				if (selecteditem != root_hItem)
				{			
					CString FolderName = m_Tree.GetItemText(selecteditem);
					strcpy(m_ListData.FolderName,FolderName.GetBuffer(FolderName.GetLength()));
					m_ListData.oeFolder.OEFolderId =   m_Tree.GetItemData(selecteditem);
					FolderName.ReleaseBuffer();
					EndDialog(1);
					return;
				}
				else
				{
					memset(&m_ListData,0,sizeof(FolderListData));
					EndDialog(1);
					return;
				}
		}
		
		 

	}

	EndDialog(0);	
}

void CMapiFolder::OnButton2() 
{
	// TODO: Add your control notification handler code here
		EndDialog(0);
}
