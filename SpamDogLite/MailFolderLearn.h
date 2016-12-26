#if !defined(AFX_MAILFOLDERLEARN_H__20ECFAB2_CD9F_448C_8F3D_4B05AFB5A84D__INCLUDED_)
#define AFX_MAILFOLDERLEARN_H__20ECFAB2_CD9F_448C_8F3D_4B05AFB5A84D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MailFolderLearn.h : header file
//
#include "msoeapi.h"
#include "MapiEx.h"
#include "../Global.h"

/////////////////////////////////////////////////////////////////////////////
// CMailFolderLearn dialog

class CMailFolderLearn : public CPropertyPage
{
	DECLARE_DYNCREATE(CMailFolderLearn)

private:
	void GetMailBoxFolder();	
	void ListFolder(HTREEITEM phItem , STOREFOLDERID pFolder);	
	void ListOLFolder(HTREEITEM phItem , LPMAPIFOLDER  pFolder);
	//void LearnMail(char Howto); //1 -> spam , 2 -> ok

	CImageList imglist;
	HTREEITEM root_hItem;

	FolderList m_FolderList;
	FolderListData m_ListData;

// Construction
public:
	CMailFolderLearn();
	~CMailFolderLearn();

// Dialog Data
	//{{AFX_DATA(CMailFolderLearn)
	enum { IDD = IDD_WIZ_FOLDERLEARN };
	CProgressCtrl	m_progress1;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMailFolderLearn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMailFolderLearn)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILFOLDERLEARN_H__20ECFAB2_CD9F_448C_8F3D_4B05AFB5A84D__INCLUDED_)
