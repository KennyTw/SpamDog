// DB.cpp: implementation of the CDB class.
//
//////////////////////////////////////////////////////////////////////
 

#include "stdafx.h"
#include "DB.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
#pragma warning (disable:4786)
#include <string.h>
#include <Windows.h> 
#include <list>
#include <string>
#include <conio.h>


size_t DBfread(void *buffer, size_t size, size_t count, FILE *stream )
{
	int readsize = fread(buffer,size,count,stream);
	if (readsize != count) throw "DB fread Error";

	return readsize;
}

size_t DBfwrite(const void *buffer, size_t size, size_t count,  FILE *stream )
{
	int writesize = fwrite(buffer, size , count , stream);
	if (writesize != count) throw "DB fwrite Error";

	return writesize;

}

int DBfseek(FILE *stream,long offset, int origin )
{
	int seekrs = fseek(stream , offset , origin);
	if (seekrs != 0 ) throw "DB fseek Error";

	return seekrs;

}

CDB::CDB(CRITICAL_SECTION*  DBSection,char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , unsigned int DataHeaderSize, int FileBuffSize)
{

	mDBSection = DBSection;
	strcpy(mIndexDBPath, IndexDBPath);
	strcpy(mOverFlowDBPath ,OverFlowDBPath);
	strcpy(mDataDBPath,DataDBPath);

	mDataHeaderSize = DataHeaderSize;
	mFileBuffSize = FileBuffSize;	


	IndexFileBuf = new char[FileBuffSize];
	OverflowFileBuf = new char[FileBuffSize];
	DataFileBuf = new char[FileBuffSize];	 

	//預設的 Bucket 數
	IniBucket = 2;

	IndexDBfp = NULL;
	OverFlowDBfp = NULL;
	DataDBfp = NULL;

	memset(&m_IndexHeader,0,sizeof(IndexHeader));


}

