
#include "Stdafx.h"

#include <objbase.h>
#include <initguid.h>
//#include "msoeapi.h" 
#include "OEapi.h"

IStoreNamespace *m_pStoreNamespace = NULL;

HRESULT OeLogin()
{

	HRESULT       hRes = -1;
	hRes = CoInitialize(0);
	if(FAILED(hRes)) 
	{
		throw "Error CoInitialize.";
		return S_FALSE;
		 
	}

	hRes = CoCreateInstance(CLSID_StoreNamespace, NULL,CLSCTX_SERVER, 
									IID_IStoreNamespace, (LPVOID*) &m_pStoreNamespace);
	if(FAILED(hRes)) 
	{
		throw "Error creating IStoreNamespace.";		 
		return S_FALSE; 
	}

	
	hRes = m_pStoreNamespace->Initialize(NULL, 1);
	if(FAILED(hRes)) 
	{
		throw "Error initializing IStoreNamespace.";
		m_pStoreNamespace->Release();
		return S_FALSE; 
		 
	}

	return hRes;

}

HRESULT OeLogout()
{

	if(m_pStoreNamespace != NULL) 
	{
		m_pStoreNamespace->Release();
		m_pStoreNamespace = NULL;
	}

 

	CoUninitialize();

	return S_OK;

}

HRESULT OeOpenInBoxFolder(IStoreFolder  **InBoxFolder)
{

	return m_pStoreNamespace->OpenSpecialFolder(FOLDER_INBOX,0,InBoxFolder);
}

void OeCloseFolder(IStoreFolder* CloseFolder)
{
	CloseFolder->Release();
}

HRESULT OeOpenTrashFolder(IStoreFolder **TrashFolder)
{
	return m_pStoreNamespace->OpenSpecialFolder(FOLDER_DELETED,0,TrashFolder);
}

HRESULT OeGetFirstSubFolder(IStoreFolder* ParentFolder , OeFOLDERPROPS *folderpros , OeFOLDERITOR *folderitor )
{

	HRESULT       hRes = -1;

	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);

	hRes = ParentFolder->GetFolderProps(0,&fprops);

	if (hRes != S_OK) return hRes;

	FOLDERPROPS cprops;
	cprops.cbSize = sizeof(FOLDERPROPS);
	HENUMSTORE hEnum;

	hRes = m_pStoreNamespace->GetFirstSubFolder(fprops.dwFolderId,&cprops,&hEnum);
	if (hRes != S_OK)  return hRes;

	folderpros->dwFolderId = cprops.dwFolderId;
	strcpy(folderpros->szName,cprops.szName);

	//folderitor->itor = 0;
	folderitor->hEnum = hEnum;

	return hRes;

 
}

HRESULT OeGetNextSubFolder(OeFOLDERPROPS *folderpros ,  OeFOLDERITOR *folderitor )
{

	HRESULT       hRes = -1;

	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);

	hRes = m_pStoreNamespace->GetNextSubFolder(folderitor->hEnum, &fprops);	
	if (hRes != S_OK)  return hRes;

	folderpros->dwFolderId = fprops.dwFolderId;
	strcpy(folderpros->szName,fprops.szName);


	return hRes;

}

void OeCloseFolderProps(OeFOLDERITOR *folderitor )
{
	m_pStoreNamespace->GetSubFolderClose(folderitor->hEnum);
}

HRESULT OeCreateFolder(IStoreFolder*   ParentFolder , char* FolderName ,  IStoreFolder** CreateFolder)
{

	HRESULT       hRes = -1;

	STOREFOLDERID CreateId; 

	FOLDERPROPS fprops;
	fprops.cbSize = sizeof(FOLDERPROPS);

	hRes = ParentFolder->GetFolderProps(0,&fprops);

	if (hRes != S_OK) return hRes;

	FOLDERPROPS cprops;
	cprops.cbSize = sizeof(FOLDERPROPS);
	

	hRes = m_pStoreNamespace->CreateFolder(fprops.dwFolderId,FolderName,0,&CreateId);
	if (hRes != S_OK)  return hRes;

	hRes = m_pStoreNamespace->OpenFolder(CreateId,0,CreateFolder);


		

	return hRes;

}

