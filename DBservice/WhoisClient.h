// WhoisClient.h: interface for the CWhoisClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WHOISCLIENT_H__5926BB3B_A83F_4397_8DA1_02D4E298A26E__INCLUDED_)
#define AFX_WHOISCLIENT_H__5926BB3B_A83F_4397_8DA1_02D4E298A26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Swsocket/CoreClass.h"

class CWhoisClient : public CBaseClient  
{
private:
	
	char m_Cmd[255];	
	
	int OnConnected(Socket_Obj *sock); //override CBaseClient::OnConnected
	int OnDataRead(Buffer_Obj *bobj);  //override CBaseClient::OnDataRead
	int OnDataWrite(Buffer_Obj *bobj); //override CBaseClient::OnDataWrite
	void OnDataFinish(); //override CBaseClient::OnDataFinish , Scoket Thread 結束前的最後一次呼叫	
	

public:	
	char m_Result[1024*10];

	CWhoisClient();  // constructor
	virtual ~CWhoisClient();  // destructor
	
	HANDLE Send(char* Host, char* Cmd , int timeout,int eventtimeout);

};

#endif // !defined(AFX_WHOISCLIENT_H__5926BB3B_A83F_4397_8DA1_02D4E298A26E__INCLUDED_)
