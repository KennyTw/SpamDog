// SetupFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDog.h"
#include "SetupFrm.h"
#include "ScanSetupFrm.h"
#include "OtherSetupFrm.h"

#include "RuleSetupFrm.h"
#include "../Swparser/MailParser.h"
#include "../Global.h"
#include "MsgSetupFrm.h"
#include "LiveUpdate.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupFrm dialog


CSetupFrm::CSetupFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetupFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupFrm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TAB1, m_setuptab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupFrm, CDialog)
	//{{AFX_MSG_MAP(CSetupFrm)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupFrm message handlers

void CSetupFrm::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	SaveColorIni();

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
	   char RulePath[_MAX_PATH];	   

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");

	   strcpy(RulePath,drive);
	   strcat(RulePath,dir);
	   
	   //#ifdef _OUTLOOK
	   strcat(RulePath,"Rule.dat");
	   //#endif

	   //#ifdef _OUTLOOKEX
	   //strcat(RulePath,"RuleOE.dat");
	   //#endif
	   

	    WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		bool FoundFile=false;

		RuleHeader rulehead;
		memset(&rulehead,0,sizeof(RuleHeader));

		hFind = FindFirstFile(RulePath,&FindFileData);
 		if (hFind != INVALID_HANDLE_VALUE) 
		{
			FoundFile = true;

			FILE *fp2 = fopen(RulePath,"rb");
			fread(&rulehead,1,sizeof(rulehead),fp2);
			fclose(fp2);

		}

		FindClose(hFind);


	    FILE *fp = fopen(RulePath,"w+b");
		fwrite(&rulehead,1,sizeof(rulehead),fp);   		
		

	CRuleSetupFrm *rulefrm;
	rulefrm = (CRuleSetupFrm *) m_setuptab.m_tabPages[0];

	//CString iniSaveStr;
	int mStrLen = 0;

	for (int i = 0 ; i < rulefrm->m_RuleList.GetItemCount() ; i++)
	{ 
		

		//save to file
		RuleFile *m_RuleFile = (RuleFile *) rulefrm->m_RuleList.GetItemData(i);
		m_RuleFile->Used = true;
		fwrite(m_RuleFile,1,sizeof(RuleFile),fp);

	
	}


	fclose(fp);

	int Apcuridx = -1;

    //CSysSetupFrm
    CMsgSetupFrm *sysfrm;
	sysfrm = (CMsgSetupFrm *) m_setuptab.m_tabPages[1];


	if (sysfrm->m_ShowTrashMsg.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","ShowTrashMsg",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","ShowTrashMsg",tmpstr,IniPath);
	
	}

	if (sysfrm->m_ShowMsg.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","ShowMsg",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","ShowMsg",tmpstr,IniPath);
	
	}




	
	 
	CString tmpstr;
	tmpstr.Format("%d",sysfrm->m_MsgDelayTime.GetPos());
	WritePrivateProfileString("SysSetup","MsgDelayTime",tmpstr,IniPath);

	tmpstr.Format("%d",sysfrm->m_MsgSpeed.GetPos());
	WritePrivateProfileString("SysSetup","MsgSpeed",tmpstr,IniPath);



	//CScanSetupFrm
	CScanSetupFrm *scanfrm;
	scanfrm = (CScanSetupFrm *) m_setuptab.m_tabPages[2];

	/*if (scanfrm->m_NullContent.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AllowNullContent",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AllowNullContent",tmpstr,IniPath);
	
	}*/

	if (scanfrm->m_NullSender.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AllowNullSender",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AllowNullSender",tmpstr,IniPath);
	
	}

	if (scanfrm->m_NullSubject.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AllowNullSubject",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AllowNullSubject",tmpstr,IniPath);
	
	}

	if (scanfrm->m_SpamAddress.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","SpamAddress",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","SpamAddress",tmpstr,IniPath);
	
	}

	if (scanfrm->m_SpamSubject.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","SpamSubject",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","SpamSubject",tmpstr,IniPath);
	
	}

	tmpstr.Format("%d",scanfrm->m_UnknownRate.GetPos());
	WritePrivateProfileString("SysSetup","UnknownRate",tmpstr,IniPath);

		
	tmpstr.Format("%d",scanfrm->m_MaxAttSize.GetPos()*1024);
	WritePrivateProfileString("SysSetup","BigMailSize",tmpstr,IniPath);

	if (scanfrm->m_MailTracer.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","MailTracer",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","MailTracer",tmpstr,IniPath);
	
	}



	bool TracerNum=true;
	CString TempCStr;
	scanfrm->m_TracerExpire.GetWindowText(TempCStr);
	TempCStr.TrimLeft();
	TempCStr.TrimRight();
	//檢查是否是數字
	//bool TrueDigit=true;
	for (int j=0 ; j < TempCStr.GetLength() ; j++)
	{
		if (!isdigit(TempCStr[j]))
		{			
			TracerNum = false;
			break;
		}
			
	}

	if (TracerNum)
	{
		WritePrivateProfileString("SysSetup","TracerExpire",TempCStr.GetBuffer(TempCStr.GetLength()),IniPath);
		TempCStr.ReleaseBuffer();

	}


	/*bool BigMailNum=true;

	

	CString TempCStr2;
	scanfrm->m_BigMail.GetWindowText(TempCStr2);
	TempCStr2.TrimLeft();
	TempCStr2.TrimRight();
	//檢查是否是數字
	//TrueDigit=true;
	for (j=0 ; j < TempCStr2.GetLength() ; j++)
	{
		if (!isdigit(TempCStr2[j]))
		{
			BigMailNum = false;
			break;
		}
			
	}*/

	CString TempCStr31;
	scanfrm->m_DNS.GetWindowText(TempCStr31);
	TempCStr31.TrimLeft();
	TempCStr31.TrimRight();

	if (TempCStr31.GetLength() > 0)
	WritePrivateProfileString("SysSetup","DnsIp",TempCStr31,IniPath);

	if (scanfrm->m_AuthEx.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AuthEx",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AuthEx",tmpstr,IniPath);
	
	}


	CString TempCStr32;
	scanfrm->m_Rbl1.GetWindowText(TempCStr32);
	TempCStr32.TrimLeft();
	TempCStr32.TrimRight();

	

	CString TempCStr33;
	scanfrm->m_Rbl2.GetWindowText(TempCStr33);
	TempCStr33.TrimLeft();
	TempCStr33.TrimRight();

	if (TempCStr32.GetLength() > 0 || (TempCStr32.GetLength() > 0 && TempCStr33.GetLength() > 0) ||  (TempCStr32.GetLength() == 0 && TempCStr33.GetLength() == 0))
	{
			WritePrivateProfileString("SysSetup","RBL1",TempCStr32,IniPath);
			WritePrivateProfileString("SysSetup","RBL2",TempCStr33,IniPath);
	}




	CString TempCStr34;
	scanfrm->m_IpKey.GetWindowText(TempCStr34);
	TempCStr34.TrimLeft();
	TempCStr34.TrimRight(); 

	if (scanfrm->m_IpStrIdent.GetCheck() == BST_CHECKED && TempCStr34.GetLength() > 0)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","IpStrIdent",tmpstr,IniPath);
		WritePrivateProfileString("SysSetup","IpKey",TempCStr34,IniPath);

	
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","IpStrIdent",tmpstr,IniPath);
		WritePrivateProfileString("SysSetup","IpKey",TempCStr34,IniPath);
	
	}





	if (scanfrm->m_NeedReport.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","NeedReport",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","NeedReport",tmpstr,IniPath);
	
	}

	if (scanfrm->m_IpIdent.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","IpSourceIdent",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","IpSourceIdent",tmpstr,IniPath);
	
	}

	/*if (scanfrm->m_ReportWithDel.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","ReportWithDel",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","ReportWithDel",tmpstr,IniPath);
	
	}*/

	if (scanfrm->m_AutoDel.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AutoDel",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AutoDel",tmpstr,IniPath);
	
	}

	if (scanfrm->m_AutoShowReport.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AutoShowReport",tmpstr,IniPath);

		bool TracerNum = true;

		CString TempCStr;
		scanfrm->m_AutoShowReportFreq.GetWindowText(TempCStr);
		TempCStr.TrimLeft();
		TempCStr.TrimRight();

		for (int j=0 ; j < TempCStr.GetLength() ; j++)
		{
			if (!isdigit(TempCStr[j]))
			{			
				TracerNum = false;
				break;
			}
			
		}

		if (!TracerNum)
		{
		
			AfxMessageBox("回報顯示時間需輸入數字!");
			return;
		}
		else
		{
			WritePrivateProfileString("SysSetup","AutoShowReportFreq",TempCStr,IniPath);
		}
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AutoShowReport",tmpstr,IniPath);
	
	}

	
	 

	





	 //CLiveUpdate
    //CLiveUpdate *lufrm;
	//lufrm = (CLiveUpdate *) m_setuptab.m_tabPages[3];

	/*if (lufrm->m_AutoUpdate.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AutoUpdate",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AutoUpdate",tmpstr,IniPath);
	
	}

	if (lufrm->m_AutoBackup.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","AutoBackup",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","AutoBackup",tmpstr,IniPath);
	
	}*/

	//COtherSetupFrm
	COtherSetupFrm *otherfrm;
	otherfrm = (COtherSetupFrm *) m_setuptab.m_tabPages[4];
	//USED

	int oldvalue = GetPrivateProfileInt("SysSetup","BootStart",0,IniPath);
	
	if (otherfrm->m_BootStart.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","BootStart",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","BootStart",tmpstr,IniPath);

		//on => off
		if (oldvalue == 1)
		{
				//如果有則刪除
			HKEY RegKey;
			RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);		
	
			DWORD m_dwCount;
			RegQueryValueEx(RegKey,"SpamDog",0,NULL,NULL,&m_dwCount);
			if (m_dwCount > 0)
			{		
				//RegSetValueEx(RegKey,"SpamDog",0,REG_SZ,(const unsigned char *)path_buffer,strlen(path_buffer));
				RegDeleteValue(RegKey,"SpamDog");

			}
        
			RegCloseKey(RegKey);
		
		}
	
	}

	if (otherfrm->m_StartWithAP.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","BootWithAp",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","BootWithAp",tmpstr,IniPath);
	
	}



	if (otherfrm->m_SysAutoDel.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","SysAutoDel",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","SysAutoDel",tmpstr,IniPath);
	
	}

	/*if (otherfrm->m_NoDogNews.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","NoDogNews",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","NoDogNews",tmpstr,IniPath);
	
	}*/


	CString TempCStr3;
	otherfrm->m_ArrangeFolder.GetWindowText(TempCStr3);
	if (TempCStr3.GetLength() > 0)
	WritePrivateProfileString("SysSetup","ArrangeMailFolder",TempCStr3,IniPath);



	 
	int curidx = otherfrm->m_ProfList.GetCurSel();
	if ( curidx > 0)
	{
		CString combotext;	
		otherfrm->m_ProfList.GetLBText(curidx,combotext);	 
		WritePrivateProfileString("SysSetup","OutlookProf",combotext,IniPath);			 
	}


	Apcuridx = otherfrm->m_ApVersion.GetCurSel() ;
	 
	char idxstr[2];
	idxstr[1] = 0;
	itoa(Apcuridx,idxstr,10);
	
	SetDefaultMailClient(Apcuridx+1);
	WritePrivateProfileString("SysSetup","ApVersion",idxstr,IniPath);
	 



	CString TempCStr4;
	otherfrm->m_SysDelDay.GetWindowText(TempCStr4);
	bool SysDelDayNum = true;
	int  DelDay = 0;
	if (TempCStr4.GetLength() > 0)
	{
	
		for (j=0 ; j < TempCStr4.GetLength() ; j++)
		{
			if (!isdigit(TempCStr4[j]))
			{
				SysDelDayNum = false;
				break;
			}
			
		}

		if (SysDelDayNum)
		{
		
			DelDay = atoi(TempCStr4.GetBuffer(TempCStr4.GetLength()));
		}

		if (SysDelDayNum  && DelDay > 0 )
		WritePrivateProfileString("SysSetup","SysDelDay",TempCStr4,IniPath);
	}


	if (TracerNum && SysDelDayNum)
	{
		if (DelDay <= 0)
		{
		
			AfxMessageBox("自動刪除系統信件天數需大於 0 日");
			return;
		}

		CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();
		if (mfrm->Settings.APVersion != Apcuridx)
		{	
			EndDialog(3);
		}
		else
		{
			EndDialog(1);
		}	
	}
	else
	{

		if (!TracerNum) 
		{
			AfxMessageBox("郵件追蹤器有效時間需輸入數字及整數");
			return;
		}

	
	
		if (!SysDelDayNum) 
		{
			AfxMessageBox("自動刪除系統信件天數 , 需輸入數字及整數");
			return;
		}

		
	}
	
}

