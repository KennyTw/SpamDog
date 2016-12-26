// OEWnd.h: interface for the COEWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OEWND_H__71B942D9_5E1D_4809_A1CD_F45716CCD337__INCLUDED_)
#define AFX_OEWND_H__71B942D9_5E1D_4809_A1CD_F45716CCD337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OEglobal.h"

class COEWnd2  
{

private:	
	HWND m_hWnd; 
	
	//ULONG mRegType;
	LPVOID mlpParam;
	LPOeOnNotify mOnNotify;
	//IStoreFolder* mRegisterFolder;
	OeREGISTERNOTIFY *mOeREGISTERNOTIFY;

protected:
		static LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static COEWnd2* GetObjectFromWindow(HWND hWnd);

public:
	COEWnd2(LPVOID lpParam ,OeREGISTERNOTIFY *OeRegisternotify, LPOeOnNotify OnNotify );

	
	virtual ~COEWnd2();
	HWND GetWinHanlde();

};

#endif // !defined(AFX_OEWND_H__71B942D9_5E1D_4809_A1CD_F45716CCD337__INCLUDED_)