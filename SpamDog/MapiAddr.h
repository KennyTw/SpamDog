// MapiAddr.h: interface for the CMapiAddr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIADDR_H__178A18F0_B9A1_47BA_9721_987A3C88D2EC__INCLUDED_)
#define AFX_MAPIADDR_H__178A18F0_B9A1_47BA_9721_987A3C88D2EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mapix.h>
#include <mapiutil.h>

class CMapiAddr  
{
public:
	CMapiAddr();
	virtual ~CMapiAddr();
	HRESULT Login (char* prof=NULL);
	HRESULT Logout();

	bool OpenAddrBook(HWND pwnd , CString* SelectedStr);

private:
	LPMAPISESSION lpMAPISession ;


};

#endif // !defined(AFX_MAPIADDR_H__178A18F0_B9A1_47BA_9721_987A3C88D2EC__INCLUDED_)
