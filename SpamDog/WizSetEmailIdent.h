#if !defined(AFX_WIZSETEMAILIDENT_H__8E2ADF0C_AAB0_45C3_B1FB_6EF6FA3D1F76__INCLUDED_)
#define AFX_WIZSETEMAILIDENT_H__8E2ADF0C_AAB0_45C3_B1FB_6EF6FA3D1F76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizSetEmailIdent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizSetEmailIdent dialog

class CWizSetEmailIdent : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizSetEmailIdent)

// Construction
public:
	CWizSetEmailIdent();
	~CWizSetEmailIdent();

// Dialog Data
	//{{AFX_DATA(CWizSetEmailIdent)
	enum { IDD = IDD_PROPPAGE_SETEMAILIDENT };
	CEdit	m_EMailList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizSetEmailIdent)
	public:
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void SendIdentEmail(char* IdentMailStr);
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizSetEmailIdent)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZSETEMAILIDENT_H__8E2ADF0C_AAB0_45C3_B1FB_6EF6FA3D1F76__INCLUDED_)
