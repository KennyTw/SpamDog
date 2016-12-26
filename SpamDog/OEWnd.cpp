// OEWnd.cpp: implementation of the COEWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SpamDog.h"
#include "OEWnd.h"
#include "OEapi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COEWnd::COEWnd(COEapi* oeapi,int HNDno)
{
	m_oeapi = oeapi;
	m_HNDno = HNDno;

	WNDCLASSEX wcex;
	memset(&wcex,0,sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);  
	wcex.lpfnWndProc	= COEWnd::WinMsgHandler;
	wcex.lpszClassName = "COEWnd";	
	RegisterClassEx(&wcex);
    
    m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,"COEWnd", "", WS_POPUP,
       0, 0, 0, 0, NULL, NULL, NULL, (void *)this); 
	//m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,"COEWnd", "", WS_POPUP,
     //  0, 0, 0, 0, NULL, NULL, NULL, 0); 	
}

COEWnd::~COEWnd()
{

}

 

LRESULT CALLBACK COEWnd::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	if (uMsg == WM_NCCREATE)
	{
		// get the pointer to the window from lpCreateParams which was set in CreateWindow
		SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// get the pointer to the window
	COEWnd* pWnd = GetObjectFromWindow(hwnd);

	// if we have the pointer, go to the message handler of the window
	// else, use DefWindowProc
	if (pWnd)
	{
		//直接處理
		//return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
		//if (uMsg == WM_NEWMSGS || uMsg == WM_DELETEMSGS)
		if (uMsg == WM_NEWMSGS)
		{
		  pWnd->m_oeapi->OnEvent(uMsg, wParam,pWnd->m_HNDno);
		}

		/*
		if (uMsg == WM_NEWMSGS || uMsg == WM_DELETEMSGS)
		{			
			SendMessage(AfxGetMainWnd()->m_hWnd,uMsg,wParam,(long)hwnd);
		}*/	

	 

	}
	
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

COEWnd* COEWnd::GetObjectFromWindow(HWND hWnd)
{
	return (COEWnd *)GetWindowLong(hWnd, GWL_USERDATA);
	//return this;
}

HWND COEWnd::GetWinHanlde()
{
	return m_hWnd;
}

