#if !defined(AFX_COLORBOX_H__0B3E1789_CD8A_4507_B332_98839D3858B8__INCLUDED_)
#define AFX_COLORBOX_H__0B3E1789_CD8A_4507_B332_98839D3858B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBox window

class CColorBox : public CStatic
{
// Construction
public:
	CColorBox();

// Attributes
public:

// Operations
public:
	void SetColor(COLORREF color);
	COLORREF GetColor();
	
private:
	COLORREF mColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    protected:	
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorBox();

	

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBOX_H__0B3E1789_CD8A_4507_B332_98839D3858B8__INCLUDED_)
