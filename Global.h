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
	 unsigned int OrgLen; //原始長度
	 unsigned int ThisLen; //此檔長度
} ZipHeader;

struct RuleHeader
{
   unsigned int LastId;
};

struct RuleFile
{
  char Rule[255];
  char Rule2[255];
  FolderListData ListData; //移動的目錄結構
  COLORREF ShowColor; //顯示顏色
  char CheckType; //1 : 寄件者  2 :　主旨
  char AlertType; // 1 : 一般   2 :  特殊
  char RuleType; //0: 一般使用者定義, 1 : 信件追蹤器, 2 : 白名單
  unsigned int RuleId;
  bool Used; //是否有效
  long ExpireTime; //失效時間
};
typedef  list<RuleFile> RuleList;
typedef  list<string> StrList;

#define ExVirus		-1
#define ExHasChecked 1



#endif