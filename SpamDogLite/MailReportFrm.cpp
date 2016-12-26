// MailReportFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "MailReportFrm.h"
#include "../Swspam/Checker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailReportFrm dialog


CMailReportFrm::CMailReportFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CMailReportFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailReportFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//CloseCounter = 120;
	//AutoClose = false;
	m_ImageList = NULL;

	LastSortColumn = -1;
	SortDir = false;
}


void CMailReportFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailReportFrm)
//	DDX_Control(pDX, IDC_STATIC_COUNTER, m_Counter);
//	DDX_Control(pDX, IDC_CHECK_DELSPAM, m_DelSpam);
	DDX_Control(pDX, IDC_LIST_MAIL, m_ListMail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailReportFrm, CDialog)
	//{{AFX_MSG_MAP(CMailReportFrm)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAIL, OnDblclkListMail)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MAIL, OnRclickListMail)
	ON_COMMAND(ID_MENUITEM32778, OnMenuitem32778)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)
	ON_COMMAND(ID_MENUITEM32779, OnMenuitem32779)
	ON_COMMAND(ID_MENUITEM32780, OnMenuitem32780)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailReportFrm message handlers

BOOL CMailReportFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//m_InModal = true;

	/* char path_buffer[_MAX_PATH];
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
	
	m_ListMail.SetExtendedStyle(LVS_EX_FULLROWSELECT );

	m_ListMail.InsertColumn(0,"類別",LVCFMT_LEFT,60);
	m_ListMail.InsertColumn(1,"寄件者",LVCFMT_LEFT,140);
	m_ListMail.InsertColumn(2,"主旨",LVCFMT_LEFT,200);
	m_ListMail.InsertColumn(3,"來源IP",LVCFMT_LEFT,100);
	m_ListMail.InsertColumn(4,"國家",LVCFMT_LEFT,60);
	m_ListMail.InsertColumn(5,"ISP",LVCFMT_LEFT,60);
	m_ListMail.InsertColumn(6,"回報E-Mail",LVCFMT_LEFT,100);

	
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

	 

	m_ListMail.SetImageList (m_ImageList,LVSIL_SMALL );


 

	if (GetPrivateProfileInt("SysSetup","ReportWithDelInFrm",1,IniPath) == 1)
	{
		m_DelSpam.SetCheck(BST_CHECKED);
	}
	else
	{
		m_DelSpam.SetCheck(BST_UNCHECKED);
	}

	


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


	ListMail();*/

	/*_ToolTipCtrl.Create( this, 
						 // the ToolTip control's style 
		                 TTS_NOPREFIX | // prevents the system from stripping the ampersand (&) 
						                // character from a string

						 TTS_BALLOON  | // the ToolTip control has the appearance of
						 // 0x40        // a cartoon "balloon," with rounded corners 
										// and a stem pointing to the item. 

						 TTS_ALWAYSTIP  // the ToolTip will appear when the
	                                    // cursor is on a tool, regardless of 
	                                    // whether the ToolTip control's owner
	                                    // window is active or inactive
					   );

	::SendMessage( (HWND)   _ToolTipCtrl.m_hWnd, 
				          (UINT)   TTM_SETTITLE, // Adds a standard icon and title string to a ToolTip    
				          (WPARAM) TTI_INFO, 
							// TTI_NONE    = 0 - no icon 
							// TTI_INFO    = 1 - information icon 
							// TTI_WARNING = 2 - warning icon 
							// TTI_ERROR   = 3 - error icon 
						  (LPARAM) (LPCTSTR) _T("請確實確定回報信件") );

 
		//register tools with the ToolTip control
	_ToolTipCtrl.SetDelayTime(1000*60,500 );
	_ToolTipCtrl.AddTool( GetDlgItem( IDC_LIST_MAIL ), _T("提醒您曾經訂閱的電子報並非垃圾郵件\n,如回報遭ISP駁回次數太多,我們將直接停權!") );
		
	 
	*/
 
	

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
void CMailReportFrm::OnButton3() 
{
	// TODO: Add your control notification handler code here
	EndDialog(4);
}*/
/*
void CMailReportFrm::ListMail()
{
	char ContactDBPath[_MAX_PATH];
	char ContactDataDBPath[_MAX_PATH];

	strcpy(ContactDBPath,ApPath);
	strcat(ContactDBPath,"//DB//Contact.db"); 

	strcpy(ContactDataDBPath,ApPath);
	strcat(ContactDataDBPath,"//DB//ContactData.db"); 
	 
	CTreeRangeDB ContactDB(ContactDBPath,ContactDataDBPath);

	 m_ListMail.DeleteAllItems();
	
	 WIN32_FIND_DATA FindFileData;
     HANDLE hFind;

	 char FindPath[_MAX_PATH];
	 strcpy(FindPath,ApPath);
	 strcat(FindPath,"Report//*.*"); 
	 //int SendFileCount = 0;

	 //得到  total number

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
						int itemcount = m_ListMail.GetItemCount();
						int nidx = -1;
						
						
						if (MailData.Type == 0)
						{
							ReportItem *mReportItem = new ReportItem;
							memset(mReportItem,0,sizeof(ReportItem));
							nidx = m_ListMail.InsertItem(itemcount,"垃圾",0);

							mReportItem->Type = 0;
							strcpy(mReportItem->FileName,FindFileData.cFileName);

							m_ListMail.SetItemData(nidx,(DWORD) mReportItem);
						} 
						else if (MailData.Type == 2)
						{
							ReportItem *mReportItem = new ReportItem;
							memset(mReportItem,0,sizeof(ReportItem));
							nidx = m_ListMail.InsertItem(itemcount,"正常",1);
							
							
							mReportItem->Type = 1;
							strcpy(mReportItem->FileName,FindFileData.cFileName);

							m_ListMail.SetItemData(nidx,(DWORD) mReportItem);


						}
						 
						if (nidx > -1)					
						{
							m_ListMail.SetItemText(nidx,1,MailData.Sender);
							m_ListMail.SetItemText(nidx,2,MailData.Subject);
							m_ListMail.SetItemText(nidx,3,MailData.IP);

							CSpamContact sc;
							sc.SetIp(MailData.IP);

							int DataFilePos = ContactDB.Search(sc.GetIPValue(),ContactDB.FirstRecPos);
							if (DataFilePos != -1)
							{
								Contact ct;
								memset(&ct,0,sizeof(Contact));

								ContactDB.SelectData(DataFilePos,(char *) &ct,sizeof(ct));

								if (ct.Email[0] != 0 &&  ct.netname[0] != 0 )
								{
									m_ListMail.SetItemText(nidx,4,ct.country);
									m_ListMail.SetItemText(nidx,5,ct.netname);
									m_ListMail.SetItemText(nidx,6,ct.Email);
								}

							}
							
						}
					}
				
					fclose(fp);
				}
				
				
			}		
		 
		 }while(FindNextFile(hFind, &FindFileData));

		 FindClose(hFind);
	 

	 }

}*/

