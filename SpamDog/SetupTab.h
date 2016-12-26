#if !defined(AFX_SETUPTAB_H__05EC093F_C7B6_4CC1_B506_78403ADAE2F1__INCLUDED_)
#define AFX_SETUPTAB_H__05EC093F_C7B6_4CC1_B506_78403ADAE2F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupTab window

class CSetupTab : public CTabCtrl
{
// Construction
public:
	CSetupTab();

private:
	//void *m_mapiex;
	//CDialog *m_tabPages[5];
	int m_nNumberOfPages;
	int m_tabCurrent;

// Operations
public:
	CDialog *m_tabPages[6];
	void JoinTabDialog();
	void SetRectangle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSetupTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSetupTab)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPTAB_H__05EC093F_C7B6_4CC1_B506_78403ADAE2F1__INCLUDED_)
