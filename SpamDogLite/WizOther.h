#if !defined(AFX_WIZOTHER_H__D05C52AE_2C2A_4297_A838_AF051C153D0E__INCLUDED_)
#define AFX_WIZOTHER_H__D05C52AE_2C2A_4297_A838_AF051C153D0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizOther.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizOther dialog

class CWizOther : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizOther)

// Construction
public:
	CWizOther();
	~CWizOther();

private:
	CFont m_font;

// Dialog Data
	//{{AFX_DATA(CWizOther)
	enum { IDD = IDD_PROPPAGE_OTHER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizOther)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizOther)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZOTHER_H__D05C52AE_2C2A_4297_A838_AF051C153D0E__INCLUDED_)
