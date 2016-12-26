#if !defined SPAMDBCLASS__INCLUDED
#define SPAMDBCLASS__INCLUDED

#include "../Swfiledb/DB.h"

typedef struct _DataFile
{
	
	//char Data[100];//真實資料 , Data 結構的必要成員
	int OkRate;
	int BadRate;
	unsigned int UsedCount;
    unsigned int SpamCount; //new
	char Status; //0: Free ,1 : used , 2 : Ip Data , 3 : Public GateWay , 4 : ENews

} DataFile;

typedef struct _SDataFile
{
	int DataFilePos;	
	DataFile mDataFile;
//	char KeyType; //0: ResList 1: SecondaryResList

} SDataFile;

typedef struct _DataFileHeader
{
	 
	int Version;
	int NullValue;
	
} DataFileHeader;

typedef struct _DataFileRes
{

  DataFile datafile;
  int SearchRes;

} DataFileRes;



class DLLDIR CSPAMDB : public CDB 
{
private:
	
public:
	CSPAMDB(CRITICAL_SECTION*  DBSection ,char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , int FileBuffSize);
	virtual ~CSPAMDB();
	virtual DataFileRes GetSpamData(char* KeyStr);	
	virtual int InsertSpamData(char* KeyStr , DataFile *datafile);
	virtual void UpdateData(char* DataBuff , int BuffSize , int DataPos , int DataOffset = 0);

};


#endif