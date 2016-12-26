#include "stdafx.h"
#include "CoreClass.h"
#include <conio.h>
#include <process.h>
/////////////////////////////////////////////////////////////////////////////
// CBaseClass

CBaseClass::CBaseClass()
{
}

CBaseClass::~CBaseClass()
{
}

//CResourceMgr
//////////////////////////////////////////////////////////

CResourceMgr::CResourceMgr()
{
    InitializeCriticalSection(&m_CritSec); 
	
	m_ThreadList = NULL;
	m_SocketList = NULL;
	
    //m_ThreadList = new CStructList();
	//m_SocketList = new CStructList();

	//ResourceHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0);
	ResourceHeap = GetProcessHeap();

	//m_BufferNo = 0;
	
}

CResourceMgr::~CResourceMgr()
{
	
	Thread_Obj *ptr = m_ThreadList;
	while (ptr)
	{	  
	    
	    Thread_Obj *nextptr = ptr->next;

		//先把 thread terminate 再來處理資源
			WSASetEvent(ptr->EndThreadEvent);
			WaitForSingleObject(ptr->Thread, INFINITE);

		FreeThreadObj(ptr);
		ptr = nextptr;		
	  
	}

	m_ThreadList = NULL;
	/*
	while (ptr)
	{	  
	    
	    Buffer_Obj *objptr = ptr->BufferList;
		while (objptr)
		{
		  Buffer_Obj *nextptr = objptr->next;		  
		    FreeBufferObj(objptr);
		  objptr = nextptr;
		}


        ptr->BufferList = NULL;
		
	  ptr = ptr->next;
	}*/
	
	Socket_Obj *ptrs = m_SocketList;
	while (ptrs)
	{	  
	  Socket_Obj *next = ptrs->next;
	    //仍需觀察是否可以如此使用
	    ptrs->OutstandingOps = 0; 
      FreeSockObj(ptrs); 
      ptrs = next;   
	}
	
	 
	m_SocketList = NULL;

	
	/*
    ptr = m_ThreadList;
	while (ptr)
	{	  
	  Thread_Obj *next = ptr->next;	  
	    FreeThreadObj(ptr);		  
	  ptr = next;
	}*/
	
    //m_ThreadList = NULL;
 
	DeleteCriticalSection (&m_CritSec);  
	//HeapDestroy(ResourceHeap);
 
}

Socket_Obj *CResourceMgr::GetSockObj(SOCKET s , int af, int Protocol)
{
	
	//Socket_Obj *newobj = new Socket_Obj;
	Socket_Obj *newobj = (Socket_Obj *) CHeap::GetHeap(sizeof(Socket_Obj),&ResourceHeap);
	//ZeroMemory(newobj,sizeof(Socket_Obj));

	//Assign
	newobj->s = s;
	newobj->af = af;
	newobj->Protocol = Protocol;

	InitializeCriticalSection(&newobj->SockCritSec);  
	newobj->LastTime = time((time_t *)0); 

	
	if (newobj)
	{	//加到 list , 存回自己
	//	newobj->m_BaseItem_Obj = m_SocketList->AddObj(newobj);
		AddSocketList(&m_SocketList,newobj);
	}


	//OutputDebugString("Get a Socket OK\n");
	return newobj;
   

}

void CResourceMgr::FreeSockObj(Socket_Obj* obj)
{
	 

	if (obj->OutstandingOps <= 0)
	{ 

	 
       
      //EnterCriticalSection(&obj->SockCritSec);  
	  //EnterCriticalSection(&m_CritSec);  
	  
	  DelSocketList(&m_SocketList,obj);
      

	   if (obj->s !=  INVALID_SOCKET)
	   {
         
		 closesocket(obj->s);

		 if (obj->SockType == ST_FORACCEPT)
		 {
		   HANDLE ServerThreadHandle = obj->ServerThreadHandle;
	       WaitForSingleObject(ServerThreadHandle, INFINITE);
		 }
		  
         obj->s = INVALID_SOCKET;
       } 
	   
	    
       //OutputDebugString("Before\n"); 
	   //LeaveCriticalSection(&obj->SockCritSec);
	    
	   DeleteCriticalSection(&obj->SockCritSec);
	   
	   //OutputDebugString("After\n");

		//記憶體釋放
	    
		CHeap::FreeHeap(obj,&ResourceHeap);

         

		//m_SocketList->FreeItem(obj->m_BaseItem_Obj);	  

	    //LeaveCriticalSection(&m_CritSec); 

	    //OutputDebugString("Free a Socket OK\n");

    }
	//else
	//{
	     //closesocket(obj->m_s);
         //obj->m_s = INVALID_SOCKET;
	//}

}