void CDB::OpenDB()
{

	if (mDBSection != NULL)
		EnterCriticalSection(mDBSection);


	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

	hFind = FindFirstFile(mIndexDBPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{
     
		IndexDBfp = fopen(mIndexDBPath,"w+b");
		if (IndexDBfp == NULL) throw "Error Open File";

		//設定 buff
		 
		if( setvbuf( IndexDBfp, IndexFileBuf, _IOFBF, mFileBuffSize ) != 0 )
		{
			_cprintf( "Incorrect type or size of buffer for IndexDBfp\n" );
			throw "Incorrect type or size of buffer for IndexDBfp";
		}

		//移到最頭
		DBfseek(IndexDBfp,0,SEEK_SET);

		//處理初始的 header
		m_IndexHeader.BucketCount = IniBucket;
		m_IndexHeader.IniBucket = IniBucket;
		DBfwrite(&m_IndexHeader,sizeof(char),sizeof(IndexHeader),IndexDBfp);
		

		Bucket m_Bucket;
		memset(&m_Bucket,0,sizeof(Bucket));
		m_Bucket.ChainData = -1;

		for (unsigned int i=0 ; i < IniBucket  ; i++)
		{
		   DBfwrite(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);
		   
		}

	 

	} 
	else
	{
		IndexDBfp = fopen(mIndexDBPath,"r+b");

		if (IndexDBfp == NULL) throw "Error Open File";
		
		if( setvbuf( IndexDBfp, IndexFileBuf, _IOFBF, mFileBuffSize ) != 0 )
		{
			_cprintf( "Incorrect type or size of buffer for IndexDBfp\n" );
		 	throw "Incorrect type or size of buffer for IndexDBfp";
		}

		//移到最頭
		DBfseek(IndexDBfp,0,SEEK_SET);
		//讀出  header
		DBfread(&m_IndexHeader,sizeof(char),sizeof(IndexHeader),IndexDBfp);

		FindClose(hFind);
		

	}	

	hFind = FindFirstFile(mOverFlowDBPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{
     
		OverFlowDBfp = fopen(mOverFlowDBPath,"w+b");
		if (OverFlowDBfp == NULL) throw "Error Open File";

		if( setvbuf( OverFlowDBfp, OverflowFileBuf, _IOFBF, mFileBuffSize ) != 0 )
		{
          _cprintf( "Incorrect type or size of buffer for OverFlowDBfp\n" );
		  throw "Incorrect type or size of buffer for OverFlowDBfp";
		}
		//移到最頭
		//fseek(OverFlowDBfp,0,SEEK_SET);
		//fwrite(&m_OverFlowHeader,sizeof(char),sizeof(OverFlowHeader),OverFlowDBfp);
	} 
	else
	{
		OverFlowDBfp = fopen(mOverFlowDBPath,"r+b"); 
		if (OverFlowDBfp == NULL) throw "Error Open File";

		if( setvbuf( OverFlowDBfp, OverflowFileBuf, _IOFBF, mFileBuffSize ) != 0 )
		{
          _cprintf( "Incorrect type or size of buffer for OverFlowDBfp\n" );
		  //ASSERT(0);
		  throw  "Incorrect type or size of buffer for OverFlowDBfp";
		}

		FindClose(hFind);
		//移到最頭
		//fseek(OverFlowDBfp,0,SEEK_SET);
		//讀出  header
		//fread(&m_OverFlowHeader,sizeof(char),sizeof(OverFlowHeader),OverFlowDBfp);
		

	}	

	hFind = FindFirstFile(mDataDBPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{
     
		DataDBfp = fopen(mDataDBPath,"w+b");
		if (DataDBfp == NULL) throw "Error Open File";

		if( setvbuf( DataDBfp, DataFileBuf, _IOFBF, mFileBuffSize ) != 0 )
		{
           _cprintf( "Incorrect type or size of buffer for DataDBfp\n" );
		   throw "Incorrect type or size of buffer for DataDBfp";
		}

		//加入 header
		if (mDataHeaderSize > 0 )
		{		
			char *buff = new char[mDataHeaderSize];
			memset(buff,0,mDataHeaderSize);
			//放入 check code 的位置
			buff[0] = 1;
			DBfseek(DataDBfp,0,SEEK_SET);
			DBfwrite(buff,sizeof(char),mDataHeaderSize,DataDBfp);
			delete buff; 
		}
	 
	} 
	else
	{
		DataDBfp = fopen(mDataDBPath,"r+b");
		if (DataDBfp == NULL) throw "Error Open File";

		if( setvbuf( DataDBfp, DataFileBuf, _IOFBF, mFileBuffSize ) != 0 )
		{
            _cprintf( "Incorrect type or size of buffer for DataDBfp\n" );
		   	//ASSERT(0);
			throw "Incorrect type or size of buffer for DataDBfp";
		}

		FindClose(hFind);

	}	

}
void CDB::CloseDB()
{

	if (IndexDBfp!=NULL) 
	{
		fclose(IndexDBfp);
		IndexDBfp = NULL;
	}

	if (OverFlowDBfp!=NULL) 
	{
		fclose(OverFlowDBfp);
		OverFlowDBfp = NULL;

	}
	if (DataDBfp!=NULL)
	{
		fclose(DataDBfp);
		DataDBfp = NULL;
	}

	if (mDBSection != NULL)
		LeaveCriticalSection(mDBSection);
}

CDB::~CDB()
{
	if (IndexDBfp!=NULL) fclose(IndexDBfp);
	if (OverFlowDBfp!=NULL) fclose(OverFlowDBfp);
	if (DataDBfp!=NULL) fclose(DataDBfp);

	delete IndexFileBuf;
	delete OverflowFileBuf;
	delete DataFileBuf;
	

}

//*************************** 求絕對 Hash Value	 
Hash CDB::GetHashValue(char* Astr)
{
	 
	Hash hash;
	memset(&hash,0,sizeof(Hash));


	
	unsigned int len = strlen(Astr);
	//unsigned int MaxValue = 0xFFFFFFFF / len;

	//求絕對 Hash Value	 
	for(unsigned int  i =0 ; i < len ; i++ )
	{		
		//RtnValue += (Astr[i] & 0xff) * (i+1) ;	 
		//RtnValue += (Astr[i] & 0xff) * 127  ;	 

		
		//hash.Value  += ((Astr[i] & 0xff) * (i + 1000)) % MaxValue ;
		hash.Value  += ((Astr[i] & 0xff) * (i + 1000));

		//hash.Value  += ((Astr[i] & 0xff) * (11 << i));
		//hash.Value  += ((Astr[i] + (i+1)) * 8 ) ;
		

	} 

	



 
	return hash;

}


//*****************  寫入 Key type = 0 為正常 mode
// type = 1 為 Rehash 使用 , 因為兩種記算方式不同

int CDB::InsertKey(char* Astr, int DataFilePos, char type)
{

	
 
	int InsertRes = -1;
	//確認大小
	if (strlen(Astr) >= DefaultKeyDataSize) 
	{
		_cprintf("OverSize\n");
		return InsertRes;
	}

	//確認 OverFlow	
	SResult sres = SelectKey(Astr,type);

	unsigned int BucketPos = sres.BucketPos;
		//讀出 bucket
	//fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos),SEEK_SET);
	Bucket *m_Bucket = &sres.SBucket;
	//fread(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);


	//資料接未找到
	if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
	{
		if (sres.FreePos != -1)
		{
			m_Bucket->IndexData[sres.FreePos].hash = sres.hash;
			m_Bucket->IndexData[sres.FreePos].Status = 1;

			strcpy(m_Bucket->IndexData[sres.FreePos].KeyData,Astr);


          /* 以下資料段 , 要外部結構決定*/
		  int datapos = -1;
		  /*
		  if (type == 0)
		  {
			fseek(DataDBfp,0,SEEK_END);
			datapos = ftell(DataDBfp);
			DataFile datafile;
			memset(&datafile,0,sizeof(DataFile));
			strcpy(datafile.Data,Astr);			
			fwrite(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);
          }
		  else
		  {*/
			  datapos = DataFilePos ;
		  //}

			m_Bucket->IndexData[sres.FreePos].DataFilePos = datapos;
		 

		 
			int offset = FIELD_OFFSET(Bucket,IndexData[sres.FreePos]);

			//fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos),SEEK_SET);
			//fwrite(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);
			DBfseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos) + offset ,SEEK_SET);			
			DBfwrite(&m_Bucket->IndexData[sres.FreePos],sizeof(char),sizeof(m_Bucket->IndexData[sres.FreePos]),IndexDBfp);		 

			InsertRes = BucketPos;
			
			//_cprintf("Data Put In %d , BucketCount %d , Split %d , IniBucket %d\n",BucketPos,m_IndexHeader.BucketCount,m_IndexHeader.SplitPos,m_IndexHeader.IniBucket);
		}
		else
		{
		    //_cprintf("overflow %d---->type: %d \n",BucketPos,type);

	 
			//尋找最後的 chain
			int chain = m_Bucket->ChainData;
			int PreChain = -1;
			IndexData m_IndexData;
			int ChainDeep = 0;
			while (chain != -1)
			{
							
				DBfseek(OverFlowDBfp,chain,SEEK_SET);
				DBfread(&m_IndexData,sizeof(char),sizeof(IndexData),OverFlowDBfp);

				//_cprintf("In Chain : %s\n",m_IndexData.KeyData);

				PreChain = chain;
				chain = m_IndexData.ChainData;

				ChainDeep++;
				
				if (chain == PreChain)
					throw "Error End Chain Found!";
			}			

			//_cprintf("Chain Deep %d\n",ChainDeep);

			//if (ChainDeep > 10 ) 
			//{	
				//Beep(1000,500);
				//Sleep(1000);
				//getch();
			//}

			//產生 OverFlowdata
			   int FreeIndexDataPos = GetFreeIndexData(OverFlowDBfp,0);			   
			   IndexData m_NewIndexData;
			   memset(&m_NewIndexData,0,sizeof(IndexData));

			   strcpy(m_NewIndexData.KeyData,Astr);
			   

			   int filedatapos = -1;
			   /*if (type == 0)
			   {
					fseek(DataDBfp,0,SEEK_END);
					filedatapos = ftell(DataDBfp);
					DataFile datafile;
					memset(&datafile,0,sizeof(DataFile));
					strcpy(datafile.Data,Astr);			
					fwrite(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);
			   }
			   else
			   {*/			   
				   filedatapos = DataFilePos;
			   //}

			   m_NewIndexData.hash = sres.hash;
			   m_NewIndexData.Status = 1;
			   m_NewIndexData.ChainData = -1;
			   m_NewIndexData.DataFilePos = filedatapos;

			   if ( FreeIndexDataPos == -1)
			   {
			       //找不到空的
				   DBfseek(OverFlowDBfp,0,SEEK_END);
			   }
			   else
			   {
			       DBfseek(OverFlowDBfp,FreeIndexDataPos,SEEK_SET);
			   }

			   int datapos = ftell(OverFlowDBfp);
			   InsertRes = datapos;
			   DBfwrite(&m_NewIndexData,sizeof(char),sizeof(IndexData),OverFlowDBfp);

			   //更新 parent chain
			   if (PreChain == -1)
			   {
			     //都沒有 chain
				   m_Bucket->ChainData = datapos;
				   int offset = FIELD_OFFSET(Bucket,ChainData);
				   DBfseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos) + offset,SEEK_SET);
				   DBfwrite(&m_Bucket->ChainData,sizeof(char),sizeof(m_Bucket->ChainData),IndexDBfp);		   
			   }	   
			   else
			   {
			       //更新上一層指向末端
				   int offset = FIELD_OFFSET(IndexData,ChainData);
				   DBfseek(OverFlowDBfp,PreChain + offset,SEEK_SET);
				   DBfwrite(&datapos,sizeof(char),sizeof(datapos),OverFlowDBfp);		 

			   }

		 
			   if (type == 0) //rehash 遞迴不再做 rehash
			   {
				  
				   //_cprintf("ReHash.........................\n"); 
				   BucketRehash();		
				    
				   	//修正 header
				   if (m_IndexHeader.SplitPos + 1 == m_IndexHeader.IniBucket)
				   {
			
						m_IndexHeader.IniBucket = m_IndexHeader.BucketCount;
						m_IndexHeader.SplitPos = 0;
			
				   }
				   else
				   {
			
						m_IndexHeader.SplitPos ++; //移動 split pos
				   }
			
						DBfseek(IndexDBfp,0,SEEK_SET);
						DBfwrite(&m_IndexHeader,sizeof(char),sizeof(IndexHeader),IndexDBfp);

				   //if (ChainDeep > 10 )
				   //{
					  // _cprintf("ReHash Spec Buccket:%d..............\n",BucketPos); 	
					   //getch();
					   //BucketRehash(BucketPos);
				   //}
			   }

			 

		}
	}

	return InsertRes;


}

