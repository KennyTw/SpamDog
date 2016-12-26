#if !defined(AFX_WIZSETOUTLOOK_H__3068C2F9_C618_4D7A_8486_6DDD7C852265__INCLUDED_)
#define AFX_WIZSETOUTLOOK_H__3068C2F9_C618_4D7A_8486_6DDD7C852265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizSetOutlook.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizSetOutlook dialog

class CWizSetOutlook : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizSetOutlook)

// Construction
public:
	
	
	
	CWizSetOutlook();
	~CWizSetOutlook();

// Dialog Data
	//{{AFX_DATA(CWizSetOutlook)
	enum { IDD = IDD_PROPPAGE_SETOUTLOOK };
	CComboBox	m_OlType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizSetOutlook)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizSetOutlook)
	afx_msg void OnRadio1();
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZSETOUTLOOK_H__3068C2F9_C618_4D7A_8486_6DDD7C852265__INCLUDED_)