Buffer_Obj* CResourceMgr::GetBufferObj(Socket_Obj* sock, unsigned int buflen)
{
	Buffer_Obj *newobj = NULL;
	
	if (sock)
    {
		    
	 
			newobj = (Buffer_Obj *) CHeap::GetHeap(sizeof(Buffer_Obj),&ResourceHeap);
			//ZeroMemory(newobj,sizeof(Buffer_Obj));

		    newobj->buf = (char *) CHeap::GetHeap(buflen+1,&ResourceHeap);
			if (newobj->buf == NULL)
			{			 
				//OutputDebugString("GetBufferObj: HeapAlloc failed");
				#ifdef _CONSOLEDBG
				_cprintf("GetBufferObj: HeapAlloc failed/n");
				#endif
				//delete newobj;
				CHeap::FreeHeap(newobj ,&ResourceHeap); 
				return NULL;       
			}

			newobj->HeapLen = CHeap::GetHeapSize(newobj->buf,&ResourceHeap);
			newobj->buflen = buflen ;
			newobj->addrlen = sizeof(newobj->addr);
			newobj->Socket = sock;
			newobj->ThreadObj = NULL;
			//newobj->bFreeBufferCalled = false;
			newobj->UserData = NULL;

			//m_BufferNo ++;
			//newobj->BufferNo = m_BufferNo;
			

			newobj->ol.hEvent = WSACreateEvent();
			if (newobj->ol.hEvent == 0) 
			{
				//OutputDebugString("GetBufferObj: WSACreateEvent failed");
				#ifdef _CONSOLEDBG
				_cprintf("GetBufferObj: WSACreateEvent failed/n");
				#endif
				CHeap::FreeHeap(newobj ,&ResourceHeap); 
				return NULL;        
            }

			//printf("GetBufferObj: Handle Occur %d\n",newobj->ol.hEvent);

		 
	} 
	else
	{	
		
		#ifdef _CONSOLEDBG
		_cprintf("CBufferObj Create with null socketobj/n");
		#endif
		throw "CBufferObj Create with null socketobj";
		return NULL;
	}


	//加入 thread
	AssignToThread(newobj);

	//OutputDebugString("Get a Buffer OK\n");
	return newobj;

}
void CResourceMgr::FreeBufferObj(Buffer_Obj* obj)
{

	
	if (obj)
	{

		    //CRITICAL_SECTION *TmpCrit = &obj->Socket->SockCritSec;		
			//EnterCriticalSection(TmpCrit);
			

		    Thread_Obj *tmpThread = obj->ThreadObj;

			if (tmpThread)
			{
			
				//if (obj->m_BaseItem_Obj)
				//{
					EnterCriticalSection(&tmpThread->ThreadCritSec);
					//項目移除
					DelBufferList(&obj->ThreadObj->BufferList, obj);
									
					CHeap::FreeHeap(obj->buf,&ResourceHeap);
					
					//處理 UserData
					if (obj->UserData)
					{     
						CHeap::FreeHeap(obj->UserData,&ResourceHeap);
						obj->UserData = NULL;
					}

					//printf("FreeBufferObj: Handle Occur %d\n",obj->ol.hEvent);

					WSACloseEvent(obj->ol.hEvent);
					
					 
					//tmpThread->m_BufferList->FreeItem(obj->m_BaseItem_Obj);				 
					tmpThread->EventCount --;
					WSASetEvent(tmpThread->Event);

					CHeap::FreeHeap(obj,&ResourceHeap);
					LeaveCriticalSection(&tmpThread->ThreadCritSec);

					//OutputDebugString("Free a Buffer OK\n");

				//}
				//else
				//{
				   // printf("obj->m_BaseItem_Obj NULL/n");
					//throw "obj->m_BaseItem_Obj NULL";
				//}

			}

	
			
			//LeaveCriticalSection(TmpCrit);
			
		


	}

	
	obj = NULL;
	
}

Thread_Obj* CResourceMgr::AssignToThread(Buffer_Obj *buf)
{

	EnterCriticalSection(&m_CritSec);
	//因為存取 m_ThreadList

	Thread_Obj *thread = NULL;

	thread = m_ThreadList;

		while (thread)
		{
     
			//thread = (Thread_Obj *)(tmpItem->m_ObjPointer);

			if (InsertBufferToThread(thread, buf) == NO_ERROR) 
			{            
				break;
			}

            //如果超過上限 , 則把 thread next 放回自己 => 產生 nil
			thread = thread->next;
        }

		if (thread == NULL)
		{
		
			//建立 thread			 
			thread = GetThreadObj(this); 
			InsertBufferToThread(thread,buf);
			
		}

		buf->ThreadObj = thread;
	    WSASetEvent(thread->Event);



	LeaveCriticalSection(&m_CritSec); 

	return thread;

}

int CResourceMgr::InsertBufferToThread(Thread_Obj* thread ,Buffer_Obj* buf)
{

	int nResult  = NO_ERROR;
	
	EnterCriticalSection(&thread->ThreadCritSec); 

	   if (thread->EventCount < MAXIMUM_WAIT_OBJECTS-1)
	   {
	   
		   //BaseItem_Obj *tmpItem = thread->m_BufferList->AddObj(buf);
		   //buf->m_BaseItem_Obj = tmpItem; //Assign BaseItem

		   AddBufferList(&thread->BufferList , buf);
		   thread->Handles[thread->EventCount ++] = buf->ol.hEvent;
		 
	   }
	   else
	   {
	      nResult = SOCKET_ERROR;
	   }

	LeaveCriticalSection(&thread->ThreadCritSec);


	return nResult;

}

