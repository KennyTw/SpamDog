// WPStatic.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "WPStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWPStatic

CWPStatic::CWPStatic()
{
	
}

CWPStatic::~CWPStatic()
{
}


BEGIN_MESSAGE_MAP(CWPStatic, CStatic)
	//{{AFX_MSG_MAP(CWPStatic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_DRAWITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWPStatic message handlers


void CWPStatic::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, SS_OWNERDRAW, 0);
	CStatic::PreSubclassWindow();
}


void CWPStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	pDC->SetBkMode(TRANSPARENT);
		
	CString sTitle;
	GetWindowText(sTitle);
	int mstrlen = sTitle.GetLength();
	char *Str = sTitle.GetBuffer(mstrlen);

	CString NewStr;
	if (sTitle.IsEmpty() == FALSE)
	{
		 
		CRect rpCaption = lpDrawItemStruct->rcItem;		
	
		CSize		sz;

		

		int CurrentY = 0;
		int CurrentX = 0;
		for(int i = 0 ; i < mstrlen ; i++)
		{
		
			if ((Str[i] == char(13) && Str[i+1] == char(10)) )
			{
			
				int fitsize = 0;			
				GetTextExtentExPoint(pDC->GetSafeHdc(), Str + CurrentX,i-CurrentX,rpCaption.Width(),&fitsize,NULL,&sz);
				if (fitsize == i-CurrentX )
				{
					//pDC->TextOut(0,CurrentY,sTitle,i+1);
					//CurrentY += 5 ;
					char tmpstr[255];
					memset(tmpstr,0,255);
					strncpy(tmpstr,Str + CurrentX,i-CurrentX);
					NewStr += tmpstr ;
					NewStr += "\r\n";

					
					//OutputDebugString(tmpstr);
					//OutputDebugString("\n");
					CurrentX = i+2;
				}
				else
				{
				
					while (fitsize < i-CurrentX )
					{
						char tmpstr[255];
						memset(tmpstr,0,255);
						strncpy(tmpstr,Str + CurrentX,fitsize);
						NewStr += tmpstr ;
						NewStr += "\r\n";
						CurrentX = CurrentX + fitsize + 2;

						GetTextExtentExPoint(pDC->GetSafeHdc(), Str + CurrentX,i-CurrentX,rpCaption.Width(),&fitsize,NULL,&sz);

						if (fitsize == i-CurrentX )
						{
						
								char tmpstr[255];
								memset(tmpstr,0,255);
								strncpy(tmpstr,Str + CurrentX,i-CurrentX );
								NewStr += tmpstr ;
								NewStr += "\r\n";

								CurrentX = CurrentX + fitsize + 2;
						}


					}
					

				}
			
			} else if (i == mstrlen-1)
			{
			
				int fitsize = 0;			
				GetTextExtentExPoint(pDC->GetSafeHdc(), Str + CurrentX,i-CurrentX+1,rpCaption.Width(),&fitsize,NULL,&sz);
				if (fitsize == i-CurrentX +1)
				{
					//pDC->TextOut(0,CurrentY,sTitle,i+1);
					//CurrentY += 5 ;
					char tmpstr[255];
					memset(tmpstr,0,255);
					strncpy(tmpstr,Str + CurrentX,i-CurrentX + 1);
					NewStr += tmpstr ;
					NewStr += "\r\n";




					//OutputDebugString(tmpstr);
					//OutputDebugString("\n");
					//CurrentX = i+2;
				}
				else
				{
				
					while (fitsize < i-CurrentX +1)
					{
						char tmpstr[255];
						memset(tmpstr,0,255);
						strncpy(tmpstr,Str + CurrentX,fitsize);
						NewStr += tmpstr ;
						NewStr += "\r\n";
						CurrentX = CurrentX + fitsize;

						GetTextExtentExPoint(pDC->GetSafeHdc(), Str + CurrentX,i-CurrentX+1,rpCaption.Width(),&fitsize,NULL,&sz);

						if (fitsize == i-CurrentX +1)
						{
						
								char tmpstr[255];
								memset(tmpstr,0,255);
								strncpy(tmpstr,Str + CurrentX,i-CurrentX + 1);
								NewStr += tmpstr ;
								NewStr += "\r\n";

								//CurrentX = i+2;
						}


					}
					

				}
			}
		 

		}

		/*

		int maxsize = 0;			
		GetTextExtentExPoint(pDC->GetSafeHdc(), sTitle,sTitle.GetLength(),rpCaption.Width(),&maxsize,NULL,&sz);

		if (maxsize == sTitle.GetLength())
		{
			pDC->TextOut(0,0,sTitle);		
		}
		else
		{
			//pDC->TextOut(0,0,sTitle,maxsize);	
			pDC->DrawText((LPCTSTR)sTitle, maxsize, &rpCaption, DT_WORDBREAK | DT_LEFT  );
		}

		*/


		pDC->DrawText((LPCTSTR)NewStr, NewStr.GetLength(), &rpCaption, DT_WORDBREAK | DT_LEFT  );
	 
		
	} 

	sTitle.ReleaseBuffer();
}