int CDB::GetFreePos(IndexData* indexdata)
{

	for(int i=0 ; i < DefaultBucketSize ; i++)
	{
	
	   if ((indexdata + i)->Status == 0 || (indexdata + i)->Status == 2)
	   {
	      return i;
	   }

	
	}

	return -1;

}

int CDB::InsertData(void* DataBuff , int BuffSize)
{

	int filedatapos = -1;

		DBfseek(DataDBfp,0,SEEK_END);
		filedatapos = ftell(DataDBfp);
		//DataFile datafile;
		//memset(&datafile,0,sizeof(DataFile));
		//strcpy(datafile.Data,Astr);			
		DBfwrite(DataBuff,sizeof(char),BuffSize,DataDBfp);

		return filedatapos;

 
}

void CDB::UpdateData(void* DataBuff , int BuffSize , int DataPos , int DataOffset)
{

	DBfseek(DataDBfp,DataPos + DataOffset ,SEEK_SET);
	DBfwrite(DataBuff,sizeof(char),BuffSize,DataDBfp);



}

int CDB::GetBucketPos(int HashValue,char type)
{

	/**
	
		IniBucket : 初始的 bucket 數目
		BucketCount : 目前的 bucket 數目
		當 SplitPos = 0 的時候 IniBucket = BucketCount
		h0 在 SplitPos = 0 時 , h0 = h1
		當 SplitPos > 0 時 h1 = 2 * h0

		SplitPos 用途在定義 rehash 的 bucket 位置

		rehash 時都用 h1 計算

	
	*/
	unsigned int h0 = HashValue % (m_IndexHeader.IniBucket);
	unsigned int BucketPos = 0;
	unsigned int h1  = 0;

 
	if (m_IndexHeader.BucketCount > m_IndexHeader.IniBucket)
	{
		h1 = HashValue % (2 * m_IndexHeader.IniBucket);
	}
	else
	{
		h1 = h0;
	}

	if (type == 0)
	{

		if (h0  >= m_IndexHeader.SplitPos  )
		{
			BucketPos = h0;
		}
		else
		{
			BucketPos = h1;
		}
	}
	else
	{
		BucketPos = h1;
		

		if (h1 >= m_IndexHeader.BucketCount)
		{
			BucketPos = h0;
			_cprintf("Error Bucket h1:%d , Count:%d\n",h1,m_IndexHeader.BucketCount);
			throw "Error Bucket";
		}
	
	}

	return BucketPos;

}

SResult CDB::SelectKey(char* Astr,char type)
{

	SResult sresult;
	memset(&sresult,-1,sizeof(SResult));

	//求出 Hash 值
	Hash hash = GetHashValue(Astr); 
	sresult.hash = hash;
	//
	int InsertRes = -1;

	//_cprintf("key:%s -> %d\n",Astr,hash.Value);

	unsigned int BucketPos = GetBucketPos(hash.Value,type);
	sresult.BucketPos = BucketPos;


	//讀出 bucket
	DBfseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos),SEEK_SET);
	Bucket m_Bucket;
	DBfread(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);

	sresult.SBucket = m_Bucket; 

	bool FindKey = false;
	//int FreePos = -1;

	for(unsigned int i=0 ; i < DefaultBucketSize ; i ++)
	{
		/*
		  IndexData[i].Status : 1 為使用中
								其餘為未使用
		*/
		if (m_Bucket.IndexData[i].Status == 1)
		{
			if (strcmp(Astr,m_Bucket.IndexData[i].KeyData) == 0)
			{
				
				  //讀出資料				  
				    sresult.DataFilePos = m_Bucket.IndexData[i].DataFilePos;
					sresult.FindPosInKey = i;
					FindKey = true;
					//_cprintf("Key Dup\n");
					break;
				  
				
			}
		
		}
		else
		{
		     
		    //如果有新的 status 要修改處
			sresult.FreePos = i;		
		}
	}

	if(!FindKey && m_Bucket.ChainData >= 0)
	{
	
			//收尋 chain
			int chain = m_Bucket.ChainData;			 
			IndexData m_IndexData;
			while (chain != -1)
			{
							
				DBfseek(OverFlowDBfp,chain,SEEK_SET);
				memset(&m_IndexData,0,sizeof(IndexData));
				DBfread(&m_IndexData,sizeof(char),sizeof(IndexData),OverFlowDBfp);			
			

				if (strcmp(Astr,m_IndexData.KeyData) == 0 && 
				    m_IndexData.Status != 2)
				{
				
				  //讀出資料				  
				  //DataFile datafile;					   				   
				  
				  //fseek(DataDBfp,m_IndexData.DataFilePos,SEEK_SET);
				  //fread(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);
				  				  
				 
				  //if (strcmp(Astr,datafile.Data) == 0)
				  //{
					//Key 重複
					//FindKey = true;
					sresult.DataFilePos = m_IndexData.DataFilePos;
					sresult.FindPosInOvr = chain;
					//_cprintf("Key Dup \n");
					break;
				 // }
				}

				if (chain == m_IndexData.ChainData && chain != -1)
				{
					_cprintf("Recursive Found !");
					throw "Recursive Found !";
				}

				chain = m_IndexData.ChainData;			
			}
		 
	
	
	}


	return sresult;


}

int CDB::GetFreeIndexData(FILE *fp , int HeaderSize)
{

	return -1;
	/*fseek(fp,HeaderSize,SEEK_SET);

	IndexData m_IndexData;
	int DataPos = -1;
	
	while (!feof(fp))
	{
	
	  fread(&m_IndexData,sizeof(char),sizeof(IndexData),fp);
	  if (m_IndexData.Status == 2)
	  {
		  DataPos = ftell(fp) -  sizeof(IndexData); 
		  break;
	  }
	
	}

	return DataPos;*/

}