Thread_Obj* CResourceMgr::GetThreadObj(LPVOID BaseClass)
{
	
	Thread_Obj *newobj = NULL;


	newobj = (Thread_Obj *) CHeap::GetHeap(sizeof(Thread_Obj),&ResourceHeap);
	//ZeroMemory(newobj,sizeof(Thread_Obj));	

	newobj->Event = WSACreateEvent();
    if (newobj->Event == 0) 
    {
        #ifdef _CONSOLEDBG 
		_cprintf("GetThreadObj: WSACreateEvent failed:/n");
		#endif
		 CHeap::FreeHeap(newobj , &ResourceHeap);
		 //delete newobj;
         return NULL;
    }

	newobj->EndThreadEvent = WSACreateEvent();
    if (newobj->EndThreadEvent == 0) 
    {
        #ifdef _CONSOLEDBG 
		_cprintf("GetThreadObj: EndThreadEvent WSACreateEvent failed:/n");
		#endif
         CHeap::FreeHeap(newobj , &ResourceHeap);
		 return NULL;
    }

	/*newobj->WaitThreadEvent = WSACreateEvent();
	if (newobj->WaitThreadEvent == 0) 
    {
         printf("GetThreadObj: WaitThreadEvent WSACreateEvent failed:/n");
         CHeap::FreeHeap(newobj , &ResourceHeap);
		 return NULL;
    }*/
 

	 InitializeCriticalSection(&newobj->ThreadCritSec);

	 newobj->Handles[0] = newobj->Event;
	 newobj->Handles[1] = newobj->EndThreadEvent;
	 //newobj->Handles[2] = newobj->WaitThreadEvent;
	 



	 newobj->EventCount = 2;


	 newobj->_BaseClass = BaseClass;
	 
	 unsigned  ThreadId;
	 //newobj->Thread = CreateThread(NULL, 0, IoThread , (LPVOID) newobj, 0, &ThreadId);				 
	 newobj->Thread = (HANDLE)_beginthreadex( NULL, 0, IoThread, (LPVOID) newobj, 0, &ThreadId );



	 AddThreadList(&m_ThreadList,newobj);

    //OutputDebugString("Get a Thread OK\n");
	return newobj;
	
	
}


void CResourceMgr::FreeThreadObj(Thread_Obj* obj)
{

    
	//EnterCriticalSection (&m_CritSec);

    Buffer_Obj *ptr = obj->BufferList;
	while (ptr)
	{	  			
			Buffer_Obj* nextobj =  ptr->next;
				FreeBufferObj(ptr);
			ptr = nextobj;			

	}

	obj->BufferList = NULL;

	DelThreadList(&m_ThreadList,obj);
	
	WSASetEvent(obj->EndThreadEvent);
	WaitForSingleObject(obj->Thread, INFINITE);
	
	 

	WSACloseEvent(obj->Event);
	WSACloseEvent(obj->EndThreadEvent);	

	CloseHandle(obj->Thread);
	//WSACloseEvent(obj->WaitThreadEvent);
	

	
	DeleteCriticalSection (&obj->ThreadCritSec);
	//CloseHandle(obj->Thread);	
	

	CHeap::FreeHeap(obj,&ResourceHeap);

	//LeaveCriticalSection (&m_CritSec);

	//OutputDebugString("Free a Thread OK\n");

 
}

void CResourceMgr::RenumberEvents(Thread_Obj *thread)
{


	
	EnterCriticalSection(&thread->ThreadCritSec);
	Buffer_Obj* obj;

		  obj = thread->BufferList;
		  thread->EventCount = 2;
		  
		  while (obj)
		  {

            thread->Handles[thread->EventCount++] = obj->ol.hEvent;						
			obj = obj->next;
		  }

		  
	 LeaveCriticalSection(&thread->ThreadCritSec);



}

Buffer_Obj* CResourceMgr::FindBufferObjByEvent(Thread_Obj* thread , WSAEVENT* hEvent)
{
	
	
	Buffer_Obj *ptr=NULL;

    EnterCriticalSection(&thread->ThreadCritSec);

    ptr = thread->BufferList;
    while (ptr)
    {
        if (ptr->ol.hEvent == *hEvent)
            break;
        ptr = ptr->next;
    }

    LeaveCriticalSection(&thread->ThreadCritSec);

    return ptr;
 
}

int CResourceMgr::HandleIO(Buffer_Obj* bobj , Thread_Obj* tobj, int IOResult)
{
  OutputDebugString("CResourceMgr::HandleIO\n");
  return IO_OK;
}

/*
int CResourceMgr::BeforeHandle()
{
   return NO_ERROR;
}*/

