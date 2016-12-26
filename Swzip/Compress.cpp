// Compress.cpp: implementation of the CCompress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Compress.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompress::CCompress()
{
    if (lzo_init() != LZO_E_OK)
	{
		//printf("lzo_init() failed !!!\n");
		throw "lzo_init() failed !!!\n";
	}
}

CCompress::~CCompress()
{

}

int CCompress::EstimateOutBuffSize(unsigned int InSize)
{
   return (InSize + InSize / 64 + 16 + 3);
}

HRESULT CCompress::DoCompress(unsigned char* InBuff,unsigned int InSize,unsigned char* OutBuff,unsigned int *OutSize)
{

	HRESULT rtncode = S_OK;

	int r = lzo1x_1_compress(InBuff,InSize,OutBuff,OutSize,wrkmem);
	if (r == LZO_E_OK)
	{
		//printf("compressed %lu bytes into %lu bytes\n",
		//	(long) in_len, (long) out_len);
		
	}
	else
	{
		/* this should NEVER happen */
		//printf("internal error - compression failed: %d\n", r);
		return S_FALSE;
	}
	/* check for an incompressible block */
	if (*OutSize >= InSize)
	{
		//printf("This block contains incompressible data.\n");
		return NO_NEED_COMPRESS;
	}


	return rtncode;
}

HRESULT CCompress::DeCompress(unsigned char* InBuff,unsigned int InSize,unsigned char* OutBuff,unsigned int *OutSize)
{
	int	r = lzo1x_decompress(InBuff,InSize,OutBuff,OutSize,NULL);
	if (r == LZO_E_OK)
	{
		//printf("decompressed %lu bytes back into %lu bytes\n",
		//	(long) out_len, (long) in_len);
		return S_OK;
	}
	else
	{
		/* this should NEVER happen */
		//printf("internal error - decompression failed: %d\n", r);
		//return 1;
		return S_FALSE;
	}

}

HRESULT CCompress::ZipFile(char *SrcFileName,char *ZipFileName)
{

	FILE *fp,*zipfp;
	fp = fopen(SrcFileName,"rb");
	if (fp != NULL)
	{
	
		fseek(fp,0,SEEK_END);
		int FileSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
	
		char *buffer = new char[FileSize];		
		int readsize = fread(buffer,1,FileSize,fp); 

		int outbuffsize = EstimateOutBuffSize(readsize);
		char *outbuffer = new char[outbuffsize]; 

		HRESULT hr = DoCompress((unsigned char *)buffer,readsize,(unsigned char *)outbuffer,(unsigned int *) &outbuffsize);

		if (hr == S_OK)
		{
			zipfp = fopen(ZipFileName,"w+b");
			if (zipfp != NULL)
			{
			//先寫入大小

				fwrite(&FileSize,1,sizeof(FileSize),zipfp);		
				fwrite(outbuffer,1,outbuffsize,zipfp);
				fclose(zipfp);
		
			}
		}

			fclose(fp);
		

		delete outbuffer;
		delete buffer;
		
		return hr;

	}
	else
	{
		return S_FALSE;
	}



}
HRESULT CCompress::UnZipFile(char *ZipFileName,char *FileName)
{
	FILE *fp,*zipfp;
	zipfp = fopen(ZipFileName,"rb");
	if (zipfp != NULL)
	{
	
		fseek(zipfp,0,SEEK_END);
		int FileSize = ftell(zipfp);
		fseek(zipfp,0,SEEK_SET);

		//先讀大小
		int OriSize = 0;
		fread(&OriSize,1,sizeof(int),zipfp);

		char *buffer = new char[FileSize];		
		int readsize = fread(buffer,1,FileSize,zipfp);

		int outbuffsize = OriSize;
		char *outbuffer = new char[outbuffsize];

		HRESULT hr = DeCompress((unsigned char *)buffer,readsize,(unsigned char *)outbuffer,(unsigned int *) &outbuffsize);
		if (hr == S_OK)
		{

			fp = fopen(FileName,"w+b");
			if (fp != NULL)
			{
		
				fwrite(outbuffer,1,outbuffsize,fp);
				fclose(fp);
		
			}
		}

		fclose(zipfp);

		delete outbuffer;
		delete buffer;

		return hr;

	}
	else
	{
		return S_FALSE;
	}




}

__declspec(dllexport) CCompress* createZipObject()
{
	return new CCompress();
}