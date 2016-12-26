// DBserviceDlg.h : header file
//

#if !defined(AFX_DBSERVICEDLG_H__3977C349_53F9_4518_9B52_185B88E2CD26__INCLUDED_)
#define AFX_DBSERVICEDLG_H__3977C349_53F9_4518_9B52_185B88E2CD26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../global.h"

/////////////////////////////////////////////////////////////////////////////
// CDBserviceDlg dialog

class CDBserviceDlg : public CDialog
{
// Construction
public:
	CDBserviceDlg(CWnd* pParent = NULL);	// standard constructor
	~CDBserviceDlg();

// Dialog Data
	//{{AFX_DATA(CDBserviceDlg)
	enum { IDD = IDD_DBSERVICE_DIALOG };
	CButton	m_CkDb;
	CEdit	m_ZipDesc;
	CEdit	m_ZipFile;
	CEdit	m_MailId;
	CStatic	m_STATICTIME;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBserviceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDBserviceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnDestroy();
	afx_msg void OnButton6();
	afx_msg void OnButton5();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HANDLE hConsole;    
	void MergeKeyFile();
	void ConvertReport();
	void ConvertMail();
	void GenFile(bool ReGen = false);
	void GenSysMail();
	void UnZipFileMail(char *FilePath , int orglen);
	void GetHostName();
	void GetReportMail();

	void ParseStr(StrList *m_list,char *InStr);
	void TrimCntrl(char* InStr);
	void SendFixMail(CString DogId , CString Ipid);
	void IpReserve(char *IP);
	void UnZipFile(char *FilePath,char *DestPath , int orglen);
	void WriteQueryLog(char *SQL);
};




class CReportMail
{
	#define APNIC 1
	#define ARIN 2
	#define LACNIC 3
	#define RIPE 4
	#define AFRINIC 5

	 

private:
	char IP[16];

	char IPs[4][4];
	int iIPs[4][1];

	char contact[255];
	void TrimAt(char *AStr);
	void TrimContact(char *AStr);
	void TrimResponse(char *AStr);
public:

	CReportMail();

	void SetIp(char *ip);
	int GetNic(char *ANic);
	void ParseAPNIC(char *WhoisStr);
	void ParseARIN(char *WhoisStr);
	//void ParseRIPE(char *WhoisStr);
	void ParseLACNIC(char *WhoisStr);

	char* GetContact();
	char* GetSpamCop();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBSERVICEDLG_H__3977C349_53F9_4518_9B52_185B88E2CD26__INCLUDED_)
