// UpgradeDlg.h : header file
//

#if !defined(AFX_UPGRADEDLG_H__481B2AF5_45B5_4966_8F85_17EFD36B7914__INCLUDED_)
#define AFX_UPGRADEDLG_H__481B2AF5_45B5_4966_8F85_17EFD36B7914__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog
#include "../Global.h"
#include "../Swspam/SpamDB.h"


typedef struct _OldBucket
{

	struct _IndexData IndexData[4];
	char Status; //0 : Used , 1 : delete
	int ChainData; // -1 : 下面沒有 overflow data

} OldBucket;

typedef struct _DataFileOld
{
	
	//char Data[100];//真實資料 , Data 結構的必要成員
	int OkRate;
	int BadRate;
	unsigned int UsedCount;
    //unsigned int SpamCount; //new
	char Status; //0: Free ,1 : used , 2 : Ip Data , 3 : Public GateWay , 4 : ENews

} DataFileOld;


class CUpgradeDlg : public CDialog
{
// Construction
public:
	CUpgradeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUpgradeDlg)
	enum { IDD = IDD_UPGRADE_DIALOG };
	CProgressCtrl	m_progress1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUpgradeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADEDLG_H__481B2AF5_45B5_4966_8F85_17EFD36B7914__INCLUDED_)
