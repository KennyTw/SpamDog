// Virus.cpp: implementation of the CVirus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Virus.h"
#include <string>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirus::CVirus()
{

}

CVirus::~CVirus()
{

}


bool CVirus::FileNameCheck(char *FileName)
{
	//scr , pif
	//CString tmpstr = FileName;
	//static const std::basic_string <char>::size_type npos = -1;

	std::string tmpstr = FileName;
	

	if (tmpstr.find(".pif") != std::string::npos)
	{
		return true;
	} 
	else if (tmpstr.find(".scr") != std::string::npos)
	{
		return true;
	} /* 取消誤判過高的 rule
	else if (CheckDotCount((char *)tmpstr.c_str()))
	{
	    return true;
	}*/ 
	
	return false;
}

bool CVirus::CheckDotCount(char *FileName)
{
	int len = strlen(FileName);
	int dotcount = 0;
	//int PreDotPos = 0 ; //兩點相接不算

	for ( int i=0 ;  i < len ; i++)
	{
		if (FileName[i] == '.' && FileName[i+1] != '.')
		{
		

			//if ((i - PreDotPos) > 1)
			//{
			
				dotcount ++;
				if (dotcount > 1)
					return true;
			//}

			//PreDotPos = i;

		} else if (FileName[i] == '(') //因為檔名有 (11.kb) 
		{
			break;		
		}
	}

	return false;

}

bool CVirus::ZipHeaderCheck(char *buff,int size)
{
	char Signature[5] = {'P','K',3,4,0};

	//ZipHeader m_zipheader;
	//memset(&m_zipheader,0,sizeof(ZipHeader));
	//memcpy(&m_zipheader,buff,sizeof(ZipHeader));

	if (strncmp(buff,Signature,4) == 0)
	{
		//是 zip

		//取出 FileLength
		WORD FileLength = 0;
		memcpy(&FileLength,buff + 26 ,2);
		//取出 FileName

		char FileName[_MAX_FNAME];
		FileName[0] = 0;		
		memcpy(&FileName, buff + 30 , FileLength);
		FileName[FileLength] = 0;

		//檢查 MZ
		if (*(buff + 30 + FileLength) == 0x4D &&
			*(buff + 30 + FileLength + 1) == 0x5A)
		{
			//為 execute file
			if (FileNameCheck(FileName))
			{
				return true;
			}
			else if (CheckDotCount(FileName))
			{
				return true;				
			}
		}
	
	
	}
	//IMAGE_DOS_SIGNATURE
	

	//讀取第一個 FileName Block
	//如果是執行檔 (PE Check), 且為雙附檔名 ,
	// 或是特定檔案 , 則判斷為病毒
	return false;
}

int CVirus::GetBlockSize()
{
  return 30 + _MAX_FNAME + 2 ; //2 for MZ space
}

