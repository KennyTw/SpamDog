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
	} /* �����~�P�L���� rule
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
	//int PreDotPos = 0 ; //���I�۱�����

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

		} else if (FileName[i] == '(') //�]���ɦW�� (11.kb) 
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
		//�O zip

		//���X FileLength
		WORD FileLength = 0;
		memcpy(&FileLength,buff + 26 ,2);
		//���X FileName

		char FileName[_MAX_FNAME];
		FileName[0] = 0;		
		memcpy(&FileName, buff + 30 , FileLength);
		FileName[FileLength] = 0;

		//�ˬd MZ
		if (*(buff + 30 + FileLength) == 0x4D &&
			*(buff + 30 + FileLength + 1) == 0x5A)
		{
			//�� execute file
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
	

	//Ū���Ĥ@�� FileName Block
	//�p�G�O������ (PE Check), �B�������ɦW ,
	// �άO�S�w�ɮ� , �h�P�_���f�r
	return false;
}

int CVirus::GetBlockSize()
{
  return 30 + _MAX_FNAME + 2 ; //2 for MZ space
}

