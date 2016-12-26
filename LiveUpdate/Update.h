// Update.h: interface for the CUpdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATE_H__C3F6B211_B69E_402D_81F1_5C509DBD9981__INCLUDED_)
#define AFX_UPDATE_H__C3F6B211_B69E_402D_81F1_5C509DBD9981__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LiveUpdateDlg.h"

typedef struct _DomainUpload
{
  bool isUpload;
} DomainUpload;



class CUpdate  
{
public:
	HRESULT GetLastDB();
	void SendDomainLogToServer();
	void UpdateIni();
	void SendReport();

	//void SyncReport();

	CUpdate(CLiveUpdateDlg *parent);
	virtual ~CUpdate();
	void SetProgress(CProgressCtrl *progress);
	void SetStatic(CStatic	*info);
	HRESULT ParseKeyFile(char *FileName);

	unsigned int GetFileVersion(char* FileName);

private:
	CProgressCtrl *m_progress;
	CStatic	*m_info;
	CLiveUpdateDlg *m_parent;

};

#endif // !defined(AFX_UPDATE_H__C3F6B211_B69E_402D_81F1_5C509DBD9981__INCLUDED_)
