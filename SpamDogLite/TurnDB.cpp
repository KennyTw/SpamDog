// TurnDB.cpp: implementation of the CTurnDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpamDogLite.h"
#include "TurnDB.h"
#include "MainFrm.h"
#include "../Swzip/Compress.h"
//#include "../DBTool/DBdata.h"

#include "OEapi.h"
#include "MapiEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTurnDB::CTurnDB()
{

}

CTurnDB::~CTurnDB()
{

}


HRESULT CTurnDB::FixDB(CStatic *Memo)
{

  CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

  if (mfrm->Settings.APVersion == AP_OUTLOOK)
  {
	g_MapiEx->NotificationOff();
  } 
  else if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
  {
	g_OEapi->NotificationOff();
  }


	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];


	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );



			char IndexDbPath[_MAX_PATH];
			char overflowDbPath[_MAX_PATH];
			char datadbPath[_MAX_PATH];	   

			strcpy(IndexDbPath,drive);
			strcat(IndexDbPath,dir);   
	   
			strcpy(overflowDbPath,IndexDbPath);
			strcpy(datadbPath,IndexDbPath);

			strcat(IndexDbPath,"DB//db1new.db");
			strcat(overflowDbPath,"DB//db2new.db");
			strcat(datadbPath,"DB//db3new.db");

			DeleteFile(IndexDbPath);
			DeleteFile(overflowDbPath);
			DeleteFile(datadbPath);

			CDB *newdb = NULL;
 
			newdb = new CDB(&mfrm->m_DBSec,IndexDbPath,overflowDbPath,datadbPath,sizeof(DataFileHeader),1024 * 10);
			newdb->OpenDB();

			//寫入新 header
			//DataFileHeader newheader;
			//newheader.Version = 1;
			//newheader.NullValue = 0;

			//newdb.UpdateData((char *) &newheader,sizeof(newheader),0,0);



		    
			//開啟 db1,2,3

			char IndexDbOldPath[_MAX_PATH];
			char overflowDbOldPath[_MAX_PATH];
			char datadbOldPath[_MAX_PATH];	   

			strcpy(IndexDbOldPath,drive);
			strcat(IndexDbOldPath,dir);   
	   
			strcpy(overflowDbOldPath,IndexDbOldPath);
			strcpy(datadbOldPath,IndexDbOldPath);

			strcat(IndexDbOldPath,"DB//102db1.db");
			strcat(overflowDbOldPath,"DB//102db2.db");
			strcat(datadbOldPath,"DB//102db3.db"); 

			FILE *fp1 = fopen(IndexDbOldPath,"rb");
			FILE *fp2 = fopen(overflowDbOldPath,"rb");
			FILE *fp3 = fopen(datadbOldPath,"rb");

			int keycount = 0;
			//跳掉 index header
			fseek(fp1,sizeof(IndexHeader),SEEK_SET);
			//fseek(fp3,sizeof(DataFileHeader),SEEK_SET);
			while ( !feof( fp1 ))
			{ 
					Bucket m_Bucket;
					memset(&m_Bucket,0,sizeof(Bucket));
					int ReadSize = fread(&m_Bucket,sizeof(char),sizeof(Bucket),fp1);
					if (ReadSize == 0 ) break;

					if (m_Bucket.Status == 0)
					{					
						for (int i = 0 ; i < DefaultBucketSize ; i++)
						{
						    if(m_Bucket.IndexData[i].Status == 1)
							{
								//printf("%s\n",m_Bucket.IndexData[i].KeyData);
								//newdb.InsertData(
								keycount ++;
								if (keycount % 100 == 0)
								{
									CString tmp;
									tmp.Format("已修復筆數 : %d",keycount);
									Memo->SetWindowText(tmp);

									 

								}
		
									MSG msg;
									while (::PeekMessage(&msg, NULL,   // pump message until none
										NULL, NULL, PM_REMOVE))   // are left in the queue
									{
										TranslateMessage(&msg);
										DispatchMessage(&msg);
									}
								

								//讀出資料		
								DataFile datafileold;
								memset(&datafileold,0,sizeof(DataFile));

								fseek(fp3,m_Bucket.IndexData[i].DataFilePos,SEEK_SET);
								fread(&datafileold,sizeof(char),sizeof(datafileold),fp3);


								DataFile datafilenew;
								memset(&datafilenew,0,sizeof(DataFile));

								datafilenew.BadRate = datafileold.BadRate;
								datafilenew.OkRate = datafileold.OkRate;
								datafilenew.Status = datafileold.Status;
								datafilenew.UsedCount = datafileold.UsedCount;

								


								int datapos = -1;
								int freepos = newdb->GetFreeDataPos(sizeof(DataFileHeader),sizeof(DataFile),FIELD_OFFSET(DataFile,Status),0);
								if (freepos > -1)
								{
									datapos = freepos;
								} else
								{		
									datapos = newdb->InsertData((char *)&datafilenew,sizeof(DataFile));		
								}

							 
								int res = newdb->InsertKey(m_Bucket.IndexData[i].KeyData,datapos);
								if (res == -1)
								{
									Memo->SetWindowText("發現異常");
									//Memo->Invalidate();
									//Memo->UpdateWindow();
									//printf("Key Write Error\n");
									//_getch();
									
								}

								/*fclose(fp1);
								fclose(fp2);
								fclose(fp3);

								printf("Key Total : %d\n",keycount);
								printf("<Press Any Key to Free>\n");
								_getch();

								return 0;*/
 
							}
						}

						if (m_Bucket.ChainData != -1)
						{
					
						//處理  chain						 
						int chain = m_Bucket.ChainData;			 
						IndexData m_IndexData;
						memset(&m_IndexData,0,sizeof(IndexData));
						while (chain != -1)
						{
							
							fseek(fp2,chain,SEEK_SET);
							fread(&m_IndexData,sizeof(char),sizeof(IndexData),fp2);			


							if (m_IndexData.Status != 2)
							{
								//printf("%s\n",m_IndexData.KeyData);							
								keycount ++;

									


								if (keycount % 100 == 0)
								{
									CString tmp;
									tmp.Format("已修復筆數 : %d",keycount);
									Memo->SetWindowText(tmp);									
								}

									MSG msg;
									while (::PeekMessage(&msg, NULL,   // pump message until none
										NULL, NULL, PM_REMOVE))   // are left in the queue
									{
										TranslateMessage(&msg);
										DispatchMessage(&msg);
									}

								//Memo->Invalidate();
								//Memo->UpdateWindow();

									//讀出資料		
								DataFile datafileold;
								memset(&datafileold,0,sizeof(DataFile));

								fseek(fp3,m_IndexData.DataFilePos,SEEK_SET);
								fread(&datafileold,sizeof(char),sizeof(datafileold),fp3);


								DataFile datafilenew;
								memset(&datafilenew,0,sizeof(datafilenew));

								datafilenew.BadRate = datafileold.BadRate;
								datafilenew.OkRate = datafileold.OkRate;
								datafilenew.Status = datafileold.Status;
								datafilenew.UsedCount = datafileold.UsedCount;


								int datapos = -1;
								int freepos = newdb->GetFreeDataPos(sizeof(DataFileHeader),sizeof(DataFile),FIELD_OFFSET(DataFile,Status),0);
								if (freepos > -1)
								{
									datapos = freepos;
								} else
								{		
									datapos = newdb->InsertData((char *)&datafilenew,sizeof(DataFile));		
								}

							 
								int res = newdb->InsertKey(m_IndexData.KeyData,datapos);
								if (res == -1)
								{
									Memo->SetWindowText("發現異常");
									//Memo->Invalidate();
									//Memo->UpdateWindow();
									//printf("Key Write Error\n");
									//_getch();
								}
							}				 

							if (chain == m_IndexData.ChainData)
							{							
								Memo->SetWindowText("發現嚴重異常");
								//Memo->Invalidate();
								//Memo->UpdateWindow();
								//printf("Recusive Found");
								//_getch();
								break;
							}
						    chain = m_IndexData.ChainData;			
						}
					
						}
					}

					
					 

			}


			fclose(fp1);
			fclose(fp2);
			fclose(fp3);

			newdb->CloseDB();
			delete newdb;

			
			//更換至現在db
			char datadbRNPath[_MAX_PATH];	   
			char indexdbRNPath[_MAX_PATH];	   
			char overdbRNPath[_MAX_PATH];
			
			//char datadbMVPath[_MAX_PATH];	   
			//char indexdbMVPath[_MAX_PATH];	   
			//char overdbMVPath[_MAX_PATH];	   

			strcpy(datadbRNPath,drive);
			strcat(datadbRNPath,dir);			
			strcat(datadbRNPath,"DB//db3.bak");

			strcpy(indexdbRNPath,drive);
			strcat(indexdbRNPath,dir);			
			strcat(indexdbRNPath,"DB//db1.bak");

			strcpy(overdbRNPath,drive);
			strcat(overdbRNPath,dir);			
			strcat(overdbRNPath,"DB//db2.bak");

			

			/*strcpy(datadbMVPath,drive);
			strcat(datadbMVPath,dir);
			strcat(datadbMVPath,"Backup//");	
			strcat(datadbMVPath,"db3.bak");

			strcpy(indexdbMVPath,drive);
			strcat(indexdbMVPath,dir);
			strcat(indexdbMVPath,"Backup//");	
			strcat(indexdbMVPath,"db1.bak");

			strcpy(overdbMVPath,drive);
			strcat(overdbMVPath,dir);
			strcat(overdbMVPath,"Backup//");	
			strcat(overdbMVPath,"db2.bak");*/

			DeleteFile(datadbRNPath);
			DeleteFile(indexdbRNPath);
			DeleteFile(overdbRNPath);
			
			rename(IndexDbOldPath,indexdbRNPath);
			rename(overflowDbOldPath,overdbRNPath);
			rename(datadbOldPath,datadbRNPath);


			rename(IndexDbPath,IndexDbOldPath);
			rename(overflowDbPath,overflowDbOldPath);
			rename(datadbPath,datadbOldPath);


			//CopyFile(indexdbRNPath,indexdbMVPath,false);
			//CopyFile(overdbRNPath,overdbMVPath,false);
			//CopyFile(datadbRNPath,datadbMVPath,false);

			DeleteFile(indexdbRNPath);
			DeleteFile(overdbRNPath);
			DeleteFile(datadbRNPath);
		

		 

 // CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

  if (mfrm->Settings.APVersion == AP_OUTLOOK)
  {

	g_MapiEx->NotificationOn();
	g_MapiEx->ScanAllBox();


  } 
  else if (mfrm->Settings.APVersion = AP_OUTLOOKEX)
  { 
	g_OEapi->NotificationOn();
	g_OEapi->ScanAllBox();
  }

			CString tmp;
			tmp.Format("已修復完畢 , 總筆數 %d",keycount);
			Memo->SetWindowText(tmp);

	return S_OK;
}

