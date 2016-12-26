#if !defined(Global_INC)
#define Global_INC
#pragma warning (disable:4786)
#include <list> 
#include <string>



#define CurrentDBVersion 1

using namespace std;

struct OEFolder
{
  DWORD OEFolderId;
};

struct OLFolder
{
  unsigned long FolderLen;
  unsigned char FolderId[255];
};

 
typedef struct _FolderListData
{

  char FolderName[255];
  
  union 
  {
	OLFolder olFolder;
	OEFolder oeFolder;
  };

} FolderListData; 

typedef list<FolderListData> FolderList;


typedef struct _RuleData
{
  wchar_t Rule[255];
  char FolderName[255];   
  COLORREF ShowColor;
  char CheckType;
  char AlertType;
} RuleData;

//typedef  list<RuleData> RuleList;
 
//old
/* 
struct RuleFile
{
  char Rule[255];
  FolderListData ListData;
  COLORREF ShowColor;
  char CheckType;
  char AlertType;
};
*/

typedef struct _UpdateHeader
{
	char Type ; //0 : good , 1 : bad , 2 : FixOK , 3: FixBad
	unsigned int ItemCount;	 
	unsigned int DataLen; 
} UpdateHeader;

typedef struct _ZipHeader
{
	 bool isCompress;
	 unsigned int OrgLen; //��l����
	 unsigned int ThisLen; //���ɪ���
} ZipHeader;

struct RuleHeader
{
   unsigned int LastId;
};

struct RuleFile
{
  char Rule[255];
  char Rule2[255];
  FolderListData ListData; //���ʪ��ؿ����c
  COLORREF ShowColor; //����C��
  char CheckType; //1 : �H���  2 :�@�D��
  char AlertType; // 1 : �@��   2 :  �S��
  char RuleType; //0: �@��ϥΪ̩w�q, 1 : �H��l�ܾ�, 2 : �զW��
  unsigned int RuleId;
  bool Used; //�O�_����
  long ExpireTime; //���Įɶ�
};
typedef  list<RuleFile> RuleList;
typedef  list<string> StrList;

#define ExVirus		-1
#define ExHasChecked 1



#endif