#if !defined(AFX_REPORTFRM_H__A525A64D_ED81_4660_B583_4419659332E8__INCLUDED_)
#define AFX_REPORTFRM_H__A525A64D_ED81_4660_B583_4419659332E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportFrm.h : header file
//
/*
typedef struct _ReportItem
{
	char FileName[MAX_PATH];
	int Type;
	bool changed;
} ReportItem;*/
/////////////////////////////////////////////////////////////////////////////
// CReportFrm dialog
#include "ReportTab.h"

class CReportFrm : public CDialog
{
// Construction
public:
	CReportFrm(CWnd* pParent = NULL);   // standard constructor

	bool m_InModal;

	bool AutoClose;
	void OnOK();
	void OnCancel();
	void ListMail();

    char IniPath[_MAX_PATH];
	char ApPath[_MAX_PATH];

// Dialog Data
	//{{AFX_DATA(CReportFrm)
	enum { IDD = IDD_REPORTFRM_DIALOG };
	CStatic	m_Counter;
	CReportTab	m_ReportTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportFrm)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CloseCounter;
	CImageList *m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CReportFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTFRM_H__A525A64D_ED81_4660_B583_4419659332E8__INCLUDED_)
