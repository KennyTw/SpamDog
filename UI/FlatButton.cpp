// FlatButton.cpp : implementation file
//

#include "stdafx.h"
#include "FlatButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 

/////////////////////////////////////////////////////////////////////////////
// CFlatButton

CFlatButton::CFlatButton()
{
	m_bMouseOnButton = false;
	m_bIsPressed = false;
	m_bneedtext = true;
	
	m_bRepaintParent = false;

	BitmapIn = NULL;
	hMask = NULL;

	mTextColor = RGB(0,0,0);
	

	mTextColor = RGB(0,0,0);
	mBorderColor = RGB(0,0,0);
	mTextBgColor = GetSysColor(COLOR_BTNFACE);
	mHoverTextColor = RGB(0,0,0);
	mHoverBgColor = GetSysColor(COLOR_BTNFACE);
	mPressTextColor =  RGB(0x66,0x66,0x66);
	mPressBgColor = GetSysColor(COLOR_BTNFACE);


	SaveIndex = 0;
 
}

CFlatButton::~CFlatButton()
{
	if (BitmapIn != NULL) DeleteObject(BitmapIn);
	if (hMask != NULL) DeleteObject(hMask);
}


BEGIN_MESSAGE_MAP(CFlatButton, CButton)
	//{{AFX_MSG_MAP(CFlatButton)
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatButton message handlers

void CFlatButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	m_bIsPressed = (lpDrawItemStruct->itemState & ODS_SELECTED);

	pDC->SetBkMode(TRANSPARENT);

	


	

	//if (m_bRepaint)
	//{
		//pDC->RestoreDC(SaveIndex);
		//SaveIndex = 0;
		//m_bRepaint = false;

	//}
	
	//if (m_brush != NULL)
	//{
		//m_brush = pDC->GetCurrentBrush();
	//}

	//pDC->FillRect(&lpDrawItemStruct->rcItem, m_brush);
	
	//pDC->GetCurrentBrush()

	
	
	COLORREF mTempText = mTextColor;
	//CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
	//CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
	//CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
	//CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black
	if (m_bIsPressed)
	{
	 
		//pDC->Draw3dRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHILIGHT));

		CBrush		brBackground3(mBorderColor);
		CBrush		brBackground2(mPressBgColor);

	 
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brBackground2 );
		pDC->FrameRect( (LPCRECT) &lpDrawItemStruct->rcItem,&brBackground3 );

		mTempText = mPressTextColor;

		//pDC->SetTextColor(RGB(255,255,255));
	 
	} else	if (m_bMouseOnButton)
	{
		//pDC->Draw3dRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));
		

		//CBrush		brBackground2(RGB(0x03,0x7D,0xDC));
		

		//CBrush		brBackground2(RGB(0,0,0));		
		//pDC->FillRect(&lpDrawItemStruct->rcItem, &brBackground2);

		//CRect rpCaption = lpDrawItemStruct->rcItem;	
		//rpCaption.DeflateRect(-1,1);	

		//if (SaveIndex == 0)
		//	SaveIndex = pDC->SaveDC();

		CBrush		brBackground3(mBorderColor);
		CBrush		brBackground2(mHoverBgColor);

	 
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brBackground2 );
		pDC->FrameRect( (LPCRECT) &lpDrawItemStruct->rcItem,&brBackground3 );


		mTempText = mHoverTextColor;
		//pDC->SetTextColor(RGB(0,0,0));
		
		//pDC->FillRect(&lpDrawItemStruct->rcItem, &brBackground2);
		//pDC->Draw3dRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHILIGHT));
		//pDC->DrawEdge(&lpDrawItemStruct->rcItem,BDR_RAISEDINNER ,0);
		//pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		//pDC->DrawFrameControl(&lpDrawItemStruct->rcItem,DFC_BUTTON ,DFCS_PUSHED);
	}

	BITMAP	csBitmapSize;
	if (BitmapIn)
	{
	 

		pDC->SetBkColor(RGB(255,255,255));
		

		HDC			hdcBmpMem	= NULL;
		HBITMAP		hbmOldBmp	= NULL;
		HDC			hdcMem		= NULL;
		HBITMAP		hbmT		= NULL;

				
		GetObject(BitmapIn, sizeof(csBitmapSize), &csBitmapSize);

		hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);
		hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, BitmapIn);
		hdcMem = CreateCompatibleDC(NULL);
		hbmT = (HBITMAP)::SelectObject(hdcMem, hMask);

		CRect	rImage;
		rImage.CopyRect(&lpDrawItemStruct->rcItem);

		if (m_bneedtext)
		{
			rImage.left += 2;  
		}
		else
		{
		   //rImage.left += csBitmapSize.bmWidth ;
		   rImage.left += ((rImage.Width() - (long) csBitmapSize.bmWidth)/2);
		}
		//rImage.left += csBitmapSize.bmWidth ;

		//rImage.left += ((rImage.Width() - (long) csBitmapSize.bmWidth)/2);
		rImage.top += ((rImage.Height() - (long) csBitmapSize.bmHeight)/2);

		BitBlt(pDC->m_hDC, rImage.left, rImage.top, csBitmapSize.bmWidth, 
            csBitmapSize.bmHeight, hdcMem, 0, 0, SRCAND);

		BitBlt(pDC->m_hDC, rImage.left, rImage.top, csBitmapSize.bmWidth, 
            csBitmapSize.bmHeight, hdcBmpMem, 0, 0, SRCPAINT);

		SelectObject(hdcMem, hbmT);
		DeleteDC(hdcMem);

		SelectObject(hdcBmpMem, hbmOldBmp);
		DeleteDC(hdcBmpMem);
	

		 
	}
	

	

	CString sTitle;
	GetWindowText(sTitle);
	if (sTitle.IsEmpty() == FALSE && m_bneedtext)
	{
		
		pDC->SetTextColor(mTempText);
		//pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));

		CRect rpCaption = lpDrawItemStruct->rcItem;		
		CRect centerRect = rpCaption;

		//求出字的長寬
		pDC->DrawText((LPCTSTR)sTitle, -1, &rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);	
		
		

		//if (BitmapIn)
		 // rpCaption.OffsetRect(csBitmapSize.bmWidth / 2,0);

		 if (BitmapIn)
		 {
		    rpCaption.OffsetRect(csBitmapSize.bmWidth  + 8 ,(centerRect.Height() - rpCaption.Height())/2);
		 }
		 else
		 {
		    //求出中心位置
			 rpCaption.OffsetRect((centerRect.Width() - rpCaption.Width())/2, (centerRect.Height() - rpCaption.Height())/2); 
		 }
		
		pDC->DrawText((LPCTSTR)sTitle, -1, &rpCaption, DT_WORDBREAK | DT_CENTER  );
	} 

	
}

void CFlatButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
	ModifyStyle(0, BS_OWNERDRAW, 0);
 
	
	CButton::PreSubclassWindow();
}

void CFlatButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	


	if (!m_bMouseOnButton)
	{
		TRACKMOUSEEVENT		csTME;

		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		_TrackMouseEvent(&csTME);

		//OutputDebugString("In\n");

		m_bMouseOnButton = true;
		Invalidate();
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CFlatButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bMouseOnButton)
	{
			m_bMouseOnButton = false;
			//m_bRepaint = true;  
			
			//Invalidate();	
			//CRect rect;
			//GetClientRect(&rect);
			//GetWindowRect(&rect);
			//GetParent()->ScreenToClient(rect);
			if (m_bRepaintParent)
			{
				GetParent()->Invalidate();
				//GetParent()->UpdateWindow();
			}
			else
			{
				Invalidate();
			}
	} 
	
	//OutputDebugString("Leave\n");
	return 0;
}

 
BOOL CFlatButton::OnClicked() 
{
	// TODO: Add your control notification handler code here
	
	return false;
}
 
