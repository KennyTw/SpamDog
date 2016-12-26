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
	//connect ���\ , �e�X���O
	return SendLn(sock,m_Cmd);
}

int CWhoisClient::OnDataRead(Buffer_Obj *bobj)
{
	CBaseClient::OnDataRead(bobj);

	//bobj->bytes; ���쪺 buffer �j�p
	//bobj->buf ; buffer ���e
	
	strncat(m_Result, bobj->buf, bobj->bytes);

	
	
	return IO_OK; //�p���n���_�ǿ�h�^�� IO_OK; return IO_ERROR �����_�ǿ�
}

int CWhoisClient::OnDataWrite(Buffer_Obj *bobj)
{

	CBaseClient::OnDataWrite(bobj);

	//buffer �e�X���\
	 

	return IO_OK;//�^�� IO_OK;���ȨõL�@��
}

void CWhoisClient::OnDataFinish()
{
	 //Scoket Thread �����e���̫�@���I�s
	OutputDebugString("OnDataFinish\r\n");
}

HANDLE CWhoisClient::Send(char *Host, char *Cmd, int timeout, int eventtimeout)
{

	// initialize command and result
	memset(m_Cmd,0,sizeof(m_Cmd)); 
	memset(m_Result, 0, sizeof(m_Result));

	strcpy(m_Cmd,Cmd); //���s�n cmd str
	strcat(m_Cmd,"\r\n");

	//80 �ݧאּ 43 port for whois
	//timeout �� connect �ɪ� timeout �ɶ�
	//eventtimeout �� thread �� WaitForMultipleObjects timeout �ɶ�
	//return   CBaseClient::Connect(IPPROTO_TCP,Host,80,timeout,eventtimeout);
	return   CBaseClient::Connect(IPPROTO_TCP,Host,43,timeout,eventtimeout);
}
