// OEapi.h: interface for the COEapi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OEAPI_H__12CFB1E3_5C3D_464A_B076_55ADDBFE27ED__INCLUDED_)
#define AFX_OEAPI_H__12CFB1E3_5C3D_464A_B076_55ADDBFE27ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "msoeapi.h" 
#include "OEWnd.h" 

#include "../Global.h"
//#include "SpamDog.h"

#include "../Swspam/Checker.h"

class COEapi  
{
private:
	friend class COEWnd;

	//CRITICAL_SECTION m_CritSec;

#define INBOX_W	1
#define OKBOX_W	2
#define SPAMBOX_W	3
#define LEARNOKBOX_W	4
#define LEARNSPAMBOX_W	5
#define OUTBOX_W	6
#define LEARNWHITEBOX_W	7
	
	char WorkPath[_MAX_PATH];

	COEWnd* m_InBoxWnd;
	COEWnd* m_OutBoxWnd;
	//COEWnd* m_OkBoxWnd;
	//COEWnd* m_SpamBoxWnd;
	COEWnd* m_LearnOkBoxWnd;
	COEWnd* m_LearnSpamBoxWnd;
	COEWnd* m_LearnWhiteBoxWnd;

	STOREFOLDERID m_InBoxId;
	STOREFOLDERID m_OutBoxId;
	STOREFOLDERID m_OkBoxId;
	STOREFOLDERID m_UnknownBoxId;
	STOREFOLDERID m_SpamBoxId;
	STOREFOLDERID m_LearnOkBoxId;
	STOREFOLDERID m_LearnSpamBoxId;
	STOREFOLDERID m_LearnWhiteBoxId;
	STOREFOLDERID m_SysMsgBoxId;
	STOREFOLDERID m_DeletedBoxId;



	IStoreFolder *m_pInBoxStoreFolder;
	IStoreFolder *m_pOutBoxStoreFolder;
	IStoreFolder *m_pOkBoxStoreFolder;
	IStoreFolder *m_pUnknownBoxStoreFolder;
	IStoreFolder *m_pSpamBoxStoreFolder;
	IStoreFolder *m_pLearnOkBoxStoreFolder;
	IStoreFolder *m_pLearnSpamBoxStoreFolder;
	IStoreFolder *m_pLearnWhiteBoxStoreFolder;
	IStoreFolder *m_pSysMsgStoreFolder;
	IStoreFolder *m_pDeletedStoreFolder;


	void GetiniFolderSetup();
	HRESULT OpenInbox();
	STOREFOLDERID FindFolder(STOREFOLDERID pFolder,char* FolderName);

	void OnEvent(UINT uMsg, WPARAM wParam , int HNDNo);
	void ProcessMsg(MESSAGEID MsgId,int HNDNo);
	void ProcessOutMsg(MESSAGEID MsgId);
	void ProcessWhiteMsg(MESSAGEID MsgId);

	void ShowMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr,char type = 1);
	void SaveDataLog(StrList *KeyList,DataFileList *datafilelist,double mailrate,IMimeMessage *imsg);

	

public:
	COEapi();
	virtual ~COEapi();

	void ScanAllBox();
	void ScanUnknownBox();
	void ArrangeMail(IStoreFolder *m_pMailStoreFolder , CProgressCtrl *progress , MESSAGEID MsgId );
	IStoreNamespace *m_pStoreNamespace;

	char SpamFolderName[255];
	char OkFolderName[255];
	char UnknownFolderName[255];
	char LearnOkFolderName[255];
	char LearnSpamFolderName[255]; 
	char LearnWhiteFolderName[255]; 
	char SysMsgFolderName[255]; 


	COLORREF SysOkMailColor,SpamMailColor,UnknownColor,VirusColor,TextColor;

	HRESULT Login();
	HRESULT NotificationOn();
	HRESULT NotificationOff();
	
	HRESULT EmptySpamFolder();
	HRESULT EmptyDeletedFolder();
	HRESULT EmptySysFolder();
	HRESULT DelSysFolderMail(int day=5);
	HRESULT DelSpamFolderMail();

	RuleList m_RuleList;

	//void ProcessLearnMsg(IStoreFolder *storefolder , MESSAGEID MsgId, char LearnType ); //Type = 1 LearnSpam,  Type = 2 LearnOk
	HRESULT MoveMsg(MESSAGEID MsgId,IStoreFolder *pSource,IStoreFolder *pDest,bool MoveAsRead = false);
	void DelMsg(MESSAGEID MsgId , IStoreFolder *pSource);

	void CreateSysMsg(char *MailContent , int MailSize);
	

};

#endif // !defined(AFX_OEAPI_H__12CFB1E3_5C3D_464A_B076_55ADDBFE27ED__INCLUDED_)
