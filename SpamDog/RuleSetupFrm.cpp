// RuleSetupFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDog.h"
#include "RuleSetupFrm.h"
//#include "../Global.h"
#include "RuleEditFrm.h"
#include "../Swparser/MailParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuleSetupFrm dialog


CRuleSetupFrm::CRuleSetupFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleSetupFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleSetupFrm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

 
}


void CRuleSetupFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleSetupFrm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
//	DDX_Control(pDX,IDC_COLORBOX_PREVIEW,m_ColorBoxPrev);
	DDX_Control(pDX, IDC_LIST1, m_RuleList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuleSetupFrm, CDialog)
	//{{AFX_MSG_MAP(CRuleSetupFrm)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleSetupFrm message handlers

BOOL CRuleSetupFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_RuleList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	/*m_RuleList.InsertColumn(0,"篩選條件",LVCFMT_LEFT,120);
	m_RuleList.InsertColumn(1,"顏色碼",LVCFMT_LEFT,60);
	m_RuleList.InsertColumn(2,"移至",LVCFMT_LEFT,60);
	m_RuleList.InsertColumn(3,"型別",LVCFMT_LEFT,50);
	m_RuleList.InsertColumn(4,"通知方式",LVCFMT_LEFT,60);*/

	m_RuleList.InsertColumn(0,"類型",LVCFMT_LEFT,50);
	m_RuleList.InsertColumn(1,"條件內容",LVCFMT_LEFT,300);

 
	m_ImageList = new CImageList;
	m_ImageList->Create(16, 16, ILC_COLOR8, 0, 3);

	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP2);
	m_ImageList->Add(&bm, RGB(0, 0, 0));

	CBitmap bm2;
	bm2.LoadBitmap(IDB_BITMAP8);
	m_ImageList->Add(&bm2, RGB(0, 0, 0));

	CBitmap bm3;
	bm3.LoadBitmap(IDB_BITMAP6);
	m_ImageList->Add(&bm3, RGB(0, 0, 0));

	m_RuleList.SetImageList (m_ImageList,LVSIL_SMALL );

	//讀出 ini

	GetIniRule(); 

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRuleSetupFrm::ShowAddrDialog()
{
	CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

    if (mfrm->Settings.APVersion == AP_OUTLOOK)
	{
		CMapiAddr  mapiaddr;

		CString Sender = "";

		mapiaddr.Login();
		mapiaddr.OpenAddrBook(this->m_hWnd,&Sender);
	}

	//m_SenderMail.SetWindowText(Sender);

}



void CRuleSetupFrm::GetIniRule()
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
	   
	   //#ifdef _OUTLOOK
	   strcat(IniPath,"Rule.dat");
	   //#endif

	   //#ifdef _OUTLOOKEX
	   //strcat(IniPath,"RuleOE.dat");
	   //#endif

	   FILE *fp = fopen(IniPath,"rb");

	   if (fp != NULL)
	   {
		   
		   

		   RuleFile tmp_RuleFile;

		   fseek(fp,sizeof(RuleHeader),SEEK_SET);
		   while(fread(&tmp_RuleFile,1,sizeof(RuleFile),fp))
		   {
			   RuleFile *m_RuleFile = new RuleFile;
			   memcpy(m_RuleFile,&tmp_RuleFile,sizeof(tmp_RuleFile));
			   
			    if (m_RuleFile->Used)
				{
					int itemcount = m_RuleList.GetItemCount();
					int nidx = 0;
					
					
					if (m_RuleFile->RuleType == 0)//一般類型
					{
						nidx = m_RuleList.InsertItem(itemcount,"自訂",0);
					} 
					else if (m_RuleFile->RuleType == 1)
					{
						nidx = m_RuleList.InsertItem(itemcount,"追蹤",1);
					}
					else if (m_RuleFile->RuleType == 2)
					{
						nidx = m_RuleList.InsertItem(itemcount,"名單",2);
					}


				
					m_RuleList.SetItemText(nidx,1,m_RuleFile->Rule);


					//FolderListData *SaveListData = new FolderListData;
					//memcpy(SaveListData,&m_RuleFile.ListData,sizeof(FolderListData));
		
					m_RuleList.SetItemData(nidx,(DWORD) m_RuleFile);

				}
			    else
				{
					delete m_RuleFile;
				}
				
		   }


	
	   
	   }

	   if (fp != NULL)
	   fclose(fp);


	   /*

	   char rtnstr[10240];
	   memset(rtnstr,0,10240);
	   int readsize = GetPrivateProfileSection("UserAlertColor",rtnstr,255,IniPath);
	   ParseIniStr(rtnstr,readsize);*/


}

