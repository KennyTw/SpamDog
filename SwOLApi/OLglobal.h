
#if !defined(Olglobal__INCLUDED_)
#define Olglobal__INCLUDED_

#include <mapix.h>
#include <mapiutil.h>
#include <MAPIDEFS.H>
#pragma warning (disable:4786)
#include <list> 
using namespace std;
 
	typedef struct tagOlREGISTERNOTIFY
    {
	    ULONG RegType;
		//bool NeedReloadEvent;
		LPMAPITABLE lpContentsTable;
		//CMAPIAdviseSink *lpMAPIAdviseSink;
		ULONG ulAdviseConnection;
		LPMAPIFOLDER RegisterFolder;
		
    } OlREGISTERNOTIFY;

	typedef struct tagOlMAILPROPS
    {
		ULONG MailIdcb;
		LPBYTE  MailIdlpb;
		//LPMAPIFOLDER* MAPIRegisterFolder;
		LPSTR szSubject;
    } OlMAILPROPS;

typedef void (CALLBACK * LPOlOnNotify)(LPVOID lpParam , ULONG ulTableEvent , OlREGISTERNOTIFY* lpRegisterNotify ,OlMAILPROPS *mailpros);

	typedef struct tagOlMAPIMSGDATA
	{		
		unsigned char InstanceKey[255];  
		unsigned char EntryId[255];
		unsigned long EntrySize;
  
	} OlMAPIMSGDATA;

	typedef list<OlMAPIMSGDATA> MsgTable;

#endif