// OEapi.cpp: implementation of the COEapi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <initguid.h>
#include "../Swvirus/Virus.h"
#include "../Swmatcher/BmMatch.h"
#include "../Swparser/MailParser.h"
#include "../Swsocket/CoreClass.h"
#include "OEapi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "MainFrm.h"
#include <conio.h>
#include <process.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


COEapi::COEapi()
{
 // InitializeCriticalSection(&m_CritSec); 
	
	m_pStoreNamespace = NULL; 
  //m_pInBoxStoreFolder = NULL;

  //m_pParent = pParent;

  m_InBoxWnd = NULL;
  m_OutBoxWnd = NULL;
  //m_OkBoxWnd = NULL;
  //m_SpamBoxWnd = NULL;
  m_LearnOkBoxWnd = NULL;
  m_LearnSpamBoxWnd = NULL;
  m_LearnWhiteBoxWnd = NULL;

  m_InBoxId = 0;
  m_OutBoxId = 0;
  m_OkBoxId = 0;
  m_SpamBoxId = 0;
  m_LearnOkBoxId = 0;
  m_LearnSpamBoxId = 0;
  m_LearnWhiteBoxId = 0;
  m_UnknownBoxId = 0;
  m_DeletedBoxId = 0;


  	m_pInBoxStoreFolder = NULL;
	m_pOutBoxStoreFolder = NULL;
	m_pOkBoxStoreFolder = NULL;
	m_pSpamBoxStoreFolder = NULL;
	m_pLearnOkBoxStoreFolder = NULL;
	m_pLearnSpamBoxStoreFolder = NULL;
	m_pLearnWhiteBoxStoreFolder = NULL;
	m_pDeletedStoreFolder = NULL;

	memset(SpamFolderName,0,sizeof(SpamFolderName));
	memset(OkFolderName,0,sizeof(OkFolderName));
	memset(UnknownFolderName,0,sizeof(UnknownFolderName));
	memset(LearnOkFolderName,0,sizeof(LearnOkFolderName));
	memset(LearnSpamFolderName,0,sizeof(LearnSpamFolderName)); 
	memset(LearnWhiteFolderName,0,sizeof(LearnWhiteFolderName)); 

#ifdef SPAMDOGFULL
	memset(SysMsgFolderName,0,sizeof(SysMsgFolderName)); 
#endif

	

	

  GetiniFolderSetup();

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

COEapi::~COEapi()
{
	NotificationOff();

	if (m_pInBoxStoreFolder != NULL)
	{
		m_pInBoxStoreFolder->Release();
	}

	if (m_pOutBoxStoreFolder != NULL)
	{
		m_pOutBoxStoreFolder->Release();
	}

	if (m_pOkBoxStoreFolder != NULL  && OkFolderName[0] != 0)
	{
		m_pOkBoxStoreFolder->Release();
	}
	 
	if (m_pSpamBoxStoreFolder != NULL)
	{
		m_pSpamBoxStoreFolder->Release();
	} 
	
	if (m_pLearnOkBoxStoreFolder != NULL)
	{
		m_pLearnOkBoxStoreFolder->Release();
	}

	if (m_pLearnSpamBoxStoreFolder != NULL)
	{
		m_pLearnSpamBoxStoreFolder->Release();
	}

	if (m_pLearnWhiteBoxStoreFolder != NULL)
	{
		m_pLearnWhiteBoxStoreFolder->Release();
	}


	if (m_pUnknownBoxStoreFolder != NULL)
	{
		m_pUnknownBoxStoreFolder->Release();
	}

	#ifdef SPAMDOGFULL
	if(m_pSysMsgStoreFolder != NULL)
	{
		m_pSysMsgStoreFolder->Release();
	}
	#endif

	if(m_pDeletedStoreFolder != NULL)
	{
		m_pDeletedStoreFolder->Release();
	}

	if(m_pStoreNamespace != NULL) 
	{
		m_pStoreNamespace->Release();
	}

 

	CoUninitialize();

	//DeleteCriticalSection (&m_CritSec); 
}


 

void COEapi::GetiniFolderSetup()
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
	#ifdef SPAMDOGFULL
	GetPrivateProfileString("FolderSetup","SysMsgFolderName","[系統訊息]",SysMsgFolderName,sizeof(SysMsgFolderName),IniPath);
	#endif
	
}



HRESULT COEapi::NotificationOn()
{
	HRESULT hr;
	//IStoreFolder *m_pStoreFolder = NULL;

	m_InBoxWnd = new COEWnd(this,INBOX_W);
	hr = m_pStoreNamespace->OpenFolder(m_InBoxId,0,&m_pInBoxStoreFolder);
	hr = m_pInBoxStoreFolder->RegisterNotification(0,m_InBoxWnd->GetWinHanlde());

	m_OutBoxWnd = new COEWnd(this,OUTBOX_W);
	hr = m_pStoreNamespace->OpenFolder(m_OutBoxId,0,&m_pOutBoxStoreFolder);
	hr = m_pOutBoxStoreFolder->RegisterNotification(0,m_OutBoxWnd->GetWinHanlde());


	if (OkFolderName[0] != 0)
	{
	   //m_OkBoxWnd = new COEWnd(this,OKBOX_W); 
	   hr = m_pStoreNamespace->OpenFolder(m_OkBoxId,0,&m_pOkBoxStoreFolder);
	   //hr = m_pOkBoxStoreFolder->RegisterNotification(0,m_OkBoxWnd->GetWinHanlde());
	   //m_pStoreFolder->Release();
	}
	else
	{
		m_pOkBoxStoreFolder= m_pInBoxStoreFolder;
	}
	
	//m_SpamBoxWnd = new COEWnd(this,SPAMBOX_W);
	hr = m_pStoreNamespace->OpenFolder(m_SpamBoxId,0,&m_pSpamBoxStoreFolder);

	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);
	hr = m_pSpamBoxStoreFolder->GetFolderProps(0,&fprops);

	
	if (SUCCEEDED(hr))
	{
			if (fprops.cMessage > 0)
			{
				CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();
				dlg->ChangeTrashIcon(1);
			}
		 
	}

	//hr = m_pSpamBoxStoreFolder->RegisterNotification(0,m_SpamBoxWnd->GetWinHanlde());
	//m_pStoreFolder->Release();

	//m_pStoreFolder=NULL;
	m_LearnOkBoxWnd = new COEWnd(this,LEARNOKBOX_W); 
	hr = m_pStoreNamespace->OpenFolder(m_LearnOkBoxId,0,&m_pLearnOkBoxStoreFolder);
	hr = m_pLearnOkBoxStoreFolder->RegisterNotification(0,m_LearnOkBoxWnd->GetWinHanlde());
	//m_pStoreFolder->Release();

	//m_pStoreFolder=NULL;
	m_LearnSpamBoxWnd = new COEWnd(this,LEARNSPAMBOX_W);
	hr = m_pStoreNamespace->OpenFolder(m_LearnSpamBoxId,0,&m_pLearnSpamBoxStoreFolder);
	hr = m_pLearnSpamBoxStoreFolder->RegisterNotification(0,m_LearnSpamBoxWnd->GetWinHanlde());
	//m_pStoreFolder->Release();

	m_LearnWhiteBoxWnd = new COEWnd(this,LEARNWHITEBOX_W);
	hr = m_pStoreNamespace->OpenFolder(m_LearnWhiteBoxId,0,&m_pLearnWhiteBoxStoreFolder);
	hr = m_pLearnWhiteBoxStoreFolder->RegisterNotification(0,m_LearnWhiteBoxWnd->GetWinHanlde());
	//m_pStoreFolder->Release();


	hr = m_pStoreNamespace->OpenFolder(m_UnknownBoxId,0,&m_pUnknownBoxStoreFolder);

#ifdef SPAMDOGFULL
	hr = m_pStoreNamespace->OpenFolder(m_SysMsgBoxId,0,&m_pSysMsgStoreFolder);
#endif
	hr = m_pStoreNamespace->OpenFolder(m_DeletedBoxId,0,&m_pDeletedStoreFolder);
	
   
 


	return hr;
}

HRESULT COEapi::NotificationOff()
{

	HRESULT hr = S_OK;
 

	if (m_InBoxWnd != NULL) 
	{
	
		hr = m_pInBoxStoreFolder->UnregisterNotification(0,m_InBoxWnd->GetWinHanlde());
		

		delete m_InBoxWnd;
	}

	if (m_OutBoxWnd != NULL) 
	{
	
		hr = m_pOutBoxStoreFolder->UnregisterNotification(0,m_OutBoxWnd->GetWinHanlde());
		

		delete m_OutBoxWnd;
	}
 
	if (m_LearnOkBoxWnd  != NULL)	
	{
		
		hr = m_pLearnOkBoxStoreFolder->UnregisterNotification(0,m_LearnOkBoxWnd->GetWinHanlde());
		
		delete m_LearnOkBoxWnd;
	}
	if (m_LearnSpamBoxWnd  != NULL) 
	{
		
		hr = m_pLearnSpamBoxStoreFolder->UnregisterNotification(0,m_LearnSpamBoxWnd->GetWinHanlde());
		
		delete m_LearnSpamBoxWnd;
	}

	if (m_LearnWhiteBoxWnd  != NULL) 
	{
		
		hr = m_pLearnWhiteBoxStoreFolder->UnregisterNotification(0,m_LearnWhiteBoxWnd->GetWinHanlde());
		
		delete m_LearnWhiteBoxWnd;
	}

	 
	return hr;
}

void COEapi::OnEvent(UINT uMsg, WPARAM wParam , int HNDNo)
{

	
	if (HNDNo == INBOX_W )
	{		
		ProcessMsg((MESSAGEID) wParam,HNDNo);	
	}
	else if (HNDNo == OKBOX_W)
	{	
		OutputDebugString("OkBox\n");		

	}
	else if (HNDNo == OUTBOX_W)
	{
	   //處理寄件匣
		ProcessOutMsg((MESSAGEID) wParam);
		
	}	
	else if (HNDNo == LEARNOKBOX_W)
	{
	
		ProcessMsg((MESSAGEID) wParam,HNDNo);	
	}
	else if (HNDNo == LEARNSPAMBOX_W)
	{
	
		ProcessMsg((MESSAGEID) wParam,HNDNo);	
	} else if (HNDNo == LEARNWHITEBOX_W)
	{
		ProcessWhiteMsg((MESSAGEID) wParam);
	}



}

