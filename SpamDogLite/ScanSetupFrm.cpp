// ScanSetupFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "ScanSetupFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScanSetupFrm dialog


CScanSetupFrm::CScanSetupFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CScanSetupFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanSetupFrm)
	//}}AFX_DATA_INIT
}


void CScanSetupFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanSetupFrm)
	DDX_Control(pDX, IDC_STATIC_AUTOREPORTTEXT, m_AutoShowReportText);
	DDX_Control(pDX, IDC_EDIT_AUTOSHOWREPORT, m_AutoShowReportFreq);
	DDX_Control(pDX, IDC_CHECK_AUTOSHOWREPORT, m_AutoShowReport);
	DDX_Control(pDX, IDC_CHECK_AUTODEL, m_AutoDel);
	DDX_Control(pDX, IDC_STATIC_MAXATTSIZE, m_MaxAttSizeValue);
	DDX_Control(pDX, IDC_SLIDER_MAXATTSIZE, m_MaxAttSize);
	DDX_Control(pDX, IDC_CHECK_NEEDREPORT, m_NeedReport);
	DDX_Control(pDX, IDC_CHECK_IPSTRIDENT, m_IpStrIdent);
	DDX_Control(pDX, IDC_CHECK_IPIDENT, m_IpIdent);
	DDX_Control(pDX, IDC_EDIT_RBL2, m_Rbl2);
	DDX_Control(pDX, IDC_EDIT_RBL1, m_Rbl1);
	DDX_Control(pDX, IDC_EDIT_IPKEY, m_IpKey);
	DDX_Control(pDX, IDC_EDIT_DNS, m_DNS);
	DDX_Control(pDX, IDC_CHECK_AUTHEX, m_AuthEx);
	DDX_Control(pDX, IDC_EDIT_TRACEREXPIRE, m_TracerExpire);
	DDX_Control(pDX, IDC_CHECK_MAILTRACER, m_MailTracer);
	DDX_Control(pDX, IDC_STATIC_UNKNOWNVALUE, m_UnknownValue);
	DDX_Control(pDX, IDC_CHECK_NULLSENDER, m_NullSender);
	DDX_Control(pDX, IDC_CHECK_NULLSUBJECT, m_NullSubject);
	DDX_Control(pDX, IDC_CHECK_SPAMADDRESS, m_SpamAddress);
	DDX_Control(pDX, IDC_CHECK_SPAMSUBJECT, m_SpamSubject);
	DDX_Control(pDX, IDC_SLIDER_UNKNOWNRATE, m_UnknownRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScanSetupFrm, CDialog)
	//{{AFX_MSG_MAP(CScanSetupFrm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_UNKNOWNRATE, OnCustomdrawSliderUnknownrate)
	ON_BN_CLICKED(IDC_CHECK_IPIDENT, OnIpIdentCheck)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MAXATTSIZE, OnCustomdrawSliderMaxattsize)
	ON_BN_CLICKED(IDC_CHECK_NEEDREPORT, OnCheckNeedreport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanSetupFrm message handlers



void CScanSetupFrm::OnCustomdrawSliderUnknownrate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	double m_pos = m_UnknownRate.GetPos();

	CString tempstr;
	tempstr.Format("%.1f",m_pos / 10);
	m_UnknownValue.SetWindowText(tempstr);

	*pResult = 0;
}

BOOL CScanSetupFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_UnknownRate.SetRange(1,9);
	m_UnknownRate.SetTicFreq(1);

	m_MaxAttSize.SetRange(3,1024);
	m_MaxAttSize.SetTicFreq(1);
	//m_MaxAttSize.SetPageSize(1024);

	// TODO: Add extra initialization here
	GetIni();

	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	if (mfrm->Settings.Licensed)
	{
	
		m_IpIdent.EnableWindow(true);
		if (m_IpIdent.GetCheck() == BST_CHECKED)
		{
		
			m_NeedReport.EnableWindow(true);
			m_IpStrIdent.EnableWindow(true);
			m_Rbl1.EnableWindow(true);
			m_Rbl2.EnableWindow(true);
			m_AuthEx.EnableWindow(true);
			m_IpKey.EnableWindow(true);
			
		}
		else
		{
			m_AutoDel.EnableWindow(false);
		}
 
	
	}
	else
	{
			m_AuthEx.EnableWindow(false);

			m_AuthEx.SetCheck(BST_UNCHECKED);
	
			m_IpIdent.EnableWindow(false);
			m_IpIdent.SetCheck(BST_CHECKED);
			//m_ReportWithDel.EnableWindow(false);
			//m_ReportWithDel.SetCheck(BST_CHECKED);
			//OnIpIdentCheck();

	}
	 
	

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


