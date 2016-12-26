// RuleEditFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDog.h"
#include "RuleEditFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuleEditFrm dialog


CRuleEditFrm::CRuleEditFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleEditFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleEditFrm)
	//}}AFX_DATA_INIT

	m_IniColor = 0;
	bMoveMail = false;
 
	 
	memset(&m_ListData,0,sizeof(FolderListData));
	m_FolderId = 0;
    m_CheckTypeValue = 1;
	m_AlertTypeValue = 1;
}


void CRuleEditFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleEditFrm)
	DDX_Control(pDX, IDC_STATIC_MEMO, m_Memo);
	DDX_Control(pDX, IDC_STATICFOLDER, m_StaticFolder);
	DDX_Control(pDX, IDC_RADIO3, m_AlertType);
	DDX_Control(pDX, IDC_EDIT2, m_Rule);
	DDX_Control(pDX, IDC_RADIO1, m_CheckType);
	DDX_Control(pDX, IDC_COLORBOX_SENDER, m_ColorBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuleEditFrm, CDialog)
	//{{AFX_MSG_MAP(CRuleEditFrm)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleEditFrm message handlers
void CRuleEditFrm::GetIniProf(char *prof,int size)
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

	   
	   GetPrivateProfileString("SysSetup","OutlookProf","",prof,size,IniPath);
}

BOOL CRuleEditFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Rule.SetWindowText(m_IniSenderName);
	m_ColorBox.SetColor(m_IniColor);
	m_Memo.SetWindowText(m_ShowMemo);

	//int ss = m_mapiex->m_FolderList.size();

	if (m_ListData.FolderName[0] != 0)
	{
		m_StaticFolder.SetWindowText(m_ListData.FolderName);
		bMoveMail = true;
	}

	
	/*
	#ifdef _OUTLOOK

	


	FolderList::iterator itor; 
	FolderList::iterator enditor = g_MapiEx->m_FolderList.end();


	FolderListData tmpListData;
	
	m_FolderCombo.SetCurSel(0);
	for (itor =  g_MapiEx->m_FolderList.begin(); itor != enditor ; itor++)
	{
			tmpListData = *itor;

			int itemidx = m_FolderCombo.AddString(tmpListData.FolderName);
			//m_FolderCombo.SetItemData(itemidx,tmpListData.FolderId);

			if (m_FolderName.Find(tmpListData.FolderName) == 0)
			{
				m_FolderCombo.SetCurSel(itemidx);
			}


	}

	#endif

	#ifdef _OUTLOOKEX
	

	
	
	FolderList::iterator itor; 
	FolderList::iterator enditor = g_OEapi->m_FolderList.end();

	
	FolderListData tmpListData;
	
	m_FolderCombo.SetCurSel(0);
	for (itor =  g_OEapi->m_FolderList.begin(); itor != enditor ; itor++)
	{
			tmpListData = *itor;

			int itemidx = m_FolderCombo.AddString(tmpListData.FolderName);
			//m_FolderCombo.SetItemData(itemidx,tmpListData.FolderId);

			if (m_FolderName.Find(tmpListData.FolderName) == 0)
			{
				m_FolderCombo.SetCurSel(itemidx);
			}


	}

	#endif*/

	if (m_CheckTypeValue == 1)
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);   
	}
	else
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);   
	}

	if (m_AlertTypeValue == 1)
	{
		CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);   
	}
	else
	{
		CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO4);   
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRuleEditFrm::OnButton1() 
{
	CString Sender = "";

  CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

  if (mfrm->Settings.APVersion == AP_OUTLOOK)
  {
	
	CMapiAddr  mapiaddr;

	char profstr[50];
    GetIniProf(profstr,sizeof(profstr));
	mapiaddr.Login(profstr);

	if (mapiaddr.OpenAddrBook(this->m_hWnd,&Sender))
	{
		m_Rule.SetWindowText(Sender);
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);   

	}

  } 
  else if (mfrm->Settings.APVersion == AP_OUTLOOKEX)
  {
	 COEAddr   mapiaddr;
	 if (mapiaddr.OpenAddrBook(this->m_hWnd,&Sender))
	 {
		m_Rule.SetWindowText(Sender);
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);   

	 }
  }




	
}

void CRuleEditFrm::OnButton4() 
{
	CColorDialog dlg;
  

	 if (dlg.DoModal() == IDOK)
	 {
		COLORREF color = dlg.GetColor();
		m_ColorBox.SetColor(color);
	    
   
	 }	
}

void CRuleEditFrm::OnButton5() 
{

	CMapiFolder mafolder;
	if (mafolder.DoModal() == 1)
	{
		m_ListData = mafolder.m_ListData;
		if (m_ListData.FolderName[0] != 0)
		{
			bMoveMail =  true;
			m_StaticFolder.SetWindowText(m_ListData.FolderName);
		}
		else
		{
			bMoveMail = false;
			m_StaticFolder.SetWindowText("不移動");
		}
		//AfxMessageBox(m_ListData.FolderName);
	}
}

void CRuleEditFrm::OnButton2() 
{
	CString Sender = "";
	m_Rule.GetWindowText(Sender);
	Sender.TrimLeft();
	Sender.TrimRight();

	if (Sender.GetLength() == 0)
	{
		MessageBox("請輸入條件!");
		m_Rule.SetFocus();
		return;
	}

	//if (Sender.Find("[") != -1 || Sender.Find("]") != -1)
	//{
	//	MessageBox("請勿使用 [ 或 ] 符號!");
//		return;
//	}

	Sender.TrimLeft();
	Sender.TrimRight();

	m_IniSenderName = Sender;
	m_IniColor = m_ColorBox.GetColor();
	
	/*
	if (m_FolderCombo.GetCurSel()  >= 0)
	{
		//m_FolderId = m_FolderCombo.GetItemData(m_FolderCombo.GetCurSel());
		if (m_FolderCombo.GetCurSel() == 0)
		{
			m_FolderName = "";
		}
		else
		{
		    m_FolderCombo.GetLBText(m_FolderCombo.GetCurSel(),m_FolderName);
		}
	}

	*/
	if (bMoveMail)
	{
		m_StaticFolder.GetWindowText(m_FolderName);
	}
	else
	{
		m_FolderName = "";
	}
	

	m_CheckTypeValue = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1 ? 1 : 2 ;
	m_AlertTypeValue = GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3 ? 1 : 2 ;

	EndDialog(1);	
}

void CRuleEditFrm::OnButton3() 
{
  EndDialog(0);	
}
