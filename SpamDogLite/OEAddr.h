// OEAddr.h: interface for the COEAddr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OEADDR_H__08E6565E_9CB0_4E7E_8787_E88394ED0FDD__INCLUDED_)
#define AFX_OEADDR_H__08E6565E_9CB0_4E7E_8787_E88394ED0FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mapix.h>
#include <wabapi.h>
typedef HRESULT (WINAPI *fWABOpen)(LPADRBOOK*,LPWABOBJECT*,LPWAB_PARAM,DWORD);

class COEAddr  
{
private:
	HMODULE hModule;

	LPADRBOOK lpAddrBook;	
	LPWABOBJECT lpWABObject;
	fWABOpen fpWABOpen;

public:
	COEAddr();
	virtual ~COEAddr();

	bool OpenAddrBook(HWND pwnd , CString* SelectedStr);

};

#endif // !defined(AFX_OEADDR_H__08E6565E_9CB0_4E7E_8787_E88394ED0FDD__INCLUDED_)
