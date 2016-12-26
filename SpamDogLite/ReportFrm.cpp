// ReportFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "ReportFrm.h"
#include "MainFrm.h"
#include "MailReportFrm.h"
#include "../Swspam/Checker.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportFrm dialog


CReportFrm::CReportFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CReportFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportFrm)
	//}}AFX_DATA_INIT
	CloseCounter = 120;
	AutoClose = false;
	m_InModal = false;
	m_ImageList = NULL;
}


void CReportFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportFrm)
	DDX_Control(pDX, IDC_STATIC_COUNTER, m_Counter);
	DDX_Control(pDX, IDC_TAB2, m_ReportTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportFrm, CDialog)
	//{{AFX_MSG_MAP(CReportFrm)
		ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportFrm message handlers

BOOL CReportFrm::OnInitDialog() 
{
	CloseCounter = 120;
	m_InModal = true;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ReportTab.InsertItem(0,"垃圾郵件");
	m_ReportTab.InsertItem(1,"正常郵件");
 
	m_ReportTab.JoinTabDialog();


	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   
	   strcpy(ApPath,drive);
	   strcat(ApPath,dir);	

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");


	CenterWindow(GetDesktopWindow());
	
	SetForegroundWindow();
	
	
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(0,"類別",LVCFMT_LEFT,60);
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(1,"寄件者",LVCFMT_LEFT,140);
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(2,"主旨",LVCFMT_LEFT,200);
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(3,"來源IP",LVCFMT_LEFT,100);
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(4,"國家",LVCFMT_LEFT,60);
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(5,"ISP",LVCFMT_LEFT,60);
	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertColumn(6,"回報E-Mail",LVCFMT_LEFT,100);

	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(0,"類別",LVCFMT_LEFT,60);
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(1,"寄件者",LVCFMT_LEFT,140);
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(2,"主旨",LVCFMT_LEFT,200);
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(3,"來源IP",LVCFMT_LEFT,100);
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(4,"國家",LVCFMT_LEFT,60);
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(5,"ISP",LVCFMT_LEFT,60);
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertColumn(6,"回報E-Mail",LVCFMT_LEFT,100);




	
	m_ImageList = new CImageList;
	m_ImageList->Create(16, 16, ILC_COLOR8, 0, 3);

	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP1);
	m_ImageList->Add(&bm, RGB(0, 0, 0));

	CBitmap bm2;
	bm2.LoadBitmap(IDB_BITMAP9);	
	m_ImageList->Add(&bm2, RGB(0, 0, 0));

	CBitmap bm3;
	bm3.LoadBitmap(IDB_BITMAP10);	
	m_ImageList->Add(&bm3, RGB(0, 0, 0));

	 

	((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetImageList (m_ImageList,LVSIL_SMALL );
	((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetImageList (m_ImageList,LVSIL_SMALL );


 

	/*if (GetPrivateProfileInt("SysSetup","ReportWithDelInFrm",1,IniPath) == 1)
	{
		m_DelSpam.SetCheck(BST_CHECKED);
	}
	else
	{
		m_DelSpam.SetCheck(BST_UNCHECKED);
	}*/

	


	if (AutoClose)
	{
		CString CounterTxt;
		CounterTxt.Format("將於 %d 秒後自動關閉",CloseCounter);
		m_Counter.SetWindowText(CounterTxt);

		SetTimer(1,1000,NULL);
	}
	else
	{
	    //m_DelSpam.ShowWindow(SW_HIDE);
		m_Counter.SetWindowText("");
	}


	ListMail();

	//Active Page
	if (((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.GetItemCount() == 0)
	{
		m_ReportTab.SetActivePage(1);		
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default


	CDialog::OnOK();
	CDialog::OnClose();
}

void CReportFrm::OnOK()
{


}

void CReportFrm::OnCancel()
{

}

void CReportFrm::OnButton1() 
{
	
	for (int k = 0 ; k < 2 ; k++)
	{
	for (int i = 0 ; i < ((CMailReportFrm *) m_ReportTab.m_tabPages[k])->m_ListMail.GetItemCount() ; i++)
	{		 
		if ( ((CMailReportFrm *) m_ReportTab.m_tabPages[k])->m_ListMail.GetItemData(i) != 0)
		{
			ReportItem *mReportItem = (ReportItem *)  ((CMailReportFrm *) m_ReportTab.m_tabPages[k])->m_ListMail.GetItemData(i);

			if (mReportItem->changed)
			{
			
				char MailPath[_MAX_PATH];
				strcpy(MailPath , ApPath);
				strcat(MailPath,"Report//"); 
				strcat(MailPath , mReportItem->FileName);

			
				if (mReportItem->Type == 2) //不回報
				{				
					DeleteFile(MailPath);
				}
				else
				{
				
					ReportMailData MailData;
					memset(&MailData,0,sizeof(MailData));

				

					FILE *fp = fopen(MailPath,"r+b");
					if (fp != NULL)
					{

						if (fread(&MailData,1,sizeof(MailData),fp) == sizeof(MailData))
						{

							if (mReportItem->Type == 1)
							{
								//正常
								MailData.Type = 2;
							}
							else if (mReportItem->Type == 0)
							{
								//垃圾
								MailData.Type = 0;
							}

							fseek(fp,0,SEEK_SET);
							fwrite(&MailData,1,sizeof(MailData),fp);
						}
					
						fclose(fp);

					}
				
				}
					
			
			}
		}
	}

	}

	//if (m_DelSpam.GetCheck() == BST_CHECKED)
	//{
	  EndDialog(20);
	//}
	//else
	//{
	  //EndDialog(2);
	//}

	m_InModal = false;
	
}

void CReportFrm::OnButton3() 
{
	// TODO: Add your control notification handler code here
	EndDialog(4);
	m_InModal = false;
}

void CReportFrm::ListMail()
{
	char ContactDBPath[_MAX_PATH];
	char ContactDataDBPath[_MAX_PATH];

	strcpy(ContactDBPath,ApPath);
	strcat(ContactDBPath,"//DB//Contact.db"); 

	strcpy(ContactDataDBPath,ApPath);
	strcat(ContactDataDBPath,"//DB//ContactData.db"); 
	
	CMainFrm *mainfrm = (CMainFrm *) AfxGetMainWnd();

	
	 ((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.DeleteAllItems();
	 ((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.DeleteAllItems();
	
	 WIN32_FIND_DATA FindFileData;
     HANDLE hFind;

	 char FindPath[_MAX_PATH];
	 strcpy(FindPath,ApPath);
	 strcat(FindPath,"Report//*.*"); 
	 //int SendFileCount = 0;

	 //得到  total number
	 int BadMailCount=0;
	 unsigned int TotalIPValue = 0;
	 unsigned int IPs[50];
	 int IPidx = 0;

	 

	 hFind = FindFirstFile(FindPath,&FindFileData);
 	 if (hFind != INVALID_HANDLE_VALUE)
	 {
	 
		 //read file
		 //if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		 //{
		     //SendFileCount++;
		 //}

		 do
		 {			
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				char MailPath[_MAX_PATH];
				strcpy(MailPath , ApPath);
				strcat(MailPath,"Report//"); 
				strcat(MailPath , FindFileData.cFileName);

				ReportMailData MailData;
				memset(&MailData,0,sizeof(MailData));

				FILE *fp = NULL;
				fp = fopen(MailPath,"rb");
				if (fp != NULL)
				{
				
					fseek(fp,0,SEEK_SET);


					if (fread(&MailData,1,sizeof(MailData),fp) == sizeof(MailData))
					{
					
						//write to report					
						if (MailData.Type == 0)
						{
							int itemcount = ((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.GetItemCount();
							int nidx = -1;

							ReportItem *mReportItem = new ReportItem;
							memset(mReportItem,0,sizeof(ReportItem));
							nidx = ((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.InsertItem(itemcount,"垃圾",0);
							strcpy(mReportItem->SortStr[0],"垃圾");

							mReportItem->Type = 0;
							strcpy(mReportItem->FileName,FindFileData.cFileName);



							((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemData(nidx,(DWORD) mReportItem);

							if (nidx > -1)					
							{
								((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemText(nidx,1,MailData.Sender);
								strcpy(mReportItem->SortStr[1],MailData.Sender);
								((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemText(nidx,2,MailData.Subject);
								strcpy(mReportItem->SortStr[2],MailData.Subject);
								((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemText(nidx,3,MailData.IP);
								strcpy(mReportItem->SortStr[3],MailData.IP);

							
								CSpamContact sc;
								sc.SetIp(MailData.IP);
								unsigned int IPValue = sc.GetIPValue();								
								
								if (IPidx < (sizeof(IPs) / sizeof(IPs[0])) )
								{
									TotalIPValue += IPValue;
									IPs[IPidx] = IPValue;
									IPidx++;
								}

								//EnterCriticalSection(&mainfrm->m_RangeDbSec);
								//CTreeRangeDB ContactDB(ContactDBPath,ContactDataDBPath);
								CTreeRangeDB *ContactDB = new CTreeRangeDB(&mainfrm->m_RangeDbSec,ContactDBPath,ContactDataDBPath);
								ContactDB->OpenDB();
								int DataFilePos = ContactDB->Search(IPValue,ContactDB->FirstRecPos);
								if (DataFilePos != -1)
								{
									Contact ct;
									memset(&ct,0,sizeof(Contact));

									ContactDB->SelectData(DataFilePos,(char *) &ct,sizeof(ct));

									//if (  ct.netname[0] != 0 )
									//{
										((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemText(nidx,4,ct.country);
										strcpy(mReportItem->SortStr[4],ct.country);
										((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemText(nidx,5,ct.netname);
										strcpy(mReportItem->SortStr[5],ct.netname);
										((CMailReportFrm *) m_ReportTab.m_tabPages[0])->m_ListMail.SetItemText(nidx,6,ct.Email);
										strcpy(mReportItem->SortStr[6],ct.Email);
									//}

								}

								ContactDB->CloseDB();
								delete ContactDB;

								//LeaveCriticalSection(&mainfrm->m_RangeDbSec);
							
							}
						} 
						else if (MailData.Type == 2)
						{
							int itemcount = ((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.GetItemCount();
							int nidx = -1;
 
							ReportItem *mReportItem = new ReportItem;
							memset(mReportItem,0,sizeof(ReportItem));
							nidx = ((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.InsertItem(itemcount,"正常",1);
							strcpy(mReportItem->SortStr[0],"正常");
							
							mReportItem->Type = 1;
							strcpy(mReportItem->FileName,FindFileData.cFileName);

							((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemData(nidx,(DWORD) mReportItem);

							if (nidx > -1)					
							{
								((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemText(nidx,1,MailData.Sender);
								strcpy(mReportItem->SortStr[1],MailData.Sender);
								((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemText(nidx,2,MailData.Subject);
								strcpy(mReportItem->SortStr[2],MailData.Subject);
								((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemText(nidx,3,MailData.IP);
								strcpy(mReportItem->SortStr[3],MailData.IP);

								CSpamContact sc;
								sc.SetIp(MailData.IP);
								unsigned int IPValue = sc.GetIPValue();

						
								//EnterCriticalSection(&mainfrm->m_RangeDbSec);

								CTreeRangeDB *ContactDB = new CTreeRangeDB(&mainfrm->m_RangeDbSec,ContactDBPath,ContactDataDBPath);
								ContactDB->OpenDB();
								int DataFilePos = ContactDB->Search(IPValue,ContactDB->FirstRecPos);
								if (DataFilePos != -1)
								{
									Contact ct;
									memset(&ct,0,sizeof(Contact));

									ContactDB->SelectData(DataFilePos,(char *) &ct,sizeof(ct));

									//if (  ct.netname[0] != 0 )
									//{
										((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemText(nidx,4,ct.country);
										strcpy(mReportItem->SortStr[4],ct.country);
										((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemText(nidx,5,ct.netname);
										strcpy(mReportItem->SortStr[5],ct.netname);
										((CMailReportFrm *) m_ReportTab.m_tabPages[1])->m_ListMail.SetItemText(nidx,6,ct.Email);
										strcpy(mReportItem->SortStr[6],ct.Email);
									//}

								}

								ContactDB->CloseDB();
								delete ContactDB;
								//LeaveCriticalSection(&mainfrm->m_RangeDbSec);

								
							
							}


						}
						 
					
					}
				
					fclose(fp);
				}
				
				
			}		
		 
		 }while(FindNextFile(hFind, &FindFileData));

		 FindClose(hFind);
	 

	 }


	 if (IPidx >= 2 )
	 {
		double IPmin = TotalIPValue / (IPidx);
		double Exx = 0;

		for (int i=0 ; i < IPidx ; i ++)
		{
			Exx = Exx + ((IPs[i] - IPmin) * (IPs[i] - IPmin));
		}

		double StdValue = sqrt(Exx / IPidx);

		//CString kk;
		//kk.Format("%f",StdValue);
		//AfxMessageBox(kk);

		if (StdValue <= 100)
			MessageBox("您回報信件的來源 IP 由於非常接近,回報前請先確認來源 IP 是否正確,這些 IP 回報後都可能會被列入黑名單\n如有問題請選擇不回報,且與我們聯絡!");
	 
	 }

}

BOOL CReportFrm::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/*if (m_DelSpam.GetCheck() == BST_CHECKED)
	{
		CString tmpstr;
		tmpstr="1";

		WritePrivateProfileString("SysSetup","ReportWithDelInFrm",tmpstr,IniPath);
	}
	else
	{
	
		CString tmpstr;
		tmpstr="0";

		WritePrivateProfileString("SysSetup","ReportWithDelInFrm",tmpstr,IniPath);
	
	}*/

	if (m_ImageList != NULL)
		delete m_ImageList;

	//Free List Data
	for (int k = 0 ; k < 2 ; k++)
	{
		for (int i = 0 ; i < ((CMailReportFrm *) m_ReportTab.m_tabPages[k])->m_ListMail.GetItemCount() ; i++)
		{		 
			if (((CMailReportFrm *) m_ReportTab.m_tabPages[k])->m_ListMail.GetItemData(i) != 0)
			delete (ReportItem *) ((CMailReportFrm *) m_ReportTab.m_tabPages[k])->m_ListMail.GetItemData(i);
		}
	}

	m_InModal = false;

	return CDialog::DestroyWindow();
}

void CReportFrm::OnTimer(UINT nIDEvent) 
{
	CloseCounter--;

	if (CloseCounter <= 0)
	{
		OnButton3();
		KillTimer(nIDEvent);
	}
	else
	{
		CString CounterTxt;
		CounterTxt.Format("將於 %d 秒後自動關閉",CloseCounter);
		m_Counter.SetWindowText(CounterTxt);
	}	
	CDialog::OnTimer(nIDEvent);
}
