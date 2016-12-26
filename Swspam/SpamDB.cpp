#include "stdafx.h"
#include "SpamDB.h"

/////////////////////////////////////////////////////////////////////////////
// CSPAMDB

CSPAMDB::CSPAMDB(CRITICAL_SECTION*  DBSection ,char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , int FileBuffSize) : CDB(DBSection,IndexDBPath ,OverFlowDBPath ,DataDBPath ,sizeof(DataFileHeader),FileBuffSize)
{
 
}

CSPAMDB::~CSPAMDB()
{
 
}

DataFileRes CSPAMDB::GetSpamData(char* KeyStr)
{
   DataFileRes mDataFile;
   memset(&mDataFile , 0 ,sizeof(DataFileRes));

   SResult sres = SelectKey(KeyStr);

   if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
   {
     //找不到資料
	   mDataFile.SearchRes = -1;
	  
   }
   else
   {   
	   //讀出資料
	   SelectData(sres.DataFilePos,(char *) &mDataFile.datafile , sizeof(mDataFile.datafile));   
	   mDataFile.SearchRes = sres.DataFilePos;
   }      


   return  mDataFile;
}

int CSPAMDB::InsertSpamData(char* KeyStr , DataFile *datafile)
{
  
	//先配置資料空間
	//尋找空的資料位置
	int RtnPos = -1;
	int datapos = -1;
	//int freepos = GetFreeDataPos(sizeof(DataFileHeader),sizeof(DataFile),FIELD_OFFSET(DataFile,Status),0);
	//if (freepos > -1)
	//{
	//	datapos = freepos;
	//} else
	//{		
		datapos = InsertData((char *)datafile,sizeof(DataFile));		
	//}

	//先寫入 Data db 再將位置產生 index db
	int res = InsertKey(KeyStr,datapos);
	if (res == -1)
	{
		//Insert 失敗 , 處理 data
		/*
		
		   如果先做 select key 就不會進到這邊
		*/
		datafile->Status = 0; //empty	
		UpdateData((char *) datafile,sizeof(datafile->Status),datapos,FIELD_OFFSET(DataFile,Status));

	}
	else
	{
	   RtnPos = datapos;
	}

	return RtnPos;
	

}

void CSPAMDB::UpdateData(char* DataBuff , int BuffSize , int DataPos , int DataOffset)
{

	CDB::UpdateData(DataBuff,BuffSize,DataPos,DataOffset);


}


__declspec(dllexport) CSPAMDB*  createSPAMDBObject(CRITICAL_SECTION*  DBSection,char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , int FileBuffSize)
{
	return new CSPAMDB(DBSection,IndexDBPath,OverFlowDBPath,DataDBPath,FileBuffSize);
}

