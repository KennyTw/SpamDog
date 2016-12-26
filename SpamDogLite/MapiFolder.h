#if !defined(AFX_MAPIFOLDER_H__B3FF9D5D_DE91_49A5_BC07_8CF631E0AB93__INCLUDED_)
#define AFX_MAPIFOLDER_H__B3FF9D5D_DE91_49A5_BC07_8CF631E0AB93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapiFolder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapiFolder dialog

//#ifdef _OUTLOOKEX
#include "msoeapi.h"
//#endif

//#ifdef _OUTLOOK
#include "MapiEx.h"
//#endif
#include "../Global.h"


class CMapiFolder : public CDialog
{
// Construction
private:
	//#ifdef _OUTLOOKEX
		void GetMailBoxFolder();	
		void ListFolder(HTREEITEM phItem , STOREFOLDERID pFolder);
	//#endif	
	
	//#ifdef _OUTLOOK
		void ListOLFolder(HTREEITEM phItem , LPMAPIFOLDER  pFolder);
	//#endif

	CImageList imglist;
	HTREEITEM root_hItem;

	FolderList m_FolderList;

public:
	CMapiFolder(CWnd* pParent = NULL);   // standard constructor
	FolderListData m_ListData;
	 

// Dialog Data
	//{{AFX_DATA(CMapiFolder)
	enum { IDD = IDD_MAPIFOLDER_DIALOG };
	CTreeCtrl	m_Tree;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapiFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapiFolder)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPIFOLDER_H__B3FF9D5D_DE91_49A5_BC07_8CF631E0AB93__INCLUDED_)
