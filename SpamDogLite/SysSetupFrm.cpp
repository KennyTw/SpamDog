// SysSetupFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "SysSetupFrm.h"
#include "MapiProf.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSetupFrm dialog


CSysSetupFrm::CSysSetupFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CSysSetupFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysSetupFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSysSetupFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSetupFrm)
	DDX_Control(pDX, IDC_COMBO2, m_ApVersion);
	DDX_Control(pDX, IDC_STATIC_UNKNOWNVALUE, m_UnknownValue);
	DDX_Control(pDX, IDC_STATIC_SPEEDVALUE, m_SpeedValue);
	DDX_Control(pDX, IDC_STATIC_DELAYVALUE, m_DelayValue);
	DDX_Control(pDX, IDC_CHECK_BADSHOW, m_ShowTrashMsg);
	DDX_Control(pDX, IDC_CHECK_BOOTSTART, m_BootStart);
	DDX_Control(pDX, IDC_CHECK_NULLCONTENT, m_NullContent);
	DDX_Control(pDX, IDC_CHECK_NULLSENDER, m_NullSender);
	DDX_Control(pDX, IDC_CHECK_NULLSUBJECT, m_NullSubject);
	DDX_Control(pDX, IDC_CHECK_SPAMADDRESS, m_SpamAddress);
	DDX_Control(pDX, IDC_CHECK_SPAMSUBJECT, m_SpamSubject);
	DDX_Control(pDX, IDC_SLIDER_UNKNOWNRATE, m_UnknownRate);
	DDX_Control(pDX, IDC_SLIDER_MSGSPEED, m_MsgSpeed);
	DDX_Control(pDX, IDC_SLIDER_MSGDELAY, m_MsgDelayTime);
	DDX_Control(pDX, IDC_COMBO1, m_ProfList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSetupFrm, CDialog)
	//{{AFX_MSG_MAP(CSysSetupFrm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MSGDELAY, OnCustomdrawSliderMsgdelay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MSGSPEED, OnCustomdrawSliderMsgspeed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_UNKNOWNRATE, OnCustomdrawSliderUnknownrate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSetupFrm message handlers

BOOL CSysSetupFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MsgDelayTime.SetRange(100,10000);
	m_MsgDelayTime.SetTicFreq(1000);

	m_MsgSpeed.SetRange(10,1000);
	m_MsgSpeed.SetTicFreq(10);

	m_UnknownRate.SetRange(1,9);
	m_UnknownRate.SetTicFreq(1);
	

	m_ProfList.Clear();
	m_ProfList.AddString("¤£³]©w");
	
	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

	if (mfrm->Settings.APVersion == AP_OUTLOOK)
	{
		CMapiProf mp;
		mp.ListProf(&m_ProfList);
	}

	m_ProfList.SetCurSel(0);

	GetIni();



  if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
  {
	  m_ProfList.EnableWindow(false);
	  m_ApVersion.SetCurSel(1);
  }
  else
  {
      m_ApVersion.SetCurSel(0);
  }
	
  
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSysSetupFrm::GetIni()
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

    char tempstr[50];
	GetPrivateProfileString("SysSetup","OutlookProf","",tempstr,50,IniPath);

	m_ProfList.SelectString(-1,tempstr);

	int ShowTrash = GetPrivateProfileInt("SysSetup","ShowTrashMsg",1,IniPath);
	if (ShowTrash == 1)
	{
		m_ShowTrashMsg.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_ShowTrashMsg.SetCheck(BST_UNCHECKED);
	}

	int MsgDelayTime = GetPrivateProfileInt("SysSetup","MsgDelayTime",3000,IniPath);
	m_MsgDelayTime.SetPos(MsgDelayTime);

	int MsgSpeed = GetPrivateProfileInt("SysSetup","MsgSpeed",200,IniPath);
	m_MsgSpeed.SetPos(MsgSpeed);

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

	int BootStart = GetPrivateProfileInt("SysSetup","BootStart",0,IniPath);
	if (BootStart == 1)
	{
		m_BootStart.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_BootStart.SetCheck(BST_UNCHECKED);
	}


	 

     
}

 

void CSysSetupFrm::OnCustomdrawSliderMsgdelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	double m_pos = m_MsgDelayTime.GetPos();

	CString tempstr;
	tempstr.Format("%.2f ¬í",m_pos / 1000);
	m_DelayValue.SetWindowText(tempstr);

	*pResult = 0;
}

void CSysSetupFrm::OnCustomdrawSliderMsgspeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	double m_pos = m_MsgSpeed.GetPos();

	CString tempstr;
	tempstr.Format("%.2f ¬í",m_pos / 1000);
	m_SpeedValue.SetWindowText(tempstr);
	
	*pResult = 0;
}

void CSysSetupFrm::OnCustomdrawSliderUnknownrate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
double m_pos = m_UnknownRate.GetPos();

	CString tempstr;
	tempstr.Format("%.1f",m_pos / 10);
	m_UnknownValue.SetWindowText(tempstr);

	*pResult = 0;
}
