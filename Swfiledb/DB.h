// DB.h: interface for the CDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_H__51D3675D_41A1_4025_B5F5_B6467E93418A__INCLUDED_)
#define AFX_DB_H__51D3675D_41A1_4025_B5F5_B6467E93418A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information
#endif 
 
#include <stdio.h>
#include <time.h>

//一個 bucket 裡面的預設空間
#define DefaultBucketSize 20
#define DefaultKeyDataSize 25

typedef struct _Hash
{
	unsigned int Value;
	//int KeyLen;
	//char SplitChar[3];
} Hash;

typedef struct _IndexData
{ 
		//int HashValue; //未經過 mod 的原始值
	    Hash hash;
		char KeyData[DefaultKeyDataSize];
		unsigned int DataFilePos; //資料的實際位置
		char Status; //0: Free ,1 : used , 2 : delete
		int ChainData;
} IndexData;



typedef struct _Bucket
{

	struct _IndexData IndexData[DefaultBucketSize];
	char Status; //0 : Used , 1 : delete
	int ChainData; // -1 : 下面沒有 overflow data

} Bucket;

/*
//兩倍大小
typedef struct _IndexData2
{ 
		 
	    Hash hash;
		char KeyData[DefaultKeyDataSize * 2];
		unsigned int DataFilePos; //資料的實際位置
		char Status; //0: Free ,1 : used , 2 : delete
		int ChainData;
} IndexData2;

typedef struct _Bucket2
{

	struct _IndexData2 IndexData[DefaultBucketSize];
	char Status; //0 : Used , 1 : delete
	int ChainData;

} Bucket2;

typedef struct _SResult2
{

	int FindPosInKey;
	int FindPosInOvr;
	int FreePos;
	int BucketPos;
	int DataFilePos;
	Hash hash;
	Bucket2 SBucket;
	

} SResult2;

*/

size_t DBfread(void *buffer, size_t size, size_t count, FILE *stream );
size_t DBfwrite(const void *buffer, size_t size, size_t count,  FILE *stream );
int DBfseek(FILE *stream,long offset, int origin );

typedef struct _IndexHeader
{

	unsigned int SplitPos;
	unsigned int IniBucket;
	unsigned int BucketCount;

} IndexHeader;

typedef struct _OverFlowHeader
{
	char check;
} OverFlowHeader;

 

typedef struct _SResult
{

	int FindPosInKey;
	int FindPosInOvr;
	int FreePos;
	int BucketPos;
	int DataFilePos;
	Hash hash;
	Bucket SBucket;
	

} SResult;

struct node
{
  unsigned int MinKeyValue;
  unsigned int MaxKeyValue;

  unsigned int LeftFilePos;
  unsigned int RightFilePos;
  unsigned int DataFilePos;
};

struct TreeDBHeader
{
  time_t LastVersionTime;  
};
 

class  DLLDIR CTreeRangeDB
{
private:
	FILE *FileFp,*DataFp;	
	TreeDBHeader header;
	bool FirstRec;
	char* m_FileDBPath;
	char* m_DataDBPath;
	CRITICAL_SECTION*  mDBSection;


public:
	int FirstRecPos;
	int InsertData(char* DataBuff , int BuffSize);
	void SelectData(int DataPos , char* FillBuff , int BuffSize);

	int InserKey(unsigned int MinKeyValue,unsigned int MaxKeyValue,unsigned int FilePos,unsigned int DataFilePos);
	int Search(unsigned int KeyValue,unsigned int FilePos);

	long RebuildDB(long ValidSec);
	
	void OpenDB();
	void CloseDB();

	CTreeRangeDB(CRITICAL_SECTION*  DBSection , char* FileDBPath,char* DataDBPath);
	~CTreeRangeDB();
};

class DLLDIR CDB 
{
private: 
	
    CRITICAL_SECTION*  mDBSection;

	char *IndexFileBuf;
	char *OverflowFileBuf;
	char *DataFileBuf;

	char mIndexDBPath[MAX_PATH];
	char mOverFlowDBPath[MAX_PATH];
	char mDataDBPath[MAX_PATH];
	unsigned int mDataHeaderSize;
	int mFileBuffSize;
	 
	FILE *IndexDBfp,*OverFlowDBfp,*DataDBfp;
	IndexHeader m_IndexHeader;
	OverFlowHeader m_OverFlowHeader;
	unsigned int IniBucket;

	Hash GetHashValue(char* Astr);
	int GetFreePos(IndexData* indexdata);
	int GetFreeIndexData(FILE *fp , int HeaderSize);
	int GetBucketPos(int HashValue,char type); //0: selection . 1: rehash
	int BucketRehash(); 

public:
	
	

	void OpenDB();
	void CloseDB();

	int InsertKey(char* Astr, int DataFilePos , char type=0);
	int InsertData(void* DataBuff , int BuffSize);
	void UpdateData(void* DataBuff , int BuffSize , int DataPos , int DataOffset = 0);
    SResult SelectKey(char* Astr,char type=0);
	int GetFreeDataPos(unsigned int HeaderSize , unsigned int RecordSize , int CheckOffset , int AvalValue);
	void SelectData(int DataPos , void* FillBuff , int BuffSize);
	 

	CDB(CRITICAL_SECTION*  DBSection , char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , unsigned int DataHeaderSize, int FileBuffSize);
	~CDB();

};

/*
class DLLDIR CDB2 
{
private: 
	
    char *IndexFileBuf;
	char *OverflowFileBuf;
	char *DataFileBuf;
	 
	FILE *IndexDBfp,*OverFlowDBfp,*DataDBfp;
	IndexHeader m_IndexHeader;
	OverFlowHeader m_OverFlowHeader;
	unsigned int IniBucket;

	Hash GetHashValue(char* Astr);
	int GetFreePos(IndexData2* indexdata);
	int GetFreeIndexData(FILE *fp , int HeaderSize);
	int GetBucketPos(int HashValue,char type); //0: selection . 1: rehash
	int BucketRehash();
 

public:

	int InsertKey(char* Astr, int DataFilePos , char type=0);
	int InsertData(char* DataBuff , int BuffSize);
	void UpdateData(char* DataBuff , int BuffSize , int DataPos , int DataOffset = 0);
    SResult2 SelectKey(char* Astr,char type=0);
	int GetFreeDataPos(unsigned int HeaderSize , unsigned int RecordSize , int CheckOffset , int AvalValue);
	void SelectData(int DataPos , char* FillBuff , int BuffSize);

	CDB2(char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , unsigned int DataHeaderSize, int FileBuffSize);
	~CDB2();

};
*/

#endif // !defined(AFX_DB_H__51D3675D_41A1_4025_B5F5_B6467E93418A__INCLUDED_)
