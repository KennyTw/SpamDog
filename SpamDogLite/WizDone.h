#if !defined(AFX_WIZDONE_H__CD01E535_CC1F_4E90_BE51_5545EB7ABFF1__INCLUDED_)
#define AFX_WIZDONE_H__CD01E535_CC1F_4E90_BE51_5545EB7ABFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizDone.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizDone dialog

class CWizDone : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizDone)

// Construction
public:
	CWizDone();
	~CWizDone();

// Dialog Data
	//{{AFX_DATA(CWizDone)
	enum { IDD = IDD_PROPPAGE_DONE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizDone)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizDone)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZDONE_H__CD01E535_CC1F_4E90_BE51_5545EB7ABFF1__INCLUDED_)