HRESULT CTurnDB::TruncDB(unsigned int CutNum , CStatic *Memo)
{
  CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

  if (mfrm->Settings.APVersion == AP_OUTLOOK)
  {
	g_MapiEx->NotificationOff();
  } 
  else if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
  { 
	g_OEapi->NotificationOff();
  }
	
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];


	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );



			char IndexDbPath[_MAX_PATH];
			char overflowDbPath[_MAX_PATH];
			char datadbPath[_MAX_PATH];	   

			strcpy(IndexDbPath,drive);
			strcat(IndexDbPath,dir);   
	   
			strcpy(overflowDbPath,IndexDbPath);
			strcpy(datadbPath,IndexDbPath);

			strcat(IndexDbPath,"DB//db1new.db");
			strcat(overflowDbPath,"DB//db2new.db");
			strcat(datadbPath,"DB//db3new.db");

			DeleteFile(IndexDbPath);
			DeleteFile(overflowDbPath);
			DeleteFile(datadbPath);
 
			CDB *newdb=NULL;
			newdb = new CDB(&mfrm->m_DBSec,IndexDbPath,overflowDbPath,datadbPath,sizeof(DataFileHeader),1024 * 10);

			newdb->OpenDB();
			//寫入新 header
			//DataFileHeader newheader;
			//newheader.Version = 1;
			//newheader.NullValue = 0;

			//newdb.UpdateData((char *) &newheader,sizeof(newheader),0,0);



		    
			//開啟 db1,2,3

			char IndexDbOldPath[_MAX_PATH];
			char overflowDbOldPath[_MAX_PATH];
			char datadbOldPath[_MAX_PATH];	   

			strcpy(IndexDbOldPath,drive);
			strcat(IndexDbOldPath,dir);   
	   
			strcpy(overflowDbOldPath,IndexDbOldPath);
			strcpy(datadbOldPath,IndexDbOldPath);

			strcat(IndexDbOldPath,"DB//102db1.db");
			strcat(overflowDbOldPath,"DB//102db2.db");
			strcat(datadbOldPath,"DB//102db3.db"); 

			FILE *fp1 = fopen(IndexDbOldPath,"rb");
			FILE *fp2 = fopen(overflowDbOldPath,"rb");
			FILE *fp3 = fopen(datadbOldPath,"rb");

			int keycount = 0;
			//跳掉 index header
			fseek(fp1,sizeof(IndexHeader),SEEK_SET);
			//fseek(fp3,sizeof(DataFileHeader),SEEK_SET);
			while ( !feof( fp1 ))
			{ 
					Bucket m_Bucket;
					memset(&m_Bucket,0,sizeof(Bucket));
					int ReadSize = fread(&m_Bucket,sizeof(char),sizeof(Bucket),fp1);
					if (ReadSize == 0 ) break;

					if (m_Bucket.Status == 0)
					{					
						for (int i = 0 ; i < DefaultBucketSize ; i++)
						{
						    if(m_Bucket.IndexData[i].Status == 1)
							{
								//printf("%s\n",m_Bucket.IndexData[i].KeyData);
								//newdb.InsertData(
								keycount ++;
								if (keycount % 100 == 0)
								{
									CString tmp;
									tmp.Format("已重組筆數 : %d",keycount);
									Memo->SetWindowText(tmp);
								}
		
									MSG msg;
									while (::PeekMessage(&msg, NULL,   // pump message until none
										NULL, NULL, PM_REMOVE))   // are left in the queue
									{
										TranslateMessage(&msg);
										DispatchMessage(&msg);
									}
								

								//讀出資料		
								DataFile datafileold;
								memset(&datafileold,0,sizeof(DataFile));

								fseek(fp3,m_Bucket.IndexData[i].DataFilePos,SEEK_SET);
								fread(&datafileold,sizeof(char),sizeof(datafileold),fp3);

								if (datafileold.UsedCount > CutNum || datafileold.Status > 1) //IP Data 不被 trim
								{


									DataFile datafilenew;
									memset(&datafilenew,0,sizeof(DataFile));

									datafilenew.BadRate = datafileold.BadRate;
									datafilenew.OkRate = datafileold.OkRate;
									datafilenew.Status = datafileold.Status;
									datafilenew.UsedCount = datafileold.UsedCount;

								


									int datapos = -1;
									int freepos = newdb->GetFreeDataPos(sizeof(DataFileHeader),sizeof(DataFile),FIELD_OFFSET(DataFile,Status),0);
									if (freepos > -1)
									{
										datapos = freepos;
									} else
									{		
										datapos = newdb->InsertData((char *)&datafilenew,sizeof(DataFile));		
									}

							 
									int res = newdb->InsertKey(m_Bucket.IndexData[i].KeyData,datapos);
									if (res == -1)
									{
										Memo->SetWindowText("發現異常");
										//Memo->Invalidate();
										//Memo->UpdateWindow();
										//printf("Key Write Error\n");
										//_getch();
									
									}

								}
								/*fclose(fp1);
								fclose(fp2);
								fclose(fp3);

								printf("Key Total : %d\n",keycount);
								printf("<Press Any Key to Free>\n");
								_getch();

								return 0;*/
 
							}
						}

						if (m_Bucket.ChainData != -1)
						{
					
						//處理  chain						 
						int chain = m_Bucket.ChainData;			 
						IndexData m_IndexData;
						memset(&m_IndexData,0,sizeof(IndexData));
						while (chain != -1)
						{
							
							fseek(fp2,chain,SEEK_SET);
							fread(&m_IndexData,sizeof(char),sizeof(IndexData),fp2);			


							if (m_IndexData.Status != 2)
							{
								//printf("%s\n",m_IndexData.KeyData);							
								keycount ++;

								if (keycount % 100 == 0)
								{
								CString tmp;
								tmp.Format("已重組筆數 : %d",keycount);
								Memo->SetWindowText(tmp);
								}

									MSG msg;
									while (::PeekMessage(&msg, NULL,   // pump message until none
										NULL, NULL, PM_REMOVE))   // are left in the queue
									{
										TranslateMessage(&msg);
										DispatchMessage(&msg);
									}

								//Memo->Invalidate();
								//Memo->UpdateWindow();

									//讀出資料		
								DataFile datafileold;
								memset(&datafileold,0,sizeof(DataFile));

								fseek(fp3,m_IndexData.DataFilePos,SEEK_SET);
								fread(&datafileold,sizeof(char),sizeof(datafileold),fp3);

								if (datafileold.UsedCount > CutNum || datafileold.Status > 1)
								{

								DataFile datafilenew;
								memset(&datafilenew,0,sizeof(datafilenew));

								datafilenew.BadRate = datafileold.BadRate;
								datafilenew.OkRate = datafileold.OkRate;
								datafilenew.Status = datafileold.Status;
								datafilenew.UsedCount = datafileold.UsedCount;


								int datapos = -1;
								int freepos = newdb->GetFreeDataPos(sizeof(DataFileHeader),sizeof(DataFile),FIELD_OFFSET(DataFile,Status),0);
								if (freepos > -1)
								{
									datapos = freepos;
								} else
								{		
									datapos = newdb->InsertData((char *)&datafilenew,sizeof(DataFile));		
								}

							 
								int res = newdb->InsertKey(m_IndexData.KeyData,datapos);
								if (res == -1)
								{
									Memo->SetWindowText("發現異常");
									//Memo->Invalidate();
									//Memo->UpdateWindow();
									//printf("Key Write Error\n");
									//_getch();
								}
								}
							}				 

							if (chain == m_IndexData.ChainData)
							{							
								Memo->SetWindowText("發現嚴重異常");
								//Memo->Invalidate();
								//Memo->UpdateWindow();
								//printf("Recusive Found");
								//_getch();
								break;
							}
						    chain = m_IndexData.ChainData;			
						}
					
						}
					}

					
					 

			}


			fclose(fp1);
			fclose(fp2);
			fclose(fp3);

			newdb->CloseDB();
			delete newdb;

			//更換至現在db
			char datadbRNPath[_MAX_PATH];	   
			char indexdbRNPath[_MAX_PATH];	   
			char overdbRNPath[_MAX_PATH];
			
			//char datadbMVPath[_MAX_PATH];	   
			//char indexdbMVPath[_MAX_PATH];	   
			//char overdbMVPath[_MAX_PATH];

			strcpy(datadbRNPath,drive);
			strcat(datadbRNPath,dir);
			strcat(datadbRNPath,"DB//db3.bak");

			strcpy(indexdbRNPath,drive);
			strcat(indexdbRNPath,dir);
			strcat(indexdbRNPath,"DB//db1.bak");

			strcpy(overdbRNPath,drive);
			strcat(overdbRNPath,dir);
			strcat(overdbRNPath,"DB//db2.bak");

			/*strcpy(datadbMVPath,drive);
			strcat(datadbMVPath,dir);
			strcat(datadbMVPath,"Backup//");	
			strcat(datadbMVPath,"db3.bak");

			strcpy(indexdbMVPath,drive);
			strcat(indexdbMVPath,dir);
			strcat(indexdbMVPath,"Backup//");	
			strcat(indexdbMVPath,"db1.bak");

			strcpy(overdbMVPath,drive);
			strcat(overdbMVPath,dir);
			strcat(overdbMVPath,"Backup//");	
			strcat(overdbMVPath,"db2.bak");*/

			DeleteFile(datadbRNPath);
			DeleteFile(indexdbRNPath);
			DeleteFile(overdbRNPath);
			
			rename(IndexDbOldPath,indexdbRNPath);
			rename(overflowDbOldPath,overdbRNPath);
			rename(datadbOldPath,datadbRNPath);


			rename(IndexDbPath,IndexDbOldPath);
			rename(overflowDbPath,overflowDbOldPath);
			rename(datadbPath,datadbOldPath);

			//CopyFile(indexdbRNPath,indexdbMVPath,false);
			//CopyFile(overdbRNPath,overdbMVPath,false);
			//CopyFile(datadbRNPath,datadbMVPath,false);

			DeleteFile(indexdbRNPath);
			DeleteFile(overdbRNPath);
			DeleteFile(datadbRNPath);

		 

