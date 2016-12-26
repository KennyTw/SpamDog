#if !defined(AFX_RULESETUPFRM_H__261C60AB_0A0B_4E54_BB27_7A8C97B1E8F1__INCLUDED_)
#define AFX_RULESETUPFRM_H__261C60AB_0A0B_4E54_BB27_7A8C97B1E8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuleSetupFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRuleSetupFrm dialog


class CRuleSetupFrm : public CDialog
{
// Construction
public:
	CRuleSetupFrm(CWnd* pParent = NULL);   // standard constructor

	COLORREF SysOkMailColor,SpamMailColor,UnknownColor,VirusColor,TextColor; 
	void ShowAddrDialog();
	//void SetMapiEx(void *mapiex);

	CListCtrl	m_RuleList;


private:
	//void *m_mapiex;
	
	CImageList *m_ImageList;
 
	
	void GetIniRule();
	//void ParseIniStr(char* IniStr, int size);

// Dialog Data
	//{{AFX_DATA(CRuleSetupFrm)
	enum { IDD = IDD_RULESETUPFRM_DIALOG };
	//CColorBox m_ColorBoxPrev;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleSetupFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleSetupFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULESETUPFRM_H__261C60AB_0A0B_4E54_BB27_7A8C97B1E8F1__INCLUDED_)
