
#if !defined(OlApi__INCLUDED_)
#define OlApi__INCLUDED_

#include "StdAfx.h"
#include "OLglobal.h"
#include "MAPIAdviseSink.h"



 

/*
extern "C" { 
       void __declspec(dllexport) DLLfun1(char**);
	   void __declspec(dllexport) DLLfun2(LPCallBack);
	  
       
};*/ 

//extern int  DLLDIR DLLArg; 



    typedef struct tagOlFOLDERPROPS
    {
		ULONG FolderIdcb;
		LPBYTE  FolderIdlpb;
		//LPMAPIFOLDER lpMAPIFOLDER;
		LPSTR szName;
    } OlFOLDERPROPS;

	typedef struct tagOlFOLDERITOR
    {
		DWORD itor;
		LPSRowSet pRows;
		//LPMAPITABLE lpContentsTable;
    } OlFOLDERITOR;



	typedef struct tagOlMAILITOR
    {
		DWORD itor;
		LPSRowSet pRows;	
    } OlMAILITOR;



	



HRESULT OlLogout();
HRESULT OlLogin(char* ProfileName);
HRESULT OlOpenDefaultMessageStore();
HRESULT OlOpenInBoxFolder(LPMAPIFOLDER *InBoxFolder);
HRESULT OlOpenTrashFolder(LPMAPIFOLDER *TrashFolder);

HRESULT OlGetFirstSubFolder(LPMAPIFOLDER ParentFolder , OlFOLDERPROPS *folderpros , OlFOLDERITOR *folderitor );
HRESULT OlGetNextSubFolder(OlFOLDERPROPS *folderpros ,  OlFOLDERITOR *folderitor );
HRESULT OlOpenFolder(OlFOLDERPROPS *folderpros , LPMAPIFOLDER *OpenFolder);
void OlCloseFolder(LPMAPIFOLDER CloseFolder);
void OlCloseFolderProps(OlFOLDERITOR *folderitor );
HRESULT OlCreateFolder(LPMAPIFOLDER ParentFolder , char* FolderName , char* FolderDesc, LPMAPIFOLDER* CreateFolder);
HRESULT OlDeleteFolder(LPMAPIFOLDER ParentFolder ,OlFOLDERPROPS *folderpros); 
HRESULT OlRegFolderNotify(LPVOID lpParam , OlREGISTERNOTIFY *regnotify , LPOlOnNotify lpOnNotify);
HRESULT OlUnRegFolderNotify(OlREGISTERNOTIFY *regnotify);
HRESULT OlMailGetSenderName(LPMESSAGE lpMessage , char **SenderName);
HRESULT OlMailGetSubject(LPMESSAGE lpMessage , char **Subject);
HRESULT OlMailGetHeader(LPMESSAGE lpMessage , char **Header);
HRESULT OlMailGetBody(LPMESSAGE lpMessage , char **Body);
HRESULT OlFreeMemory(char *mem);

HRESULT OlGetFirstMail(LPMAPIFOLDER MailFolder  , OlMAILPROPS *mailpros , OlMAILITOR *mailitor);
HRESULT OlGetNextMail(OlMAILPROPS *mailpros ,  OlMAILITOR *mailitor );
HRESULT OlOpenMail(OlMAILPROPS *mailpros , LPMESSAGE *lpMessage);
void OlCloseMail(LPMESSAGE lpMessage);
void OlCloseMailProps(OlMAILITOR* mailitor );
HRESULT OlMoveMail(OlMAILPROPS *mailpros , LPMAPIFOLDER FromFolder ,LPMAPIFOLDER ToFolder );
HRESULT OlDeleteMail(OlMAILPROPS *mailpros , LPMAPIFOLDER MailFolder );
HRESULT OlCreateMail(LPMAPIFOLDER MailFolder , char *SenderEmail , char *Subject , char *MailContent );



void ParseRTFBuf(char *buf,unsigned int*len);
void ParseRTF(char *buf,unsigned int *len, int *idx , int *parseidx ,int *bignoreidx);

#endif