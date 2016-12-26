#if !defined(AFX_SETUPFRM_H__A1B761ED_B617_4DBB_A97B_CB17971BF705__INCLUDED_)
#define AFX_SETUPFRM_H__A1B761ED_B617_4DBB_A97B_CB17971BF705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupFrm.h : header file
//
#include "SetupTab.h"
/////////////////////////////////////////////////////////////////////////////
// CSetupFrm dialog

class CSetupFrm : public CDialog
{
// Construction
public:
	CSetupFrm(CWnd* pParent = NULL);   // standard constructor
	COLORREF SysOkMailColor,SpamMailColor,UnknownColor,VirusColor,TextColor; 
	CString m_SenderSearchStr;
private:
	void SaveColorIni();

// Dialog Data
	//{{AFX_DATA(CSetupFrm)
	enum { IDD = IDD_SETUPFRM_DIALOG };
	CSetupTab	m_setuptab;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupFrm)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPFRM_H__A1B761ED_B617_4DBB_A97B_CB17971BF705__INCLUDED_)
