#if !defined(AFX_WIZWAITMAILIDENT_H__F3DFCB3A_EFFC_4EE4_A11F_DC945065B368__INCLUDED_)
#define AFX_WIZWAITMAILIDENT_H__F3DFCB3A_EFFC_4EE4_A11F_DC945065B368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizWaitMailIdent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizWaitMailIdent dialog

class CWizWaitMailIdent : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizWaitMailIdent)

// Construction
public:
	CWizWaitMailIdent();
	~CWizWaitMailIdent();

// Dialog Data
	//{{AFX_DATA(CWizWaitMailIdent)
	enum { IDD = IDD_PROPPAGE_WAITMAILIDENT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizWaitMailIdent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizWaitMailIdent)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZWAITMAILIDENT_H__F3DFCB3A_EFFC_4EE4_A11F_DC945065B368__INCLUDED_)
