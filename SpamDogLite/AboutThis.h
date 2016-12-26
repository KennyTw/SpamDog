#if !defined(AFX_ABOUTTHIS_H__83B717CC_7B56_479C_92D9_9C8010DBD642__INCLUDED_)
#define AFX_ABOUTTHIS_H__83B717CC_7B56_479C_92D9_9C8010DBD642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutThis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutThis dialog

class CAboutThis : public CDialog
{
// Construction
public:
	CAboutThis(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutThis)
	enum { IDD = IDD_ABOUTTHIS_DIALOG };
	CStatic	m_Licensed;
	CStatic	m_DogId;
	CStatic	m_ApLastUpdate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutThis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutThis)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton13();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton3();
	afx_msg void OnButton16();
	afx_msg void OnButton14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTTHIS_H__83B717CC_7B56_479C_92D9_9C8010DBD642__INCLUDED_)