void CResourceMgr::AddThreadList(Thread_Obj **head, Thread_Obj *obj)
{

	Thread_Obj *end=NULL, 
               *ptr=NULL;

    // Find the end of the list
    ptr = *head;
    if (ptr)
    {
        while (ptr->next)
            ptr = ptr->next;
        end = ptr;
    }

    obj->next = NULL;
    obj->prev = end;

    if (end == NULL)
    {
        // List is empty
        *head = obj;
    }
    else
    {
        // Put new object at the end 
        end->next = obj;
        obj->prev = end;
    }

}
void CResourceMgr::DelThreadList(Thread_Obj **head, Thread_Obj *obj)
{
	   // Make sure list isn't empty
    if (*head != NULL)
    {
        // Fix up the next and prev pointers
        if (obj->prev)
            obj->prev->next = obj->next;
        if (obj->next)
            obj->next->prev = obj->prev;

        if (*head == obj)
            (*head) = obj->next;
    }

    //return obj;
}
void CResourceMgr::AddSocketList(Socket_Obj **head ,Socket_Obj *obj)
{

	Socket_Obj *end=NULL, 
               *ptr=NULL;

    // Find the end of the list
    ptr = *head;
    if (ptr)
    {
        while (ptr->next)
            ptr = ptr->next;
        end = ptr;
    }

    obj->next = NULL;
    obj->prev = end;

    if (end == NULL)
    {
        // List is empty
        *head = obj;
    }
    else
    {
        // Put new object at the end 
        end->next = obj;
        obj->prev = end;
    }

}

void CResourceMgr::AddBufferList(Buffer_Obj **head ,Buffer_Obj *obj)
{
	Buffer_Obj *end=NULL, 
               *ptr=NULL;

    // Find the end of the list
    ptr = *head;
    if (ptr)
    {
        while (ptr->next)
            ptr = ptr->next;
        end = ptr;
    }

    obj->next = NULL;
    obj->prev = end;

    if (end == NULL)
    {
        // List is empty
        *head = obj;
    }
    else
    {
        // Put new object at the end 
        end->next = obj;
        obj->prev = end;
    }
}
void CResourceMgr::DelSocketList(Socket_Obj **head ,Socket_Obj *obj)
{

   // Make sure list isn't empty
    if (*head != NULL)
    {
        // Fix up the next and prev pointers
        if (obj->prev)
            obj->prev->next = obj->next;
        if (obj->next)
            obj->next->prev = obj->prev;

        if (*head == obj)
            (*head) = obj->next;
    }

}

void CResourceMgr::DelBufferList(Buffer_Obj **head ,Buffer_Obj *obj)
{

   // Make sure list isn't empty
    if (*head != NULL)
    {
        // Fix up the next and prev pointers
        if (obj->prev)
            obj->prev->next = obj->next;
        if (obj->next)
            obj->next->prev = obj->prev;

        if (*head == obj)
            (*head) = obj->next;
    }

}

// CHEAP 記憶體處理
//////////////////////////////////////////////////////////////////////
 
void *CHeap::GetHeap(int size, HANDLE* ResourceHeap )
{
   //char logstr[50];
   //wsprintf(logstr,"GetHeap +%d\n",size);
   //OutputDebugString(logstr);
  
   void *HeapChar = HeapAlloc(*ResourceHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS , size);   
   if (HeapChar == NULL)
   { 
	    OutputDebugString("HeapAlloc NULL!");
		throw ;//"HeapAlloc NULL!";		
   } 
   
    
   
   return HeapChar;
}

int CHeap::GetHeapSize(void *buf, HANDLE* ResourceHeap)
{
	unsigned int size = HeapSize(*ResourceHeap,0,buf);
	return size;
}

void CHeap::FreeHeap(void *obj, HANDLE* ResourceHeap)
{
  
  //char tmp[50];
  //wsprintf(tmp,"DelHeap -%d\n",GetHeapSize(obj,ResourceHeap));
  //OutputDebugString(tmp);
	
   
   if (!HeapFree(*ResourceHeap, 0, obj))
   {
       OutputDebugString("HeapFree Error!\n");
	   throw;// "HeapFree Error";
   } 

   obj = NULL;

}

// CCoreSocket 記憶體處理
//////////////////////////////////////////////////////////////////////

int CCoreSocket::Startup()
{
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  return iResult;
}

void CCoreSocket::Cleanup()
{
	WSACleanup();
}


 