//	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();
    Memo->SetWindowText("重新掃描收信匣中");
	if (mfrm->Settings.APVersion == AP_OUTLOOK)
	{

		g_MapiEx->NotificationOn();

		if (mfrm->Settings.ShowMsg)
		{
			mfrm->Settings.ShowMsg = false; //暫時不顯示
			g_MapiEx->ScanAllBox();
			mfrm->Settings.ShowMsg = true; //暫時不顯示
		}
		else
		{
		    g_MapiEx->ScanAllBox();
		}
		


	} else if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
	{ 
		g_OEapi->NotificationOn();
		//g_OEapi->ScanAllBox();
	 

		if (mfrm->Settings.ShowMsg)
		{
			mfrm->Settings.ShowMsg = false; //暫時不顯示
		 
			g_OEapi->ScanAllBox();
			mfrm->Settings.ShowMsg = true;  
		}
		else
		{
		    g_OEapi->ScanAllBox();
		}

	}

			CString tmp;
			tmp.Format("已重組完畢 , 總筆數 %d",keycount);
			Memo->SetWindowText(tmp);

	return S_OK;
}

/*
HRESULT CTurnDB::ParseKeyFile(char *FileName)
{
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	 char PgPath[_MAX_PATH];
	 char Db1Path[_MAX_PATH];
	 char Db2Path[_MAX_PATH];
	 char Db3Path[_MAX_PATH];


	  //char szFileName[MAX_PATH];
      HINSTANCE hInstance = GetModuleHandle(NULL);
      GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	  _splitpath( path_buffer, drive, dir, fname, ext );
	  strcpy(PgPath,drive);
	  strcat(PgPath,dir);

	  strcpy(Db1Path,PgPath);
	  strcpy(Db2Path,PgPath);
	  strcpy(Db3Path,PgPath);

	  strcat(Db1Path,"db1.db");
	  strcat(Db2Path,"db2.db");
	  strcat(Db3Path,"db3.db");



	FILE *fp = fopen(FileName, "rb");
	if (fp != NULL)
	{
	
		int readbyte = 0;

//#ifdef _OUTLOOK
//	g_MapiEx->NotificationOff();
//#endif
//#ifdef _OUTLOOKEX
//	g_OEapi->NotificationOff();
//#endif


		CSPAMDB spdb(Db1Path,Db2Path,Db3Path,1024 * 10);

		do
		{
			bool bCompress=false;
			readbyte = fread(&bCompress,1,sizeof(bool),fp);
			if (readbyte == 0) break;

			int bufflen=0;
			unsigned int outbufflen=0;
			readbyte = fread(&bufflen,1,sizeof(bufflen),fp);

			char *buff = new char[bufflen];
			char *buffdec = new char[bufflen];

			//正試版要能處理此部份 , 因為目前的計算有問題


			readbyte = fread(buff,1,bufflen,fp);
			
			if (bCompress)
			{
				//解壓縮
				CCompress cc;
				cc.DeCompress((unsigned char *) buff,bufflen,(unsigned char *)buffdec,&outbufflen);

				UpdateFile m_UpdateFile;		

				memcpy(&m_UpdateFile,buffdec,sizeof(UpdateFile));

			 

				int len = strlen(buffdec + sizeof(UpdateFile));
				int SavePos = 0;
				int ParseCount = 0;

				

				
				for(int i = 0 ; i < len ; i ++)
				{
				
					if (*(buffdec + sizeof(UpdateFile) + i) == char(13))
					{
					
						ParseCount ++;

						char tmpstr[50];
						memset(tmpstr,0,50);
						strncpy(tmpstr,buffdec + sizeof(UpdateFile) + SavePos , i - SavePos);

						if (spdb.GetSpamData(tmpstr).SearchRes == -1)
						{
							DataFile df;
							memset(&df,0,sizeof(df));
							if (m_UpdateFile.Type == 1)
							{
							  df.BadRate = 1;
							}
							else
							{
							  df.OkRate = 1;
							}
							df.Status = 1;

							spdb.InsertSpamData(tmpstr,&df);
						
						}

						OutputDebugString(tmpstr);
						OutputDebugString("\n");

						SavePos = i + 1;

						if (ParseCount >= m_UpdateFile.NumberItem) 
						{
							
							//繼續讀取下一個 section
							break;
							
						}
					
					}
				
				}

					 
			}



			delete buff;
			delete buffdec;
		} while (readbyte > 0);


//#ifdef _OUTLOOK

//	g_MapiEx->NotificationOn();
//	g_MapiEx->ScanAllBox();


//#endif

//#ifdef _OUTLOOKEX
//	g_OEapi->NotificationOn();
//	g_OEapi->ScanAllBox();
//#endif


		fclose(fp);
	}

	//更新 last version
	return S_OK;
}


*/