void CRuleSetupFrm::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for (int i = 0 ; i < m_RuleList.GetItemCount() ; i++)
	{		 
		if (m_RuleList.GetItemData(i) != 0)
		delete (RuleFile *) m_RuleList.GetItemData(i);
	}

	delete m_ImageList;
	// TODO: Add your message handler code here
	
}

void CRuleSetupFrm::OnButton2() 
{
	CRuleEditFrm editfrm(NULL);
	editfrm.m_CheckTypeValue = 1;
	editfrm.m_AlertTypeValue = 1;

	if (editfrm.DoModal() > 0)
	{
	
		int itemcount = m_RuleList.GetItemCount();	

		//CString sender;
		//editfrm.m_SenderMail.GetWindowText(sender);

		

		//int itemidx = m_RuleList.InsertItem(itemcount,editfrm.m_IniSenderName);	
 
			int itemidx = m_RuleList.InsertItem(itemcount,"自訂",0);	

			RuleFile *m_RuleFile = new RuleFile;
			memset(m_RuleFile,0,sizeof(RuleFile));
			m_RuleList.SetItemData(itemidx,(DWORD) m_RuleFile);

			strcpy(m_RuleFile->Rule,editfrm.m_IniSenderName);
			m_RuleList.SetItemText(itemidx,1,m_RuleFile->Rule);

			m_RuleFile->ShowColor = editfrm.m_IniColor;
			m_RuleFile->AlertType = editfrm.m_AlertTypeValue;
			m_RuleFile->CheckType = editfrm.m_CheckTypeValue;


		//CString colortext;
		//colortext.Format("0x%x",editfrm.m_IniColor);	

		//m_RuleList.SetItemText(itemidx,1,colortext);
		//m_RuleList.SetItemText(itemidx,2,editfrm.m_FolderName);

		/*if (editfrm.m_CheckTypeValue == 1)
		{
			m_RuleList.SetItemText(itemidx,3,"寄件者");
		}
		else
		{
			m_RuleList.SetItemText(itemidx,3,"主旨");
		}

		if (editfrm.m_AlertTypeValue == 1)
		{
			m_RuleList.SetItemText(itemidx,4,"一般");
		}
		else
		{
			m_RuleList.SetItemText(itemidx,4,"特殊");
		}*/

		
		
		if (editfrm.m_ListData.FolderName[0] != 0)
		{
			//FolderListData *ListData = new FolderListData;
			//m_RuleFile->ListData
			memcpy(&m_RuleFile->ListData,&editfrm.m_ListData,sizeof(FolderListData));
		
			//m_RuleList.SetItemData(itemidx,(DWORD) ListData);
		}


	}
	
}

