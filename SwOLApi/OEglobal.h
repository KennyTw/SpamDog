#if !defined(OEglobal__INCLUDED_)
#define OEglobal__INCLUDED_

#include "msoeapi.h" 
//#include "OEWnd.h"

	typedef struct tagOeMailObj
    {
	    IStoreFolder* MailFolder;
		MESSAGEID MsgId;		
    } OeMailObj;

typedef void (CALLBACK * LPOeOnNotify)(LPVOID lpParam , ULONG ulTableEvent,OeMailObj* MailObj);

	class COEWnd2 ; //let COEWnd become null class name
	typedef struct tagOeREGISTERNOTIFY
    {
	    ULONG RegType;
		IStoreFolder* RegisterFolder;
		COEWnd2* RegWnd;
		
		
    } OeREGISTERNOTIFY;

#endif