HRESULT COEapi::OpenInbox()
{
   //處理 folder

	//HRESULT hr = m_pStoreNamespace->OpenSpecialFolder(FOLDER_INBOX,0,&m_pInBoxStoreFolder);
	//if (FAILED(hr))
	//{	
	//	goto quit;
	//}

	STOREFOLDERID dwFolderId = FOLDERID_ROOT;
	FOLDERPROPS props,props2;
	HENUMSTORE hEnum,hEnum2;
	
	bool bOkFolder=false;
	bool bSpamFolder=false;
	bool bUnknownFolder=false;
	bool bLearnOkFolder=false;
	bool bLearnSpamFolder=false;
	bool bLearnWhiteFolder=false;
	bool bSysMsgFolder = false;
		

	hEnum2 = hEnum = NULL;
	
	props.cbSize = sizeof(FOLDERPROPS);
	props2.cbSize = sizeof(FOLDERPROPS);

	HRESULT hr = m_pStoreNamespace->GetFirstSubFolder(dwFolderId, &props, &hEnum);
	if(FAILED(hr)) 
	{
		//throw _T("Error initializing IStoreNamespace.");
		//m_pStoreNamespace->Release();
		 
		//return S_FALSE;
		goto quit;
	}
 
	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) 
	{
		
		//nIndex = m_listFolder.AddString(props.szName);		
	    if (props.sfType == FOLDER_INBOX)
		{
		 

			m_InBoxId = props.dwFolderId;
			OutputDebugString(props.szName);
			OutputDebugString("\n");
			
			//列出下層
			hr = m_pStoreNamespace->GetFirstSubFolder(props.dwFolderId, &props2, &hEnum2);
			if(FAILED(hr)) 
			{
						goto quit;
			}

			while(SUCCEEDED(hr) && hr != S_FALSE && hEnum2 != NULL)
			{
			
				
				//FolderListData mListData;
				//strcpy(mListData.FolderName,props2.szName);
				
				//mListData.oeFolder.OEFolderId = props2.dwFolderId;				
				//m_FolderList.insert(m_FolderList.end(),mListData);

				//OutputDebugString(props2.szName);
				//OutputDebugString("\n");

				if (strcmp(SpamFolderName,props2.szName) == 0)
				{
				    m_SpamBoxId = props2.dwFolderId;
					bSpamFolder = true;
				}
				else if (strcmp(OkFolderName,props2.szName) == 0)
				{				
					m_OkBoxId = props2.dwFolderId;
					bOkFolder = true;
				}
				else if (strcmp(UnknownFolderName,props2.szName) == 0)
				{
				  	m_UnknownBoxId = props2.dwFolderId;
					bUnknownFolder = true;
				}
				else if (strcmp(LearnOkFolderName,props2.szName) == 0)
				{
				 	m_LearnOkBoxId = props2.dwFolderId;
					bLearnOkFolder = true;
				}
				else if (strcmp(LearnSpamFolderName,props2.szName) == 0)
				{
				  	m_LearnSpamBoxId = props2.dwFolderId;
					bLearnSpamFolder = true;
				}
				else if (strcmp(LearnWhiteFolderName,props2.szName) == 0)
				{
				  	m_LearnWhiteBoxId = props2.dwFolderId;
					bLearnWhiteFolder = true;
				}
				#ifdef SPAMDOGFULL
				else if (strcmp(SysMsgFolderName,props2.szName) == 0)
				{
				  	m_SysMsgBoxId = props2.dwFolderId;
					bSysMsgFolder = true;
				}
				#endif

				hr = m_pStoreNamespace->GetNextSubFolder(hEnum2, &props2);			
			}

			if(hEnum2) 
			{
				m_pStoreNamespace->GetSubFolderClose(hEnum2);
			}

			//break;

		
		}
		else if (props.sfType == FOLDER_SENT  )
		{
			m_OutBoxId = props.dwFolderId;
			OutputDebugString(props.szName);
			OutputDebugString("\n");
		}
		else if (props.sfType == FOLDER_DELETED )
		{
			m_DeletedBoxId = props.dwFolderId;
			 
		}

		hr = m_pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	if(hEnum) 
	{
		m_pStoreNamespace->GetSubFolderClose(hEnum);
	}

	//建立目錄
	if (OkFolderName[0] == 0)
	{
		  m_OkBoxId = m_InBoxId;
	}
	
	if (!bOkFolder)
	{
		m_pStoreNamespace->CreateFolder(m_InBoxId,OkFolderName,0,&m_OkBoxId);
	}

	if (!bSpamFolder)
	{
		m_pStoreNamespace->CreateFolder(m_InBoxId,SpamFolderName,0,&m_SpamBoxId);
	}

	if (!bUnknownFolder)
	{
		m_pStoreNamespace->CreateFolder(m_InBoxId,UnknownFolderName,0,&m_UnknownBoxId);
	}

	if (!bLearnOkFolder)
	{
		m_pStoreNamespace->CreateFolder(m_InBoxId,LearnOkFolderName,0,&m_LearnOkBoxId);
	}

	if (!bLearnSpamFolder)
	{
		m_pStoreNamespace->CreateFolder(m_InBoxId,LearnSpamFolderName,0,&m_LearnSpamBoxId);
	}

	if (!bLearnWhiteFolder)
	{
	    m_pStoreNamespace->CreateFolder(m_InBoxId,LearnWhiteFolderName,0,&m_LearnWhiteBoxId);
	}

	#ifdef SPAMDOGFULL
	if (!bSysMsgFolder)
	{
		m_pStoreNamespace->CreateFolder(m_InBoxId,SysMsgFolderName,0,&m_SysMsgBoxId);	
	}
	#endif

quit:
	return hr;
}


HRESULT COEapi::Login()
{
	HRESULT hr = CoInitialize(0);
	if(FAILED(hr)) 
	{
		throw _T("Error CoInitialize.");
		return S_FALSE;
		 
	}

	hr = CoCreateInstance(CLSID_StoreNamespace, NULL, CLSCTX_SERVER, 
									IID_IStoreNamespace, (LPVOID*) &m_pStoreNamespace);
	if(FAILED(hr)) 
	{
		throw _T("Error creating IStoreNamespace.");		 
		return S_FALSE; 
	}


	
	hr = m_pStoreNamespace->Initialize(AfxGetMainWnd()->m_hWnd, 1);
	if(FAILED(hr)) 
	{
		throw _T("Error initializing IStoreNamespace.");
		m_pStoreNamespace->Release();
		return S_FALSE; 
		 
	}

	 
	hr = OpenInbox();

	

	return hr;

}

void COEapi::ProcessWhiteMsg(MESSAGEID MsgId)
{
	//加入 rule

	char *Sender=NULL;
	CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();

	MESSAGEPROPS props;
	props.cbSize = sizeof(MESSAGEPROPS);
	HRESULT hr = m_pLearnWhiteBoxStoreFolder->GetMessageProps(MsgId,0,&props);

	
		
	if (SUCCEEDED(hr))
	{ 
			Sender = props.pszDisplayFrom;
	 

			/*IMimeMessage *imsg;		
			HRESULT hr = m_pLearnWhiteBoxStoreFolder->OpenMessage(MsgId,IID_IMimeMessage,(VOID **) &imsg);

			if (SUCCEEDED(hr))
			{

				ADDRESSPROPS addprops;
				hr = imsg->GetSender(&addprops);
				if (SUCCEEDED(hr))
					Sender = addprops.pszFriendly;*/


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
				strcat(m_RuleFile.Rule,Sender);	


				//檢查重覆
				bool bDoubleAdd = false;
				RuleFile old_m_RuleFile;
				while(fread(&old_m_RuleFile,1,sizeof(RuleFile),fp))
				{
					if (strcmp(old_m_RuleFile.Rule,m_RuleFile.Rule) == 0)
					{
						//重覆
						if (old_m_RuleFile.Used) //有效鍵
						{
							//fclose(fp);
							//imsg->Release();
							//delete TotalMailBuff;
							//信件移回
						    //MoveMsg(MsgId,m_pLearnWhiteBoxStoreFolder , m_pInBoxStoreFolder);

							//dlg->m_MainStatic.SetWindowText("Express 監控中..");
							//dlg->ChangeIcon(0);
							bDoubleAdd = true;
							break;

							//return;
						}
						
					}
				}
							
	 

					//unsigned int newidx = rulehead.LastId + 1;
					//rulehead.LastId = newidx;

					//update 更新idx
					//fseek(fp,0,SEEK_SET);
					//fwrite(&rulehead,1,sizeof(rulehead),fp);
						m_RuleFile.AlertType = 1;//正常
						m_RuleFile.CheckType = 1;//寄件者
						m_RuleFile.ShowColor = SysOkMailColor;
						m_RuleFile.RuleType = 2; //追蹤器
						//m_RuleFile.RuleId = newidx;
						m_RuleFile.Used = true;
						


					

                   //信件移動						
					//ArrangeMail(m_pLearnWhiteBoxStoreFolder , NULL , MsgId);
				    
				

					if (!bDoubleAdd)
					{ 
						

					/*	STOREFOLDERID FolderIdRoot = FindFolder(m_InBoxId , dlg->Settings.ArrangeMailFolder);
						if (FolderIdRoot > 0)
						{
							STOREFOLDERID FolderIdMail = FindFolder(FolderIdRoot , Sender);
							if (FolderIdMail > 0 )
							{
									strcpy(m_RuleFile.ListData.FolderName, Sender);
									m_RuleFile.ListData.oeFolder.OEFolderId =  FolderIdMail;
									
							}
						}*/

						fseek(fp,0,SEEK_END); 
						//save to file
						fwrite(&m_RuleFile,1,sizeof(RuleFile),fp); 


					}
				 

					
					fclose(fp);				

					MoveMsg(MsgId,m_pLearnWhiteBoxStoreFolder,m_pInBoxStoreFolder);
					m_pLearnWhiteBoxStoreFolder->FreeMessageProps (&props);



					CString tmpstr = Sender;
					tmpstr += " 已加入白名單!";											

					ShowMsg(m_RuleFile.ShowColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()),m_RuleFile.AlertType);					
					tmpstr.ReleaseBuffer();		

					dlg->m_MainStatic.SetWindowText("Express 監控中..");
					dlg->ChangeIcon(0);

					dlg->GetIniRule();

					

					return;
			}



}

void COEapi::ProcessOutMsg(MESSAGEID MsgId)
{
	//處理是否開啟
	CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();
	if (!dlg->Settings.MailTracer) return;

		char *Subject=NULL;
		char *Sender=NULL;
	//	IMimeMessage *imsg;
		HRESULT hr;

		MESSAGEPROPS props;
		props.cbSize = sizeof(MESSAGEPROPS);
		hr = m_pOutBoxStoreFolder->GetMessageProps(MsgId,0,&props);
		
		if (SUCCEEDED(hr))
		{ 
			Sender = props.pszDisplayFrom;
			Subject = props.pszSubject;			 

			//移除特定字
			//FW: 先不處理
			//|| strnicmp(Subject,"FW: ",4) == 0
			if (strnicmp(Subject,"RE: ",4) == 0 )
			{
				strcpy(Subject,Subject+4);
			} 
			else if (strnicmp(Subject,"FW: ",4) == 0 )
			{
				return;
			}
		}

		
		/*
		IMimeMessage *imsg;	 
		hr = m_pOutBoxStoreFolder->OpenMessage(MsgId,IID_IMimeMessage,(VOID **) &imsg);

		if (SUCCEEDED(hr))
		{
			ADDRESSPROPS addprops;
			hr = imsg->GetSender(&addprops);
			if (SUCCEEDED(hr))
			{
				Sender = addprops.pszFriendly;
			}
		}*/

	
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


		//檢查重覆
		RuleFile old_m_RuleFile;
		while(fread(&old_m_RuleFile,1,sizeof(RuleFile),fp))
		{
			if (strcmp(old_m_RuleFile.Rule,m_RuleFile.Rule) == 0)
			{
				//重覆
			   if (old_m_RuleFile.Used) //有效鍵
			   {
					fclose(fp);
					//imsg->Release();
					m_pOutBoxStoreFolder->FreeMessageProps (&props);


					return;
			   }


			}
		}
		 
	 

			unsigned int newidx = rulehead.LastId + 1;
			rulehead.LastId = newidx;

			//update 更新idx
			fseek(fp,0,SEEK_SET);
			fwrite(&rulehead,1,sizeof(rulehead),fp);
			fseek(fp,0,SEEK_END);

		 
		

			m_RuleFile.AlertType = 2;//特別
			m_RuleFile.CheckType = 2;//依主旨
			m_RuleFile.ShowColor = SysOkMailColor;
			m_RuleFile.RuleType = 1; //追蹤器
			m_RuleFile.RuleId = newidx;
			m_RuleFile.Used = true;

			//有效期限 , 七天
			CTime t1((time_t) CTime::GetCurrentTime().GetTime());
			CTimeSpan ts1( dlg->Settings.TracerExpire, 0, 0, 0);    
			CTime t2(t1+ts1);

			m_RuleFile.ExpireTime = (long) t2.GetTime();
		

			//save to file
			fwrite(&m_RuleFile,1,sizeof(RuleFile),fp);


 			

			fclose(fp);


	    //imsg->Release();
		m_pOutBoxStoreFolder->FreeMessageProps (&props);

		
		dlg->GetIniRule();

	 

}