/*
void CMailReportFrm::OnButton1() 
{
	// TODO: Add your control notification handler code here

	//correct report type
	for (int i = 0 ; i < m_ListMail.GetItemCount() ; i++)
	{		 
		if (m_ListMail.GetItemData(i) != 0)
		{
			ReportItem *mReportItem = (ReportItem *) m_ListMail.GetItemData(i);

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


	if (m_DelSpam.GetCheck() == BST_CHECKED)
	{
	  EndDialog(20);
	}
	else
	{
	  EndDialog(2);
	}
	
}*/
/*
void CMailReportFrm::OnButton2() 
{
	// TODO: Add your control notification handler code here
	if (m_DelSpam.GetCheck() == BST_CHECKED)
	{
	  EndDialog(30);
	}
	else
	{
	  EndDialog(3);
	}
}*/

void CMailReportFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default


	CDialog::OnOK();
	CDialog::OnClose();
}

void CMailReportFrm::OnOK()
{


}

void CMailReportFrm::OnCancel()
{

}
/*
BOOL CMailReportFrm::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_DelSpam.GetCheck() == BST_CHECKED)
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
	
	}

	if (m_ImageList != NULL)
		delete m_ImageList;

	//Free List Data
	for (int i = 0 ; i < m_ListMail.GetItemCount() ; i++)
	{		 
		if (m_ListMail.GetItemData(i) != 0)
		delete (ReportItem *) m_ListMail.GetItemData(i);
	}

 
	return CDialog::DestroyWindow();
}*/

/*
void CMailReportFrm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

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
}*/

int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRetVal;

	ReportItem* pData1 = (ReportItem *)lParam1;
	ReportItem* pData2 = (ReportItem *)lParam2;

 
	if (((SortItem *) lParamSort)->SortDir)
	{

		nRetVal = strcmp(pData2->SortStr[((SortItem *) lParamSort)->SortIdx],
                                 pData1->SortStr[((SortItem *) lParamSort)->SortIdx]);

	}
	else
	{
		nRetVal = strcmp(pData1->SortStr[((SortItem *) lParamSort)->SortIdx],
                                 pData2->SortStr[((SortItem *) lParamSort)->SortIdx]);
	}

	


	
	return nRetVal;
}


void CMailReportFrm::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW *pLV = (NMLISTVIEW *) pNMHDR;

	SortItem m_SortItem;
	m_SortItem.SortIdx = pLV->iItem;

	if (LastSortColumn == pLV->iItem && !SortDir)
	{	
		m_SortItem.SortDir = true; //反向
		SortDir = true;
	}
	else
	{
		m_SortItem.SortDir = false; //正向
		SortDir = false;
	}

	LastSortColumn = pLV->iItem;


	m_ListMail.SortItems(SortFunc, (LPARAM) &m_SortItem);

	*pResult = 0;
}


