// ColorBox.cpp : implementation file
//

#include "stdafx.h" 
#include "ColorBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBox

CColorBox::CColorBox()
{
	mColor = GetSysColor(COLOR_BTNFACE);
}

CColorBox::~CColorBox()
{
}


BEGIN_MESSAGE_MAP(CColorBox, CStatic)
	//{{AFX_MSG_MAP(CColorBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_DRAWITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBox message handlers

void CColorBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CBrush		brBackground(mColor);
	pDC->FillRect(&lpDrawItemStruct->rcItem, &brBackground);


	CBrush		brBackground3(RGB(0,0,0)); 	
	pDC->FrameRect( (LPCRECT) &lpDrawItemStruct->rcItem,&brBackground3 );

}

void CColorBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_OWNERDRAW, 0);
	
}

void CColorBox::SetColor(COLORREF color)
{

	mColor = color;
	Invalidate();

}

COLORREF CColorBox::GetColor()
{

	return mColor;
}
