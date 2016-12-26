// OEAddr.cpp: implementation of the COEAddr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpamDogLite.h"
#include "OEAddr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COEAddr::COEAddr()
{
	HKEY keyResult;
	BYTE keyValue[MAX_PATH];
	DWORD dataout=800;

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, WAB_DLL_PATH_KEY, 0, KEY_ALL_ACCESS, &keyResult); //"Software\\Microsoft\\WAB\\DLLPath"
	long result = RegQueryValueEx(keyResult, "", 0, 0, keyValue, &dataout);
	RegCloseKey(keyResult);

	char PathToWAB[255];
	//Get the full path of WAB and store in PathToWAB
	strcpy(PathToWAB, (char*)keyValue);
	
	//Now let us load the library
	hModule = LoadLibrary(PathToWAB);

	

	fpWABOpen = NULL;
	fpWABOpen = (fWABOpen)GetProcAddress(hModule, "WABOpen");	
	if (fpWABOpen == NULL) throw "GetProcAddress WABOpen";	
	
	HRESULT hr = (fpWABOpen)(&lpAddrBook, &lpWABObject, NULL, 0);
	if (FAILED(hr)) throw "WABOpen Call Fail";


}

COEAddr::~COEAddr()
{
	if (lpAddrBook != NULL)
	   lpAddrBook->Release();

	if (lpWABObject != NULL)
		lpWABObject->Release();
	
	FreeLibrary(hModule);
}



bool COEAddr::OpenAddrBook(HWND pwnd , CString* SelectedStr)
{

bool isSelected = false;

	   LPADRLIST     lpadrlist  = NULL;
	   LPSTR    rglpszDestTitles[1];
       ULONG    rgulDestComps[1];
       ULONG    ulFlags           = 0;
       ULONG    ulUIParam         = 0;

       ulFlags = ulFlags | DIALOG_MODAL;

       rglpszDestTitles[0]        = "選擇";
       rgulDestComps[0]           = MAPI_TO;

       ADRPARM  adrparm;
       adrparm.cbABContEntryID    = 0;
       adrparm.lpABContEntryID    = NULL;
       adrparm.ulFlags            = ulFlags;
       adrparm.lpReserved         = NULL;
       adrparm.ulHelpContext      = 0;
       adrparm.lpszHelpFileName   = NULL;
       adrparm.lpfnABSDI          = NULL;
       adrparm.lpfnDismiss        = NULL;
       adrparm.lpvDismissContext  = NULL;
       adrparm.lpszCaption        = "選擇寄件者";
       adrparm.lpszNewEntryTitle  = "";
       adrparm.lpszDestWellsTitle = "選擇寄件者:";
       adrparm.cDestFields        = 1;
       adrparm.nDestFieldFocus    = 0;
       adrparm.lppszDestTitles    = rglpszDestTitles;
       adrparm.lpulDestComps      = rgulDestComps;
       adrparm.lpContRestriction  = NULL;
       adrparm.lpHierRestriction  = NULL;

	   lpAddrBook->Address((ULONG *) &pwnd, &adrparm, &lpadrlist );
	   ///lpAddrBook->Address(NULL, &adrparm, &lpadrlist );

	   if (lpadrlist != NULL)
	   {
        isSelected = true;
		for (unsigned int i = 0 ; i < lpadrlist->cEntries ; i ++)
		{
			
		   for (unsigned int j = 0 ; j < lpadrlist->aEntries[i].cValues ; j++)
		   {
			   if (lpadrlist->aEntries[i].rgPropVals[j].ulPropTag ==PR_DISPLAY_NAME  )
				//if (lpadrlist->aEntries[i].rgPropVals[j].ulPropTag ==PR_EMAIL_ADDRESS  )
			    
			   {
				   	(*SelectedStr) += lpadrlist->aEntries[i].rgPropVals[j].Value.lpszA ;
					(*SelectedStr) += ";";				 
			   }
		   
		   } 
	   
		}

		if (!SelectedStr->IsEmpty())
		{
			SelectedStr->Delete(SelectedStr->GetLength()-1,1);
		}
	   }

 
	   

	return isSelected;
}