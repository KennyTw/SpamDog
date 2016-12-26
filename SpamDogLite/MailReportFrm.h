#if !defined(AFX_MAILREPORTFRM_H__13191FCB_421F_4A4C_8829_05E2419B37CD__INCLUDED_)
#define AFX_MAILREPORTFRM_H__13191FCB_421F_4A4C_8829_05E2419B37CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MailReportFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMailReportFrm dialog

typedef struct _ReportItem
{
	char FileName[MAX_PATH];
	int Type;
	bool changed;
	char SortStr[7][255]; //7 欄位數
	
} ReportItem;

typedef struct _SortItem
{
	int SortIdx;
	bool SortDir; //true 逆向, false 順向

} SortItem;



class CMailReportFrm : public CDialog
{
// Construction
public:
	CMailReportFrm(CWnd* pParent = NULL);   // standard constructor

	
	void OnOK();
	void OnCancel();
	void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult);

	//void ListMail();

    //char IniPath[_MAX_PATH];
	//char ApPath[_MAX_PATH];
	//bool AutoClose;

// Dialog Data
	//{{AFX_DATA(CMailReportFrm)
	enum { IDD = IDD_MAILREPORTFRM_DIALOG };
	//CStatic	m_Counter;
	//CButton	m_DelSpam;
	CListCtrl	m_ListMail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailReportFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	int CloseCounter;
	CImageList *m_ImageList;
	int LastSortColumn;
	bool SortDir;
	//CToolTipCtrl   _ToolTipCtrl;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMailReportFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDblclkListMail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListMail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitem32778();
	afx_msg void OnMenuitem32779();
	afx_msg void OnMenuitem32780();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILREPORTFRM_H__13191FCB_421F_4A4C_8829_05E2419B37CD__INCLUDED_)
