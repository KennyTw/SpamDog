// MapiProf.h: interface for the CMapiProf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIPROF_H__0BDC88AE_FB2E_4826_9D57_F822C81D57AB__INCLUDED_)
#define AFX_MAPIPROF_H__0BDC88AE_FB2E_4826_9D57_F822C81D57AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <mapix.h>
#include <mapiutil.h>



class CMapiProf  
{
public:
	CMapiProf();
	virtual ~CMapiProf();

		
	void ListProf(CComboBox *Combo);

};

#endif // !defined(AFX_MAPIPROF_H__0BDC88AE_FB2E_4826_9D57_F822C81D57AB__INCLUDED_)
