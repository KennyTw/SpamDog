/////////////////////////////////////////////////////////////////////////////
// ClientClass

#if !defined CLIENTCLASS__INCLUDED
#define CLIENTCLASS__INCLUDED

#include "Coreclass.h"
/////////////////////////////////////////////////////////////////////////////
// CDNSclient


class DLLDIR CDNSclient : public CBaseClient
{
private:
    
    WORD FID,FBitCode,FQDCount;//,FANCount,FNSCount,FARCount;

	char* m_Domain;
	int m_ResType;
	//int m_timeout;

	char m_QueryResult[255];
	//int m_DataLen;

	//void SetQr(const WORD Value);
	//void SetOpCode(const WORD Value);
	//void SetRd(const WORD Value);


	void  ConvertDomainStr(char *Domain);
	void  ParseStr(char *DomainStr , int* index , int DataLen, char* ResultStr);
	void  ParseDNS(char *DomainStr , int* index , int *NSType, int* Prefer, char* RRDomain, int DataLen, char* ResultStr);
	int  OnConnected(Socket_Obj *sock);
	int  OnDataRead(Buffer_Obj *bobj);


public: 
	#define qtMX 15
	#define qtNS  2
	#define qtA   1
	#define qtSOA 6
	#define qtPTR 12


	CDNSclient(); 
	virtual ~CDNSclient();
	HANDLE Resolve(char* DnsServer , char *Domain , int ResType);
	char* GetQueryResult();

	
};


/////////////////////////////////////////////////////////////////////////////
// CSMTPclient

typedef struct _Mail_Obj
{  
   char* SmtpIp;
   char* MailFrom;
   char* Sender;
   char* RcptTo;
   char* Subject;
   char* Body;
   char* HELO;

} Mail_Obj;

class DLLDIR CSMTPclient : public CBaseClient
{
private :
	char PrevCmd[5];
	Mail_Obj *m_Mail_Obj;

	int  OnDataRead(Buffer_Obj *bobj);
	void GetDomain(char* email , char*Domain);
	 
 

public:
	
	CSMTPclient();
	HANDLE SendMail(Mail_Obj* mailobj ,int timeout,int eventtimeout);

};


/////////////////////////////////////////////////////////////////////////////
// CPOPclient

class DLLDIR CPOPclient : public CBaseClient
{
private:
	#define  NONE  0
	#define  P_USER 1
	#define  P_PASS 2
	#define  P_STAT 3
    #define  P_QUIT 4
	#define  P_RETR 5
	#define  P_LIST 6
	#define P_UIDL 7
	#define P_FIRSTRETR 8

	char* m_LoginId;
	char* m_Password;

	int m_TotalMail;
	int m_index;
	char m_UIDL[255];

	FILE *fp;

	int  OnDataRead(Buffer_Obj *bobj);


public:

	CPOPclient();
	HANDLE CheckMail(char* HostIp, char* LoginId,char* Password ,int timeout,int eventtimeout);

};


/////////////////////////////////////////////////////////////////////////////
// CHTTPclient

class DLLDIR CHTTPclient : public CBaseClient
{
private:
	
	//char SendCmd[1024*20];
	char *SendCmd;
	//char ResStr[2048];
	char Header[2048];
	char m_DNSServer[17];
	char TempFileName[MAX_PATH];

	FILE *fp;
 
	int  OnConnected(Socket_Obj *sock);
	int  OnDataRead(Buffer_Obj *bobj);
	int  OnDataWrite(Buffer_Obj *bobj);
	void OnDataFinish();
	int GetHeader(char* buff,int size);


public:
	int ByteTransfer;
	int TotalTransfer;
	int ByteSend;

	CHTTPclient();

	virtual void SetDNS(char* DNSServer);
	virtual HANDLE GET(char* Host, char* PathFile ,int timeout,int eventtimeout);
	HANDLE POST(char* Host, char* PathFile ,char* PostStr,int timeout,int eventtimeout);

	virtual HANDLE IPGET(char* IP,char* Host, char* PathFile ,int timeout,int eventtimeout);
	HANDLE IPPOST(char* IP,char* Host, char* PathFile ,char* PostStr,int timeout,int eventtimeout);

	virtual char *GetTempFilePath();
	int GetByteTransfer();
	int GetTotalTransfer();
	virtual bool DoTransSucceed();
    //char* GetReturn();
};


#endif