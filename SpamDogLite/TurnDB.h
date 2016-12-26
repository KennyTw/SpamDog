// TurnDB.h: interface for the CTurnDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TURNDB_H__8157E75B_9A2D_48FB_A5A4_7809F1C16910__INCLUDED_)
#define AFX_TURNDB_H__8157E75B_9A2D_48FB_A5A4_7809F1C16910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTurnDB  
{
public:
	CTurnDB();
	virtual ~CTurnDB();

	HRESULT FixDB(CStatic *Memo);
	HRESULT TruncDB(unsigned int CutNum , CStatic *Memo);
	//HRESULT ParseKeyFile(char *FileName);

};

#endif // !defined(AFX_TURNDB_H__8157E75B_9A2D_48FB_A5A4_7809F1C16910__INCLUDED_)
