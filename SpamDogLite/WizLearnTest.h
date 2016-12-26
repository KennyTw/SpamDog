#if !defined(AFX_WIZLEARNTEST_H__077ACE0E_E7F9_40DB_A4FB_D505130B6368__INCLUDED_)
#define AFX_WIZLEARNTEST_H__077ACE0E_E7F9_40DB_A4FB_D505130B6368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizLearnTest.h : header file
//

#include "../SwOlApi/OlApi.h"
#include "../SwOlApi/OeApi.h"

/////////////////////////////////////////////////////////////////////////////
// CWizLearnTest dialog

class CWizLearnTest : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizLearnTest)

// Construction
public:
	CWizLearnTest();
	~CWizLearnTest();

	LPMAPIFOLDER  lpSpamFolder; //spam	
	LPMAPIFOLDER  lpLearnOkFolder;
	LPMAPIFOLDER  lpLearnSpamFolder;
	LPMAPIFOLDER  lpUnknownFolder;
	LPMAPIFOLDER  lpWhiteFolder;
	LPMAPIFOLDER  lpSysMsgFolder;

	//LPMAPIFOLDER  lpTestFolder; 
  
 
	IStoreFolder *m_pUnknownBoxStoreFolder;
	IStoreFolder *m_pSpamBoxStoreFolder;
	IStoreFolder *m_pLearnOkBoxStoreFolder;
	IStoreFolder *m_pLearnSpamBoxStoreFolder;
	IStoreFolder *m_pLearnWhiteBoxStoreFolder;
	IStoreFolder *m_pSysMsgStoreFolder;
	//IStoreFolder *m_pTestFolder;

	char OutlookType;
	OlREGISTERNOTIFY olNotify;
	OeREGISTERNOTIFY oeNotify;
	CStatic	m_MainText;

 
private:
	
	void ApRelease();
	bool bLogin;
// Dialog Data
	//{{AFX_DATA(CWizLearnTest)
	enum { IDD = IDD_PROPPAGE_LEARNTEST };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizLearnTest)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizLearnTest)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnOlNotify(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZLEARNTEST_H__077ACE0E_E7F9_40DB_A4FB_D505130B6368__INCLUDED_)
