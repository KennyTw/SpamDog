#if !defined(AFX_REPORTTAB_H__BBA48DC4_BB4C_4ACE_83E2_5CABDB4730D1__INCLUDED_)
#define AFX_REPORTTAB_H__BBA48DC4_BB4C_4ACE_83E2_5CABDB4730D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportTab window

class CReportTab : public CTabCtrl
{
// Construction
public:
	CReportTab();

// Attributes
private:
	//void *m_mapiex;
	//CDialog *m_tabPages[5];

	int m_tabCurrent;

// Operations
public:
	int m_nNumberOfPages;

	CDialog *m_tabPages[2];
	void JoinTabDialog();
	void SetRectangle();
	void SetActivePage(int nindex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReportTab)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTTAB_H__BBA48DC4_BB4C_4ACE_83E2_5CABDB4730D1__INCLUDED_)
