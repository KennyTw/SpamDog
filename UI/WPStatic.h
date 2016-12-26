#if !defined(AFX_WPSTATIC_H__76D70B89_ADE0_459A_87ED_F22DB7ECFA2A__INCLUDED_)
#define AFX_WPSTATIC_H__76D70B89_ADE0_459A_87ED_F22DB7ECFA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WPStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWPStatic window

class CWPStatic : public CStatic
{
// Construction
public:
	CWPStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWPStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWPStatic();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// Generated message map functions
protected:
	//{{AFX_MSG(CWPStatic)
		// NOTE - the ClassWizard will add and remove member functions here.
		
	
	//}}AFX_MSG

	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WPSTATIC_H__76D70B89_ADE0_459A_87ED_F22DB7ECFA2A__INCLUDED_)