int CCoreSocket::PostRecv(Buffer_Obj *recvobj, unsigned int recvOp , unsigned int OffsetSize)
{

	if (recvobj == NULL) 
	{		
		throw "PostRecv With NULL recvobj";
		return S_FALSE;
	}
	
	if (recvobj->Socket == NULL) 
	{
		throw "PostRecv With NULL socketobj";
		return S_FALSE;
	}


	

	/*
	if ((recvobj->bFreeBufferCalled) || (recvobj->bFreeing))
	{
		char BugStr[50];
		wsprintf(BugStr,"PostRecv : recvobj.bFreeing  or bFreeBufferCalled  Error\n","");
		OutputDebugString(BugStr);
		throw BugStr;
		return S_FALSE;

	}

	if ((recvobj->m_Socket->bFreeing) || (recvobj->m_Socket->bFreeSocketCalled))
	{
	    char BugStr[50];
		wsprintf(BugStr,"PostRecv : recvobj->m_Socket->bFreeing  or recvobj->m_Socket->bFreeSocketCalled  Error\n","");
		OutputDebugString(BugStr);
		throw BugStr;
		return S_FALSE;	
	}*/

	//long *OutStanding = &recvobj->Socket->OutstandingOps;
	//CRITICAL_SECTION *CritSec = &recvobj->Socket->SockCritSec;

	EnterCriticalSection(&recvobj->Socket->SockCritSec);
	//printf("A PostRecv InterlockedIncrement %d\n",recvobj->Socket->s);

	   recvobj->operation = recvOp;

	   WSABUF wbuf;	    
	   int iResult = 0;
	   DWORD  bytes = NULL;
	   DWORD  flags = NULL;

	   wbuf.buf = recvobj->buf + OffsetSize;
	   wbuf.len = recvobj->buflen - OffsetSize ; 

	   

	   if (recvobj->Socket->Protocol == IPPROTO_TCP)
       {
         iResult = WSARecv(
                recvobj->Socket->s,
                &wbuf,
                1,
               &bytes,
               &flags,
               &recvobj->ol,
			   NULL
                );
       }
       else if (recvobj->Socket->Protocol = IPPROTO_UDP)
       {
          iResult = WSARecvFrom(
                recvobj->Socket->s,
                &wbuf,
                1,
               &bytes,
               &flags,
               &recvobj->addr,
               &recvobj->addrlen,
               &recvobj->ol,
                NULL
                );
       }


	   if (iResult == SOCKET_ERROR) 
       {
          int LastError = WSAGetLastError();
		  if (LastError != WSA_IO_PENDING) 
		  { 
			  
			    //char BugStr[50];
				#ifdef _CONSOLEDBG
				_cprintf("PostRecv: WSARecv failed Error : %d\n",LastError);
				#endif
				//OutputDebugString(BugStr);
			 
				//InterlockedDecrement( &recvobj->Socket->OutstandingOps);
				LeaveCriticalSection(&recvobj->Socket->SockCritSec);
            
				return SOCKET_ERROR;
		  }

       }


	
	   //printf("PostRecv InterlockedIncrement %d\n",recvobj->Socket->s);
	   //InterlockedIncrement(&recvobj->Socket->OutstandingOps);
	   //recvobj->Socket->OutstandingOps++;
	   //recvobj->m_OutStanding = true;
	   InterlockedIncrement( &recvobj->Socket->OutstandingOps);
	   LeaveCriticalSection(&recvobj->Socket->SockCritSec);

	
	return NO_ERROR;
}

int CCoreSocket::PostSend(Buffer_Obj *sendobj, unsigned int recvOp)
{
	if (sendobj == NULL) 
	{
		
		throw "PostSend With NULL recvobj";
		return S_FALSE;
	}
	


	//printf("PostSend: Handle Occur %d\n",sendobj->ol.hEvent);



	/*
	if ((sendobj->bFreeBufferCalled) || (sendobj->bFreeing))
	{
		char BugStr[50];
		wsprintf(BugStr,"PostSend : recvobj.bFreeing  or bFreeBufferCalled  Error\n","");
		OutputDebugString(BugStr);
		//throw BugStr;
		return S_FALSE;

	}

	if ((sendobj->m_Socket->bFreeing) || (sendobj->m_Socket->bFreeSocketCalled))
	{
	    char BugStr[50];
		wsprintf(BugStr,"PostSend : recvobj->m_Socket->bFreeing  or recvobj->m_Socket->bFreeSocketCalled  Error\n","");
		OutputDebugString(BugStr);
		//throw BugStr;
		return S_FALSE;	
	}*/

	
	//long *OutStanding = &sendobj->Socket->OutstandingOps;
	//CRITICAL_SECTION *CritSec = &sendobj->Socket->SockCritSec;

	EnterCriticalSection(&sendobj->Socket->SockCritSec);

	WSABUF wbuf;
	int iResult = 0;
	DWORD   bytes;


	sendobj->operation = recvOp;
	wbuf.buf = sendobj->buf;
    wbuf.len = sendobj->buflen;



	/*if (sendobj->Socket->s == INVALID_SOCKET) 
	{
		printf("sendobj->Socket->s == INVALID_SOCKET");
		throw "PostSend With NULL socketobj";
		return S_FALSE;
	}*/

//	printf("A PostSend InterlockedIncrement %d\n",sendobj->Socket->s);


	 

	  if (sendobj->Socket->Protocol == IPPROTO_TCP)
	  {
		  iResult  = WSASend(sendobj->Socket->s,
			                 &wbuf,
							 1,
							 &bytes,
							 0,
							 &sendobj->ol,
							 NULL);
      } 
	  else
      {
			//UDP 程序有改變
		    //期望 sendobj->m_addr 能直接拿到對方資訊

		    //copymemory(sendobj->m_addr,@saRemote,sizeof(saRemote));
			iResult = WSASendTo(
				   sendobj->Socket->s,
				   &wbuf,
				   1,
				   &bytes,
				   0,
				   &sendobj->addr,
				   sendobj->addrlen,
				   &sendobj->ol,
				   NULL);
	  }	
	  
	   if (iResult == SOCKET_ERROR) 
       {
          int LastError = WSAGetLastError();
		  if (LastError != WSA_IO_PENDING) 
		  { 
			  
			    //char BugStr[50];
				#ifdef _CONSOLEDBG
				_cprintf("PostSend: WSASend failed Error : %d\n",LastError);
				#endif
				//OutputDebugString(BugStr);
			 
				//InterlockedDecrement(&sendobj->Socket->OutstandingOps);
				LeaveCriticalSection(&sendobj->Socket->SockCritSec);
            
				return SOCKET_ERROR;
		  }

       }


      //printf("A PostSend InterlockedIncrement %d\n",sendobj->Socket->s);
	  InterlockedIncrement(&sendobj->Socket->OutstandingOps);
	 // printf("PostSend InterlockedIncrement %d\n",sendobj->Socket->s);
	  //InterlockedIncrement(&sendobj->Socket->OutstandingOps);
	  //sendobj->m_OutStanding = true;
	  LeaveCriticalSection(&sendobj->Socket->SockCritSec);




	return S_OK;
}

 