/*
void COEapi::ProcessLearnMsg(IStoreFolder *storefolder, MESSAGEID MsgId, char LearnType )
{

	CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();

		char *Sender=NULL;
		char *Subject=NULL;		
		
		char IP[16];
		IP[0] = 0;

		char ReceiveTime[255];
		ReceiveTime[0] = 0;

		char MsgIdStr[255];
		//itoa(MsgId , MsgIdStr , 10);

		CChecker mc;

		IStream *pTextStream = NULL;
		HRESULT hr2;


		IMimeMessage *imsg;
		HRESULT hr;

		MESSAGEPROPS props;
		hr = storefolder->GetMessageProps(MsgId,0,&props);

		if (SUCCEEDED(hr))
		{				
			Subject = props.pszSubject;			

			SYSTEMTIME SystemCreate;
				
			FileTimeToSystemTime(&props.ftReceived,&SystemCreate);

			CString msgidstr;
			msgidstr.Format("%02d%02d%02d%02d%02d%02d%04d",
										SystemCreate.wYear,
										SystemCreate.wMonth,
										SystemCreate.wDay,
										SystemCreate.wHour,
										SystemCreate.wMinute,
										SystemCreate.wSecond,
										SystemCreate.wMilliseconds); 

			strcpy(MsgIdStr,msgidstr.GetBuffer(msgidstr.GetLength()));
			msgidstr.ReleaseBuffer();
		}

		hr = storefolder->OpenMessage(MsgId,IID_IMimeMessage,(VOID **) &imsg);
		if (SUCCEEDED(hr))
		  hr2 = storefolder->OpenMessage(MsgId,IID_IStream,(VOID **) &pTextStream);

			//LARGE_INTEGER    dMove ={0}  ; 
			//pTextStream->Seek(dMove,STREAM_SEEK_SET,NULL);
					
			char buffer[4096*2];
			buffer[0] = 0;

			ULONG ulReaded = 0;
			
			hr2 = pTextStream->Read(buffer, sizeof(buffer)-1, &ulReaded);
			if(SUCCEEDED(hr2)) 
			{
				buffer[ulReaded] = 0;
			}
			

			if (buffer[0] != 0)
			{
				//取出 header
				int len = strlen(buffer);

						for (int i = 0 ; i < len ; i ++)
						{
							if (buffer[i] == 13 &&
								buffer[i+1] == 10 &&
								buffer[i+2] == 13 &&
								buffer[i+3] == 10)
							{
								buffer[i] = 0;
							
								char IniPath[_MAX_PATH];	   			   

								strcpy(IniPath,WorkPath);										   
								strcat(IniPath,"SpamDog.ini");

								if (dlg->Settings.IPident[0] == 0)
								{
									//mc->GetLastReceiveBy(buffer,dlg->Settings.IPident);
									mc->GetHeaderIP(buffer,dlg->Settings.IPident,IP,ReceiveTime); 
									// + |
									//Save To iniFile
									if (dlg->Settings.IPident[0] != 0)
									{
										strcat(dlg->Settings.IPident,"|");
										WritePrivateProfileString("SysSetup","IPident",dlg->Settings.IPident,IniPath);

										//mc->GetHeaderIP(buffer,dlg->Settings.IPident,IP,ReceiveTime); //再取一次
									}
									
								}
								else
								{
								    char TempIdent[255];
									strcpy(TempIdent, dlg->Settings.IPident);

									if (mc.GetHeaderIP(buffer,TempIdent,IP,ReceiveTime) == 0) 
									{
									  //沒找到 ident
										if (TempIdent[0] != 0)
										{
										 
											
											strcat(dlg->Settings.IPident,TempIdent);
											strcat(dlg->Settings.IPident,"|");

											WritePrivateProfileString("SysSetup","IPident",dlg->Settings.IPident,IniPath);

											
										}

									}
  
									//Save To iniFile
								}							
								 
								break;
							
						}
					}


					pTextStream->Release();

				}



		if (SUCCEEDED(hr))
		{
			 
			ADDRESSPROPS addprops;
			hr = imsg->GetSender(&addprops);
			if (SUCCEEDED(hr))
				Sender = addprops.pszFriendly;

			//檢查附檔病毒
			//取 Attatch
			ULONG AttCount = 0; 
			HBODY *AttBodys = NULL;
			IMalloc *malloc=NULL;
			CoGetMalloc(1,&malloc);

			//bool HasVirus = false;

			int buffsize = 4096;				 
			char *MailBuff = new char[buffsize];
			//Simple Null
			MailBuff[0] = 0;
			int TotalSize = 0;

			hr = imsg->GetAttachments(&AttCount,&AttBodys);
			if (SUCCEEDED(hr))
			{
				for (int i= 0 ;  i < AttCount ; i++)
				{
					IMimeBody *pBody=NULL;
					hr = imsg->BindToObject(AttBodys[i],IID_IMimeBody, (LPVOID *)&pBody);

					if (SUCCEEDED(hr))
					{
						char *display=NULL;
						pBody->GetDisplayName(&display);				

						CString FileName = display;
						//CVirus cv;

						//if ((pBody->IsContentType("multipart","html") == S_OK ) || FileName.Find(".htm") != -1)
						if ((pBody->IsContentType(NULL,"html") == S_OK ) ||  (pBody->IsContentType(NULL,"rfc822") == S_OK )
							||  (pBody->IsContentType(NULL,"plain") == S_OK )) 
						{
						
							IStream *stream = NULL;
							if (SUCCEEDED(pBody->GetData(IET_BINARY , &stream)))
							{
							
								bool done = false;

								while (!done)
								{ 
									ULONG readsize; 
									HRESULT hRes = stream->Read(MailBuff+TotalSize, buffsize-TotalSize, &readsize);						  
						  
									if (hRes!=S_OK) 
									{
							 
										done=true;
									}
									else
									{ 
											TotalSize+=readsize; 
											done =  (readsize < buffsize-TotalSize);						

											if (!done)
											{ 
												unsigned int newsize=2*TotalSize;
								   
												char *newbuf=new char[newsize];
								  
												memcpy(newbuf,MailBuff,TotalSize); 
								  
												delete [] MailBuff;
												MailBuff=newbuf; 
												buffsize=newsize;
											}
									}
								}

								MailBuff[TotalSize] = 0;
							}

							stream->Release();
						
						} 

						malloc->Free(display);
					}
				}
				 
			}

			malloc->Free(AttBodys);



			int Textbuffsize = 4096;				 
			char *MailTextbuf = new char[Textbuffsize];
			MailTextbuf[0] = 0;



			HBODY hbody;
			IStream *pIStream = NULL;
			//取 html
			hr = imsg->GetTextBody(TXT_HTML,IET_BINARY,(IStream **) &pIStream,&hbody);
			if (SUCCEEDED(hr))
			{
			

				//int TotalSize = 0;
				bool done = false;

				while (!done)
				{ 
					ULONG readsize; 
					HRESULT hRes = pIStream->Read(MailBuff+TotalSize, buffsize-TotalSize, &readsize);						  
						  
					if (hRes!=S_OK) 
					{
							 
							  done=true;
					}
					else
					{ 
							  TotalSize+=readsize; 
							  done =  (readsize < buffsize-TotalSize);						

						      if (!done)
							  { 
								  unsigned int newsize=2*TotalSize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,MailBuff,TotalSize); 
								  
								  delete [] MailBuff;
								  MailBuff=newbuf; 
								  buffsize=newsize;
							  }
					}
				}

				MailBuff[TotalSize] = 0;

				if (TotalSize > 0)
				{				
					CMailCodec mailc;
					delete MailTextbuf;
					MailTextbuf = new char[TotalSize];
					mailc.TrimHTML(MailBuff,MailTextbuf);
					 
				}

				pIStream->Release();
			}
			else
			{
				//取 html 師敗
				hr = imsg->GetTextBody(TXT_PLAIN,IET_BINARY,(IStream **) &pIStream,&hbody);
				if (SUCCEEDED(hr))
				{
						int TotalSize = 0;
						bool done = false;

						while (!done)
						{ 
							ULONG readsize; 
							HRESULT hRes = pIStream->Read(MailTextbuf+TotalSize, Textbuffsize-TotalSize, &readsize);						  
						  
							if (hRes!=S_OK) 
							{
								 
							  done=true;
							}
							else
							{ 
							  TotalSize+=readsize; 
							  done =  (readsize < Textbuffsize-TotalSize);						

						      if (!done)
							  { 
								  unsigned int newsize=2*TotalSize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,MailTextbuf,TotalSize); 
								  
								  delete [] MailTextbuf;
								  MailTextbuf=newbuf; 
								  Textbuffsize=newsize;
							  }
							}
						}

					MailTextbuf[TotalSize] = 0;	
					//if (TotalSize > 0)
					//{
					//	MailTextbuf = MailBuff;
					//}
					pIStream->Release();
				}
			}
		

				mc.GetCheckKeys(MailBuff,MailTextbuf,Sender,Subject,IP,dlg->Settings.SpamSubject);							
				if (mc.ResList.size() > 0  || mc.SecResList.size() > 0)
				{
										
					mc.GetSpamData();					
					
					if (mc.DataList.size()  > 0 || mc.SecDataList.size() > 0)
					{

						if (LearnType == 2)
						{
							mc.UpdateSpamRate(1,0); //+ OK 
						} else
						{
							mc.UpdateSpamRate(0,1); //+ Spam
						}
					}
				}



				delete MailTextbuf;			 
				delete MailBuff;


		}


		storefolder->FreeMessageProps (&props);

}
*/
void COEapi::ProcessMsg(MESSAGEID MsgId,int HNDNo)
{
		//IStoreFolder *m_pStoreFolder = NULL;
	    //HRESULT hr = m_pStoreNamespace->OpenFolder(m_InBoxId,0,&m_pStoreFolder);	 

		
		//EnterCriticalSection(&m_CritSec);	

	
	

		CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd();

		//if (dlg->Settings.ADText != 0)
		//  dlg->ShowToolTip(dlg->Settings.ADTitle,dlg->Settings.ADText, 10 * 1000);

		//if (!dlg->Settings.Licensed )
		//dlg->Settings.ShowAD = true;

			//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, dlg->m_hWnd,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				while (::PeekMessage(&msg, dlg->MsgFrm.m_hWnd,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

		

		dlg->Settings.LastProcessTime = CTime::GetCurrentTime();

		dlg->m_MainStatic.SetWindowText("信件檢查中");
		dlg->ChangeIcon(1);

		dlg->GetClientProcessId();//取 Client Id

		LPSTR Sender=NULL;
		//char *Sender=NULL;
		char *Subject=NULL;	
		bool LocalMail = false;
		MailPath m_MailPath;
		m_MailPath.NextWrite = -2;


		char IP[16];
		IP[0] = 0;

		char ReceiveTime[255];
		ReceiveTime[0] = 0;

		//unsigned ThreadId;

		char MsgIdStr[255];
		MsgIdStr[0] = 0;
		//itoa(MsgId , MsgIdStr , 10);

		int Mailsize = 0;
		int bufsize = 10240;
		char* TotalMailBuff = new char[bufsize];

		bool SysCheckMail = false;
		bool IsSpam = false;

		//EnterCriticalSection(&dlg->m_CritSec);
		CChecker *mc = new CChecker(&dlg->m_DBSec);

		IStream *pTextStream = NULL;
		HRESULT hr2;


		IMimeMessage *imsg=NULL;
		HRESULT hr;

		MESSAGEPROPS props;
		props.cbSize = sizeof(MESSAGEPROPS);
		if (HNDNo == INBOX_W)
		{
		    hr = m_pInBoxStoreFolder->GetMessageProps(MsgId,0,&props);
		}
		else if (HNDNo == LEARNOKBOX_W)
		{
			hr = m_pLearnOkBoxStoreFolder->GetMessageProps(MsgId,0,&props);
		}
		else if (HNDNo == LEARNSPAMBOX_W)
		{
			hr = m_pLearnSpamBoxStoreFolder->GetMessageProps(MsgId,0,&props);
		}

	
		if (SUCCEEDED(hr))
		{		
			//OutputDebugString(props.pszSubject);	
			
			Subject = props.pszSubject;

			/*char IniPath[_MAX_PATH];	   

			strcpy(IniPath,dlg->PGPath);		 	   
			strcat(IniPath,"SpamDog.ini");

			WritePrivateProfileString("SysSetup","LastSubject",Subject,IniPath);*/

			if (!dlg->Settings.Licensed || (dlg->Settings.IpSourceIdent && dlg->Settings.NeedReport ))
			{

				SYSTEMTIME SystemCreate;
				FileTimeToSystemTime(&props.ftReceived,&SystemCreate);

				CString msgidstr;
				msgidstr.Format("%02d%02d%02d%02d%02d%02d%06d",
											SystemCreate.wYear,
											SystemCreate.wMonth,
											SystemCreate.wDay,
											SystemCreate.wHour,
											SystemCreate.wMinute,
											SystemCreate.wSecond,
											props.cbMessage); 

				strcpy(MsgIdStr,msgidstr.GetBuffer(msgidstr.GetLength()));
				msgidstr.ReleaseBuffer();

			}
			

		
		}

		
		

		if (HNDNo == INBOX_W )
		{				
			hr = m_pInBoxStoreFolder->OpenMessage(MsgId,IID_IMimeMessage,(VOID **) &imsg);	 
			hr2 = -1;


			if (SUCCEEDED(hr))			
			  hr2 = m_pInBoxStoreFolder->OpenMessage(MsgId,IID_IStream,(VOID **) &pTextStream);
			
		}
		else if (HNDNo == LEARNOKBOX_W)
		{
			hr = m_pLearnOkBoxStoreFolder->OpenMessage(MsgId,IID_IMimeMessage,(VOID **) &imsg);		
			hr2 = -1;

			if (SUCCEEDED(hr))
			hr2 = m_pLearnOkBoxStoreFolder->OpenMessage(MsgId,IID_IStream,(VOID **) &pTextStream);

		} else if (HNDNo == LEARNSPAMBOX_W)
		{
			hr = m_pLearnSpamBoxStoreFolder->OpenMessage(MsgId,IID_IMimeMessage,(VOID **) &imsg);
			hr2 = -1;
			
			if (SUCCEEDED(hr))
			hr2 = m_pLearnSpamBoxStoreFolder->OpenMessage(MsgId,IID_IStream,(VOID **) &pTextStream);
		}

		
				
				if (SUCCEEDED(hr2))
				{
					
					//讀出所有資料					 
				
					bool done = false;
					while (!done)
					{ 
							ULONG readsize; 
							HRESULT hRes = pTextStream->Read(TotalMailBuff+Mailsize, bufsize-Mailsize, &readsize);						  
						  
							if (hRes!=S_OK) 
							{							
							  done=true;
							}
							else
							{ 
							   

							  Mailsize+=readsize; 
							  done =  (readsize < bufsize-Mailsize);						  

						      if (!done)
							  { 
								  unsigned int newsize=2*Mailsize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,TotalMailBuff,Mailsize); 
								  //delete[] htmlbuf;
								  delete [] TotalMailBuff;
								  TotalMailBuff=newbuf; 
								  bufsize=newsize;
							  }
							}
					}

			 
					TotalMailBuff[Mailsize] = 0;

					//讀出 header
					

					LARGE_INTEGER    dMove ={0}  ; 
					pTextStream->Seek(dMove,STREAM_SEEK_SET,NULL);
					
					char buffer[4096*2];
					buffer[0] = 0;

					ULONG ulReaded = 0;
					//do {
							hr2 = pTextStream->Read(buffer, sizeof(buffer)-1, &ulReaded);
							if(SUCCEEDED(hr2)) 
							{
								buffer[ulReaded] = 0;
							}

						//} while(SUCCEEDED(hr2) && ulReaded != 0);

					if (buffer[0] != 0)
					{
						//取出 header
						int len = strlen(buffer);

						for (int i = 0 ; i < len ; i ++)
						{
							if (buffer[i] == 13 &&
								buffer[i+1] == 10 &&
								buffer[i+2] == 13 &&
								buffer[i+3] == 10)
							{
							
								

								buffer[i] = 0;	
								
							
#ifdef SPAMDOGFULL
								if (Subject != NULL && strncmp(Subject,"SpamDog Mail GateWay Testing",28) == 0)
								{
									CChecker::ReportSysMail(MsgIdStr,buffer);
									SysCheckMail = true;
								}
								else
								{
#endif
									if (dlg->Settings.Licensed && !dlg->Settings.IpSourceIdent && dlg->Settings.AuthEx)
									{
										if (mc->CheckLocalMail(buffer))
										  LocalMail = true;
									}
									else
									{
										int rtn = 0;

										if (dlg->Settings.Licensed && dlg->Settings.IpStrIdent)
										{
											rtn =	mc->GetKeyStrIP(buffer,dlg->Settings.IpKeyStr,IP); 
											if (rtn == -2)
												rtn =	mc->GetHeaderIP(Subject,buffer,IP,&m_MailPath); 

										}
										else
										{
											rtn =	mc->GetHeaderIP(Subject,buffer,IP,&m_MailPath); 
										}
										
										if (rtn == 1) LocalMail = true;
									}


									if (dlg->Settings.RBLMaintain)
									{

										if (dlg->Settings.HeaderKeyStr[0] != 0)
										{
										
											mc->GetHeaderKeyStrIP(buffer,dlg->Settings.HeaderKeyStr,IP);

										} else if (dlg->Settings.HeaderKeyPos > 0)
										{
											mc->GetKeyPosIP(buffer,dlg->Settings.HeaderKeyPos,IP);										
										}								
									
									
									}

#ifdef SPAMDOGFULL
								}
#endif									
							 

							 
								break;
							}
						}
					}


					pTextStream->Release();

				}
		 


		if (SUCCEEDED(hr))
		{
			 #ifdef SPAMDOGFULL
			if (SysCheckMail)
			{
			

					CString tmpstr;
					
					tmpstr = "發現SpamDog系統信件 \r\n\r\n主旨: ";	
					tmpstr += Subject;		
					 
					MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSysMsgStoreFolder);					 
						 
					ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();					
					
					goto quit;
			
			}
		   #endif

			Sender = props.pszDisplayFrom;