HRESULT OeDeleteFolder(OeFOLDERPROPS *folderpros)
{
		HRESULT       hRes = -1;

		hRes = m_pStoreNamespace->DeleteFolder(folderpros->dwFolderId,0);


		return hRes;

}

HRESULT OeRegFolderNotify(LPVOID lpParam , IStoreFolder* RegisterFolder ,OeREGISTERNOTIFY *regnotify , LPOeOnNotify lpOnNotify)
{
	HRESULT       hRes = -1;

	regnotify->RegisterFolder = RegisterFolder;
	COEWnd2* m_OeWnd = new COEWnd2(lpParam,regnotify,lpOnNotify);

	hRes = RegisterFolder->RegisterNotification(0,m_OeWnd->GetWinHanlde());


	regnotify->RegWnd = m_OeWnd;

	return hRes;



}

HRESULT OeUnRegFolderNotify(OeREGISTERNOTIFY *regnotify)
{
	HRESULT       hRes = -1;

	hRes = regnotify->RegisterFolder->UnregisterNotification(0,regnotify->RegWnd->GetWinHanlde());

	
	delete regnotify->RegWnd;
	regnotify->RegWnd = NULL;
	regnotify->RegisterFolder = NULL;


	return hRes;

}

HRESULT OeOpenFolder(OeFOLDERPROPS *folderpros , IStoreFolder **OpenFolder)
{
	return m_pStoreNamespace->OpenFolder(folderpros->dwFolderId,0,OpenFolder);
}


HRESULT OeMailGetSenderName(OeMailObj* MailObj , char **SenderName)
{

	HRESULT       hRes = -1;

		MESSAGEPROPS props;
		props.cbSize = sizeof(MESSAGEPROPS);

		hRes = MailObj->MailFolder->GetMessageProps(MailObj->MsgId,0,&props);

		int len = strlen(props.pszDisplayFrom);

		char *newstr = new char[len+1];
		strcpy(newstr,props.pszDisplayFrom);

		*SenderName = newstr;

		MailObj->MailFolder->FreeMessageProps(&props);

	return hRes;

}

HRESULT OeMailGetSubject(OeMailObj* MailObj , char **Subject)
{
	HRESULT       hRes = -1;

		MESSAGEPROPS props;
		props.cbSize = sizeof(MESSAGEPROPS);

		hRes = MailObj->MailFolder->GetMessageProps(MailObj->MsgId,0,&props);

		int len = strlen(props.pszSubject);

		char *newstr = new char[len+1];
		strcpy(newstr,props.pszSubject);

		*Subject = newstr;

		MailObj->MailFolder->FreeMessageProps(&props);

	return hRes;
}

HRESULT OeMailGetMailContent(OeMailObj* MailObj , char **MailContent)
{
	HRESULT       hRes = -1;

		IStream *pTextStream = NULL;
		hRes = MailObj->MailFolder->OpenMessage(MailObj->MsgId,IID_IStream,(VOID **) &pTextStream);

		if (hRes == S_OK)
		{
				STATSTG statstg;
				memset (&statstg, 0, sizeof(statstg));
				hRes = pTextStream->Stat(&statstg, STATFLAG_NONAME);
				if (hRes == S_OK)
				{
					char *AllocMem = NULL;
					unsigned int len = statstg.cbSize.LowPart + sizeof(char);

					AllocMem = new char[len];
					pTextStream->Read((void*)AllocMem, statstg.cbSize.LowPart, NULL);

					AllocMem[statstg.cbSize.LowPart] = 0;

					*MailContent = AllocMem;

					pTextStream->Release();
					
				}
		
		}

	return hRes;
}
 
HRESULT OeFreeMemory(char *mem)
{
	delete mem;

	return S_OK;

}

