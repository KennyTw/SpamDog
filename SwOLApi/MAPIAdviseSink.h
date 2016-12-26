// MAPIAdviseSink.h: interface for the CMAPIAdviseSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIADVISESINK_H__065EB66D_A903_4D0C_B9C9_309B6F3985FF__INCLUDED_)
#define AFX_MAPIADVISESINK_H__065EB66D_A903_4D0C_B9C9_309B6F3985FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <MAPIDEFS.H>

#include "OLglobal.h"
 
#define RG_ADDMAIL	0
#define RG_ADDMAIL_NOREFLESH 1


class CMAPIAdviseSink2 : public IMAPIAdviseSink  
{
public:
	CMAPIAdviseSink2(LPVOID lpParam,LPMDB lpMDB,OlREGISTERNOTIFY *regnotify,LPOlOnNotify OnNotify);
	virtual ~CMAPIAdviseSink2();
	
public:
    STDMETHODIMP QueryInterface (REFIID riid, LPVOID * ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP_(ULONG) OnNotify (ULONG cNotify, LPNOTIFICATION lpNotifications);

	void TableLoad();
private :    
    LONG               m_cRef;
	LPMDB   mlpMDB;
	LPVOID mlpParam;
	OlREGISTERNOTIFY *mregnotify;
	//ULONG mRegType;
	//bool mNeedReloadEvent;
	
	
	LPOlOnNotify mOnNotify;
	//LPMAPITABLE mlpContentsTable;

	MsgTable mMsgTable;


	void ProcessReloadTableAdd(LPSRowSet pRows);
	void ProcessReloadTableDel(LPSRowSet pRows);

};

#endif // !defined(AFX_MAPIADVISESINK_H__065EB66D_A903_4D0C_B9C9_309B6F3985FF__INCLUDED_)
