// BmMatch.h: interface for the CBmMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMMATCH_H__69878B60_B183_43BF_AC10_1CF11F3B5F4C__INCLUDED_)
#define AFX_BMMATCH_H__69878B60_B183_43BF_AC10_1CF11F3B5F4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information
#endif

class DLLDIR CBmMatch  
{
private:
	int *bmGs , *bmBc , *suff;

	int mStrLen;
	int mInStrLen;
	wchar_t* mAStr;
	wchar_t* mPatStr;
	 
	
	void preBmGs() ;
	void suffixes();
	void preBmBc();

public:
	CBmMatch();
	virtual ~CBmMatch();

	void StrMatch(wchar_t* AStr,wchar_t* PatStr);
	int  StrMatchOne(wchar_t* AStr,wchar_t* PatStr);
	int  StriMatchOne(wchar_t* AStr,wchar_t* PatStr);
 


};

#endif // !defined(AFX_BMMATCH_H__69878B60_B183_43BF_AC10_1CF11F3B5F4C__INCLUDED_)