#ifdef _CONSOLEDBG
			_cprintf("%s\r\n",Sender );
#endif

		
			//檢查附檔病毒
			//取 Attatch
			ULONG AttCount = 0; 
			HBODY *AttBodys = NULL;
			IMalloc *malloc=NULL;
			CoGetMalloc(1,&malloc);

			bool HasVirus = false;
		

			int buffsize = 4096;				 
			char *MailBuff = new char[buffsize];
			//Simple Null
			MailBuff[0] = 0;
			int TotalSize = 0;

			hr = imsg->GetAttachments(&AttCount,&AttBodys);
			if (SUCCEEDED(hr))
			{
				for (int i= 0 ;  i < AttCount ; i++)
				{
					IMimeBody *pBody=NULL;
					hr = imsg->BindToObject(AttBodys[i],IID_IMimeBody, (LPVOID *)&pBody);

					if (SUCCEEDED(hr))
					{
						char *display=NULL;
						pBody->GetDisplayName(&display);				

						CString FileName = display;
						CVirus cv;

						//if ((pBody->IsContentType("multipart","html") == S_OK ) || FileName.Find(".htm") != -1)
						if ((pBody->IsContentType(NULL,"html") == S_OK ) ||  (pBody->IsContentType(NULL,"rfc822") == S_OK )
							||  (pBody->IsContentType(NULL,"plain") == S_OK )) 
						{
						
							IStream *stream = NULL;
							if (SUCCEEDED(pBody->GetData(IET_BINARY , &stream)))
							{
							
								bool done = false;

								while (!done)
								{ 
									ULONG readsize; 
									HRESULT hRes = stream->Read(MailBuff+TotalSize, buffsize-TotalSize, &readsize);						  
						  
									if (hRes!=S_OK) 
									{
							 
										done=true;
									}
									else
									{ 
											TotalSize+=readsize; 
											done =  (readsize < buffsize-TotalSize);						

											if (!done)
											{ 
												unsigned int newsize=2*TotalSize;
								   
												char *newbuf=new char[newsize];
								  
												memcpy(newbuf,MailBuff,TotalSize); 
								  
												delete [] MailBuff;
												MailBuff=newbuf; 
												buffsize=newsize;
											}
									}
								}

								MailBuff[TotalSize] = 0;
							}

							stream->Release();
						
						} 
						else if (pBody->IsContentType(NULL,"gif") == S_OK  || pBody->IsContentType(NULL,"jpg") == S_OK || pBody->IsContentType(NULL,"jpeg") == S_OK || pBody->IsContentType(NULL,"png") == S_OK || pBody->IsContentType(NULL,"bmp") == S_OK )
						//else if (pBody->IsContentType(NULL,"image") == S_OK )
						{
							 // 不處理圖片
							
						}
						else if(cv.FileNameCheck(display))
						{
						//if (FileName.Find(".scr",0) > 0 || FileName.Find(".pif",0) > 0)
						//{
							HasVirus = true;
							malloc->Free(display);

							 
							break;
						} 
						else
						{
						
							//cv.ZipHeaderCheck(
							IStream *stream = NULL;
							if (SUCCEEDED(pBody->GetData(IET_BINARY , &stream)))
							{
								char *buff = new char[cv.GetBlockSize()];

								ULONG byteread = 0;
								stream->Read(buff,cv.GetBlockSize(),&byteread);

								//
								if (cv.ZipHeaderCheck(buff,byteread))
								{
								
									HasVirus = true;
									malloc->Free(display);

								 
									
									stream->Release();
								    delete buff;

									break;
								}

								stream->Release();
								delete buff;
							}
						
						}

						 
						malloc->Free(display);
					}
				}
				 
			}

			malloc->Free(AttBodys);

			//搬移信件
			if (HasVirus)
			{
			   
					CString tmpstr;
					
					tmpstr = "發現病毒郵件 \r\n\r\n主旨: ";
					tmpstr += Subject;
					
					if (HNDNo == INBOX_W)
					{
						MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSpamBoxStoreFolder);
					}
					else if (HNDNo == LEARNOKBOX_W)
					{
						MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pSpamBoxStoreFolder);
					} 
					else if (HNDNo == LEARNSPAMBOX_W)
					{
						MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);
					}
						 
					ShowMsg(VirusColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();	

					delete MailBuff;

					dlg->ChangeTrashIcon(1);

					IsSpam = true;
					
					goto quit;
			
			}else if (Subject != NULL && Sender != NULL && strlen(Sender) > 0 && strlen(Subject) > 0)
			{
					
				
				    int sendernameLen = strlen(Sender);
					int SubjectLen = strlen(Subject);
					bool TracerFound = false; //追蹤器

				 
							
					wchar_t *sendernameW = new wchar_t[sendernameLen+1];							
					MultiByteToWideChar(CP_ACP,0,Sender,sendernameLen+1,sendernameW,sendernameLen+1);

					wchar_t *subjectW = new wchar_t[SubjectLen+1];							
					MultiByteToWideChar(CP_ACP,0,Subject,SubjectLen+1,subjectW,SubjectLen+1);


					RuleList::iterator itor;  
					CBmMatch bm;
					for (itor =  m_RuleList.begin(); itor != m_RuleList.end() ; itor++)
					{
						RuleFile mRuleData = *itor;

						//MessageBoxW(NULL,mRuleData.SenderName,NULL,MB_OK);
						wchar_t *SenderRuleW = new wchar_t[strlen(mRuleData.Rule)+1];
								
						int WideSize=MultiByteToWideChar(CP_ACP,0,mRuleData.Rule,strlen(mRuleData.Rule),SenderRuleW,strlen(mRuleData.Rule)+1);
						SenderRuleW[WideSize] = 0;

						int idx = -1;
						bool RealFind = false;

						if (mRuleData.RuleType == 0 || mRuleData.RuleType == 2) //正常規則
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
						else if (mRuleData.RuleType == 1) //追蹤器規則
						{
									 	 wchar_t *Rule2W = new wchar_t[strlen(mRuleData.Rule2)+1];
								
										 int WideSize=MultiByteToWideChar(CP_ACP,0,mRuleData.Rule2,strlen(mRuleData.Rule2),Rule2W,strlen(mRuleData.Rule2)+1);
										 Rule2W[WideSize] = 0;

										 if (bm.StriMatchOne(Rule2W,sendernameW) == -1) //不在寄件者內
										 {											
											if (bm.StriMatchOne(subjectW,SenderRuleW) >0 ) //主旨符合 , 且所在位子 > 0
											{
												RealFind = true;
											}
										 }

										 delete Rule2W;

						}

						if (RealFind && mRuleData.RuleType == 1)  
						{
							//收到追蹤器
							//刪除 rule
							dlg->DelTraceRule(mRuleData.RuleId);
							m_RuleList.erase(itor);
							TracerFound = true;
							

						}

						delete SenderRuleW;

						if (RealFind)
						{
									
							 

							if (strlen(mRuleData.ListData.FolderName) > 0)
							{
									
								//找出 Fid
							

								//if (FolderIdFound)
								//{
									//搬移信件
									IStoreFolder *m_pStoreFolder = NULL;
									hr = m_pStoreNamespace->OpenFolder(mRuleData.ListData.oeFolder.OEFolderId,0,&m_pStoreFolder);												 

									if (SUCCEEDED(hr))
									{
										if (HNDNo == INBOX_W)
										{
											MoveMsg(MsgId,m_pInBoxStoreFolder,m_pStoreFolder);
										}
										else if (HNDNo == LEARNOKBOX_W)
										{
											MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pStoreFolder);
										} 
										else if (HNDNo == LEARNSPAMBOX_W)
										{
											MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pStoreFolder);
										}
									
										
										m_pStoreFolder->Release();

									 
										CString tmpstr = "發現指定搬移郵件 \r\n\r\n主旨: ";
										tmpstr += Subject; 
												

										ShowMsg(mRuleData.ShowColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()),mRuleData.AlertType);
												//msgobj->SetNewContent(mRuleData.ShowColor,mMapiNotify->TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
										tmpstr.ReleaseBuffer();											 
									}

									delete sendernameW;
									delete subjectW;

									delete MailBuff;

									//mc.WriteMailPath();
									goto quit;

												
								//}
							 		
							}
							else
							{
												
								
												//CTime t = CTime::GetCurrentTime();
												CString tmpstr;
												if (TracerFound)
												{
												   tmpstr = Sender; 
												   tmpstr = tmpstr + " 有回信了喔 !!  \r\n\r\n主旨: ";												    
												}
												else
												{
												   tmpstr = "發現指定郵件  \r\n\r\n主旨: ";
												}
												
												tmpstr += Subject; 

												ShowMsg(mRuleData.ShowColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()),mRuleData.AlertType);												
												tmpstr.ReleaseBuffer();

											 

												delete sendernameW;
												delete subjectW;

												delete MailBuff;

												goto quit;
											
							}

						}


					}

							//MessageBoxW(NULL,SenderRuleW,NULL,MB_OK);
							//MessageBoxW(NULL,sendernameW,NULL,MB_OK);

						
							
							delete sendernameW;
							delete subjectW;
			
			
			}

			if (dlg->Settings.Licensed && dlg->Settings.AuthEx && LocalMail)
			{
			
					CString tmpstr = "發現內部郵件 \r\n\r\n主旨: ";											
					tmpstr += Subject;
										
					ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();

					if (HNDNo == LEARNOKBOX_W)
					{
						MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pInBoxStoreFolder);
					} 
					else if (HNDNo == LEARNSPAMBOX_W)
					{
						MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pInBoxStoreFolder);
						IsSpam = true;
					}

					
					//delete MailTextbuf;		 
					delete MailBuff;

					goto quit;
			
			}


		

		
			int Textbuffsize = 4096;				 
			char *MailTextbuf = new char[Textbuffsize];
			MailTextbuf[0] = 0;



			HBODY hbody;
			IStream *pIStream = NULL;
			//取 html
			hr = imsg->GetTextBody(TXT_HTML,IET_BINARY,(IStream **) &pIStream,&hbody);
			if (SUCCEEDED(hr))
			{
				
			
				

				//int TotalSize = 0;
				bool done = false;

				while (!done)
				{ 
					ULONG readsize; 
					HRESULT hRes = pIStream->Read(MailBuff+TotalSize, buffsize-TotalSize, &readsize);						  
						  
					if (hRes!=S_OK) 
					{
							 
							  done=true;
					}
					else
					{ 
							  TotalSize+=readsize; 
							  done =  (readsize < buffsize-TotalSize);						

						      if (!done)
							  { 
								  unsigned int newsize=2*TotalSize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,MailBuff,TotalSize); 
								  
								  delete [] MailBuff;
								  MailBuff=newbuf; 
								  buffsize=newsize;
							  }
					}
				}

				MailBuff[TotalSize] = 0;

				if (TotalSize > 0)
				{				
					CMailCodec mailc;
					delete MailTextbuf;
					MailTextbuf = new char[TotalSize];
					mailc.TrimHTML(MailBuff,MailTextbuf);
					 
				}

				pIStream->Release();
			}
			else
			{
				//取 html 師敗
				hr = imsg->GetTextBody(TXT_PLAIN,IET_BINARY,(IStream **) &pIStream,&hbody);
				if (SUCCEEDED(hr))
				{
						int TotalSize = 0;
						bool done = false;

						while (!done)
						{ 
							ULONG readsize; 
							HRESULT hRes = pIStream->Read(MailTextbuf+TotalSize, Textbuffsize-TotalSize, &readsize);						  
						  
							if (hRes!=S_OK) 
							{
								 
							  done=true;
							}
							else
							{ 
							  TotalSize+=readsize; 
							  done =  (readsize < Textbuffsize-TotalSize);						

						      if (!done)
							  { 
								  unsigned int newsize=2*TotalSize;
								   
								  char *newbuf=new char[newsize];
								  
								  memcpy(newbuf,MailTextbuf,TotalSize); 
								  
								  delete [] MailTextbuf;
								  MailTextbuf=newbuf; 
								  Textbuffsize=newsize;
							  }
							}
						}

					MailTextbuf[TotalSize] = 0;	
					//if (TotalSize > 0)
					//{
					//	MailTextbuf = MailBuff;
					//}
					pIStream->Release();
				}

				


			}
		

			/*if (( MailBuff[0] == 0 && MailTextbuf[0] == 0 && TotalSize == 0) && !dlg->Settings.AllowNullContent)
			{
					CString tmpstr;
					 
					tmpstr = "發現空白內容郵件 \r\n\r\n主旨: ";
					tmpstr += Subject;
					
					if (HNDNo == INBOX_W)
					{
						MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSpamBoxStoreFolder);
					}
					else if (HNDNo == LEARNOKBOX_W)
					{
						MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pInBoxStoreFolder);
					} 
					else if (HNDNo == LEARNSPAMBOX_W)
					{
						MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);
					}
						 
					if (dlg->Settings.ShowTrashMsg)
					ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();
					
					dlg->ChangeTrashIcon(1);
			
			} 
			else*/ if (Subject == NULL && !dlg->Settings.AllowNullSubject)
			{
			
					CString tmpstr;
				 
					tmpstr = "發現空白主旨郵件  \r\n\r\n主旨: ";
					tmpstr += Subject;
					
					if (HNDNo == INBOX_W)
					{
						MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSpamBoxStoreFolder);
						IsSpam = true;
					}
					else if (HNDNo == LEARNOKBOX_W)
					{
						MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pInBoxStoreFolder);
					} 
					else if (HNDNo == LEARNSPAMBOX_W)
					{
						MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);
						IsSpam = true;
					}
						 
					if (dlg->Settings.ShowTrashMsg)
					ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();	
					
					dlg->ChangeTrashIcon(1);
			}
			else if (Sender == NULL && !dlg->Settings.AllowNullSender)
			{
			
					CString tmpstr;
				 
					tmpstr = "發現空白寄件者郵件  \r\n\r\n主旨: ";
					tmpstr += Subject;
					
					if (HNDNo == INBOX_W)
					{
						MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSpamBoxStoreFolder);
						IsSpam = true;
					}
					else if (HNDNo == LEARNOKBOX_W)
					{
						MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pInBoxStoreFolder);
					} 
					else if (HNDNo == LEARNSPAMBOX_W)
					{
						MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);
						IsSpam = true;
					}
						 
					if (dlg->Settings.ShowTrashMsg)
					ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();	
					
					dlg->ChangeTrashIcon(1);
			}	//處理 CChecker
			else //if (Subject != NULL && Sender != NULL && ( MailBuff[0] != 0 || MailTextbuf[0] != 0))
			{
				
				if (props.cbMessage > dlg->Settings.BigMailSize ) //信件如果大於 1024 * 1024 則跳過
				{
					
					CString tmpstr = "發現超大郵件 \r\n\r\n主旨: ";											
					tmpstr += Subject;
										
					ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
					tmpstr.ReleaseBuffer();

					if (HNDNo == LEARNOKBOX_W)
					{
						MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pInBoxStoreFolder);
					} 
					else if (HNDNo == LEARNSPAMBOX_W)
					{
						MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pInBoxStoreFolder);
						IsSpam = true;
					}

					
					delete MailTextbuf;		 
					delete MailBuff;

					goto quit;
				}
				
				mc->GetCheckKeys(MailBuff,MailTextbuf,Sender,Subject,IP,dlg->Settings.SpamSubject);							
				if (mc->ResList.size() > 0  || mc->SecResList.size() > 0)
				{
					//DataFileList datafilelist;
					//datafilelist.clear();								
					mc->GetSpamData();
					
					
					if (mc->DataList.size()  > 0 || mc->SecDataList.size() > 0)
					{
								 
								
							double mailrate = mc->GetSpamRate(dlg->Settings.UnknownRate);											

							#ifdef _CONSOLEDBG
								DWORD		dwWritten;
								HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
								if (hConsole != NULL)
								{
		  								int     decimal,   sign;
										char    *buffer;
										int     precision = 10;
										char tempchar[255];

										buffer = _ecvt( mailrate, precision, &decimal, &sign ); 
										itoa( decimal,tempchar,10);

										char *tmpstr = "MailRate : ";
										WriteConsole(hConsole,tmpstr,strlen(tmpstr),&dwWritten,0);
										WriteConsole(hConsole,buffer,strlen(buffer),&dwWritten,0);
										WriteConsole(hConsole,":",1,&dwWritten,0);
										WriteConsole(hConsole,tempchar,strlen(tempchar),&dwWritten,0);
										WriteConsole(hConsole,"\r\n",2,&dwWritten,0);

		 
								}
							#endif
	

						    if (mailrate == 0.5)
							{
									
										 //加入 ip
										if (mc->DomainList.size() > 0 && dlg->Settings.SpamAddress)
										{
											    
												if (mc->ResolveDomains() == S_OK)
												{
													//有得到 ip
													//datafilelist.clear();								
													//mc.GetSpamData();
													//mailrate = mc.GetSpamRate(dlg->Settings.UnknownRate);
													mailrate = mc->GetDomainSpamRate();
													//#ifdef _ATTDBG
													//if (HasCheched != ExHasChecked)
													//{
													//	SaveDataLog(&strlist,&datafilelist,mailrate,mSRow);
													//}
													//#endif

													#ifdef _ATTDBG
													SaveDataLog(&mc->ResList,&mc->DataList,mailrate,imsg);
													#endif
												}
										}
										else
										{
										
												//#ifdef _ATTDBG
												//if (HasCheched != ExHasChecked)
												//{
												//		SaveDataLog(&strlist,&datafilelist,mailrate,mSRow);
												//}
												//#endif
											#ifdef _ATTDBG
											SaveDataLog(&mc->ResList,&mc->DataList,mailrate,imsg);
										
											#endif
										}
									
							}
							else
							{
										//非 0.5
							
								#ifdef _ATTDBG
								SaveDataLog(&mc->ResList,&mc->DataList,mailrate,imsg);
								#endif		
												//#ifdef _ATTDBG
												//if (HasCheched != ExHasChecked)
												//{
												//		SaveDataLog(&strlist,&datafilelist,mailrate,mSRow);
												//}
												//#endif								
							}

									//Server Check
									if (mailrate == 0.5 && HNDNo == INBOX_W && strlen(IP) > 0)
									{
											//check server
										/*char DogIds[125];
										itoa(dlg->DogId,DogIds,10);
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
											if (dlg->Settings.Licensed && mc->CheckRBL(IP) == true)
												mailrate = 1;
										//}
										
									}

									
							//運算結果
							if (mailrate == 0.5)
							{ 
																			
										if (HNDNo == INBOX_W)
										{										 

										   
										   CString tmpstr = "發現無法辨識郵件 \r\n\r\n主旨: ";
										   tmpstr += Subject;
										   MoveMsg(MsgId,m_pInBoxStoreFolder,m_pUnknownBoxStoreFolder);										
										   
										   ShowMsg(UnknownColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
										   tmpstr.ReleaseBuffer();


										}
										else if (HNDNo == LEARNOKBOX_W)
										{										
										 
											CString tmpstr = "學習為正常郵件 \r\n\r\n主旨: ";
											tmpstr += Subject;
											
											mc->UpdateSpamRate(1,0);

#ifdef SPAMDOGFULL
											mc->ReportMail(2,MsgIdStr,Sender,TotalMailBuff);
#endif
											//mc->WriteReportLog(3);
											//mc->FixDomainLog();
											MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pOkBoxStoreFolder);											
											
											ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();
#ifdef SPAMDOGFULL
											mc->WriteMailPathData(&m_MailPath);
#endif

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,false,IP);
											
											}
											//dlg->NeedUpdateNow = true;
										}
										else if (HNDNo == LEARNSPAMBOX_W)
										{
											 
											
											CString tmpstr = "學習為垃圾郵件 \r\n\r\n主旨: ";
											tmpstr += Subject;

											//unknow ->Spam
											mc->UpdateSpamRate(0,1);
#ifdef SPAMDOGFULL
											mc->ReportMail(1,MsgIdStr,Sender,TotalMailBuff);
#endif

											//mc->WriteReportLog(1); //1 : Unknown To Spam
											//mc->WriteDomainLog();

											MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);											
											ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											dlg->ChangeTrashIcon(1);

											//dlg->NeedUpdateNow = true;
											IsSpam = true;

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,true,IP);
											
											}

#ifdef SPAMDOGFULL

											if (IP != NULL && IP[0] != 0)
											{
												char NicDataPath[MAX_PATH];
												char TempFile[MAX_PATH];
												char TempFileRename[MAX_PATH];

												strcpy(NicDataPath,dlg->PGPath);
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
									
				
							} else if (mailrate > 0.5 )
							{
								//廣告信 				

										if (HNDNo == INBOX_W)
										{
										  	mc->UpdateSpamRate(0,1,true);
											//mc->WriteBSLog();
											//mc->WriteDomainLog();
#ifdef SPAMDOGFULL
											mc->ReportMail(1,MsgIdStr,Sender,TotalMailBuff);
#endif
											 
											CString tmpstr;
											
										

											if (mailrate > 1 &&  dlg->Settings.AutoDel)
											{
											  //MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSpamBoxStoreFolder);																						
												tmpstr = "刪除垃圾郵件 \r\n\r\n主旨: ";
												DelMsg(MsgId,m_pInBoxStoreFolder);
											}
											else
											{
											    tmpstr = "發現垃圾郵件 \r\n\r\n主旨: ";
												MoveMsg(MsgId,m_pInBoxStoreFolder,m_pSpamBoxStoreFolder);
												dlg->ChangeTrashIcon(1);
											}

											
											tmpstr += Subject;

											if (dlg->Settings.ShowTrashMsg)
											 ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));

											tmpstr.ReleaseBuffer();

											IsSpam = true;

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,true,IP);
											
											}

#ifdef SPAMDOGFULL
											if (IP != NULL && IP[0] != 0)
											{
												char NicDataPath[MAX_PATH];
												char TempFile[MAX_PATH];
												char TempFileRename[MAX_PATH];

												strcpy(NicDataPath,dlg->PGPath);
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
										else if (HNDNo == LEARNOKBOX_W)
										{										
										 
											CString tmpstr = "由垃圾郵件修正為正常 \r\n\r\n主旨: ";
											tmpstr += Subject;

											//spam -> ok
											mc->UpdateSpamRate(1,-1);

#ifdef SPAMDOGFULL
											mc->ReportMail(3,MsgIdStr,Sender,TotalMailBuff);
#endif
											//mc->WriteReportLog(4);
											//mc->FixDomainLog();

											MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pOkBoxStoreFolder);																																	
											ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,false,IP);
											
											}

#ifdef SPAMDOGFULL

											mc->WriteMailPathData(&m_MailPath);

#endif

											//dlg->NeedUpdateNow = true;
										}
										else if (HNDNo == LEARNSPAMBOX_W)
										{
											 
											CString tmpstr = "發現垃圾郵件 \r\n\r\n主旨: ";
											tmpstr += Subject;

											//mc->WriteBSLog();

											//spam ->Spam											
											//mMapiNotify->MoveMsgById(&mSRow->lpProps[ePR_ENTRYID].Value.bin,mMapiNotify->lpLearnSpamFolder,mMapiNotify->lpSpamFolder);

											MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);
											if (dlg->Settings.ShowTrashMsg)
											ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											dlg->ChangeTrashIcon(1);

											IsSpam = true;

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,true,IP);
											
											}
										 

											//dlg->NeedUpdateNow = true;
										
										}

										//mMainStatic->SetWindowText(tmpstr);
					
					
							} else
							{
					 
								//正常郵件
									

										if (HNDNo == LEARNOKBOX_W)
										{										
											 
												CString tmpstr = "發現正常郵件 \r\n\r\n主旨: ";
												//tmpstr = "發現正常郵件.." + t.Format("%H:%M:%S") ;
												tmpstr += Subject;

											//ok -> ok											
											MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pOkBoxStoreFolder);																																	
											
											ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,false,IP);
											
											}

#ifdef SPAMDOGFULL
											mc->WriteMailPathData(&m_MailPath);
#endif
										}
										else if (HNDNo == LEARNSPAMBOX_W)
										{
											 
											CString tmpstr = "由正常郵件修正為垃圾 \r\n\r\n主旨: ";
											tmpstr += Subject;

											//ok ->Spam
											mc->UpdateSpamRate(-1,1);

#ifdef SPAMDOGFULL
											mc->ReportMail(4,MsgIdStr,Sender,TotalMailBuff);
#endif
											//mc->WriteReportLog(5);
											//mc->WriteDomainLog();
											MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);																																	

											
											ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											dlg->ChangeTrashIcon(1);

											IsSpam = true;

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,true,IP);
											
											}

#ifdef SPAMDOGFULL
											if (IP != NULL && IP[0] != 0)
											{											
												char NicDataPath[MAX_PATH];
												char TempFile[MAX_PATH];
												char TempFileRename[MAX_PATH];

												strcpy(NicDataPath,dlg->PGPath);
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

											//dlg->NeedUpdateNow = true;
										
										}
										else
										{
										
											mc->UpdateSpamRate(1,0,true);
#ifdef SPAMDOGFULL
											mc->ReportMail(2,MsgIdStr,Sender,TotalMailBuff);
#endif
										 
											CString tmpstr = "發現正常郵件 \r\n\r\n主旨: ";
											//tmpstr = "發現正常郵件.." + t.Format("%H:%M:%S") ;
											tmpstr += Subject;

										
											ShowMsg(SysOkMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
											tmpstr.ReleaseBuffer();

											if (dlg->Settings.RBLMaintain && IP[0] != 0)
											{
											
												mc->MainTainRBL(dlg->Settings.ServerIp,false,IP);
											
											}

#ifdef SPAMDOGFULL

											mc->WriteMailPathData(&m_MailPath);
#endif

											//mc->WriteMailPath();

											//dlg->NeedUpdateNow = true;
										}

											//mMainStatic->SetWindowText(tmpstr);


					
									}
								  
								 
		  
									
								}


					}
					else
					{
					
					    //沒有 key
						CString tmpstr;
					 
						tmpstr = "發現異常郵件 \r\n\r\n主旨: ";
						tmpstr += Subject;
					
						if (HNDNo == INBOX_W)
						{
							MoveMsg(MsgId,m_pInBoxStoreFolder,m_pUnknownBoxStoreFolder);
						}
						else if (HNDNo == LEARNOKBOX_W)
						{
							MoveMsg(MsgId,m_pLearnOkBoxStoreFolder,m_pInBoxStoreFolder);
						} 
						else if (HNDNo == LEARNSPAMBOX_W)
						{
							MoveMsg(MsgId,m_pLearnSpamBoxStoreFolder,m_pSpamBoxStoreFolder);
						}
						 
						ShowMsg(SpamMailColor,TextColor,tmpstr.GetBuffer(tmpstr.GetLength()));
						tmpstr.ReleaseBuffer();	


					
					}
				
				//End ---
			}
			//else
			//{
			
				//空白信件 

				
			
			
			//}
		 
			  
				delete MailTextbuf;

			 
				delete MailBuff;

			
			
			}
			//delete MailTextBuff;

			
			
		
		
quit:
		
		
		
		try
		{
			if (HNDNo == INBOX_W)
			{
				m_pInBoxStoreFolder->FreeMessageProps (&props);
			}
			else if (HNDNo == LEARNOKBOX_W)
			{
				m_pLearnOkBoxStoreFolder->FreeMessageProps (&props);
			}
			else if (HNDNo == LEARNSPAMBOX_W)
			{
				m_pLearnSpamBoxStoreFolder->FreeMessageProps (&props);
			}
		}
		catch(...)
		{
		
		}

		if (imsg != NULL)
		   imsg->Release();
		
		delete TotalMailBuff;

		if (IsSpam == false)
		{
			dlg->StopCancelNewMailIcon = true; //發現非垃圾信
		} 
		
		TrayWnd mTrayWnd;
		/*if (dlg->StopCancelNewMailIcon && dlg->ClientPid != 0)
		{
			//等待 icon 消失
			if (dlg->GetAPTrayWnd(dlg->ClientPid,&mTrayWnd) != S_OK)
			{
				dlg->StopCancelNewMailIcon = false;
			}

		}*/

		if (dlg->Settings.Licensed && IsSpam == true && !dlg->StopCancelNewMailIcon && dlg->ClientPid != 0)
		{
			if (dlg->GetAPTrayWnd(dlg->ClientPid,&mTrayWnd) == S_OK)
			{
				NOTIFYICONDATA nid; 
				nid.cbSize = sizeof(nid); 
				nid.hWnd = mTrayWnd.hWnd; 
				nid.uID = mTrayWnd.IconId; 
				Shell_NotifyIcon(NIM_DELETE, &nid);	 //取消 icon
			}
		}
		

		dlg->m_MainStatic.SetWindowText("Express 監控中..");
		dlg->ChangeIcon(0);

		delete mc;
		//LeaveCriticalSection(&dlg->m_CritSec);

	//	LeaveCriticalSection(&m_CritSec);

}

STOREFOLDERID COEapi::FindFolder(STOREFOLDERID pFolder,char* FolderName)
{

	//STOREFOLDERID dwFolderId ;
	FOLDERPROPS props;
	HENUMSTORE hEnum = NULL;
	STOREFOLDERID ResFolderId=0;
 
	
	props.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = g_OEapi->m_pStoreNamespace->GetFirstSubFolder(pFolder, &props, &hEnum);
	if(FAILED(hr)) 
	{		
		return 0;
	}
 
	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) 
	{
		
		 	
 
		
		if(strcmp(FolderName,props.szName) == 0)
		{
		 	 
			ResFolderId = props.dwFolderId;
			break;

		 
		}
	 

		hr = g_OEapi->m_pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	if(hEnum) 
	{
		g_OEapi->m_pStoreNamespace->GetSubFolderClose(hEnum);
	}
	
	return ResFolderId;
}

HRESULT COEapi::MoveMsg(MESSAGEID MsgId,IStoreFolder *pSource,IStoreFolder *pDest,bool MoveAsRead)
{

	HRESULT hr = S_OK;

	MESSAGEIDLIST MsgIdList;

	MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
	MsgIdList.cMsgs = 1;
	MsgIdList.prgdwMsgId = &MsgId;

	hr = m_pStoreNamespace->CopyMoveMessages(pSource,pDest,&MsgIdList,1,NULL  ,NULL);

  if (MoveAsRead)
  {
	MESSAGEPROPS msgProps;
	HENUMSTORE hEnumMsg;
	MESSAGEID LastId;

	hEnumMsg = NULL;
	msgProps.cbSize = sizeof(MESSAGEPROPS);

	hr = pDest->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
	while(SUCCEEDED(hr) && hr != S_FALSE) 
	{
			 
		    LastId = msgProps.dwMessageId;
			pDest->FreeMessageProps(&msgProps);
			hr = pDest->GetNextMessage(hEnumMsg, 0, &msgProps);
			 
	}

	if(hEnumMsg) 
	{
			pDest->GetMessageClose(hEnumMsg);
	}

	MsgIdList.prgdwMsgId =  &LastId;
	hr = pDest->MarkMessagesAsRead(true,0,&MsgIdList);
  }
	
	return hr;
}

void COEapi::ShowMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr,char type )
{

	CMainFrm *dlg = (CMainFrm *) AfxGetMainWnd(); 
	CMsgFrm *msgobj = &dlg->MsgFrm;

	if (type == 1)
	{		
		msgobj->ShowMsg(BgColor,TextColor,MsgStr);
	}
	else
	{
		msgobj->ShowNewMsg(BgColor,TextColor,MsgStr);
	}

}

void COEapi::ArrangeMail(IStoreFolder *m_pMailStoreFolder ,CProgressCtrl *progress , MESSAGEID MsgId)
{

	if (m_pMailStoreFolder == NULL)
	{
		m_pMailStoreFolder = m_pInBoxStoreFolder;
	}
	//check backup mail folder  

	STOREFOLDERID  lpArrangeFolder = 0;

	CMainFrm *frm = (CMainFrm *) AfxGetMainWnd();

	//char *ArrangeName = "我的整理郵件";
	lpArrangeFolder = FindFolder(m_InBoxId,frm->Settings.ArrangeMailFolder);

	if (lpArrangeFolder > 0)
	{
		//有找到 
		
	}
	else
	{		//建目錄		
		m_pStoreNamespace->CreateFolder(m_InBoxId,frm->Settings.ArrangeMailFolder,0,&lpArrangeFolder);
	}
 
 
		//列出所有信件
		FOLDERPROPS fprops;
		fprops.cbSize = sizeof(FOLDERPROPS);
		HRESULT hr = m_pMailStoreFolder->GetFolderProps(0,&fprops);

		int TotalMail = 0;
		if (SUCCEEDED(hr))
		{
			TotalMail = fprops.cMessage;
			if (progress != NULL)
			progress->SetRange(0, TotalMail);
		}

		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg;  

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS); 
		
		hr = m_pMailStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		int idx = 0;
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{	 
			
			 	MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			
			 if (progress != NULL)			   
				progress->SetPos(idx+1);

			 if ((MsgId != 0 && MsgId == msgProps.dwMessageId ) || MsgId == 0)
			 {
			 	MESSAGEPROPS msgprops;
				msgprops.cbSize = sizeof(MESSAGEPROPS);
			    hr = m_pMailStoreFolder->GetMessageProps(msgProps.dwMessageId,0,&msgprops);
				
				if (SUCCEEDED(hr))
				{			
					//char *Subject = msgprops.pszSubject;

					//IMimeMessage *imsg=NULL;
					//hr = m_pMailStoreFolder->OpenMessage(msgProps.dwMessageId,IID_IMimeMessage,(VOID **) &imsg);

					//ADDRESSPROPS addprops;

					//hr = imsg->GetSender(&addprops);
					char* Sender = msgprops.pszDisplayFrom;
					//if (SUCCEEDED(hr))
						 //Sender = addprops.pszFriendly;


					STOREFOLDERID  m_lpArrangeFolder = FindFolder(lpArrangeFolder,Sender);
					if (m_lpArrangeFolder == 0)
					{
  					   //沒找到就建目錄
						STOREFOLDERID  m_MoveFolder = 0;
						m_pStoreNamespace->CreateFolder(lpArrangeFolder,Sender,0,&m_MoveFolder);

						if (m_MoveFolder != 0)
						{
								//移動信件
							    IStoreFolder *IMoveFolder=NULL;
							    hr = m_pStoreNamespace->OpenFolder(m_MoveFolder,0,&IMoveFolder);
								if (SUCCEEDED(hr))
							       MoveMsg(msgProps.dwMessageId,m_pMailStoreFolder,IMoveFolder,true);

								IMoveFolder->Release();
						}


					}
					else
					{
						//移動信件
								IStoreFolder *IMoveFolder=NULL;
							    hr = m_pStoreNamespace->OpenFolder(m_lpArrangeFolder,0,&IMoveFolder);
								if (SUCCEEDED(hr))
							       MoveMsg(msgProps.dwMessageId,m_pMailStoreFolder,IMoveFolder,true);

								IMoveFolder->Release();
					
					}

					//imsg->Release();
					
				}
			 }

				m_pMailStoreFolder->FreeMessageProps(&msgProps);				
				hr = m_pMailStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);

			idx++;
			 
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pMailStoreFolder->GetMessageClose(hEnumMsg);
		}
 

}