void CMailReportFrm::OnDblclkListMail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	NMLISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR; 

	ReportItem *mReportItem = (ReportItem *) m_ListMail.GetItemData(pNMListView->iItem); 

	mReportItem->changed = true;

	if (mReportItem->Type == 0)
	{
		m_ListMail.SetItem(pNMListView->iItem,0,LVIF_TEXT|LVIF_IMAGE ,"正常",1,0, 0, 0);
		strcpy(mReportItem->SortStr[0],"正常");
		mReportItem->Type = 1;
		//m_ListMail.SetItemData(pNMListView->iItem,1);
	} 
	else if (mReportItem->Type == 1)
	{	
	
		m_ListMail.SetItem(pNMListView->iItem,0,LVIF_TEXT|LVIF_IMAGE ,"不回報",2,0, 0, 0);
		strcpy(mReportItem->SortStr[0],"不回報");
		mReportItem->Type = 2;
		//m_ListMail.SetItemData(pNMListView->iItem,2);

	} 
	else if (mReportItem->Type == 2)
	{
		m_ListMail.SetItem(pNMListView->iItem,0,LVIF_TEXT|LVIF_IMAGE ,"垃圾",0,0, 0, 0);
		strcpy(mReportItem->SortStr[0],"垃圾");
		mReportItem->Type = 0;
		//m_ListMail.SetItemData(pNMListView->iItem,0);

	}


	
	*pResult = 0;
}
 
void CMailReportFrm::OnRclickListMail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW *pLV = (NMLISTVIEW *) pNMHDR;

	
	CString ItemText = m_ListMail.GetItemText(pLV->iItem,0);

	CMenu menu;
			
	if (!menu.LoadMenu(IDR_MENU2))
	return ;
	
	CMenu* pSubMenu = menu.GetSubMenu(0);

	if (ItemText.Find("垃圾") == 0 )
	{
		pSubMenu->CheckMenuItem(ID_MENUITEM32778,MF_CHECKED);
		pSubMenu->CheckMenuItem(ID_MENUITEM32779,MF_UNCHECKED);
		pSubMenu->CheckMenuItem(ID_MENUITEM32780,MF_UNCHECKED);
	}
	else if (ItemText.Find("正常") == 0 )
	{
		pSubMenu->CheckMenuItem(ID_MENUITEM32778,MF_UNCHECKED);
	    pSubMenu->CheckMenuItem(ID_MENUITEM32779,MF_CHECKED);
		pSubMenu->CheckMenuItem(ID_MENUITEM32780,MF_UNCHECKED);
	}
	else if (ItemText.Find("不回報") == 0 )
	{
	    pSubMenu->CheckMenuItem(ID_MENUITEM32778,MF_UNCHECKED);
		pSubMenu->CheckMenuItem(ID_MENUITEM32779,MF_UNCHECKED);
		pSubMenu->CheckMenuItem(ID_MENUITEM32780,MF_CHECKED);
	}

	



		    CPoint pos;
			GetCursorPos(&pos);		
	
			//CWnd *mainWnd = AfxGetMainWnd();
			this->SetForegroundWindow();
			
			//mainWnd->SetForegroundWindow();
			
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON
				|TPM_RIGHTBUTTON, pos.x, pos.y, this, NULL);

	*pResult = 0;
}

void CMailReportFrm::OnMenuitem32778() 
{
	// TODO: Add your command handler code here
	int  nItem = -1;
	nItem = m_ListMail.GetNextItem(nItem,LVNI_SELECTED);

	ReportItem *mReportItem = (ReportItem *) m_ListMail.GetItemData(nItem); 
	mReportItem->changed = true;
	
	if (mReportItem->Type != 0)
	{
		m_ListMail.SetItem(nItem,0,LVIF_TEXT|LVIF_IMAGE ,"垃圾",0,0, 0, 0);
		strcpy(mReportItem->SortStr[0],"垃圾");
		mReportItem->Type = 0;
	 

	}



}

 

void CMailReportFrm::OnMenuitem32779() 
{
	// TODO: Add your command handler code here
	int  nItem = -1;
	nItem = m_ListMail.GetNextItem(nItem,LVNI_SELECTED);

	ReportItem *mReportItem = (ReportItem *) m_ListMail.GetItemData(nItem); 
	mReportItem->changed = true;
	
	if (mReportItem->Type != 1)
	{
		m_ListMail.SetItem(nItem,0,LVIF_TEXT|LVIF_IMAGE ,"正常",1,0, 0, 0);
		strcpy(mReportItem->SortStr[0],"正常");
		mReportItem->Type = 1;
	 

	}
	
}

void CMailReportFrm::OnMenuitem32780() 
{
	// TODO: Add your command handler code here
	int  nItem = -1;
	nItem = m_ListMail.GetNextItem(nItem,LVNI_SELECTED);

	ReportItem *mReportItem = (ReportItem *) m_ListMail.GetItemData(nItem); 
	mReportItem->changed = true;
	
	if (mReportItem->Type != 2)
	{
		m_ListMail.SetItem(nItem,0,LVIF_TEXT|LVIF_IMAGE ,"不回報",2,0, 0, 0);
		strcpy(mReportItem->SortStr[0],"不回報");
		mReportItem->Type = 2;
	 

	}
	
}
/*

BOOL CMailReportFrm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_MOUSEMOVE )
	{
		_ToolTipCtrl.RelayEvent( pMsg ); // pass the mouse message to the ToolTip control for processing
	}
	return CDialog::PreTranslateMessage(pMsg);
}
*/