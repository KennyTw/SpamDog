// Compress.h: interface for the CCompress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPRESS_H__E3263EB0_BE62_42ED_A624_1223437601CE__INCLUDED_)
#define AFX_COMPRESS_H__E3263EB0_BE62_42ED_A624_1223437601CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information
#endif

#include <stdio.h>

#include "minilzo.h"

 
/* Work-memory needed for compression. Allocate memory in units
 * of `lzo_align_t' (instead of `char') to make sure it is properly aligned.
 */

#define HEAP_ALLOC(var,size) \
	lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

#define NO_NEED_COMPRESS 1

class DLLDIR CCompress  
{
public:
	CCompress();
	virtual ~CCompress();

	int EstimateOutBuffSize(unsigned int InSize);
	HRESULT DoCompress(unsigned char* InBuff,unsigned int InSize,unsigned char* OutBuff,unsigned int *OutSize);
	virtual HRESULT DeCompress(unsigned char* InBuff,unsigned int InSize,unsigned char* OutBuff,unsigned int *OutSize);

	HRESULT ZipFile(char *SrcFileName,char *ZipFileName);
	virtual HRESULT UnZipFile(char *ZipFileName,char *FileName);
};

#endif // !defined(AFX_COMPRESS_H__E3263EB0_BE62_42ED_A624_1223437601CE__INCLUDED_)
