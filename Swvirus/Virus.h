// Virus.h: interface for the CVirus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRUS_H__B29330E9_108B_414E_9C04_29A6E75EAD33__INCLUDED_)
#define AFX_VIRUS_H__B29330E9_108B_414E_9C04_29A6E75EAD33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information
#endif 

class DLLDIR CVirus  
{


public:
	int GetBlockSize();
	CVirus();
	virtual ~CVirus();
	bool CheckDotCount(char *FileName);
	bool FileNameCheck(char *FileName);
	bool ZipHeaderCheck(char *buff,int size);
	 

};

#endif // !defined(AFX_VIRUS_H__B29330E9_108B_414E_9C04_29A6E75EAD33__INCLUDED_)
