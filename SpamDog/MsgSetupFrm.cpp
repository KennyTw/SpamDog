// MsgSetupFrm.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "MsgSetupFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgSetupFrm dialog


CMsgSetupFrm::CMsgSetupFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgSetupFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgSetupFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMsgSetupFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgSetupFrm)
	DDX_Control(pDX, IDC_CHECK_SHOWMSG, m_ShowMsg);
	DDX_Control(pDX, IDC_COLORBOX_XX, m_ColorBoxXX);
	DDX_Control(pDX, IDC_COLORBOX_VR, m_ColorBoxVR);
	DDX_Control(pDX, IDC_COLORBOX_TEXT, m_ColorText);
	DDX_Control(pDX, IDC_COLORBOX_SPAM, m_ColorBoxSPAM);
	DDX_Control(pDX, IDC_COLORBOX_OK, m_ColorBoxOK);
	DDX_Control(pDX, IDC_STATIC_SPEEDVALUE, m_SpeedValue);
	DDX_Control(pDX, IDC_STATIC_DELAYVALUE, m_DelayValue);
	DDX_Control(pDX, IDC_CHECK_BADSHOW, m_ShowTrashMsg);
	DDX_Control(pDX, IDC_SLIDER_MSGSPEED, m_MsgSpeed);
	DDX_Control(pDX, IDC_SLIDER_MSGDELAY, m_MsgDelayTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgSetupFrm, CDialog)
	//{{AFX_MSG_MAP(CMsgSetupFrm)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MSGDELAY, OnCustomdrawSliderMsgdelay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MSGSPEED, OnCustomdrawSliderMsgspeed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgSetupFrm message handlers
void CMsgSetupFrm::GetIni()
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

	int ShowMsg = GetPrivateProfileInt("SysSetup","ShowMsg",1,IniPath);
	if (ShowMsg == 1)
	{
		m_ShowMsg.SetCheck(BST_CHECKED);
	}
	else
	{
	  	m_ShowMsg.SetCheck(BST_UNCHECKED);
	}

}

void CMsgSetupFrm::GetIniColor()
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

	  
	SysOkMailColor = GetPrivateProfileInt("SysAlertColor","SysOkMailColor",16753478,IniPath);
	SpamMailColor = GetPrivateProfileInt("SysAlertColor","SpamMailColor",32896,IniPath);
	UnknownColor = GetPrivateProfileInt("SysAlertColor","UnknownColor",12632256,IniPath);
	VirusColor = GetPrivateProfileInt("SysAlertColor","VirusColor",4194368,IniPath);
	TextColor = GetPrivateProfileInt("SysAlertColor","TextColor",0,IniPath);

 

	m_ColorBoxOK.SetColor(SysOkMailColor);
	m_ColorBoxSPAM.SetColor(SpamMailColor);
	m_ColorBoxXX.SetColor(UnknownColor);
	m_ColorBoxVR.SetColor(VirusColor);
	m_ColorText.SetColor(TextColor);


}

void CMsgSetupFrm::OnButton13() 
{
   	 CColorDialog dlg;
  // dlg.DoModal();

   if (dlg.DoModal() == IDOK)
   {
		COLORREF color = dlg.GetColor();
		m_ColorBoxOK.SetColor(color);
		SysOkMailColor = color;
	    
   
   }	
	
}

void CMsgSetupFrm::OnButton14() 
{
 CColorDialog dlg;
  // dlg.DoModal();

   if (dlg.DoModal() == IDOK)
   {
		COLORREF color = dlg.GetColor();
		m_ColorBoxSPAM.SetColor(color);
		SpamMailColor = color;
	    
   
   }		
}

void CMsgSetupFrm::OnButton15() 
{
  CColorDialog dlg;
   

   if (dlg.DoModal() == IDOK)
   {
		COLORREF color = dlg.GetColor();
		m_ColorText.SetColor(color);
		TextColor = color; 
   }		
}

void CMsgSetupFrm::OnButton5() 
{
	 CColorDialog dlg;
  // dlg.DoModal();

   if (dlg.DoModal() == IDOK)
   {
		COLORREF color = dlg.GetColor();
		m_ColorBoxXX.SetColor(color);
	    UnknownColor = color;
   
   }		
}

void CMsgSetupFrm::OnButton6() 
{
 CColorDialog dlg;
  // dlg.DoModal();

   if (dlg.DoModal() == IDOK)
   {
		COLORREF color = dlg.GetColor();
		m_ColorBoxVR.SetColor(color);
		VirusColor = color;
	    
   
   }		
}

BOOL CMsgSetupFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MsgDelayTime.SetRange(100,10000);
	m_MsgDelayTime.SetTicFreq(1000);

	m_MsgSpeed.SetRange(10,1000);
	m_MsgSpeed.SetTicFreq(10);
	
	// TODO: Add extra initialization here
	GetIniColor();
	GetIni();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgSetupFrm::OnCustomdrawSliderMsgdelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	double m_pos = m_MsgDelayTime.GetPos();

	CString tempstr;
	tempstr.Format("%.2f ¬í",m_pos / 1000);
	m_DelayValue.SetWindowText(tempstr);

	*pResult = 0;
}

void CMsgSetupFrm::OnCustomdrawSliderMsgspeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	double m_pos = m_MsgSpeed.GetPos();

	CString tempstr;
	tempstr.Format("%.2f ¬í",m_pos / 1000);
	m_SpeedValue.SetWindowText(tempstr);
	
	*pResult = 0;
}
