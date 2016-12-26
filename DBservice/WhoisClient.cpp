// WhoisClient.cpp: implementation of the CWhoisClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WhoisClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWhoisClient::CWhoisClient()
{
	CCoreSocket::Startup();
}

CWhoisClient::~CWhoisClient()
{
	CCoreSocket::Cleanup();

}

int CWhoisClient::OnConnected(Socket_Obj *sock)
{
	//connect 成功 , 送出指令
	return SendLn(sock,m_Cmd);
}

int CWhoisClient::OnDataRead(Buffer_Obj *bobj)
{
	CBaseClient::OnDataRead(bobj);

	//bobj->bytes; 收到的 buffer 大小
	//bobj->buf ; buffer 內容
	
	strncat(m_Result, bobj->buf, bobj->bytes);

	
	
	return IO_OK; //如不要中斷傳輸則回傳 IO_OK; return IO_ERROR 為中斷傳輸
}

int CWhoisClient::OnDataWrite(Buffer_Obj *bobj)
{

	CBaseClient::OnDataWrite(bobj);

	//buffer 送出成功
	 

	return IO_OK;//回傳 IO_OK;其於值並無作用
}

void CWhoisClient::OnDataFinish()
{
	 //Scoket Thread 結束前的最後一次呼叫
	OutputDebugString("OnDataFinish\r\n");
}

HANDLE CWhoisClient::Send(char *Host, char *Cmd, int timeout, int eventtimeout)
{

	// initialize command and result
	memset(m_Cmd,0,sizeof(m_Cmd)); 
	memset(m_Result, 0, sizeof(m_Result));

	strcpy(m_Cmd,Cmd); //先存好 cmd str
	strcat(m_Cmd,"\r\n");

	//80 需改為 43 port for whois
	//timeout 為 connect 時的 timeout 時間
	//eventtimeout 為 thread 的 WaitForMultipleObjects timeout 時間
	//return   CBaseClient::Connect(IPPROTO_TCP,Host,80,timeout,eventtimeout);
	return   CBaseClient::Connect(IPPROTO_TCP,Host,43,timeout,eventtimeout);
}
