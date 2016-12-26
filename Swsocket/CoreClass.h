/////////////////////////////////////////////////////////////////////////////
// CoreClass

#if !defined CORECLASS__INCLUDED
#define CORECLASS__INCLUDED

#include <winsock2.h>
#include <stdio.h>
#include <time.h>


#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else   
   #define DLLDIR  __declspec(dllimport)   // import DLL information    
#endif 



class DLLDIR CBaseClass
{
  public:
	CBaseClass();	// standard constructor
	virtual ~CBaseClass();
};

/////////////////////////////////////////////////////////////////////////////
// _Socket_Obj

typedef struct _Socket_Obj
{  
  CRITICAL_SECTION     SockCritSec;

  SOCKET s; 	
  unsigned int SockType;
  unsigned int af ;
  int Protocol;

  unsigned int lastcmd;

 
  

  time_t LastTime;
  long OutstandingOps;

  HANDLE ServerThreadHandle;

  #define ST_NORMAL       0
  #define ST_FORACCEPT   1

  struct _Socket_Obj  *prev,*next; 

} Socket_Obj;

/////////////////////////////////////////////////////////////////////////////
// _Buffer_Obj

typedef struct _Buffer_Obj
{  
   
	#define DEFAULT_BUFFERSIZE  2048
	#define DEFAULT_OVERLAPPED_COUNT    1


	#define OP_BUFFER 1
	#define OP_WRITE 2
	
   WSAOVERLAPPED ol;
   char*   buf;
   LPSTR RemoveIp;
   unsigned int buflen ;
   unsigned int HeapLen ;
   unsigned int operation ;
   unsigned int bytes;

   //unsigned int BufferNo;
   

   struct _Socket_Obj* Socket;
   struct _Thread_Obj* ThreadObj;

   void* UserData;

   sockaddr addr;
   int addrlen ;
   //bool bFreeBufferCalled;
   //bool m_OutStanding;

   //BaseItem_Obj* m_BaseItem_Obj;
   struct _Buffer_Obj *next, *prev;

} Buffer_Obj;

/////////////////////////////////////////////////////////////////////////////
// _Thread_Obj

typedef struct _Thread_Obj
{  
  
	HANDLE Event ;
	HANDLE EndThreadEvent;
	//HANDLE WaitThreadEvent;
	//HANDLE ResumeThreadEvent;
    HANDLE Thread ;
     

	int EventCount;
    HANDLE      Handles[MAXIMUM_WAIT_OBJECTS];
    
    CRITICAL_SECTION ThreadCritSec;
    //BaseItem_Obj* m_BaseItem_Obj;
 
	LPVOID _BaseClass;
	//CStructList* m_BufferList;

	Buffer_Obj *BufferList;
	struct _Thread_Obj *prev,*next; 

} Thread_Obj;

 

/////////////////////////////////////////////////////////////////////////////
// CResourceMgr
class DLLDIR CResourceMgr : public CBaseClass
{
private:

	//unsigned int m_BufferNo;
	
	Thread_Obj *m_ThreadList;
	Socket_Obj *m_SocketList;

	CRITICAL_SECTION m_CritSec;	

	Thread_Obj* AssignToThread(Buffer_Obj *buf);
	int InsertBufferToThread(Thread_Obj* thread ,Buffer_Obj* buf);
	

	void AddThreadList(Thread_Obj **head, Thread_Obj *obj);
	void DelThreadList(Thread_Obj **head, Thread_Obj *obj);
	void AddSocketList(Socket_Obj **head ,Socket_Obj *obj);
	void DelSocketList(Socket_Obj **head ,Socket_Obj *obj);
	void AddBufferList(Buffer_Obj **head ,Buffer_Obj *obj); 
	void DelBufferList(Buffer_Obj **head ,Buffer_Obj *obj); 

 protected:
    HANDLE ResourceHeap;
	//ThreadObj
	Thread_Obj* GetThreadObj(LPVOID BaseClass);
	

public:
	CResourceMgr();
	virtual ~CResourceMgr();

	//SocketOBJ
	Socket_Obj* GetSockObj(SOCKET s , int af, int Protocol);
	void FreeSockObj(Socket_Obj* obj);
	//BufferOBJ
	Buffer_Obj* GetBufferObj(Socket_Obj* sock, unsigned int buflen);
	void FreeBufferObj(Buffer_Obj* obj);
	void FreeThreadObj(Thread_Obj* obj);


	static void RenumberEvents(Thread_Obj* thread);
	static Buffer_Obj* FindBufferObjByEvent(Thread_Obj* thread , WSAEVENT* hEvent);

	int SendLn(Socket_Obj* clientobj,char* SendStr);
	


	#define IO_ERROR   -1
	#define IO_OK  1
	#define IO_NOTPOSTRECV  2
		  
	virtual int HandleIO(Buffer_Obj* bobj , Thread_Obj* tobj , int IOResult);
	//virtual int BeforeHandle();
	
};

/////////////////////////////////////////////////////////////////////////////
// CCoreSocket
class DLLDIR CCoreSocket : public CBaseClass
{
  public:
    static int Startup();
	static void Cleanup();
 
	static int PostRecv(Buffer_Obj *recvobj, unsigned int recvOp , unsigned int OffsetSize);
	static int PostSend(Buffer_Obj *sendobj, unsigned int recvOp);
 
	static int Connect(int protocol , char* RemoteIp , int port , int timeout);
};

/////////////////////////////////////////////////////////////////////////////
// CBaseClient

class DLLDIR CBaseClient : public CResourceMgr
{
private:

	int m_protocol ;
	char m_RemoteIp[255];
	int m_port;
	int m_timeout;
	
	//SOCKET  m_SocketHandle;
	
	
	

protected:
	sockaddr_in saRemote; 

public:
	int m_eventtimeout;

	

	CBaseClient();
	~CBaseClient();
     
	HANDLE Connect(int protocol , char* RemoteIp , int port , int timeout, int eventtimeout);
	

	virtual int HandleIO(Buffer_Obj* bobj , Thread_Obj* tobj , int IOResult);
	virtual int BeforeHandle();
	virtual int OnConnected(Socket_Obj *sock);
	virtual int OnBufferError(Buffer_Obj *bobj);
	virtual int OnDataRead(Buffer_Obj *bobj);
	virtual int OnDataWrite(Buffer_Obj *bobj);
	virtual void OnDataFinish();

	int SendBuffer(Socket_Obj* clientobj,char* buf,int len);
};



// CHeap Funcion 
//////////////////////////////////////////////////////////////////////
class DLLDIR CHeap  : public CBaseClass
{
public : 
    static void *GetHeap(int size, HANDLE* ResourceHeap);
	static int GetHeapSize(void *buf, HANDLE* ResourceHeap);
    static void FreeHeap(void *obj, HANDLE* ResourceHeap);
};

unsigned __stdcall IoThread(LPVOID lpParam);
//unsigned __stdcall ConnectThread(LPVOID lpParam);

#endif