int CCoreSocket::Connect(int protocol , char* RemoteIp , int port , int timeout)
{
    SOCKET s;
	sockaddr_in saRemote;



	int rc = NO_ERROR;

	if (protocol == IPPROTO_UDP)
	{
        s =  socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ;
		//saRemote.sin_family = AF_INET;
		//saRemote.sin_port = htons(port);
		//saRemote.sin_addr.s_addr = htonl(INADDR_ANY);
  
		//bind(s, (SOCKADDR *) &saRemote, sizeof(saRemote));

    }
	else
    {
		s =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

		WSAEVENT ConnectEvent =  WSACreateEvent();
		rc = WSAEventSelect(s , ConnectEvent , FD_CONNECT);

		saRemote.sin_family = AF_INET;
		saRemote.sin_port =  htons(port);
		saRemote.sin_addr.s_addr = inet_addr(RemoteIp);

		rc = connect(s,(SOCKADDR*) &saRemote,sizeof(saRemote));
		if (rc == SOCKET_ERROR)
		{

			int LastError = WSAGetLastError();
				if ( LastError !=  WSAEWOULDBLOCK)
				{
					#ifdef _CONSOLEDBG
					_cprintf("socket Connect failed: %d\n",LastError);
					#endif
					WSACloseEvent(ConnectEvent);
            
					return SOCKET_ERROR;
				}
			}

			WSANETWORKEVENTS NetworkEvents;
			int RecvCode = WSAWaitForMultipleEvents(1,&ConnectEvent, false, timeout, false);
			if (RecvCode == WSA_WAIT_EVENT_0)
			{

		
				RecvCode = WSAEnumNetworkEvents(s, ConnectEvent, &NetworkEvents);

				if  (RecvCode == SOCKET_ERROR)
				{
               //WSACloseEvent(ConnectEvent);
               
					rc = SOCKET_ERROR;
               
				}
				else
				{


					if ((NetworkEvents.lNetworkEvents & FD_CONNECT) && (NetworkEvents.iErrorCode[FD_CONNECT_BIT] == 0))
					{
						// clear select
						//Result  :=   NO_ERROR;
						//Result := PostRecv(buffobj,OP,0);
						// return NO_ERROR;
						rc = NO_ERROR;
					}
					else
					{
						rc = SOCKET_ERROR;
					}

				}

			}
			else if  (RecvCode == WSA_WAIT_TIMEOUT)
			{
				rc =  SOCKET_ERROR;
          //exit;
			} else if (RecvCode == WSA_WAIT_FAILED)
			{
				int LastError = WSAGetLastError();
           
				#ifdef _CONSOLEDBG
				_cprintf("WSA_WAIT_FAILED failed: %d\n",LastError);
				#endif

				rc =  SOCKET_ERROR;
					
          
			}



			WSAEventSelect(s , ConnectEvent , 0);
			WSACloseEvent(ConnectEvent);

	
	//}

	if (rc != NO_ERROR)
	{
		closesocket(s);
		s = SOCKET_ERROR;
	}

	return s;
}

int CResourceMgr::SendLn(Socket_Obj* clientobj,char* SendStr)
{

	int rc = 0;
	//int sendlen = strlen(SendStr)+2;
	int sendlen = strlen(SendStr);

	Buffer_Obj *sendobj = GetBufferObj(clientobj, sendlen);
	memcpy(sendobj->buf, SendStr, sendlen);

	/*
	int i = 13;
	memcpy(sendobj->buf+(sendlen-2),&i,1);
	i = 10;
	memcpy(sendobj->buf+(sendlen-1),&i,1);

    */
	if (rc = CCoreSocket::PostSend(sendobj,OP_WRITE) != NO_ERROR)
	{
				 FreeBufferObj(sendobj);
				 //error = SOCKET_ERROR;
	}

    

	return rc;
}


////
//// CBaseClient
//////////////////////////////////////////////////////////////////////////////
CBaseClient::CBaseClient()
{
   //m_SocketHandle = INVALID_SOCKET;
   m_RemoteIp[0] = 0;
   m_eventtimeout = INFINITE;

   
}

CBaseClient::~CBaseClient()
{
   
  
/*if (m_SocketHandle != INVALID_SOCKET)
   {
	   closesocket(m_SocketHandle);
	   m_SocketHandle = INVALID_SOCKET;

   }*/
}

int CBaseClient::SendBuffer(Socket_Obj* clientobj,char* buf,int len)
{
	int rc = 0;
	int sendlen = len;

	Buffer_Obj *sendobj = GetBufferObj(clientobj, sendlen);
	memcpy(sendobj->buf, buf, sendlen);	

	memcpy(&sendobj->addr,&saRemote,sizeof(saRemote));
	sendobj->addrlen = sizeof(saRemote);


	if (rc = CCoreSocket::PostSend(sendobj,OP_WRITE) != NO_ERROR)
	{
				 FreeBufferObj(sendobj);
				 //error = SOCKET_ERROR;
	}

    

	return rc;


}

HANDLE CBaseClient::Connect(int protocol , char* RemoteIp , int port , int timeout ,int eventtimeout)
{
	
	//int rc = NO_ERROR;

	m_protocol = protocol;    
	strcpy(m_RemoteIp,RemoteIp);
	m_port = port;
	m_timeout = timeout;
	m_eventtimeout = eventtimeout;

	Thread_Obj *newthread = GetThreadObj(this); //thread obj 交給上層釋放
												//自己不做


	return newthread->Thread; 
}



int CBaseClient::BeforeHandle()
{

   //連線
	int rc = NO_ERROR;

  //if (m_SocketHandle == INVALID_SOCKET)
  //{


	int s = CCoreSocket::Connect(m_protocol,m_RemoteIp,m_port,m_timeout);

	if (s != SOCKET_ERROR)
	{
	 
	
	 
		saRemote.sin_family = AF_INET;
		saRemote.sin_port =  htons(m_port);
		saRemote.sin_addr.s_addr = inet_addr(m_RemoteIp);

		//m_SocketHandle = s;
		Socket_Obj *sock = GetSockObj(s,AF_INET,m_protocol);



		//rc =  OnConnected(sock);
		//if (rc == NO_ERROR)
		//{
		
			for (int i = 0 ; i < DEFAULT_OVERLAPPED_COUNT ; i++)
			{

				Buffer_Obj *buf = GetBufferObj(sock,DEFAULT_BUFFERSIZE);			

				rc = CCoreSocket::PostRecv(buf,OP_BUFFER,0);
				if (rc != NO_ERROR)
				{		
					FreeBufferObj(buf);
					FreeSockObj(sock);
					break;
				} 
			}
		//}
		//else
	//	{
	//	   rc = SOCKET_ERROR;
	//	}

			//先 postrecv 在 send
			if (rc == NO_ERROR)
			{
			  rc = OnConnected(sock);
			}

			

	}
	else
	{
	    OutputDebugString("Connect Error \n");
		rc = SOCKET_ERROR;
	}

  //}
 // else
  //{
  
	 //  printf("Socket Already Got Error \n");
	//   rc = SOCKET_ERROR;
  //}

   return rc;
}

int CBaseClient::OnConnected(Socket_Obj *sock)
{

	//return SendLn(sock,"GET /");
	return NO_ERROR;

}

int CBaseClient::OnDataRead(Buffer_Obj *bobj)
{ 
	if (bobj->bytes <= DEFAULT_BUFFERSIZE)
	{
		  bobj->buf[bobj->bytes] = '\0';
	}

	 //printf("%s",bobj->buf);	

	 return IO_OK;


}

int CBaseClient::OnDataWrite(Buffer_Obj *bobj)
{
	return IO_OK;
}


void CBaseClient::OnDataFinish()
{
}

int CBaseClient::OnBufferError(Buffer_Obj *bobj)
{

	return IO_OK;
}

int CBaseClient::HandleIO(Buffer_Obj* bobj , Thread_Obj* tobj , int IOResult)
{

   if (IOResult != IO_OK)
   { 
	   OnBufferError(bobj);
	   return IOResult;
   }
   else
   {
	if (bobj->operation == OP_WRITE)
	 {		 
	    OnDataWrite(bobj);
		return IO_NOTPOSTRECV;
	 }
	 else if (bobj->operation == OP_BUFFER)
	 {
       
	   
		 return OnDataRead(bobj);
	 }
	 else
	 {
		 return IOResult;
	 }
   }
}

 

unsigned __stdcall IoThread(LPVOID lpParam)
{
	 
   	int          index=0,
                 count=0,
                 rc=0,
                 i=0;


	

	
	Thread_Obj  *thread=NULL; 
	thread = (Thread_Obj *)lpParam;

	HANDLE m_ThreadHandle = thread->Thread; 

	CBaseClient *BaseClient = (CBaseClient *) thread->_BaseClass;
	CResourceMgr::RenumberEvents(thread);

	
 
  
  if (BaseClient->BeforeHandle() == NO_ERROR)
  {
 
	while(1)
	{
		
		//printf("WaitForMultipleObjects In\n");

	
		rc = WaitForMultipleObjects(
                thread->EventCount,
                thread->Handles,
                FALSE,
                BaseClient->m_eventtimeout
                );

		//printf("WaitForMultipleObjects Out\n");
	

        if (rc == WAIT_FAILED || rc == WAIT_TIMEOUT)
        {
            if (GetLastError() == ERROR_INVALID_HANDLE)
            {
                CResourceMgr::RenumberEvents(thread);
                continue;
            }
            else
            {
              
                //Sleep(10);
				#ifdef _CONSOLEDBG
				_cprintf("Time Out \n");
				#endif
				break;
            }
        }

		if (rc == WAIT_OBJECT_0 + 1)
		{
			break;
		} 
		

		//count = thread->EventCount;
        //for(i=0; i < count ;i++)
        //{

             

		//	rc = WaitForSingleObject(
         //           thread->Handles[i],
         //           0
         //           );
        //    if (rc == WAIT_TIMEOUT)
        //    {
                // This event wasn't signaled continue to the next one
        //        continue;
        //    }
            //index = i;
		    index = rc;

			//printf("%d \n",index);
            // Reset the event first
            WSAResetEvent(thread->Handles[index]);

            if (index == 0)
            {
                // The thread's event was signaled indicating new I/O assigned
                CResourceMgr::RenumberEvents(thread);
                //break;
            }
			else if (index > 1)
            {
                // Otherwise, an overlapped I/O operation completed, service it
				
				Buffer_Obj *tmpBuffObj = CResourceMgr::FindBufferObjByEvent(thread, &thread->Handles[index]);
			 
				tmpBuffObj->ThreadObj = thread;
				//tmpBuffObj->Socket->LastTime = time((time_t *)0);
				
				
				Socket_Obj *sock =  tmpBuffObj->Socket;
				//char tmpchar[50];
				//printf("IoThread: Handle Occur %d\n",thread->Handles[index]);
				//OutputDebugString(tmpchar);
				
	
				int iResult = 0;
				DWORD  bytes = NULL;
				DWORD flags = NULL;
				int LastError = 0;
				int IOResult = 0;
	
				iResult = WSAGetOverlappedResult(
					sock->s,
					&tmpBuffObj->ol,
					&bytes,
					false,
					&flags
					);

				if (iResult == false || bytes == 0)
				{
	 
					
					
					LastError = WSAGetLastError();
					

					if (LastError == WSAECONNRESET)
					{
						OutputDebugString("IoThread: WSAECONNRESET\n");
					}
					else if (LastError ==  WSAENOTSOCK)
					{
						OutputDebugString("IoThread: WSAENOTSOCK\n");
					}
					else
					{
						OutputDebugString("HandleIo: WSAGetOverlappedResult failed\n");
					};

				

					// return IO_ERROR;
					IOResult = IO_ERROR;


	 
				}
				else
				{
				
					tmpBuffObj->bytes = bytes;
					IOResult = IO_OK;

				}


				
				
				
				EnterCriticalSection(&sock->SockCritSec);				 
				   IOResult = BaseClient->HandleIO(tmpBuffObj,thread,IOResult);			  
				LeaveCriticalSection(&sock->SockCritSec);

				if (IOResult == IO_OK)
				{
					
					//tmpBuffObj
					if (CCoreSocket::PostRecv(tmpBuffObj,OP_BUFFER,0) != NO_ERROR)
					{
						BaseClient->FreeBufferObj(tmpBuffObj);					
					}
				
				}
				else if (IOResult == IO_ERROR || IOResult == IO_NOTPOSTRECV)
				{				
					
					BaseClient->FreeBufferObj(tmpBuffObj);
				
				}

			 
				InterlockedDecrement(&sock->OutstandingOps);
				
			 
				
				//OutputDebugString("IoThread: InterlockedDecrement\n");

				if ((sock->OutstandingOps == 0)) 
				{
					BaseClient->FreeSockObj(sock);
					goto EndProcess;
				}
            }
		//}
	}

 }
 else
 {
	#ifdef _CONSOLEDBG
	 _cprintf("Before Handle Error\n");
	#endif
 }


EndProcess:

 	BaseClient->OnDataFinish();
    

	

	//CloseHandle(m_ThreadHandle);
	//SetEvent(m_ThreadHandle);
	//m_ThreadHandle = NULL;

	//ExitThread(0);
	_endthreadex(0);
	
	


    return 0;
}


