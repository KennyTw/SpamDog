#if !defined(OEapi__INCLUDED_)
#define OEapi__INCLUDED_

#include "Oeglobal.h" 
#include "OEWnd.h"


    typedef struct tagOeFOLDERPROPS
    {
		STOREFOLDERID dwFolderId;
		char szName[CCHMAX_FOLDER_NAME];
    } OeFOLDERPROPS;

	typedef struct tagOeFOLDERITOR
    {
		//DWORD itor;
		HENUMSTORE hEnum;	 
    } OeFOLDERITOR;

	typedef struct tagOeMAILPROPS
    {
		OeMailObj MailObj;
		char szSubject[CCHMAX_FOLDER_NAME];
    } OeMAILPROPS;

	typedef struct tagOeMAILITOR
    {		 
		IStoreFolder* MailFolder;
		HENUMSTORE hEnum;	 
    } OeMAILITOR;

	





HRESULT OeLogin();
HRESULT OeLogout();

HRESULT OeOpenInBoxFolder(IStoreFolder  **InBoxFolder);
void OeCloseFolder(IStoreFolder *CloseFolder);
HRESULT OeOpenTrashFolder(IStoreFolder **TrashFolder);
HRESULT OeOpenFolder(OeFOLDERPROPS *folderpros , IStoreFolder **OpenFolder);

HRESULT OeGetFirstSubFolder(IStoreFolder* ParentFolder , OeFOLDERPROPS *folderpros , OeFOLDERITOR *folderitor );
HRESULT OeGetNextSubFolder(OeFOLDERPROPS *folderpros ,  OeFOLDERITOR *folderitor );
void OeCloseFolderProps(OeFOLDERITOR *folderitor );

HRESULT OeCreateFolder(IStoreFolder*   ParentFolder , char* FolderName ,  IStoreFolder** CreateFolder);
HRESULT OeDeleteFolder(OeFOLDERPROPS *folderpros); 

HRESULT OeRegFolderNotify(LPVOID lpParam , IStoreFolder* RegisterFolder ,OeREGISTERNOTIFY *regnotify , LPOeOnNotify lpOnNotify);
HRESULT OeUnRegFolderNotify(OeREGISTERNOTIFY *regnotify);

HRESULT OeMailGetSenderName(OeMailObj* MailObj , char **SenderName);
HRESULT OeMailGetSubject(OeMailObj* MailObj , char **Subject);
HRESULT OeMailGetMailContent(OeMailObj* MailObj , char **MailContent); 
HRESULT OeFreeMemory(char *mem);

HRESULT OeGetFirstMail(IStoreFolder* MailFolder  , OeMAILPROPS *mailpros , OeMAILITOR *mailitor);
HRESULT OeGetNextMail(OeMAILPROPS *mailpros ,  OeMAILITOR *mailitor );
void OeCloseMailProps(OeMAILITOR* mailitor );

HRESULT OeMoveMail(OeMailObj* MailObj , IStoreFolder* ToFolder );
HRESULT OeDeleteMail(OeMailObj* MailObj );
HRESULT OeCreateMail(IStoreFolder* MailFolder  , char *MailContent );


#endif