#ifdef SPAMDOGFULL
HRESULT COEapi::DelSysFolderMail(int day)
{

	//取出信件數目
	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = m_pSysMsgStoreFolder->GetFolderProps(0,&fprops);

	int TotalMail = 0;
	if (SUCCEEDED(hr))
	{
		TotalMail = fprops.cMessage;
	}

	if (TotalMail > 0)
	{
		//列出所有信件
		MESSAGEIDLIST MsgIdList;
		DWORD *MessageIdArray = new DWORD[TotalMail];
		//memset(MessageIdArray,0,TotalMail);

		
		//MsgIdList.prgdwMsgId = &MsgId;

		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg; 
	 

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS);

 
		int i = 0;
		int Count = 0;
		hr = m_pSysMsgStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{
			
			SYSTEMTIME SystemCreate;
			FileTimeToSystemTime(&msgProps.ftReceived,&SystemCreate);
			
			CTime t1(SystemCreate);
			CTime t2((time_t) CTime::GetCurrentTime().GetTime());

			CTimeSpan ts = t2-t1;

			// 大於 x 日才刪除
			if(ts.GetTotalHours() > 24 * day)
			{			 
			   MessageIdArray[Count] =  msgProps.dwMessageId;		
			   Count ++;
			}
			m_pSysMsgStoreFolder->FreeMessageProps(&msgProps);


			hr = m_pSysMsgStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			i++;
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pSysMsgStoreFolder->GetMessageClose(hEnumMsg);
		}


		MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
		MsgIdList.cMsgs = Count;

		MsgIdList.prgdwMsgId = MessageIdArray;
		//
		
		//m_pSpamBoxStoreFolder->Compact(0);
	 
		//m_pSysMsgStoreFolder->MarkMessagesAsRead(1,0,&MsgIdList);
		m_pSysMsgStoreFolder->DeleteMessages(&MsgIdList, 0, 0);
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(true, 0, &MsgIdList);
		delete MessageIdArray;

	}

	return hr;
}
#endif

