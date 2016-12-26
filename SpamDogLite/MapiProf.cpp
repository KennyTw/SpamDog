// MapiProf.cpp: implementation of the CMapiProf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpamDogLite.h"
#include "MapiProf.h"


#include <mapix.h>
#include <mapiutil.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapiProf::CMapiProf()
{

}

CMapiProf::~CMapiProf()
{

}


void CMapiProf::ListProf(CComboBox *Combo)
{

	HRESULT       hRes;
	

	LPPROFADMIN pAdminProfiles = NULL; // Pointer to IProfAdmin object
	hRes = MAPIAdminProfiles ( 0L, &pAdminProfiles );
	
	if (hRes != S_OK) return;

	LPMAPITABLE pTable = NULL;
	pAdminProfiles->GetProfileTable(0,&pTable);

	LPSRowSet pRows = NULL;

	static SizedSPropTagArray(1,sptCols) = { 1, PR_DISPLAY_NAME  };

     hRes = HrQueryAllRows(
      pTable,
      (LPSPropTagArray) &sptCols,
      NULL,//restriction...we're not using this parameter
      NULL,//sort order...we're not using this parameter
      0,
      &pRows);

	 for (unsigned int i = 0 ; i < pRows->cRows ; i ++)
	 {
	 
		 if (pRows->aRow[i].lpProps[0].ulPropTag == PR_DISPLAY_NAME)
		 {
		 
			Combo->AddString(pRows->aRow[i].lpProps[0].Value.lpszA); 		 
		 }
	 
	 }

	 FreeProws(pRows);

    pTable->Release();
	pAdminProfiles->Release();


	return ;




}