int CDB::BucketRehash()
{


			using namespace std ;
			typedef list<string> LISTSTR;
			LISTSTR::iterator itor;
			LISTSTR strlist;         // default constructor

			

			typedef list<int> LISTINT;
			LISTINT::iterator itor2;
			LISTINT intlist;  
			
			int splitpos=-1;
				
			
		
				//header
				m_IndexHeader.BucketCount++; 
				splitpos = m_IndexHeader.SplitPos;

				DBfseek(IndexDBfp,0,SEEK_END);
				
				Bucket m_Bucket;
				memset(&m_Bucket,0,sizeof(Bucket));
				m_Bucket.ChainData = -1;
				DBfwrite(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);
			
			

			    //split
				//讀出 splitbucket 								
				DBfseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * splitpos),SEEK_SET);
				Bucket m_spBucket;
				memset(&m_spBucket,0,sizeof(Bucket));
				DBfread(&m_spBucket,sizeof(char),sizeof(Bucket),IndexDBfp);
			
		 
		

			//列出所有成員
			for(unsigned int i=0 ; i < DefaultBucketSize ; i ++)
			{
				if (m_spBucket.IndexData[i].Status == 1)
				{
				 
				      //先刪除
					  m_spBucket.IndexData[i].Status = 2; //del
					  
					  int offset = FIELD_OFFSET(Bucket,IndexData[i]);
					  offset += FIELD_OFFSET(IndexData,Status);
					  DBfseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * splitpos) + offset ,SEEK_SET);
					  DBfwrite(&m_spBucket.IndexData[i].Status,sizeof(char),sizeof(m_spBucket.IndexData[i].Status),IndexDBfp);

					   
					  //讀出資料					 
					  //DataFile datafile;					   					   
					  //fseek(DataDBfp,m_spBucket.IndexData[i].DataFilePos ,SEEK_SET);
					  //fread(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);

					  
					  strlist.insert(strlist.end(),m_spBucket.IndexData[i].KeyData);
					  intlist.insert(intlist.end(),m_spBucket.IndexData[i].DataFilePos);
					  
				}	
		
			}

			if (m_spBucket.ChainData > -1)
			{

					//尋找最後的 chain
					int chain = m_spBucket.ChainData;
					 
					IndexData m_IndexData;
					//刪除全線資料
					while (chain != -1)
					{
							
						DBfseek(OverFlowDBfp,chain,SEEK_SET);
						DBfread(&m_IndexData,sizeof(char),sizeof(IndexData),OverFlowDBfp);
						//刪除資料
						m_IndexData.Status = 2; //del							
						int offset = FIELD_OFFSET(IndexData,Status);
						DBfseek(OverFlowDBfp,chain+ offset ,SEEK_SET);
						DBfwrite(&m_IndexData.Status,sizeof(char),sizeof(m_IndexData.Status),OverFlowDBfp);

						//
						//讀出資料					 
						//DataFile datafile;					   					   
						//fseek(DataDBfp,m_IndexData.DataFilePos ,SEEK_SET);
						//fread(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);

						//放到 list 中
						strlist.insert(strlist.end(),m_IndexData.KeyData);
						intlist.insert(intlist.end(),m_IndexData.DataFilePos);

						chain = m_IndexData.ChainData;	

					}
					
					chain = m_spBucket.ChainData;

					//刪除父層
					m_spBucket.ChainData = -1;
					int offset = FIELD_OFFSET(Bucket,ChainData);							 
					DBfseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * (splitpos)) + offset ,SEEK_SET);
					DBfwrite(&m_spBucket.ChainData,sizeof(char),sizeof(m_spBucket.ChainData),IndexDBfp);

 
				 
			}

 


			
		    for (itor =  strlist.begin(),itor2 = intlist.begin(); itor != strlist.end() && itor2 != intlist.end() ; itor++ ,itor2++ )
			{
             
				string tmpstr =  *itor;	
				int datapos = *itor2;

				InsertKey((char *) tmpstr.c_str(),datapos,1);
				 
			}



	return 0;
}

int CDB::GetFreeDataPos(unsigned int HeaderSize , unsigned int RecordSize , int CheckOffset , int AvalValue)
{

	//int GetRes = -1;
	/*
	fseek(DataDBfp,HeaderSize,SEEK_SET);
	char *tmpbuf = new char[RecordSize];
	
	while (!feof(DataDBfp))
	{
	
	  fread(tmpbuf,sizeof(char),RecordSize,DataDBfp);
	  //檢查檢查碼
	  if (*(tmpbuf + CheckOffset) == AvalValue )
	  {
		  GetRes = ftell(DataDBfp) -  RecordSize; //往前推到紀錄的起始位置
		  break;
	  }
	
	}

	delete tmpbuf;*/

	//由於取消 DataValid 的想法  , 所以不須要在找一次 FreeDataPos
	//直接回傳 -1
	 

	return -1;

}

void CDB::SelectData(int DataPos , void* FillBuff , int BuffSize)
{

	DBfseek(DataDBfp,DataPos,SEEK_SET);
	DBfread(FillBuff,sizeof(char),BuffSize,DataDBfp);

}

