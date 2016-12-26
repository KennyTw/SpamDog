#if !defined(AFX_WIZGETDOGID_H__60D3F7DE_AC1D_4831_BAB7_C9AB588F2357__INCLUDED_)
#define AFX_WIZGETDOGID_H__60D3F7DE_AC1D_4831_BAB7_C9AB588F2357__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizGetDogId.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizGetDogId dialog

class CWizGetDogId : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizGetDogId)

// Construction
public:
	CWizGetDogId();
	~CWizGetDogId();

// Dialog Data
	//{{AFX_DATA(CWizGetDogId)
	enum { IDD = IDD_PROPPAGE_GETDOGID };
	CStatic	m_MainText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizGetDogId)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizGetDogId)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZGETDOGID_H__60D3F7DE_AC1D_4831_BAB7_C9AB588F2357__INCLUDED_)
