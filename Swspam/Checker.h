#if !defined(CLASS_CLASSCHECKER)
#define CLASS_CLASSCHECKER

#pragma warning (disable:4786)
#include <list>
#include <string>

#include "SpamDB.h"
#include "../Global.h"
#pragma warning(disable: 4251)


using namespace std;

typedef  list<SDataFile> DataFileList; //DataFile


typedef struct _DomainUpload
{
  bool isUpload;
  char ReportType; //0 : 正常 Bad Key , 1 : UnkonwnToSpam Key , 2 : 正常 ok key , 3 : UnkonwnToOk Key ,4 : Error Spam->Ok , 5 : Error Ok->Spam
} DomainUpload;

typedef struct _BlockSubject
{
  bool isUpload;
  char ReportType; 
  long AddTime;
} BlockSubject;

typedef struct _ReportData
{   
  
  char ReportType;  // 0 : 未使用 , 1 : Spam , 2: ok
  bool isUpload;
  //char Subject[255];
  //char MailTime[35];// Mail Send Time
} ReportData;

typedef struct _ReportMailData
{   
  char Subject[255];
  char Type; //0 : spam mail , 1: system check mail , 2 : ok mail
  char Sender[34];// Mail Sender
  char IP[20];
} ReportMailData;

//與 DBtool 重覆 名稱不同
 
struct UpdateFile2
{
	char Type ; //0 : good , 1 : bad
	unsigned int NumberItem;
	 
}; 

/*
typedef struct _MailPath
{   
  char PathKey[120];
  char PrevPath[255]; 
  int PrevSize;
  char CurrPath[255]; 
  int CurrSize;
  
} MailPath;*/

typedef struct _MailPath
{   
  char PathKey[120];
  char PathStr[5][255]; 
  char PathSubject[5][8];
  int NextWrite ;
} MailPath;



struct Contact
{	
	char netname[255];
	char country[3];
	char Email[255];
};

class DLLDIR CSpamContact
{
private:
	#define APNIC 1
	#define ARIN 2
	#define LACNIC 3
	#define RIPE 4
	#define AFRINIC 5
	#define BRNIC 6

	char *InetNumS;
	char *InetNumE;
	char *netname;
	char *country;
	char *contact;


	

	char IPs[4][4];
	int iIPs[4][1];
	

	void ParseAPNIC(char *WhoisStr);
	void ParseRIPE(char *WhoisStr);
	void ParseARIN(char *WhoisStr);	
	void ParseLACNIC(char *WhoisStr);
	char ParseLACNICHeader(char *WhoisStr);

public:
	bool bSpamCop;
	/*char InetNumS[16];
	char InetNumE[16];
	char netname[255];
	char country[3];
	char contact[255];*/





	char IP[16];
	char ReverseIP[16];
	void SetIp(char *ip);
	void SetIp(unsigned int ip);
	int GetNic(char *ANic);
	int GetNicWeb(char *ANic);
	

	void GetContact(char *_InetNumS , char* _InetNumE, char* _netname,char* _country,char* _contact);
	unsigned long GetIPValue();
	char* GetSpamCop();
	void TrimResponse(char *AStr);

	CSpamContact();
	~CSpamContact();

};

//-------------------------------------------




class DLLDIR CChecker
{
private:
	
	CRITICAL_SECTION*  mDBSection;
	char *m_subject;
	char *m_sender;
	char *m_receiveIP;

	//char DnsIp[16];
	//char Rbl1[255];
	//char Rbl2[255];

	char IniPath[_MAX_PATH];

	//char AllByStr2[255];

	//std::string AllByStr2;
	
	 

	SDataFile m_receiveDataFile;	

    char DbSavePath[_MAX_PATH];
    std::string Checkerstdstr ;
 
	 CSPAMDB *db;
	 DataFileHeader mfileheader;

	 //CTreeRangeDB *ContactDB;

	 //CDB2  *dmDB;

	 //CDB *IpDB;

	 int KeyCount;

	 

	 int ParseKey(char *Source ,StrList *KeyList , char* CutStr);
	 int ParseNumKey(char *Source ,StrList *KeyList );
	 void TrimPathKey(char *Path);
	 void ToGramKey(char *AStr);
	 int GetPathIdent(char *Path1 , char *Path2); //回傳第幾個 receieve
	 static void SendMailData(char *ServerIP , char *FilePath , char* DogID );
	 
	 
public:

	
	StrList ResList;
	StrList SecResList;
	StrList DomainResList;
	

	StrList DomainList;

	DataFileList DataList;
	DataFileList SecDataList;

	bool bPassKey(char * KeyStr ,int startpos );
	bool bPassKey2(char * KeyStr);

	CChecker(CRITICAL_SECTION*  DBSection , char* DBFileName = "102db" , char *IniFileName = "SpamDog.ini");
	~CChecker();
	static string lowercase(string Astr);

	void GetGramKeys(char* InStr);
	void GetCheckKeys(char* Source , char* BodySource ,char * SenderName, char *Subject ,char *ReceiveIP ,  bool SpamSubject = true);
	void GetSpamData();
	double GetSpamRate(double LimitRate = 0.5);	
	double GetDomainSpamRate();	
	void UpdateSpamRate(int OKFix, int BadFix ,bool AutoUpdate = false); // 更正數值
	//void UpdateSpamRateLimit(DataFileList* newdatafilelist,int OKFix, int BadFix); // 更正數值
	HRESULT ResolveDomains();
	void ResolveIP(char *IP , char *Result);
	//void WriteKeyUsedLog(char* key);

	//void WriteDomainLog();
	//void FixDomainLog();
	void WriteReportLog(char ReportType = 0);
	//void WriteBSLog(char ReportType = 0);

	//static void SendDomainLogToServer(char* ServerIP);
	static void SendReportToServer(char* ServerIP,char* DogID , HWND ProgressBar = NULL);
	//static void LiveUpdate(char* ServerIp);
	static HRESULT ParseKeyFile(CRITICAL_SECTION*  DBSection,char *FileName , HWND ProgressBar = NULL);
 
	

	void DeleteDomainLog();

	bool SimpleCheckDomain(char* Domain);
	
	char *GetKeyGenStr();
	char  GetHeaderIP(char* Subject, char* header , char *IP ,MailPath *mailpath);
	void  WriteMailPathData(MailPath *mailpath);
	char  GetKeyStrIP(char* header ,char *KeyStr , char *IP); //以關鍵字找 receieve
	char  GetKeyPosIP(char* header ,int KeyPos , char *IP); // 以 receieve 順序找 source ip
	void  GetHeaderKeyStrIP(char* header , char* headerkey , char *IP);

	bool  CheckLocalMail(char* header);
	bool  GetMailCharset(char* MailContent , char* Charset);
	//void  WriteWhoisInfo(char* IP);
	

	void  GetLastReceiveBy(char* header , char* ReceiveBy); //如果沒有 ident string , 則取 last receieve 的 by .
	void  GetDomainIdent(char* ReceiveBy , char* DomainIdent);

	void  ReportMail(char RpType , char* MsgID ,  char *SenderName  , char *MailContent  );//RpType = 1->Spam , 2->OK , 3->Spam-OK , 4, Ok->Spam
	static void ReportSysMail(char* MsgID , char *MailContent );
	int CheckServerSpam(char *ServerIP ,char* DogID , char *IP);
	void MainTainRBL(char *ServerIP ,bool BadIp,char *IP);

	bool CheckRBL(char* IP);

	//void WriteMailPath();
};
 
#endif