/*
//*********************************************************************************************************************
CDB2::CDB2(char* IndexDBPath , char* OverFlowDBPath , char* DataDBPath , unsigned int DataHeaderSize, int FileBuffSize)
{

	IndexFileBuf = new char[FileBuffSize];
	OverflowFileBuf = new char[FileBuffSize];
	DataFileBuf = new char[FileBuffSize];	 

	//預設的 Bucket 數
	IniBucket = 2;

	IndexDBfp = NULL;
	OverFlowDBfp = NULL;
	DataDBfp = NULL;

	memset(&m_IndexHeader,0,sizeof(IndexHeader));

	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

	hFind = FindFirstFile(IndexDBPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{
     
		IndexDBfp = fopen(IndexDBPath,"w+b");
		if (IndexDBfp == NULL) throw "Error Open File";
		//設定 buff
		 
		if( setvbuf( IndexDBfp, IndexFileBuf, _IOFBF, FileBuffSize ) != 0 )
		{
			_cprintf( "Incorrect type or size of buffer for IndexDBfp\n" );
			throw "Incorrect type or size of buffer for IndexDBfp";
		}

		//移到最頭
		fseek(IndexDBfp,0,SEEK_SET);

		//處理初始的 header
		m_IndexHeader.BucketCount = IniBucket;
		m_IndexHeader.IniBucket = IniBucket;
		fwrite(&m_IndexHeader,sizeof(char),sizeof(IndexHeader),IndexDBfp);

		Bucket2 m_Bucket;
		memset(&m_Bucket,0,sizeof(Bucket2));
		m_Bucket.ChainData = -1;

		for (unsigned int i=0 ; i < IniBucket  ; i++)
		{
		   fwrite(&m_Bucket,sizeof(char),sizeof(Bucket2),IndexDBfp);
		}

	 

	} 
	else
	{
		IndexDBfp = fopen(IndexDBPath,"r+b"); 
		if (IndexDBfp == NULL) throw "Error Open File";
		
		if( setvbuf( IndexDBfp, IndexFileBuf, _IOFBF, FileBuffSize ) != 0 )
		{
			_cprintf( "Incorrect type or size of buffer for IndexDBfp\n" );
		 	throw "Incorrect type or size of buffer for IndexDBfp";
		}

		//移到最頭
		fseek(IndexDBfp,0,SEEK_SET);
		//讀出  header
		fread(&m_IndexHeader,sizeof(char),sizeof(IndexHeader),IndexDBfp);
		

	}	

	hFind = FindFirstFile(OverFlowDBPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{
     
		OverFlowDBfp = fopen(OverFlowDBPath,"w+b");
		if (OverFlowDBfp == NULL) throw "Error Open File";

		if( setvbuf( OverFlowDBfp, OverflowFileBuf, _IOFBF, FileBuffSize ) != 0 )
		{
          _cprintf( "Incorrect type or size of buffer for OverFlowDBfp\n" );
		  throw "Incorrect type or size of buffer for OverFlowDBfp";
		}
		//移到最頭
		//fseek(OverFlowDBfp,0,SEEK_SET);
		//fwrite(&m_OverFlowHeader,sizeof(char),sizeof(OverFlowHeader),OverFlowDBfp);
	} 
	else
	{
		OverFlowDBfp = fopen(OverFlowDBPath,"r+b"); 
		if (OverFlowDBfp == NULL) throw "Error Open File";

		if( setvbuf( OverFlowDBfp, OverflowFileBuf, _IOFBF, FileBuffSize ) != 0 )
		{
          _cprintf( "Incorrect type or size of buffer for OverFlowDBfp\n" );
		  //ASSERT(0);
		  throw  "Incorrect type or size of buffer for OverFlowDBfp";
		}
		//移到最頭
		//fseek(OverFlowDBfp,0,SEEK_SET);
		//讀出  header
		//fread(&m_OverFlowHeader,sizeof(char),sizeof(OverFlowHeader),OverFlowDBfp);
		

	}	

	hFind = FindFirstFile(DataDBPath,&FindFileData);
 	if (hFind == INVALID_HANDLE_VALUE) 
	{
     
		DataDBfp = fopen(DataDBPath,"w+b");	
		if (DataDBfp == NULL) throw "Error Open File";

		if( setvbuf( DataDBfp, DataFileBuf, _IOFBF, FileBuffSize ) != 0 )
		{
           _cprintf( "Incorrect type or size of buffer for DataDBfp\n" );
		   throw "Incorrect type or size of buffer for DataDBfp";
		}

		//加入 header
		if (DataHeaderSize > 0 )
		{		
			char *buff = new char[DataHeaderSize];
			memset(buff,0,DataHeaderSize);
			fseek(DataDBfp,0,SEEK_SET);
			fwrite(buff,sizeof(char),DataHeaderSize,DataDBfp);
			delete buff; 
		}
	 
	} 
	else
	{
		DataDBfp = fopen(DataDBPath,"r+b");
		if (DataDBfp == NULL) throw "Error Open File";

		if( setvbuf( DataDBfp, DataFileBuf, _IOFBF, FileBuffSize ) != 0 )
		{
            _cprintf( "Incorrect type or size of buffer for DataDBfp\n" );
		   	//ASSERT(0);
			throw "Incorrect type or size of buffer for DataDBfp";
		}

	}	

	FindClose(hFind);


}

CDB2::~CDB2()
{
	if (IndexDBfp!=NULL) fclose(IndexDBfp);
	if (OverFlowDBfp!=NULL) fclose(OverFlowDBfp);
	if (DataDBfp!=NULL) fclose(DataDBfp);

	delete IndexFileBuf;
	delete OverflowFileBuf;
	delete DataFileBuf;
	

}

//*************************** 求絕對 Hash Value	 
Hash CDB2::GetHashValue(char* Astr)
{
	 
	Hash hash;
	memset(&hash,0,sizeof(Hash));
	
	unsigned int len = strlen(Astr);

	//求絕對 Hash Value	 
	for(unsigned int  i =0 ; i < len ; i++ )
	{		
		//RtnValue += (Astr[i] & 0xff) * (i+1) ;	 
		//RtnValue += (Astr[i] & 0xff) * 127  ;	 
		hash.Value  += (Astr[i] & 0xff)   ;	 

	} 
 
	return hash;

}


//*****************  寫入 Key type = 0 為正常 mode
// type = 1 為 Rehash 使用 , 因為兩種記算方式不同

int CDB2::InsertKey(char* Astr, int DataFilePos, char type)
{

 
	int InsertRes = -1;
	//確認大小
	if (strlen(Astr) >= DefaultKeyDataSize * 2) 
	{
		OutputDebugString("OverSize\n");
		return InsertRes;
	}

	//確認 OverFlow	
	SResult2 sres = SelectKey(Astr,type);

	unsigned int BucketPos = sres.BucketPos;
		//讀出 bucket
	//fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos),SEEK_SET);
	Bucket2 *m_Bucket = &sres.SBucket;
	//fread(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);


	//資料接未找到
	if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
	{
		if (sres.FreePos != -1)
		{
			m_Bucket->IndexData[sres.FreePos].hash = sres.hash;
			m_Bucket->IndexData[sres.FreePos].Status = 1;

			strcpy(m_Bucket->IndexData[sres.FreePos].KeyData,Astr);


          // 以下資料段 , 要外部結構決定
		  int datapos = -1;
		  
			  datapos = DataFilePos ;
		  //}

			m_Bucket->IndexData[sres.FreePos].DataFilePos = datapos;
		 

		 
			int offset = FIELD_OFFSET(Bucket2,IndexData[sres.FreePos]);

			//fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket) * BucketPos),SEEK_SET);
			//fwrite(&m_Bucket,sizeof(char),sizeof(Bucket),IndexDBfp);
			fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket2) * BucketPos) + offset ,SEEK_SET);			
			fwrite(&m_Bucket->IndexData[sres.FreePos],sizeof(char),sizeof(m_Bucket->IndexData[sres.FreePos]),IndexDBfp);		 

			InsertRes = BucketPos;
			
			_cprintf("Data Put In %d , BucketCount %d , Split %d , IniBucket %d\n",BucketPos,m_IndexHeader.BucketCount,m_IndexHeader.SplitPos,m_IndexHeader.IniBucket);
		}
		else
		{
		    _cprintf("OverFlow %d\n",BucketPos);

	 
			//尋找最後的 chain
			int chain = m_Bucket->ChainData;
			int PreChain = -1;
			IndexData2 m_IndexData;
			while (chain != -1)
			{
							
				fseek(OverFlowDBfp,chain,SEEK_SET);
				fread(&m_IndexData,sizeof(char),sizeof(IndexData2),OverFlowDBfp);
				PreChain = chain;
				chain = m_IndexData.ChainData;
				
				if (chain == PreChain)
					throw "Error End Chain Found!";
			}			


			//產生 OverFlowdata
			   int FreeIndexDataPos = GetFreeIndexData(OverFlowDBfp,0);			   
			   IndexData2 m_NewIndexData;
			   memset(&m_NewIndexData,0,sizeof(IndexData2));

			   strcpy(m_NewIndexData.KeyData,Astr);
			   

			   int filedatapos = -1;
			   			   
				   filedatapos = DataFilePos;
			   

			   m_NewIndexData.hash = sres.hash;
			   m_NewIndexData.Status = 1;
			   m_NewIndexData.ChainData = -1;
			   m_NewIndexData.DataFilePos = filedatapos;

			   if ( FreeIndexDataPos == -1)
			   {
			       //找不到空的
				   fseek(OverFlowDBfp,0,SEEK_END);
			   }
			   else
			   {
			       fseek(OverFlowDBfp,FreeIndexDataPos,SEEK_SET);
			   }

			   int datapos = ftell(OverFlowDBfp);
			   InsertRes = datapos;
			   fwrite(&m_NewIndexData,sizeof(char),sizeof(IndexData2),OverFlowDBfp);

			   //更新 parent chain
			   if (PreChain == -1)
			   {
			     //都沒有 chain
				   m_Bucket->ChainData = datapos;
				   int offset = FIELD_OFFSET(Bucket2,ChainData);
				   fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket2) * BucketPos) + offset,SEEK_SET);
				   fwrite(&m_Bucket->ChainData,sizeof(char),sizeof(m_Bucket->ChainData),IndexDBfp);		   
			   }	   
			   else
			   {
			       int offset = FIELD_OFFSET(IndexData2,ChainData);
				   fseek(OverFlowDBfp,PreChain + offset,SEEK_SET);
				   fwrite(&datapos,sizeof(char),sizeof(datapos),OverFlowDBfp);		 

			   }

		 
			   if (type == 0) //rehash 遞迴不再做 rehash
			   {
				  
				   _cprintf("ReHash\n"); 
				   BucketRehash();		
				    
				   	//修正 header
				   if (m_IndexHeader.SplitPos + 1 == m_IndexHeader.IniBucket)
				   {
			
						m_IndexHeader.IniBucket = m_IndexHeader.BucketCount;
						m_IndexHeader.SplitPos = 0;
			
				   }
				   else
				   {
			
						m_IndexHeader.SplitPos ++; //移動 split pos
				   }
			
						fseek(IndexDBfp,0,SEEK_SET);
						fwrite(&m_IndexHeader,sizeof(char),sizeof(IndexHeader),IndexDBfp);
			   }

			 

		}
	}

	return InsertRes;


}

int CDB2::GetFreePos(IndexData2* indexdata)
{

	for(int i=0 ; i < DefaultBucketSize ; i++)
	{
	
	   if ((indexdata + i)->Status == 0 || (indexdata + i)->Status == 2)
	   {
	      return i;
	   }

	
	}

	return -1;

}

int CDB2::InsertData(char* DataBuff , int BuffSize)
{


		fseek(DataDBfp,0,SEEK_END);
		int filedatapos = ftell(DataDBfp);
		//DataFile datafile;
		//memset(&datafile,0,sizeof(DataFile));
		//strcpy(datafile.Data,Astr);			
		fwrite(DataBuff,sizeof(char),BuffSize,DataDBfp);

		return filedatapos;


	return 0;

}

void CDB2::UpdateData(char* DataBuff , int BuffSize , int DataPos , int DataOffset)
{

	fseek(DataDBfp,DataPos + DataOffset ,SEEK_SET);
	fwrite(DataBuff,sizeof(char),BuffSize,DataDBfp);


}

int CDB2::GetBucketPos(int HashValue,char type)
{

	
	
	//	IniBucket : 初始的 bucket 數目
		//BucketCount : 目前的 bucket 數目
	//	當 SplitPos = 0 的時候 IniBucket = BucketCount
	//	h0 在 SplitPos = 0 時 , h0 = h1
	//	當 SplitPos > 0 時 h1 = 2 * h0

	//	SplitPos 用途在定義 rehash 的 bucket 位置

	//	rehash 時都用 h1 計算

	

	unsigned int h0 = HashValue % (m_IndexHeader.IniBucket);
	unsigned int BucketPos = 0;
	unsigned int h1  = 0;

 
	if (m_IndexHeader.BucketCount > m_IndexHeader.IniBucket)
	{
		h1 = HashValue % (2 * m_IndexHeader.IniBucket);
	}
	else
	{
		h1 = h0;
	}

	if (type == 0)
	{

		if (h0  >= m_IndexHeader.SplitPos  )
		{
			BucketPos = h0;
		}
		else
		{
			BucketPos = h1;
		}
	}
	else
	{
		BucketPos = h1;
		

		if (h1 >= m_IndexHeader.BucketCount)
		{
			BucketPos = h0;
			_cprintf("Error Bucket \n");
			throw "Error Bucket";
		}
	
	}

	return BucketPos;

}

SResult2 CDB2::SelectKey(char* Astr,char type)
{

	SResult2 sresult;
	memset(&sresult,-1,sizeof(SResult2));

	//求出 Hash 值
	Hash hash = GetHashValue(Astr); 
	sresult.hash = hash;
	//
	int InsertRes = -1;

	unsigned int BucketPos = GetBucketPos(hash.Value,type);
	sresult.BucketPos = BucketPos;


	//讀出 bucket
	fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket2) * BucketPos),SEEK_SET);
	Bucket2 m_Bucket;
	fread(&m_Bucket,sizeof(char),sizeof(Bucket2),IndexDBfp);

	sresult.SBucket = m_Bucket; 

	bool FindKey = false;
	//int FreePos = -1;

	for(unsigned int i=0 ; i < DefaultBucketSize ; i ++)
	{
		 
		 // IndexData[i].Status : 1 為使用中
		//						其餘為未使用
		 
		if (m_Bucket.IndexData[i].Status == 1)
		{
			if (strcmp(Astr,m_Bucket.IndexData[i].KeyData) == 0)
			{
				
				  //讀出資料				  
				    sresult.DataFilePos = m_Bucket.IndexData[i].DataFilePos;
					sresult.FindPosInKey = i;				
					_cprintf("Key Dup\n");
					break;
				  
				
			}
		
		}
		else
		{
		     
		    //如果有新的 status 要修改處
			sresult.FreePos = i;		
		}
	}

	if(!FindKey && m_Bucket.ChainData >= 0)
	{
	
			//收尋 chain
			int chain = m_Bucket.ChainData;			 
			IndexData2 m_IndexData;
			while (chain != -1)
			{
							
				fseek(OverFlowDBfp,chain,SEEK_SET);
				memset(&m_IndexData,0,sizeof(IndexData2));
				fread(&m_IndexData,sizeof(char),sizeof(IndexData2),OverFlowDBfp);			
			

				if (strcmp(Astr,m_IndexData.KeyData) == 0 && 
				    m_IndexData.Status != 2)
				{
				
				  //讀出資料				  
				  //DataFile datafile;					   				   
				  
				  //fseek(DataDBfp,m_IndexData.DataFilePos,SEEK_SET);
				  //fread(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);
				  				  
				 
				  //if (strcmp(Astr,datafile.Data) == 0)
				  //{
					//Key 重複
					//FindKey = true;
					sresult.DataFilePos = m_IndexData.DataFilePos;
					sresult.FindPosInOvr = chain;
					_cprintf("Key Dup \n");
					break;
				 // }
				}

				if (chain == m_IndexData.ChainData && chain != -1)
				{
					_cprintf("Recursive Found !");
					throw "Recursive Found !";
				}

				chain = m_IndexData.ChainData;			
			}
		 
	
	
	}


	return sresult;


}

int CDB2::GetFreeIndexData(FILE *fp , int HeaderSize)
{

	fseek(fp,HeaderSize,SEEK_SET);

	IndexData2 m_IndexData;
	int DataPos = -1;
	
	while (!feof(fp))
	{
	
	  fread(&m_IndexData,sizeof(char),sizeof(IndexData2),fp);
	  if (m_IndexData.Status == 2)
	  {
		  DataPos = ftell(fp) -  sizeof(IndexData2); 
		  break;
	  }
	
	}

	return DataPos;

}

int CDB2::BucketRehash()
{

		 
			//header
			m_IndexHeader.BucketCount++; 
			using namespace std ;
			typedef list<string> LISTSTR;
			LISTSTR::iterator itor;
			LISTSTR strlist;         // default constructor

			

			typedef list<int> LISTINT;
			LISTINT::iterator itor2;
			LISTINT intlist;  
			
				
			int splitpos = m_IndexHeader.SplitPos;
			 

		 		
				fseek(IndexDBfp,0,SEEK_END);
				Bucket2 m_Bucket;
				memset(&m_Bucket,0,sizeof(Bucket2));
				m_Bucket.ChainData = -1;
				fwrite(&m_Bucket,sizeof(char),sizeof(Bucket2),IndexDBfp);

			    //split
				//讀出 splitbucket 								
				fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket2) * splitpos),SEEK_SET);
				Bucket2 m_spBucket;
				memset(&m_spBucket,0,sizeof(Bucket2));
				fread(&m_spBucket,sizeof(char),sizeof(Bucket2),IndexDBfp);
			
		 
		

			//列出所有成員
			for(unsigned int i=0 ; i < DefaultBucketSize ; i ++)
			{
				if (m_spBucket.IndexData[i].Status == 1)
				{
				 
				      //先刪除
					  m_spBucket.IndexData[i].Status = 2; //del
					  
					  int offset = FIELD_OFFSET(Bucket2,IndexData[i]);
					  offset += FIELD_OFFSET(IndexData2,Status);
					  fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket2) * splitpos) + offset ,SEEK_SET);
					  fwrite(&m_spBucket.IndexData[i].Status,sizeof(char),sizeof(m_spBucket.IndexData[i].Status),IndexDBfp);

					   
					  //讀出資料					 
					  //DataFile datafile;					   					   
					  //fseek(DataDBfp,m_spBucket.IndexData[i].DataFilePos ,SEEK_SET);
					  //fread(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);

					  
					  strlist.insert(strlist.end(),m_spBucket.IndexData[i].KeyData);
					  intlist.insert(intlist.end(),m_spBucket.IndexData[i].DataFilePos);
					  
				}	
		
			}

			if (m_spBucket.ChainData > -1)
			{

					//尋找最後的 chain
					int chain = m_spBucket.ChainData;
					 
					IndexData2 m_IndexData;
					//刪除全線資料
					while (chain != -1)
					{
							
						fseek(OverFlowDBfp,chain,SEEK_SET);
						fread(&m_IndexData,sizeof(char),sizeof(IndexData2),OverFlowDBfp);
						//刪除資料
						m_IndexData.Status = 2; //del							
						int offset = FIELD_OFFSET(IndexData2,Status);
						fseek(OverFlowDBfp,chain+ offset ,SEEK_SET);
						fwrite(&m_IndexData.Status,sizeof(char),sizeof(m_IndexData.Status),OverFlowDBfp);

						//
						//讀出資料					 
						//DataFile datafile;					   					   
						//fseek(DataDBfp,m_IndexData.DataFilePos ,SEEK_SET);
						//fread(&datafile,sizeof(char),sizeof(DataFile),DataDBfp);

						//放到 list 中
						strlist.insert(strlist.end(),m_IndexData.KeyData);
						intlist.insert(intlist.end(),m_IndexData.DataFilePos);

						chain = m_IndexData.ChainData;	

					}
					
					chain = m_spBucket.ChainData;

					//刪除父層
					m_spBucket.ChainData = -1;
					int offset = FIELD_OFFSET(Bucket2,ChainData);							 
					fseek(IndexDBfp,sizeof(IndexHeader) + (sizeof(Bucket2) * (splitpos)) + offset ,SEEK_SET);
					fwrite(&m_spBucket.ChainData,sizeof(char),sizeof(m_spBucket.ChainData),IndexDBfp);

 
				 
			}

 


			
		    for (itor =  strlist.begin(),itor2 = intlist.begin(); itor != strlist.end() && itor2 != intlist.end() ; itor++ ,itor2++ )
			{
             
				string tmpstr =  *itor;	
				int datapos = *itor2;

				InsertKey((char *) tmpstr.c_str(),datapos,1);
				 
			}



	return 0;
}

int CDB2::GetFreeDataPos(unsigned int HeaderSize , unsigned int RecordSize , int CheckOffset , int AvalValue)
{

	int GetRes = -1;
 
	return GetRes;

}

void CDB2::SelectData(int DataPos , char* FillBuff , int BuffSize)
{

	fseek(DataDBfp,DataPos,SEEK_SET);
	fread(FillBuff,sizeof(char),BuffSize,DataDBfp);

}

*/