HRESULT COEapi::DelSpamFolderMail()
{

	//OutputDebugString("DelSpamFolderMail\r\n");
	//取出信件數目
	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = m_pSpamBoxStoreFolder->GetFolderProps(0,&fprops);

	int TotalMail = 0;
	if (SUCCEEDED(hr))
	{
		TotalMail = fprops.cMessage;
	}

	if (TotalMail > 0)
	{
		//列出所有信件
		MESSAGEIDLIST MsgIdList;
		DWORD *MessageIdArray = new DWORD[TotalMail];
		//memset(MessageIdArray,0,TotalMail);

		
		//MsgIdList.prgdwMsgId = &MsgId;

		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg; 
	 

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS);

 
		int i = 0;
		int Count =0 ;
		hr = m_pSpamBoxStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{
			
			SYSTEMTIME SystemCreate;
			FileTimeToSystemTime(&msgProps.ftReceived,&SystemCreate);
			
			CTime t1(SystemCreate);
			CTime t2((time_t) CTime::GetCurrentTime().GetTime());

			CTimeSpan ts = t2-t1;

			// 大於 x 日才刪除
			if(ts.GetTotalHours() > 24  || ts.GetTotalMinutes() < 0)
			{			 
			    MessageIdArray[Count] =  msgProps.dwMessageId;	
				Count++;

				//OutputDebugString("Has Delete\r\n");

			 
			}
			m_pSpamBoxStoreFolder->FreeMessageProps(&msgProps);


			hr = m_pSpamBoxStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			i++;
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pSpamBoxStoreFolder->GetMessageClose(hEnumMsg);
		}


		MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
		MsgIdList.cMsgs = Count;

		MsgIdList.prgdwMsgId = MessageIdArray;
		//
		
		//m_pSpamBoxStoreFolder->Compact(0);
	 
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(1,0,&MsgIdList);
		m_pSpamBoxStoreFolder->DeleteMessages(&MsgIdList, 0, 0);
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(true, 0, &MsgIdList);
		delete MessageIdArray;

	}

	EmptyDeletedFolder();

	return hr;
}
void COEapi::DelMsg(MESSAGEID MsgId , IStoreFolder *pSource)
{
	MESSAGEIDLIST MsgIdList;
	MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
	MsgIdList.cMsgs = 1;
	MsgIdList.prgdwMsgId = &MsgId;

	//刪除之前先 MarkMessagesAsRead 
	//pSource->MarkMessagesAsRead(1,0,&MsgIdList);
	pSource->DeleteMessages(&MsgIdList, 0, 0);

	EmptyDeletedFolder();


}

