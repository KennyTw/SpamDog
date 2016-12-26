// MainFrm.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser3.h"
//}}AFX_INCLUDES
 
#if !defined(AFX_MAINFRM_H__90320B63_220F_44D5_81CF_7BF1CAB72A83__INCLUDED_)
#define AFX_MAINFRM_H__90320B63_220F_44D5_81CF_7BF1CAB72A83__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMainFrm dialog
     

#include "MsgFrm.h" 
#include "../UI/FlatButton.h"
//#include "WebFrm.h"
#include "ReportFrm.h"

#define AP_OUTLOOK   0
#define AP_OUTLOOKEX 1




//Global Class Definition ///////////////////////////////////////

     class CMapiEx; 
	 extern  CMapiEx *g_MapiEx;

	 class COEapi;
	 extern  COEapi *g_OEapi;   	

//For IpHelper /////////////////////////////////

#define MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.

	

	typedef struct {
		char String[4 * 4];
	} IP_ADDRESS_STRING, *PIP_ADDRESS_STRING, IP_MASK_STRING, *PIP_MASK_STRING;

	typedef struct _IP_ADDR_STRING {
		struct _IP_ADDR_STRING* Next;
		IP_ADDRESS_STRING IpAddress;
		IP_MASK_STRING IpMask;
		DWORD Context;
	} IP_ADDR_STRING, *PIP_ADDR_STRING;

	typedef struct _IP_ADAPTER_INFO {
		struct _IP_ADAPTER_INFO* Next;
		DWORD ComboIndex;
		char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
		char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
		UINT AddressLength;
		BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
		DWORD Index;
		UINT Type;
		UINT DhcpEnabled;
		PIP_ADDR_STRING CurrentIpAddress;
		IP_ADDR_STRING IpAddressList;
		IP_ADDR_STRING GatewayList;
		IP_ADDR_STRING DhcpServer;
		BOOL HaveWins;
		IP_ADDR_STRING PrimaryWinsServer;
		IP_ADDR_STRING SecondaryWinsServer;
		time_t LeaseObtained;
		time_t LeaseExpires;
	} IP_ADAPTER_INFO, *PIP_ADAPTER_INFO;

	//Try to access the function inside the dll
	typedef DWORD (WINAPI *lpfnGetAdaptersInfo)(
			PIP_ADAPTER_INFO pAdapterInfo,
			PULONG pOutBufLen
		);
//-----------------------------------------------------------
struct APsetup
{
	unsigned int MsgDelayTime;
	unsigned int MsgSpeed;
	bool ShowTrashMsg;
	float UnknownRate;
	bool SpamAddress;
	bool SpamSubject;
	bool AllowNullSender;
	bool AllowNullSubject;
	//bool AllowNullContent;
	bool BootStart;
	bool AutoUpdate;
	bool AutoBackup;
	long LastUpdateTime;
	long LastBackTime;
	char APVersion; 
	char ServerIp[16]; //Server Ip
	CTime LastProcessTime;
	CTime LastReportTime;
	bool MailTracer; //信件追蹤
	int TracerExpire; //追蹤失效時間
	bool BootWithAp; //起動 ap
	bool ShowMsg; // 顯示任何訊息
	char ArrangeMailFolder[255]; //郵件整理目錄
	int BigMailSize; //超大郵件大小
	bool SysAutoDel;
	int SysDelDay;
	bool AuthEx;
	bool AutoDel;
	char DnsIp[16];
	char MAC[13];
	long LicenseValidTime;//贊助到期時間
	long LicenseLastCheckTime;//贊助最後檢查時間
	char ADTitle[64];
	char ADText[256];
	char ADUrl[256];
	char LastTipType;
	bool Licensed;
	char IpKeyStr[255];
	bool IpStrIdent;
	bool IpSourceIdent;
	bool NeedReport;
	bool NoDogNews;
//	bool ReportWithDel;
	bool ShowAD;
	bool AutoShowReport;
	int AutoShowReportFreq;
	bool bRBL;
	bool ArrangeConfirm;
	char HeaderKeyStr[255];
	int HeaderKeyPos;
	bool RBLMaintain;
	bool AutoArrange;
	int ArrangeMin;	
};	 

typedef struct _TrayWnd 
{
  HWND hTrayWnd;
  HWND hWnd;
  int IconId; 
  int Index;
} TrayWnd;


typedef  std::list<std::string> StrList;