CTreeRangeDB::CTreeRangeDB(CRITICAL_SECTION*  DBSection,char* FileDBPath,char* DataDBPath)
{

	FirstRec = false;
	mDBSection = DBSection;

	m_FileDBPath = FileDBPath;
	m_DataDBPath = DataDBPath;

	FileFp = NULL;
	DataFp = NULL;
 

 

}
CTreeRangeDB::~CTreeRangeDB()
{

	if (FileFp != NULL)
	{
	   fclose(FileFp);
	   FileFp = NULL;
	}

	if (DataFp != NULL)
	{
	   fclose(DataFp);
	   DataFp = NULL;
	}
}

void CTreeRangeDB::OpenDB()
{

	if (mDBSection != NULL)
		EnterCriticalSection(mDBSection);

	FirstRec = false; 

	FileFp = NULL;
	FileFp = fopen(m_FileDBPath,"r+b");
	if (FileFp == NULL)
	{
		FileFp = fopen(m_FileDBPath,"w+b");
		memset(&header,0,sizeof(header));

		time_t   nowtime;
		time( &nowtime );
		header.LastVersionTime = nowtime;

		DBfwrite(&header,1,sizeof(header),FileFp);		

		FirstRec = true;
	}
	else
	{
		DBfseek(FileFp,0,SEEK_SET);
		DBfread(&header,1,sizeof(header),FileFp);
		DBfseek(FileFp,0,SEEK_END);
		if (ftell(FileFp) == sizeof(header))
			FirstRec = true;
	}
 

	DataFp = NULL;
	DataFp = fopen(m_DataDBPath,"r+b");
	if (DataFp == NULL)
	{
		DataFp = fopen(m_DataDBPath,"w+b");
		//memset(&header,0,sizeof(header));
		//DBfwrite(&header,1,sizeof(header),DataFp);
	}

	FirstRecPos = sizeof(header);
}
void CTreeRangeDB::CloseDB()
{

	if (FileFp != NULL)
	{
	   fclose(FileFp);
	   FileFp = NULL;
	}

	if (DataFp != NULL)
	{
	   fclose(DataFp);
	   DataFp = NULL;
	}

	if (mDBSection != NULL)
		LeaveCriticalSection(mDBSection);

}

