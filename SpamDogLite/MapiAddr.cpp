// MapiAddr.cpp: implementation of the CMapiAddr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpamDogLite.h"
#include "MapiAddr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapiAddr::CMapiAddr()
{
	lpMAPISession = NULL;
}

CMapiAddr::~CMapiAddr()
{
	Logout();
}

HRESULT CMapiAddr::Login (char* prof)
{
	HRESULT       hRes;
	 

	hRes = MAPIInitialize(NULL);
	if (FAILED(hRes)) goto quit;


    hRes = MAPILogonEx(0,
      prof,//profile name
      NULL,//password - This parameter should ALWAYS be NULL
      MAPI_LOGON_UI, //Allow a profile picker box to show if not logged in
      &lpMAPISession);//handle of session
	if (FAILED(hRes)) goto quit;

quit:
	return 0;
}
HRESULT CMapiAddr::Logout()
{
	
	HRESULT       hRes;

	if (lpMAPISession != NULL)
	{
	hRes = lpMAPISession->Logoff(0,MAPI_LOGOFF_UI,0); 
	MAPIUninitialize();

	lpMAPISession = NULL;
	}

	return 0;

}

bool CMapiAddr::OpenAddrBook(HWND pwnd,CString* SelectedStr)
{
	LPADRBOOK addBook;
	bool isSelected = false;

	if (lpMAPISession != NULL)
	{
	
	lpMAPISession->OpenAddressBook(NULL,NULL,NULL,&addBook);
	 

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
       //adrparm.lpszNewEntryTitle  = "For this test dialog";
	   adrparm.lpszNewEntryTitle  = NULL;
       adrparm.lpszDestWellsTitle = "選擇寄件者:";
       adrparm.cDestFields        = 1;
       adrparm.nDestFieldFocus    = 0;
       adrparm.lppszDestTitles    = rglpszDestTitles;
       adrparm.lpulDestComps      = rgulDestComps;
       adrparm.lpContRestriction  = NULL;
       adrparm.lpHierRestriction  = NULL;

       //addBook->Address( &ulUIParam, &adrparm, &lpadrlist );
	   addBook->Address((ULONG *) &pwnd, &adrparm, &lpadrlist );

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
				   //AfxMessageBox(lpadrlist->aEntries[i].rgPropVals[j].Value.lpszA);
					/*if (strcmp(lpadrlist->aEntries[i].rgPropVals[j].Value.lpszA,"SMTP") == 0)
				   {
 
					if (lpadrlist->aEntries[i].rgPropVals[j].ulPropTag == PR_EMAIL_ADDRESS)
					{
			   
						//AfxMessageBox(lpadrlist->aEntries[i].rgPropVals[j].Value.lpszA);
					

					
					break;
					}*/
				//}
			   }
		   
		   } 
	   
		}

		if (!SelectedStr->IsEmpty())
		{
			SelectedStr->Delete(SelectedStr->GetLength()-1,1);
		}
	   }

	 }

	return isSelected;
}