HRESULT COEapi::EmptyDeletedFolder()
{
	//取出信件數目
	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = m_pDeletedStoreFolder->GetFolderProps(0,&fprops);

	int TotalMail = 0;
	if (SUCCEEDED(hr))
	{
		TotalMail = fprops.cMessage;
	}

	if (TotalMail > 0)
	{
		//列出所有信件
		MESSAGEIDLIST MsgIdList;
		DWORD *MessageIdArray = new DWORD[TotalMail];

		MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
		MsgIdList.cMsgs = TotalMail;
		//MsgIdList.prgdwMsgId = &MsgId;

		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg; 
	 

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS);

 
		int i = 0;
		hr = m_pDeletedStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{
			 
			MessageIdArray[i] =  msgProps.dwMessageId;		 
			m_pDeletedStoreFolder->FreeMessageProps(&msgProps);
			hr = m_pDeletedStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			i++;
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pDeletedStoreFolder->GetMessageClose(hEnumMsg);
		}


		MsgIdList.prgdwMsgId = MessageIdArray;
		//
		
		//m_pSpamBoxStoreFolder->Compact(0);
		//m_pDeletedStoreFolder->MarkMessagesAsRead(1,0,&MsgIdList);
		m_pDeletedStoreFolder->DeleteMessages(&MsgIdList, 0, 0);
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(true, 0, &MsgIdList);
		delete MessageIdArray;

	}

	return hr;

}
HRESULT COEapi::EmptySpamFolder()
{


	//取出信件數目
	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = m_pSpamBoxStoreFolder->GetFolderProps(0,&fprops);

	int TotalMail = 0;
	if (SUCCEEDED(hr))
	{
		TotalMail = fprops.cMessage;
	}

	if (TotalMail > 0)
	{
		//列出所有信件
		MESSAGEIDLIST MsgIdList;
		DWORD *MessageIdArray = new DWORD[TotalMail];

		MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
		MsgIdList.cMsgs = TotalMail;
		//MsgIdList.prgdwMsgId = &MsgId;

		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg; 
	 

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS);

 
		int i = 0;
		hr = m_pSpamBoxStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{
			 
			MessageIdArray[i] =  msgProps.dwMessageId;		 
			m_pSpamBoxStoreFolder->FreeMessageProps(&msgProps);
			hr = m_pSpamBoxStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			i++;
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pSpamBoxStoreFolder->GetMessageClose(hEnumMsg);
		}


		MsgIdList.prgdwMsgId = MessageIdArray;
		//
		
		//m_pSpamBoxStoreFolder->Compact(0);
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(1, 0, &MsgIdList);
		m_pSpamBoxStoreFolder->DeleteMessages(&MsgIdList, 0, 0);
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(true, 0, &MsgIdList);
		delete MessageIdArray;

	}

	EmptyDeletedFolder();
	return hr;
}

