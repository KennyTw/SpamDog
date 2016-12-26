/////////////////////////////////////////////////////////////////////////////
// MailParser

#if !defined MAILPARSER__INCLUDED
#define MAILPARSER__INCLUDED

#include <wchar.h>
#include <stdio.h>

#ifdef DLLDIR_EX    
   #define DLLDIR  __declspec(dllexport)   // export DLL information    
#else    
   #define DLLDIR  __declspec(dllimport)   // import DLL information    
#endif

typedef struct _MailData
{
		char* mailHeader;
		char* mailBody;
} MailData;

typedef struct _ContentType
{

	char* text;
	char* type;
	char* boundary;
	char* charset;
	char* name;

} ContentType;



typedef struct _ReceievedData
{
	char* Receieved;
	_ReceievedData *next;

} ReceievedData;

typedef struct _ReceivedList
{
	ReceievedData *m_ReceievedData;
	int size;

} ReceivedList;


typedef struct _MailHeader
{
		char*  From;
		char*  Sender;
		char*  To;
		char*  Cc;
		char*  Bcc;
		char*  Subject;
		char*  Reply_To;
		char*  Content_Transfer_Encoding;
		ContentType*  Content_Type;
		char*  Return_Path;
		//char*  Received;
		ReceivedList m_ReceievedList;
		char*  Content_ID;
		char*  Message_Id;
		char*  Date;
		bool  IsAttachment;

		//SpamDog 專用
		char*  SpamDogMailFrom;
		char*  SpamDogRcptTo;
		char*  SpamDogRemoteIp;
		char*  SpamDogXMailer;
} MailHeader;

typedef struct _MailBoundary
{
		//char*  BoundPos;
		//int  BoundLen;
		char *BoundContent;
		MailHeader *BoundHeader;
		MailData *BoundMail;
		unsigned int level;
        bool IsRealData;
		_MailBoundary *next;

} MailBoundary;



/////////////////////////////////////////////////////////////////////////////
// CEmail
class DLLDIR CEmail
{
private:
     unsigned int FileSize;
	 //char* mailContent;	 

	 MailBoundary* BoundaryParser(char* MailContent, char* BoundStr, MailBoundary* AMailBoundary);
	 void AddReceivedList(ReceivedList* list , char *ReceievedStr);
	 void FreeReceivedList(ReceivedList* list);
	 void Trim(char *AStr);
	 void ReplaceToSpace(char *AStr);
	  
	 


	 
public: 
	CEmail();
	~CEmail(); 

	void TrimQuote(char *AStr , char Quote);

	void SplitEmailAddress(char *AddressStr , char* PersonalName , char* EMail);

	MailData* GetMailData(char* MailContent);	
	void FreeMailData(MailData* AMailData);

    MailHeader* GetMailHeader(char* MailHeader);
	void FreeMailHeader(MailHeader* AMailHeader);


    MailBoundary* GetBoundary(char* MailContent, char* FirstBoundStr);
	void FreeBoundary(MailBoundary* AMailBoundary);

	bool CheckValid(char *Email);
 	


};

/////////////////////////////////////////////////////////////////////////////
// CMailCodec
class DLLDIR CMailCodec
{

private:

	//char *BaseBuffer , *QPBuffer , *HeaderBuffer;
	//char *UniBuffer;
	void ParseRTF(char *buf,unsigned int *len, int *idx , int *parseidx ,int *bignoreidx);
	char* ImapBase64M(wchar_t* InStrW , unsigned int size);	
	void DeImapBase64M(char* InStr,unsigned int len , char** OutBuff,unsigned int *OutSize);	

public:
	static char Base64Table [128];
	static char Base64Table2 [128]; //http 傳輸用
	static char Base64EnTable[65];
	static char Base64EnTable2[65]; //http 傳輸用
	static char Base64TableM[128];
	static char Base64EnTableM[65];
	static int ImapFolderTable[128];

	static int QPTable[128];
	static int IntTable[128];
	static int StrToInt(char* Astr);
	char *DeHeaderCode(char *AStr);
	void FreeDeHeaderCode(char *AStr);

	void Free(char *Buffer);

	


	CMailCodec();
	~CMailCodec();

	char* Base64Decode(char* InStr);
	void Base64Decode(char* InStr,char** OutBuff,unsigned int *OutSize);
	void FreeBase64Decode(char* Buffer);
	char* Base64Encode(char* InStr , unsigned int size);
	void FreeBase64Encode(char* Buffer);

	char* Base64Encode2(char* InStr , unsigned int size);
	void Base64Decode2(char* InStr,char** OutBuff,unsigned int *OutSize);

	char* ImapUTF7Encode(char* InStr , unsigned int size);	
	void ImapUTF7Decode(char* InStr,char** OutBuff,unsigned int *OutSize);
	
 
	void ParseRTFBuf(char *buf,unsigned int*len);
	

	char* QPDecode(char* InStr , char* StyleCode = "=");
	void FreeQPDecode(char* Buffer);

 

	char* UniDecode(char* InStr);
	void FreeUniDecode(char* Buffer);

	char* HeaderDecode(char* InStr);
	void FreeHeaderDecode(char* Buffer);

	void TrimHTML(char* InStr, char* OutBuffer);
};

#endif