class CMainFrm : public CDialog
{
// Construction
public:
	CMainFrm(CWnd* pParent = NULL);	// standard constructor
	~CMainFrm();

	CRITICAL_SECTION m_CritSec;	
	CRITICAL_SECTION m_RangeDbSec;
	CRITICAL_SECTION m_DBSec;

	HANDLE ReporterThread;
	HANDLE SysMailThread;
	HANDLE DelSysMailThread;
	HANDLE DelTrashMailThread;
	HANDLE CheckLicenseThread;

	bool HandUpdate;
	int DogId;
	bool NeedUpdateNow;
	int ClientPid;
	bool StopCancelNewMailIcon; //停止取消 New Mail Icon

	StrList CharsetIgList;

	CMsgFrm MsgFrm;
	//CWebFrm WebFrm;	
	APsetup Settings;
	char PGPath[_MAX_PATH];	  

	void ChangeIcon(char Status);
	void ChangeTrashIcon(char Status);

	void OnOK();
	void OnCancel();
	bool ProcessExists(char *szExename , int* Pid = NULL); 
	void GetIniRule();
	void DelTraceRule(int RuleId);
	void GetClientProcessId();
	void CheckLicenseValid();
	bool CheckReportMailExist();
	void ShowToolTip(char* Title,char* Text , int Timeout, char ShowType); //0:no link 1:link
	void InitialValue();
	HRESULT GetAPTrayWnd(DWORD pid , TrayWnd* traywnd);
	afx_msg void OnCloseSpamDog(WPARAM wParam, LPARAM lParam); 
	 
// Dialog Data
	//{{AFX_DATA(CMainFrm)
	enum { IDD = IDD_SPAMDOG_DIALOG };
	CStatic	m_MainStatic;
	CFlatButton m_Trash;
	CFlatButton m_Setup;
	CFlatButton m_Close;
	CFlatButton m_ScanBox;
	CFlatButton m_LearnWiz;
	CToolTipCtrl   ToolTipCtrl;
	CWebBrowser2	m_IE;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	
// Implementation
private:
#ifdef _CONSOLEDBG
	HANDLE hConsole;    
#endif
	
	NOTIFYICONDATA tnd;
	bool bRefresh;
	LPDISPATCH SavepDisp;

#ifdef SPAMDOGFULL
	NOTIFYICONDATA tnd2;
#endif

	
	bool FirstStart;
	CReportFrm mMailReportFrm;
	

	//bool GoUpdate;

	void Getini(char statuscode=0);

	
	void CheckDBSize();
	bool GetDogId();
	void GetMacAddress(char *Mac);
	void GetIniProf(char *prof,int size);
	void ExecuteMailClient();
	void SendMailNow();
	void AddTrayIcon();
	void TurnOnLicensedFunc();
	void TurnOffLicensedFunc();
	
	HWND GetTrayNotifyWnd();	
	
	static BOOL CALLBACK FindTrayWnd(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK FindToolBarInTrayWnd(HWND hwnd, LPARAM lParam);

	LRESULT OnTaskBarCreated(WPARAM wp, LPARAM lp);
	
	//void GetSysMsg();

protected:
	HICON m_hIcon;
	HICON m_hIcon2;
	HICON m_hIcon3;
	HICON m_hIcon4;

	// Generated message map functions
	//{{AFX_MSG(CMainFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNotifyIcon(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnNotifyIcon2(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnLicenseGoAd(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnLicenseMacError(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnLicenseCheckLicensed(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnLicenseConnectError(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnLicenseLock(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnClose();
	afx_msg void OnMenuitem32774();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnCheck1();
	afx_msg void OnMenuitem32771();
	afx_msg void OnMenuitem32772();
	afx_msg void OnMenuitem32773();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMenuitem32775();
	afx_msg void OnMenuitem32776();
	afx_msg void OnMenuitem32777();
	afx_msg void OnMenuitem32781();
	afx_msg void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnBeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	afx_msg void OnDownloadCompleteExplorer1();
	afx_msg void OnMenuitem32782();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


unsigned __stdcall GetPmail(LPVOID lpParam);
unsigned __stdcall CheckLicense(LPVOID lpParam);
unsigned __stdcall WriteWhoisInfo(LPVOID lpParam);
//DWORD WINAPI DelSysMail(LPVOID lpParam);
//DWORD WINAPI DelTrashMail(LPVOID lpParam);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__90320B63_220F_44D5_81CF_7BF1CAB72A83__INCLUDED_)