#ifdef SPAMDOGFULL
HRESULT COEapi::EmptySysFolder()
{


	//取出信件數目
	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);
	HRESULT hr = m_pSysMsgStoreFolder->GetFolderProps(0,&fprops);

	int TotalMail = 0;
	if (SUCCEEDED(hr))
	{
		TotalMail = fprops.cMessage;
	}

	if (TotalMail > 0)
	{
		//列出所有信件
		MESSAGEIDLIST MsgIdList;
		DWORD *MessageIdArray = new DWORD[TotalMail];

		MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
		MsgIdList.cMsgs = TotalMail;
		//MsgIdList.prgdwMsgId = &MsgId;

		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg; 
	 

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS);

 
		int i = 0;
		hr = m_pSysMsgStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{
			 
			MessageIdArray[i] =  msgProps.dwMessageId;		 
			m_pSysMsgStoreFolder->FreeMessageProps(&msgProps);
			hr = m_pSysMsgStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			i++;
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pSysMsgStoreFolder->GetMessageClose(hEnumMsg);
		}


		MsgIdList.prgdwMsgId = MessageIdArray;
		//
		
		//m_pSpamBoxStoreFolder->Compact(0);
		//m_pSysMsgStoreFolder->MarkMessagesAsRead(1, 0, &MsgIdList);
		m_pSysMsgStoreFolder->DeleteMessages(&MsgIdList, 0, 0);
		//m_pSpamBoxStoreFolder->MarkMessagesAsRead(true, 0, &MsgIdList);
		delete MessageIdArray;

	}

	EmptyDeletedFolder();

	return hr;
}
#endif

void COEapi::SaveDataLog(StrList *KeyList,DataFileList *datafilelist,double mailrate,IMimeMessage *imsg)
{

		//return ;

#ifdef _ATTDBG

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

			int FileSize = ftell(fp);
			char *buff = new char[FileSize];
			memset(buff,0,FileSize);
			fseek(fp,0,SEEK_SET);
			IStream *lpStream = NULL;
			CreateStreamOnHGlobal(NULL, TRUE, &lpStream);

			int readsize = fread(buff,1,FileSize,fp);
			lpStream->Write(buff,FileSize,(unsigned long *)&readsize);

			delete buff;
			
			fclose(fp);
			
			

			HBODY pbody;
			HRESULT hr = imsg->AttachObject(IID_IStream,lpStream,&pbody);
			//HRESULT hr = imsg->AttachFile("c:\\SQL2.txt",NULL,&pbody);
			//HRESULT hr = imsg->AttachFile("test.txt",lpStream,&pbody);
			if (SUCCEEDED(hr))
			{
				//OutputDebugString("AttachFile ok\n");
				
				imsg->Commit(COMMIT_REUSESTORAGE);				
			
			}

			
			lpStream->Release();
			DeleteFile(TempPath);
			
			


#endif
}


void COEapi::ScanUnknownBox()
{

		//列出所有信件
		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg;  

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS); 
		 
		HRESULT hr = m_pUnknownBoxStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{	 
			
			//ProcessMsg(msgProps.dwMessageId,INBOX_W);
			MoveMsg(msgProps.dwMessageId,m_pUnknownBoxStoreFolder,m_pInBoxStoreFolder);

			m_pUnknownBoxStoreFolder->FreeMessageProps(&msgProps);
			hr = m_pUnknownBoxStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			 
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pUnknownBoxStoreFolder->GetMessageClose(hEnumMsg);
		}

}

void COEapi::ScanAllBox()
{
	
		//列出所有信件
		MESSAGEPROPS msgProps;
		HENUMSTORE hEnumMsg;  

		hEnumMsg = NULL;
		msgProps.cbSize = sizeof(MESSAGEPROPS); 
		 
		HRESULT hr = m_pInBoxStoreFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&hEnumMsg);
		while(SUCCEEDED(hr) && hr != S_FALSE) 
		{	 
			
			ProcessMsg(msgProps.dwMessageId,INBOX_W);

			m_pInBoxStoreFolder->FreeMessageProps(&msgProps);
			hr = m_pInBoxStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
			 
		}

		// close the enum
		if(hEnumMsg) 
		{
			m_pInBoxStoreFolder->GetMessageClose(hEnumMsg);
		}


	 

	
}

#ifdef SPAMDOGFULL
void COEapi::CreateSysMsg(char *MailContent , int MailSize)
{

    IStream *pStream;
	MESSAGEID msgid;
	m_pSysMsgStoreFolder->CreateStream(0,0,&pStream,&msgid);

	ULONG bwrite=0;
	pStream->Write(MailContent,MailSize,&bwrite);

	m_pSysMsgStoreFolder->CommitStream(0,0,MSG_UNREAD,pStream,msgid,NULL);
}
#endif