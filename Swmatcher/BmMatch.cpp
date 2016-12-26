// BmMatch.cpp: implementation of the CBmMatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BmMatch.h"

//#include <windows.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBmMatch::CBmMatch()
{
  bmGs = NULL;
  suff = NULL;
  //bmBc = new int[0xFFFF];
  bmBc =   new int[0xFFFFF];
  mStrLen = 0;
  mInStrLen = 0;
  mAStr = NULL;
  mPatStr = NULL;

  //MessageBox(NULL,"很抱歉此版本已停止使用!\n請到 http://www.softworking.com\n下載 SpamDog v1.01 !","SpamDog 版本停用通知",MB_OK);
  

}

CBmMatch::~CBmMatch()
{
  delete bmBc;
  if (bmGs != NULL)
	  delete bmGs;
  if (suff != NULL)
	  delete suff;

}

//尚未完成的 function , 回傳值要能多個
void CBmMatch::StrMatch(wchar_t* AStr,wchar_t* PatStr)
{
	if (bmGs != NULL)
	  delete bmGs;
    if (suff != NULL)
	  delete suff;

	mAStr = AStr;
	mPatStr = PatStr;
  
	mStrLen = wcslen(PatStr);
	mInStrLen = wcslen(AStr);
	bmGs = new int[mStrLen];
	suff = new int[mStrLen];
	

	preBmGs();
	preBmBc();

   /* Searching */
   int j = 0;
   int i = 0;
   while (j <= mInStrLen - mStrLen) 
   {
      for (i = mStrLen - 1; i >= 0 && mPatStr[i] == mAStr[i + j]; --i);
      if (i < 0) 
	  {
         //OUTPUT(j);
		  //CString tmp;
		  //tmp.Format("%d",j);
		  //AfxMessageBox(tmp);
         j += bmGs[0];
      }
      else
         j += max(bmGs[i], bmBc[mAStr[i + j]] - mStrLen + 1 + i);
   }


}

 

int  CBmMatch::StriMatchOne(wchar_t* AStr,wchar_t* PatStr)
{
	if (bmGs != NULL)
	  delete bmGs;
    if (suff != NULL)
	  delete suff;

	mPatStr = PatStr; 
	mAStr = AStr; 
  
	mStrLen = wcslen(PatStr);
	mInStrLen = wcslen(AStr);

	bmGs = new int[mStrLen];
	suff = new int[mStrLen];

	//處理 lower case
	for (int k = 0; k < mInStrLen; k++)
      mAStr[k] = towupper(mAStr[k]);

	for (int l = 0; l < mStrLen; l++)
      mPatStr[l] = towupper(mPatStr[l]);
	

	preBmGs();
	preBmBc();

   /* Searching */
   int j = 0;
   int i = 0;
   while (j <= mInStrLen - mStrLen) 
   {
      for (i = mStrLen - 1; i >= 0 && mPatStr[i] == mAStr[i + j]; --i);
      if (i < 0) 
	  {
         //OUTPUT(j);
		  //CString tmp;
		  //tmp.Format("%d",j);
		  //AfxMessageBox(tmp);
		  return j;
         j += bmGs[0];
      }
      else
         j += max(bmGs[i], bmBc[mAStr[i + j]] - mStrLen + 1 + i);
   }


return -1;
}
int  CBmMatch::StrMatchOne(wchar_t* AStr,wchar_t* PatStr)
{
	if (bmGs != NULL)
	  delete bmGs;
    if (suff != NULL)
	  delete suff;

	mAStr = AStr;
	mPatStr = PatStr;
  
	mStrLen = wcslen(PatStr);
	mInStrLen = wcslen(AStr);
	bmGs = new int[mStrLen];
	suff = new int[mStrLen];
	

	preBmGs();
	preBmBc();

   /* Searching */
   int j = 0;
   int i = 0;
   while (j <= mInStrLen - mStrLen) 
   {
      for (i = mStrLen - 1; i >= 0 && mPatStr[i] == mAStr[i + j]; --i);
      if (i < 0) 
	  {
         //OUTPUT(j);
		  //CString tmp;
		  //tmp.Format("%d",j);
		  //AfxMessageBox(tmp);
		  return j;
         j += bmGs[0];
      }
      else
         j += max(bmGs[i], bmBc[mAStr[i + j]] - mStrLen + 1 + i);
   }


return -1;
}

void CBmMatch::preBmGs() 
{
   int i = 0, j = 0;

   suffixes();

   for (i = 0; i < mStrLen; ++i)
      bmGs[i] = mStrLen;

   j = 0;
   for (i = mStrLen - 1; i >= -1; --i)
      if (i == -1 || suff[i] == i + 1)
         for (; j < mStrLen - 1 - i; ++j)
            if (bmGs[j] == mStrLen)
               bmGs[j] = mStrLen - 1 - i;

   for (i = 0; i <= mStrLen - 2; ++i)
      bmGs[mStrLen - 1 - suff[i]] = mStrLen - 1 - i;

}
void CBmMatch::suffixes()
{
	
  int f=0, g=0, i=0; 
 
   suff[mStrLen - 1] = mStrLen;
   g = mStrLen - 1;
   for (i = mStrLen - 2; i >= 0; --i) 
   {
      if (i > g && suff[i + mStrLen - 1 - f] < i - g)
         suff[i] = suff[i + mStrLen - 1 - f];
      else 
	  {
         if (i < g)
            g = i;
         f = i;
         while (g >= 0 && mPatStr[g] == mPatStr[g + mStrLen - 1 - f])
            --g;
         suff[i] = f - g;
      }
   }

}
void CBmMatch::preBmBc()
{

   int i = 0;
   //memset(bmBc,mStrLen,255 * 2);
    for (i = 0; i < 0xFFFFF; ++i)
      bmBc[i] = mStrLen;
   

   for (i = 0; i < mStrLen - 1; ++i)
      bmBc[mPatStr[i]] = mStrLen - i - 1;
}


 