void CRuleSetupFrm::OnButton1() 
{
	 int CurSel= m_RuleList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	 if (CurSel != -1)
	 {
	
		RuleFile *m_RuleFile = (RuleFile *) m_RuleList.GetItemData(CurSel);
		 
		//CString SenderName = m_RuleList.GetItemText(CurSel,0);
		//CString ColorStr = m_RuleList.GetItemText(CurSel,1);

		//char *tmpchr = ColorStr.GetBuffer(ColorStr.GetLength());
		//CMailCodec codec;
		//int color = codec.StrToInt(tmpchr+1);


		//ShowAddrDialog();
		CRuleEditFrm editfrm(NULL);	

		editfrm.m_IniSenderName = m_RuleFile->Rule;
		editfrm.m_IniColor = m_RuleFile->ShowColor;	

		if (m_RuleFile->ListData.FolderName[0] == 0)
		{
			editfrm.m_FolderName = "不移動";
		}
		else
		{
			editfrm.m_FolderName = m_RuleFile->ListData.FolderName;
			editfrm.m_ListData =  m_RuleFile->ListData;
		
		}

	 
		CTime m_time((time_t) m_RuleFile->ExpireTime);
		
		if (m_RuleFile->RuleType == 1 )
		editfrm.m_ShowMemo =  m_time.Format("到期時間 : %Y/%m/%d %H:%M:%S");
		//

		

		//if (m_RuleList.GetItemData(CurSel) != 0)
		
		
		//if (strcmp(m_RuleList.GetItemText(CurSel,3),"寄件者") == 0)
		//{
				editfrm.m_CheckTypeValue = m_RuleFile->CheckType;
		//}
		//else
		//{
				//editfrm.m_CheckTypeValue = 2;
		//}

		//if (strcmp(m_RuleList.GetItemText(CurSel,4),"一般") == 0)
		//{
				editfrm.m_AlertTypeValue = m_RuleFile->AlertType;
		//}
		//else
		//{
				//editfrm.m_AlertTypeValue = 2;
		//}


		
		if (editfrm.DoModal() == 1)
		{
		
			
			strcpy(m_RuleFile->Rule,editfrm.m_IniSenderName);
			m_RuleList.SetItemText(CurSel,1,m_RuleFile->Rule);

			m_RuleFile->ShowColor = editfrm.m_IniColor;
			m_RuleFile->AlertType = editfrm.m_AlertTypeValue;
			m_RuleFile->CheckType = editfrm.m_CheckTypeValue;
 
		
			if (editfrm.m_ListData.FolderName[0] != 0)
			{		 
				memcpy(&m_RuleFile->ListData,&editfrm.m_ListData,sizeof(FolderListData));					 
			}
			else
			{
				memset(&m_RuleFile->ListData,0,sizeof(m_RuleFile->ListData));
			}
			
			
			//m_RuleList.SetItemText(CurSel,0,editfrm.m_IniSenderName);

			/*CString colortext;
			colortext.Format("0x%x",editfrm.m_IniColor);

			m_RuleList.SetItemText(CurSel,1,colortext);
			m_RuleList.SetItemText(CurSel,2,editfrm.m_FolderName);

			if (editfrm.m_CheckTypeValue == 1)
			{
				m_RuleList.SetItemText(CurSel,3,"寄件者");
			}
			else
			{
				m_RuleList.SetItemText(CurSel,3,"主旨");
			}

			if (editfrm.m_AlertTypeValue == 1)
			{
				m_RuleList.SetItemText(CurSel,4,"一般");
			}
			else
			{
				m_RuleList.SetItemText(CurSel,4,"特殊");
			}*/


			//if ( m_RuleList.GetItemData(CurSel) != 0)			
			//  delete (FolderListData *) m_RuleList.GetItemData(CurSel);

			//if (editfrm.m_ListData.FolderName[0] != 0)
			//{
				//FolderListData *ListData = new FolderListData;
				//memcpy(ListData,&editfrm.m_ListData,sizeof(FolderListData));
				
				//m_RuleList.SetItemData(CurSel,(DWORD) ListData);

				
			//}
			//else
			//{
			//	m_RuleList.SetItemData(CurSel,0);	
			//}
		}
		
	 }	
}

void CRuleSetupFrm::OnButton3() 
{
	int CurSel= m_RuleList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	 if (CurSel != -1)
	 {		

		 if (m_RuleList.GetItemData(CurSel) != 0)
			delete (RuleFile *) m_RuleList.GetItemData(CurSel);
		 m_RuleList.DeleteItem(CurSel);

		 if (m_RuleList.GetItemCount() > 0)
		 m_RuleList.SetItemState(CurSel, LVNI_FOCUSED | LVIS_SELECTED , LVNI_FOCUSED | LVIS_SELECTED );

		 
	 }	
}

