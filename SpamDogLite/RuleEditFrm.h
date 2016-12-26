#if !defined(AFX_RULEEDITFRM_H__C7F9175E_C810_45B9_B8F5_50CA2A2D81D7__INCLUDED_)
#define AFX_RULEEDITFRM_H__C7F9175E_C810_45B9_B8F5_50CA2A2D81D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuleEditFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRuleEditFrm dialog
#include "../Global.h"
#include "../UI/ColorBox.h"

//#ifdef _OUTLOOKEX
#include "OEAddr.h"
#include "OEapi.h"
//#endif

#include "MainFrm.h" 
#include "MapiFolder.h"

//#ifdef _OUTLOOK
#include "MapiAddr.h"
//#endif

class CRuleEditFrm : public CDialog
{
// Construction
public:
	CRuleEditFrm(CWnd* pParent = NULL);   // standard constructor
	CString m_IniSenderName;
	COLORREF m_IniColor;
	unsigned long m_FolderId;
	CString m_FolderName;
	int m_CheckTypeValue;
	int m_AlertTypeValue;
	FolderListData m_ListData;
	CString m_ShowMemo;

private:
	bool bMoveMail;
	//CMapiEx *m_mapiex;	 
	//COEapi *m_oeapi;


		 
	
	void GetIniProf(char *prof,int size);
// Dialog Data
	//{{AFX_DATA(CRuleEditFrm)
	enum { IDD = IDD_RULEEDITFRM_DIALOG };
	//CStatic	m_Memo;
	CStatic	m_StaticFolder;
	CStatic	m_Memo;
	CButton	m_AlertType;
	CEdit	m_Rule;
	CButton	m_CheckType;
	CColorBox m_ColorBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleEditFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleEditFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULEEDITFRM_H__C7F9175E_C810_45B9_B8F5_50CA2A2D81D7__INCLUDED_)
