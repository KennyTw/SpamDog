// OEWnd.h: interface for the COEWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OEWND_H__71B942D9_5E1D_4809_A1CD_F45716CCD337__INCLUDED_)
#define AFX_OEWND_H__71B942D9_5E1D_4809_A1CD_F45716CCD337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COEapi;
class COEWnd  
{

private:	
	HWND m_hWnd; 
	COEapi* m_oeapi;
	int m_HNDno;


protected:
		static LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static COEWnd* GetObjectFromWindow(HWND hWnd);

public:
	COEWnd(COEapi* oeapi,int HNDno);
	virtual ~COEWnd();
	HWND GetWinHanlde();

};

#endif // !defined(AFX_OEWND_H__71B942D9_5E1D_4809_A1CD_F45716CCD337__INCLUDED_)