void CTreeRangeDB::SelectData(int DataPos , char* FillBuff , int BuffSize)
{

	DBfseek(DataFp,DataPos,SEEK_SET);
	DBfread(FillBuff,sizeof(char),BuffSize,DataFp);

}

long CTreeRangeDB::RebuildDB(long ValidSec)
{
	time_t   nowtime;
	time( &nowtime );

	//longer than one month
	if (difftime(nowtime , header.LastVersionTime) > ValidSec)
	{

			fclose(FileFp);
			fclose(DataFp);

			DataFp = fopen(m_DataDBPath,"w+b");
		

			FileFp = fopen(m_FileDBPath,"w+b");
			header.LastVersionTime = nowtime;
			DBfwrite(&header,1,sizeof(header),FileFp);
			
			

			FirstRecPos = sizeof(header);
			FirstRec = true;
	} 

	return 0;

}

int CTreeRangeDB::InsertData(char* DataBuff , int BuffSize)
{


		DBfseek(DataFp,0,SEEK_END);
		int filedatapos = ftell(DataFp);		
		DBfwrite(DataBuff,sizeof(char),BuffSize,DataFp);

		//fflush(DataFp);

		return filedatapos;

}



int CTreeRangeDB::Search(unsigned int KeyValue,unsigned int FilePos)
{


	int rc = -1;



	if (FilePos == 0) return rc;

		node ANode;
		memset(&ANode,0,sizeof(ANode));
		DBfseek(FileFp,FilePos,SEEK_SET);

		//此處刻意不出 error , 所使用 fread
		if (fread(&ANode,1,sizeof(ANode),FileFp) == sizeof(ANode))
		{

			if(KeyValue >= ANode.MinKeyValue && KeyValue <= ANode.MaxKeyValue )
			{
				return ANode.DataFilePos;
			}
			if (KeyValue < ANode.MinKeyValue)
			{
				return Search(KeyValue, ANode.LeftFilePos);
			}
			else
			{
				return Search(KeyValue, ANode.RightFilePos);
			}
		}



	return rc;

}