void CSetupFrm::OnButton2() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

BOOL CSetupFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_setuptab.InsertItem(0,"自訂規則");
	m_setuptab.InsertItem(1,"訊息設定");
	m_setuptab.InsertItem(2,"掃描設定");
	m_setuptab.InsertItem(3,"資料庫設定"); 
	m_setuptab.InsertItem(4,"其他設定"); 
	m_setuptab.InsertItem(5,"關於"); 

	//m_setuptab.SetMapiEx(m_mapiex);
	m_setuptab.JoinTabDialog();

	 

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupFrm::SaveColorIni()
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

	  
	   CRuleSetupFrm *rulefrm;
	   rulefrm = (CRuleSetupFrm *) m_setuptab.m_tabPages[1];

	    

    char colorbuffer [255];
	itoa(rulefrm->SysOkMailColor,colorbuffer,10);
	SysOkMailColor = rulefrm->SysOkMailColor;
	WritePrivateProfileString("SysAlertColor","SysOkMailColor",colorbuffer,IniPath);
	//memset(colorbuffer,0,255);
	itoa(rulefrm->SpamMailColor,colorbuffer,10);
	SpamMailColor = rulefrm->SpamMailColor;
	WritePrivateProfileString("SysAlertColor","SpamMailColor",colorbuffer,IniPath);
	//memset(colorbuffer,0,255);
	itoa(rulefrm->UnknownColor,colorbuffer,10);
	UnknownColor = rulefrm->UnknownColor;
	WritePrivateProfileString("SysAlertColor","UnknownColor",colorbuffer,IniPath);
	//memset(colorbuffer,0,255);
	itoa(rulefrm->VirusColor,colorbuffer,10);
	VirusColor = rulefrm->VirusColor;
	WritePrivateProfileString("SysAlertColor","VirusColor",colorbuffer,IniPath);

	itoa(rulefrm->TextColor,colorbuffer,10);
	TextColor = rulefrm->TextColor;
	WritePrivateProfileString("SysAlertColor","TextColor",colorbuffer,IniPath);

	

}
