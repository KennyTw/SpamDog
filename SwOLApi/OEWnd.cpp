// OEWnd.cpp: implementation of the COEWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h" 
 
#include "OEWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COEWnd2::COEWnd2(LPVOID lpParam ,OeREGISTERNOTIFY *OeRegisternotify, LPOeOnNotify OnNotify )
{
 

	mlpParam = lpParam;
	//mRegType = RegType;
	mOnNotify = OnNotify;
	//mRegisterFolder = RegisterFolder;

	mOeREGISTERNOTIFY = OeRegisternotify;


	WNDCLASSEX wcex;
	memset(&wcex,0,sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);  
	wcex.lpfnWndProc	= COEWnd2::WinMsgHandler;
	wcex.lpszClassName = "COEWnd2";	
	
	RegisterClassEx(&wcex);
    
    m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,"COEWnd2", "", WS_POPUP,
       0, 0, 0, 0, NULL, NULL, NULL, (void *)this); 
	//m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,"COEWnd", "", WS_POPUP,
     //  0, 0, 0, 0, NULL, NULL, NULL, 0);
	
	//
}

COEWnd2::~COEWnd2()
{

	//CoUninitialize();
}

 

LRESULT CALLBACK COEWnd2::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	if (uMsg == WM_NCCREATE)
	{
		// get the pointer to the window from lpCreateParams which was set in CreateWindow
		SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// get the pointer to the window
	COEWnd2* pWnd = GetObjectFromWindow(hwnd);

	// if we have the pointer, go to the message handler of the window
	// else, use DefWindowProc
	if (pWnd)
	{
		
		//直接處理
		//return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
		//if (uMsg == WM_NEWMSGS || uMsg == WM_DELETEMSGS)
		if ((uMsg == WM_NEWMSGS || uMsg == WM_DELETEMSGS ||
			 uMsg == WM_MARKEDASREAD || uMsg == WM_MARKEDASUNREAD) 			
			&& (pWnd->mOeREGISTERNOTIFY->RegType & uMsg) == uMsg)
		{
		  //pWnd->m_oeapi->OnEvent(uMsg, wParam,pWnd->m_HNDno);
			OeMailObj mailobj;
			mailobj.MsgId = (MESSAGEID) wParam ;
			mailobj.MailFolder = pWnd->mOeREGISTERNOTIFY->RegisterFolder;

			
			pWnd->mOnNotify(pWnd->mlpParam,uMsg,&mailobj);

		    
		}

		/*
		if (uMsg == WM_NEWMSGS || uMsg == WM_DELETEMSGS)
		{			
			SendMessage(AfxGetMainWnd()->m_hWnd,uMsg,wParam,(long)hwnd);
		}*/	

	 

	}
	
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

COEWnd2* COEWnd2::GetObjectFromWindow(HWND hWnd)
{
	return (COEWnd2 *)GetWindowLong(hWnd, GWL_USERDATA);
	//return this;
}

HWND COEWnd2::GetWinHanlde()
{
	return m_hWnd;
}