afx_msg void CScanSetupFrm::OnIpIdentCheck()
{
	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	
	

	if (m_IpIdent.GetCheck() != BST_CHECKED)
	{
		AfxMessageBox("取消 IP 來源辨識法 , 將會一併取消信件回報及信件來源IP判讀關鍵字等功能 !");
		m_NeedReport.SetCheck(BST_UNCHECKED);
	    m_IpStrIdent.SetCheck(BST_UNCHECKED);
		//m_ReportWithDel.SetCheck(BST_UNCHECKED);
		m_AutoDel.SetCheck(BST_UNCHECKED);

		m_IpStrIdent.EnableWindow(false);
		m_NeedReport.EnableWindow(false);
		m_IpKey.EnableWindow(false);
		//m_ReportWithDel.EnableWindow(false);
		m_AutoDel.EnableWindow(false);

		m_Rbl1.EnableWindow(false);
		m_Rbl2.EnableWindow(false);
		//m_AuthEx.EnableWindow(false);
		m_AutoShowReport.SetCheck(BST_UNCHECKED);
		m_AutoShowReport.EnableWindow(false);		
		m_AutoShowReportFreq.EnableWindow(false);		
		m_AutoShowReportText.EnableWindow(false);

	}
	else
	{
	
		

		if (mfrm->Settings.Licensed)
		{
			m_NeedReport.EnableWindow(true);
			m_IpKey.EnableWindow(true);
			m_IpStrIdent.EnableWindow(true);
			m_Rbl1.EnableWindow(true);
			m_Rbl2.EnableWindow(true);
			//m_ReportWithDel.EnableWindow(true);
			
		}
		else
		{
			//m_ReportWithDel.EnableWindow(false);
			
		}
		
		//m_ReportWithDel.SetCheck(BST_CHECKED);
		m_AutoDel.EnableWindow(true);
		
	 
		m_NeedReport.SetCheck(BST_CHECKED);
		

		m_AutoShowReport.SetCheck(BST_CHECKED);
		m_AutoShowReport.EnableWindow(true);		
		m_AutoShowReportFreq.EnableWindow(true);		
		m_AutoShowReportText.EnableWindow(true);

	
		//m_AuthEx.EnableWindow(true);

	}

}