int CTreeRangeDB::InserKey(unsigned int MinKeyValue,unsigned int MaxKeyValue,unsigned int FilePos,unsigned int DataFilePos)
{
		int rc = -1;

		if (FirstRec)	
		{

				node NewANode;
				memset(&NewANode,0,sizeof(NewANode));

				NewANode.DataFilePos = DataFilePos;
				NewANode.MinKeyValue = MinKeyValue;
				NewANode.MaxKeyValue = MaxKeyValue;				

				fseek(FileFp,0,SEEK_END);
				int filepos = ftell(FileFp);	
				DBfwrite(&NewANode,1,sizeof(NewANode),FileFp);

				//fflush(FileFp);

				return filepos;
	
		}



		if (FilePos == 0) return rc;
	 
		node ANode;
		memset(&ANode,0,sizeof(ANode));
		DBfseek(FileFp,FilePos,SEEK_SET);
		DBfread(&ANode,1,sizeof(ANode),FileFp);

		if (MaxKeyValue < ANode.MinKeyValue)
		{
			if(ANode.LeftFilePos !=0)
			{			    
				InserKey(MinKeyValue,MaxKeyValue, ANode.LeftFilePos,DataFilePos);
			}
			else
			{
				node NewANode;
				memset(&NewANode,0,sizeof(NewANode));

				NewANode.DataFilePos = DataFilePos;
				NewANode.MinKeyValue = MinKeyValue;
				NewANode.MaxKeyValue = MaxKeyValue;				

				DBfseek(FileFp,0,SEEK_END);
				int filepos = ftell(FileFp);	
				DBfwrite(&NewANode,1,sizeof(NewANode),FileFp);
				
				//更新上層				
				ANode.LeftFilePos = filepos;
				DBfseek(FileFp,FilePos + FIELD_OFFSET(node,LeftFilePos),SEEK_SET);
				DBfwrite(&ANode.LeftFilePos,1,sizeof(ANode.LeftFilePos),FileFp);

				//fflush(FileFp);
			}  
	
		}
		else if (MinKeyValue > ANode.MaxKeyValue)
		{
		
			if(ANode.RightFilePos !=0)
			{
			    
				InserKey(MinKeyValue,MaxKeyValue, ANode.RightFilePos,DataFilePos);
			}
			else
			{
				node NewANode;
				memset(&NewANode,0,sizeof(NewANode));

				NewANode.DataFilePos = DataFilePos;
				NewANode.MinKeyValue = MinKeyValue;			    
				NewANode.MaxKeyValue = MaxKeyValue;			    

				DBfseek(FileFp,0,SEEK_END);
				int filepos = ftell(FileFp);	
				DBfwrite(&NewANode,1,sizeof(NewANode),FileFp);
				
				//更新上層				
				ANode.RightFilePos = filepos;
				DBfseek(FileFp,FilePos + FIELD_OFFSET(node,RightFilePos),SEEK_SET);
				DBfwrite(&ANode.RightFilePos,1,sizeof(ANode.RightFilePos),FileFp);

				//fflush(FileFp);
			}  

		
		}
		else
		{
			//切割
			 

			if (ANode.MinKeyValue > MinKeyValue)
			{
				InserKey(MinKeyValue,ANode.MinKeyValue -1, FirstRecPos,DataFilePos);
			}

			if (MaxKeyValue > ANode.MaxKeyValue)
			{
			    InserKey(ANode.MaxKeyValue+1,MaxKeyValue, FirstRecPos,DataFilePos);
			}
		
		}

	 


		return rc;

}


 