void CRuleSetupFrm::OnButton7() 
{
	int CurSel= m_RuleList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	 if (CurSel != -1 && CurSel-1 >= 0)
	 {
		
		CString Item1 = m_RuleList.GetItemText(CurSel,0);
		CString Item2 = m_RuleList.GetItemText(CurSel,1);
		CString Item3 = m_RuleList.GetItemText(CurSel,2);
		CString Item4 = m_RuleList.GetItemText(CurSel,3);
		CString Item5 = m_RuleList.GetItemText(CurSel,4);

		DWORD SaveData = m_RuleList.GetItemData(CurSel);

		RuleFile *m_RuleFile = (RuleFile *) SaveData;


	

		m_RuleList.DeleteItem(CurSel);
		CurSel--;

		int newitem = m_RuleList.InsertItem(CurSel,Item1,m_RuleFile->RuleType);
		m_RuleList.SetItemText(newitem,1,Item2);
		m_RuleList.SetItemText(newitem,2,Item3);
		m_RuleList.SetItemText(newitem,3,Item4);
		m_RuleList.SetItemText(newitem,4,Item5);
		m_RuleList.SetItemData(newitem,SaveData);

		m_RuleList.SetItemState(newitem, LVIS_SELECTED | LVIS_FOCUSED ,LVIS_SELECTED | LVIS_FOCUSED);
		
	 

	 }

	 m_RuleList.SetFocus();
	
}

void CRuleSetupFrm::OnButton8() 
{
	 int CurSel= m_RuleList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	 if (CurSel != -1 && CurSel+1 <= m_RuleList.GetItemCount())
	 {
		CString Item1 = m_RuleList.GetItemText(CurSel,0);
		CString Item2 = m_RuleList.GetItemText(CurSel,1);
		CString Item3 = m_RuleList.GetItemText(CurSel,2);
		CString Item4 = m_RuleList.GetItemText(CurSel,3);
		CString Item5 = m_RuleList.GetItemText(CurSel,4);
		DWORD SaveData = m_RuleList.GetItemData(CurSel);

		RuleFile *m_RuleFile = (RuleFile *) SaveData;

		m_RuleList.DeleteItem(CurSel);
		CurSel++;

		int newitem = m_RuleList.InsertItem(CurSel,Item1,m_RuleFile->RuleType);
		m_RuleList.SetItemText(newitem,1,Item2);
		m_RuleList.SetItemText(newitem,2,Item3);
		m_RuleList.SetItemText(newitem,3,Item4);
		m_RuleList.SetItemText(newitem,4,Item5);
		m_RuleList.SetItemData(newitem,SaveData);

		m_RuleList.SetItemState(newitem,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED);
		
	 	
	 }

	 m_RuleList.SetFocus();	
}



void CRuleSetupFrm::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	// Has the item's state changed?
	if ((pNMListView->uChanged & LVIF_STATE) != LVIF_STATE)
	{
		return; // state has not changed
	}
	// Has the selected state changed?
	if (!(pNMListView->uNewState & LVIS_SELECTED) || (pNMListView->uOldState & LVIS_SELECTED))
	{
		return; // selected state has not changed
	}


	/*
	int nItem = pNMListView->iItem; 

	CString ColorStr = m_RuleList.GetItemText(nItem,1);
	CString SenderMail = m_RuleList.GetItemText(nItem,0);

 
	char *tmpchr = ColorStr.GetBuffer(ColorStr.GetLength());
	

	CMailCodec codec;
	int color = codec.StrToInt(tmpchr+1);
	m_ColorBoxPrev.SetColor(color);
	ColorStr.ReleaseBuffer();*/

	*pResult = 0;
}

void CRuleSetupFrm::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButton1();
	*pResult = 0;
}
