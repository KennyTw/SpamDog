// MAPIAdviseSink.h: interface for the CMAPIAdviseSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIADVISESINK_H__1F166C28_2C2B_4EA9_91D0_F9C204F2B3A6__INCLUDED_)
#define AFX_MAPIADVISESINK_H__1F166C28_2C2B_4EA9_91D0_F9C204F2B3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MAPIDEFS.H>
#include <mapiutil.h>

#include "../Swspam/Checker.h"




class CMAPIAdviseSink : public IMAPIAdviseSink  
{
public :
    CMAPIAdviseSink  (char Type );
	~CMAPIAdviseSink ();
	void ProcessMsg(SRow *mSRow ); 
	//void ProcessLearnMsg(SRow *mSRow , char LearnType); //Type = 1 LearnSpam,  Type = 2 LearnOk
	void ProcessOutMsg(SRow *mSRow);
	void ProcessWhiteMsg(SRow *mSRow);
public:
    STDMETHODIMP QueryInterface (REFIID riid, LPVOID * ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP_(ULONG) OnNotify (ULONG cNotify, LPNOTIFICATION lpNotifications);
private :    
	void SaveDataLog(StrList *KeyList,DataFileList *datafilelist,double mailrate,SRow *mSRow);
	void ShowMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr,char type = 1);

	
 
    LONG               m_cRef;
	char mtype;
	char WorkPath[_MAX_PATH];

	

};

#endif // !defined(AFX_MAPIADVISESINK_H__1F166C28_2C2B_4EA9_91D0_F9C204F2B3A6__INCLUDED_)