HRESULT OeGetFirstMail(IStoreFolder* MailFolder  , OeMAILPROPS *mailpros , OeMAILITOR *mailitor)
{

	HRESULT       hRes = -1;

	MESSAGEPROPS msgProps;	 
	msgProps.cbSize = sizeof(MESSAGEPROPS);
	
	hRes = MailFolder->GetFirstMessage(0,	0,MSGPROPS_FAST,&msgProps,&mailitor->hEnum);

	if (hRes != S_OK) return hRes;
	
	mailpros->MailObj.MsgId = msgProps.dwMessageId;
	mailpros->MailObj.MailFolder = MailFolder;

	if (strlen(msgProps.pszSubject) < sizeof(mailpros->szSubject))
	{
		strcpy(mailpros->szSubject, msgProps.pszSubject);
	}
	else
	{
		strncpy(mailpros->szSubject, msgProps.pszSubject , sizeof(mailpros->szSubject) - 1);
		mailpros->szSubject[sizeof(mailpros->szSubject)-1] = 0;
	}

	mailitor->MailFolder = MailFolder;
	MailFolder->FreeMessageProps(&msgProps);



	return hRes;

}
HRESULT OeGetNextMail(OeMAILPROPS *mailpros ,  OeMAILITOR *mailitor )
{
	HRESULT       hRes = -1;

	MESSAGEPROPS msgProps;	 
	msgProps.cbSize = sizeof(MESSAGEPROPS);

	hRes = mailitor->MailFolder->GetNextMessage(mailitor->hEnum, 0, &msgProps);

	if (hRes != S_OK) return hRes;

	mailpros->MailObj.MsgId = msgProps.dwMessageId;
	mailpros->MailObj.MailFolder = mailitor->MailFolder;

	if (strlen(msgProps.pszSubject) < sizeof(mailpros->szSubject))
	{
		strcpy(mailpros->szSubject, msgProps.pszSubject);
	}
	else
	{
		strncpy(mailpros->szSubject, msgProps.pszSubject , sizeof(mailpros->szSubject) - 1);
		mailpros->szSubject[sizeof(mailpros->szSubject)-1] = 0;
	}

	mailitor->MailFolder->FreeMessageProps(&msgProps);



	return hRes;
}

void OeCloseMailProps(OeMAILITOR* mailitor )
{
	mailitor->MailFolder->GetMessageClose(mailitor->hEnum);
	mailitor->hEnum = NULL;
	mailitor->MailFolder = NULL;
}

HRESULT OeMoveMail(OeMailObj* MailObj , IStoreFolder* ToFolder )
{
	HRESULT       hRes = -1;

	MESSAGEIDLIST MsgIdList;

	MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
	MsgIdList.cMsgs = 1;
	MsgIdList.prgdwMsgId = &MailObj->MsgId;

	hRes = m_pStoreNamespace->CopyMoveMessages(MailObj->MailFolder,ToFolder,&MsgIdList,1,NULL  ,NULL);

	return hRes;

}

HRESULT OeDeleteMail(OeMailObj* MailObj )
{
	HRESULT       hRes = -1;

	MESSAGEIDLIST MsgIdList;

	MsgIdList.cbSize = sizeof(MESSAGEIDLIST);
	MsgIdList.cMsgs = 1;
	MsgIdList.prgdwMsgId = &MailObj->MsgId;

	hRes = MailObj->MailFolder->DeleteMessages(&MsgIdList, 0, 0);

	return hRes;

}

HRESULT OeCreateMail(IStoreFolder* MailFolder  , char *MailContent )
{

	HRESULT       hRes = -1;

	int len = strlen(MailContent);
	if (len <= 0 ) return -1;

	IStream *pStream;
	MESSAGEID msgid;
	MailFolder->CreateStream(0,0,&pStream,&msgid);

	ULONG bwrite=0;
	pStream->Write(MailContent,len,&bwrite);

	hRes = MailFolder->CommitStream(0,0,MSG_UNREAD,pStream,msgid,NULL);

	return hRes;
}