void CFlatButton::SetBitMap(int MapResId)
{
	//HINSTANCE	hInstResource	= NULL;
	
	// Find correct resource handle
	//hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(MapResId), RT_BITMAP);

	//BitmapIn = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(MapResId), IMAGE_BITMAP, 0, 0, 0);

		if (BitmapIn != NULL) 
		{
			DeleteObject(BitmapIn);
			BitmapIn = NULL;
		}

		if (hMask != NULL) 
		{
			DeleteObject(hMask);
			hMask = NULL;
		}

		BitmapIn = ::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(MapResId));
		
		BITMAP	csBitmapSize;		
		GetObject(BitmapIn, sizeof(csBitmapSize), &csBitmapSize);
   
		HDC			hdcSrc		= NULL;
		HDC			hdcDest		= NULL;
		HBITMAP		hbmSrcT		= NULL;
		HBITMAP		hbmDestT	= NULL;
		COLORREF	crSaveBk;
		COLORREF	crSaveDestText;

		hMask = CreateBitmap(csBitmapSize.bmWidth, csBitmapSize.bmHeight, 1, 1, NULL);		
		//hdcDest	= CreateCompatibleDC(NULL);

		hdcSrc	= ::CreateCompatibleDC(NULL);
		hdcDest	= ::CreateCompatibleDC(NULL);

		hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, BitmapIn);
		hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);		

		crSaveBk = SetBkColor(hdcSrc, GetPixel(hdcSrc,0,0));

		BitBlt(hdcDest, 0, 0, csBitmapSize.bmWidth, csBitmapSize.bmHeight, hdcSrc, 0, 0, SRCCOPY);
		crSaveDestText = SetTextColor(hdcSrc, RGB(255, 255, 255));
		
		SetBkColor(hdcSrc,RGB(0, 0, 0));
		BitBlt(hdcSrc, 0, 0, csBitmapSize.bmWidth, csBitmapSize.bmHeight, hdcDest, 0, 0, SRCAND);

		SetTextColor(hdcDest, crSaveDestText);

		SetBkColor(hdcSrc, crSaveBk);
		SelectObject(hdcSrc, hbmSrcT);
		SelectObject(hdcDest, hbmDestT);

		DeleteDC(hdcSrc);
		DeleteDC(hdcDest);

	

		Invalidate();

		
	 
}
 
void CFlatButton::SetButtonColor(COLORREF TextColor,	
						COLORREF BorderColor,
						COLORREF TextBgColor,
						COLORREF HoverTextColor,
						COLORREF HoverBgColor,
						COLORREF PressTextColor,
						COLORREF PressBgColor)
{

	mTextColor = TextColor;
	mBorderColor = BorderColor;
	mTextBgColor = TextBgColor;
	mHoverTextColor = HoverTextColor;
	mHoverBgColor = HoverBgColor;
	mPressTextColor = PressTextColor;
	mPressBgColor = PressBgColor;


	Invalidate();
}
 
void CFlatButton::SetButtonTextColor(COLORREF TextColor,
							COLORREF HoverTextColor,
							COLORREF PressTextColor)
{
	mTextColor = TextColor;
	mHoverTextColor = HoverTextColor;
	mPressTextColor = PressTextColor;
	Invalidate();
}

void CFlatButton::SetButtonBgColor(COLORREF TextBgColor,
						  COLORREF HoverBgColor,
						  COLORREF PressBgColor)
{
	mTextBgColor = TextBgColor;
	mHoverBgColor = HoverBgColor;
	mPressBgColor = PressBgColor;


	Invalidate();
}

void CFlatButton::SetButtonBorderColor(COLORREF BorderColor)
{
	mBorderColor = BorderColor;
	Invalidate();
}
 
void CFlatButton::SetNeedText(bool needtext)
{
   m_bneedtext = needtext;
   Invalidate();

}

void CFlatButton::SetRepaintParent(bool repaint)
{
    m_bRepaintParent = repaint;
}