void CScanSetupFrm::GetIni()
{
	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   char IniPath[_MAX_PATH];	   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");
    

	int UnknownRate = GetPrivateProfileInt("SysSetup","UnknownRate",5,IniPath);
	m_UnknownRate.SetPos(UnknownRate);

	int SpamAddress = GetPrivateProfileInt("SysSetup","SpamAddress",0,IniPath);
	if (SpamAddress == 1)
	{
		m_SpamAddress.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_SpamAddress.SetCheck(BST_UNCHECKED);
	}

	int SpamSubject = GetPrivateProfileInt("SysSetup","SpamSubject",1,IniPath);
	if (SpamSubject == 1)
	{
		m_SpamSubject.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_SpamSubject.SetCheck(BST_UNCHECKED);
	}

	int AllowNullSender = GetPrivateProfileInt("SysSetup","AllowNullSender",0,IniPath);
	if (AllowNullSender == 1)
	{
		m_NullSender.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_NullSender.SetCheck(BST_UNCHECKED);
	}

	int AllowNullSubject = GetPrivateProfileInt("SysSetup","AllowNullSubject",0,IniPath);
	if (AllowNullSubject == 1)
	{
		m_NullSubject.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_NullSubject.SetCheck(BST_UNCHECKED);
	}

	/*int AllowNullContent = GetPrivateProfileInt("SysSetup","AllowNullContent",0,IniPath);
	if (AllowNullContent == 1)
	{
		m_NullContent.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_NullContent.SetCheck(BST_UNCHECKED);
	}*/

	int MailTracer = GetPrivateProfileInt("SysSetup","MailTracer",0,IniPath);
	if (MailTracer == 1)
	{
		m_MailTracer.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_MailTracer.SetCheck(BST_UNCHECKED);
	}

	int AuthEx = GetPrivateProfileInt("SysSetup","AuthEx",0,IniPath);
	if (AuthEx == 1)
	{
		m_AuthEx.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_AuthEx.SetCheck(BST_UNCHECKED);
	}

	char tempstr[50];
	GetPrivateProfileString("SysSetup","TracerExpire","7",tempstr,50,IniPath);
	m_TracerExpire.SetWindowText(tempstr);

	int MaxAttSize = GetPrivateProfileInt("SysSetup","BigMailSize",1048576,IniPath);
	m_MaxAttSize.SetPos(MaxAttSize / 1024);

	//GetPrivateProfileString("SysSetup","BigMailSize","1048576",tempstr,50,IniPath);
	//m_MaxAttSize.SetWindowText(tempstr);

	GetPrivateProfileString("SysSetup","DnsIp","168.95.1.1",tempstr,50,IniPath);
	m_DNS.SetWindowText(tempstr);
	
	GetPrivateProfileString("SysSetup","RBL1","rbl.softworking.com",tempstr,50,IniPath);
	m_Rbl1.SetWindowText(tempstr);

	GetPrivateProfileString("SysSetup","RBL2","sbl-xbl.spamhaus.org",tempstr,50,IniPath);
	m_Rbl2.SetWindowText(tempstr);

	GetPrivateProfileString("SysSetup","IpKey","",tempstr,50,IniPath);
	m_IpKey.SetWindowText(tempstr);

	int IpIdent = GetPrivateProfileInt("SysSetup","IpSourceIdent",1,IniPath);
	if (IpIdent == 1)
	{		
		m_IpIdent.SetCheck(BST_CHECKED);
	}
	else
	{
	    m_IpIdent.SetCheck(BST_UNCHECKED);
	}

	int IpStrIdent = GetPrivateProfileInt("SysSetup","IpStrIdent",0,IniPath);
	if (IpStrIdent == 1)
	{		
		m_IpStrIdent.SetCheck(BST_CHECKED);
	}
	else
	{
	    m_IpStrIdent.SetCheck(BST_UNCHECKED);
	}

	
	  CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	  if (m_IpIdent.GetCheck() != BST_CHECKED)
	  {
			//AfxMessageBox("取消 IP 來源辨識法 , 將會一併取消信件回報及信件來源IP判讀關鍵字等功能 !");
			//m_NeedReport.SetCheck(BST_UNCHECKED);
			//m_IpStrIdent.SetCheck(BST_UNCHECKED);

			m_IpStrIdent.EnableWindow(false);
			m_NeedReport.EnableWindow(false);
			m_IpKey.EnableWindow(false);

			//m_AutoShowReport.SetCheck(BST_CHECKED);
			m_AutoShowReport.EnableWindow(false);		
			m_AutoShowReportFreq.EnableWindow(false);		
			m_AutoShowReportText.EnableWindow(false);

	  }
	  else
	  {
		
			if (mfrm->Settings.Licensed)
			{
				m_NeedReport.EnableWindow(true);
				m_IpKey.EnableWindow(true);
				m_IpStrIdent.EnableWindow(true);
			}
		
			m_AutoShowReport.EnableWindow(true);		
			m_AutoShowReportFreq.EnableWindow(true);		
			m_AutoShowReportText.EnableWindow(true);

			//m_NeedReport.SetCheck(BST_CHECKED);
	  }

	int NeedReport = GetPrivateProfileInt("SysSetup","NeedReport",1,IniPath);
	if (NeedReport == 1)
	{
		
		m_NeedReport.SetCheck(BST_CHECKED);

		/*int ReportWithDel = GetPrivateProfileInt("SysSetup","ReportWithDel",0,IniPath);
		if (ReportWithDel == 1)
		{		
			m_ReportWithDel.SetCheck(BST_CHECKED);
		}
		else
		{
			m_ReportWithDel.SetCheck(BST_UNCHECKED);
		}*/

		int AutoShowReport = GetPrivateProfileInt("SysSetup","AutoShowReport",1,IniPath);
		if (AutoShowReport == 1)
		{		
			m_AutoShowReport.SetCheck(BST_CHECKED);
		}
		else
		{
			m_AutoShowReport.SetCheck(BST_UNCHECKED);
		}

	}
	else
	{
	    m_NeedReport.SetCheck(BST_UNCHECKED);

		//m_ReportWithDel.EnableWindow(false);		
		//m_ReportWithDel.SetCheck(BST_UNCHECKED);

		m_AutoShowReport.SetCheck(BST_UNCHECKED);
		m_AutoShowReport.EnableWindow(false);		
		m_AutoShowReportFreq.EnableWindow(false);		
		m_AutoShowReportText.EnableWindow(false);
	}


	char AutoShowReportFreq[50];
	GetPrivateProfileString("SysSetup","AutoShowReportFreq","30",AutoShowReportFreq,50,IniPath);
	m_AutoShowReportFreq.SetWindowText(AutoShowReportFreq);


	int AutoDel = GetPrivateProfileInt("SysSetup","AutoDel",0,IniPath);
	if (AutoDel == 1)
	{
		m_AutoDel.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_AutoDel.SetCheck(BST_UNCHECKED);
	}


}

void CScanSetupFrm::OnCustomdrawSliderMaxattsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	double m_pos = m_MaxAttSize.GetPos();

	CString tempstr;
	tempstr.Format("%d", (int) m_pos );
	m_MaxAttSizeValue.SetWindowText(tempstr);

 
	*pResult = 0;
}

void CScanSetupFrm::OnCheckNeedreport() 
{
	// TODO: Add your control notification handler code here
	

	if (m_NeedReport.GetCheck() != BST_CHECKED)
	{
		//m_ReportWithDel.SetCheck(BST_UNCHECKED);
		//m_ReportWithDel.EnableWindow(false);

		m_AutoShowReport.SetCheck(BST_UNCHECKED);
		m_AutoShowReport.EnableWindow(false);		
		m_AutoShowReportFreq.EnableWindow(false);		
		m_AutoShowReportText.EnableWindow(false);


	}
	else
	{
	
		//m_ReportWithDel.EnableWindow(true);		
		//m_ReportWithDel.SetCheck(BST_CHECKED);

	
		m_AutoShowReport.SetCheck(BST_CHECKED);
		m_AutoShowReport.EnableWindow(true);		
		m_AutoShowReportFreq.EnableWindow(true);		
		m_AutoShowReportText.EnableWindow(true);
	}
	
}
