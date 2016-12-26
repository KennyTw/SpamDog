// MapiEx.h: interface for the CMapiEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIEX_H__F16CE65F_93B8_4826_8707_9346129CB156__INCLUDED_)
#define AFX_MAPIEX_H__F16CE65F_93B8_4826_8707_9346129CB156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

   #include "../Swspam/Checker.h"
   #include <mapix.h>
   #include <mapiutil.h>
   #include "MAPIAdviseSink.h"

   #include <afxtempl.h>


#define INBOX_F	1
#define LEARNOKBOX_F	2
#define LEARNSPAMBOX_F	3
#define OUTBOX_F	4
#define LEARNWHITEBOX_F	5


typedef struct _MAPIMSGID 
{
  char MsgId[100];  
} MAPIMSGID;


class CMapiEx  
{
public:
	void ArrangeMail(LPMAPITABLE lpTable , LPMAPIFOLDER  lpFolder,CProgressCtrl *progress ,SRow *mSRow);

	CMapiEx();
	virtual ~CMapiEx();

	LPMAPIFOLDER  lpInboxFolder;
	LPMAPIFOLDER  lpOutboxFolder;
	LPMAPIFOLDER  lpSpamFolder; //spam
	LPMAPIFOLDER  lpNormalFolder;
	LPMAPIFOLDER  lpLearnOkFolder;
	LPMAPIFOLDER  lpLearnSpamFolder;
	LPMAPIFOLDER  lpUnknownFolder;
	LPMAPIFOLDER  lpWhiteFolder;

#ifdef SPAMDOGFULL
	LPMAPIFOLDER  lpSysMsgFolder;
#endif

	LPMAPITABLE lpContentsTable ,  lpOutContentsTable , lpLearnOkContentsTable , lpLearnSpamContentsTable;
	LPMAPITABLE lpWhiteContentsTable;
	#ifdef SPAMDOGFULL
	LPMAPITABLE lpSysMsgContentsTable;
	#endif
	LPMDB         lpMDB ;

	RuleList m_RuleList;

	char SpamFolderName[255];
	char OkFolderName[255];
	char UnknownFolderName[255];
	char LearnOkFolderName[255];
	char LearnSpamFolderName[255]; 
	char LearnWhiteFolderName[255];
	#ifdef SPAMDOGFULL
	char SysMsgFolderName[255]; 
	#endif

	COLORREF SysOkMailColor,SpamMailColor,UnknownColor,VirusColor,TextColor;


	void ScanAllBox();
	void ScanUnknownBox();


	HRESULT FindFolder(LPMAPIFOLDER ParentFolder , LPMAPIFOLDER* ResultFolder , LPSTR FolderName);

	HRESULT Login (char Type = 1,char *profile=NULL);
	HRESULT Logout();
	HRESULT GetRowMsgItem(SRow *mSRow , LPSTR* MsgContent ,LPSTR* TextContent , LPSTR* Subject , LPSTR* SenderName , int *AttCount);
	HRESULT MoveMsgById(SBinary *sbin,LPMAPIFOLDER  lpFromFolder,LPMAPIFOLDER  lpMoveToFolder);
	HRESULT DelMsgById(SBinary *sbin,LPMAPIFOLDER  lpFromFolder);
	HRESULT EmptySpamFolder();
	#ifdef SPAMDOGFULL
	HRESULT EmptySysFolder();
	#endif
	HRESULT NotificationOn();
	HRESULT NotificationOff();
	//HRESULT LearnAllFolder(OLFolder* folder , char howlearn ,CProgressCtrl *progress); //howlearn = 0 learnok , 1 learn bad
	#ifdef SPAMDOGFULL
	void CreateSysMsg(char *MailHeader , char *MailContent , char *SenderEmail , char *Subject);
	HRESULT DelSysFolderMail(int day=5);
	#endif

	
	CList<MAPIMSGID, MAPIMSGID> MailKeyTable;
	void InBoxTableReload();
 

private:
	

	LPMAPISESSION lpMAPISession ;
	CMAPIAdviseSink *lpMAPIAdviseSink;
	CMAPIAdviseSink *lpMAPIAdviseOkSink;
	CMAPIAdviseSink *lpMAPIAdviseSpamSink;
	CMAPIAdviseSink *lpMAPIAdviseOutSink;
	CMAPIAdviseSink *lpMAPIAdviseWhiteSink;

	ULONG ulAdviseConnection ,ulAdviseOutConnection, ulAdviseOkConnection , ulAdviseSpamConnection;
	ULONG ulAdviseWhiteConnection;

	char *htmlbuf;
    unsigned int htmlsize;
    unsigned int bufsize;

	char *bodybuf;
    unsigned int bodysize;
    unsigned int bodybufsize;

	HRESULT OpenDefaultMessageStore();
	HRESULT OpenInbox();
	HRESULT ListMessages(char Type = 1);
	void GetiniFolderSetup();
	void ReadStream(LPSTREAM lpStream);
	void ReadBodyStream(LPSTREAM lpStream);

	enum MsgEnum { 
	  ePR_INSTANCE_KEY,
	  ePR_SENDER_NAME,
	  ePR_SUBJECT,
      ePR_ENTRYID,
	  ePR_RTF_IN_SYNC,
	  ePR_BODY,
      NUM_COLS};

};

#endif // !defined(AFX_MAPIEX_H__F16CE65F_93B8_4826_8707_9346129CB156__INCLUDED_)
