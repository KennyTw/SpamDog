//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_WEBFRM_H__B7019621_5485_4775_A99E_9777C1175AC5__INCLUDED_)
#define AFX_WEBFRM_H__B7019621_5485_4775_A99E_9777C1175AC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWebFrm dialog

class CWebFrm : public CDialog
{
// Construction
public:
	CWebFrm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWebFrm)
	enum { IDD = IDD_WEBFRM_DIALOG };
	CWebBrowser2	m_Web;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWebFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBFRM_H__B7019621_5485_4775_A99E_9777C1175AC5__INCLUDED_)
