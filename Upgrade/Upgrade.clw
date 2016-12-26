; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUpgradeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Upgrade.h"

ClassCount=3
Class1=CUpgradeApp
Class2=CUpgradeDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_UPGRADE_DIALOG

[CLS:CUpgradeApp]
Type=0
HeaderFile=Upgrade.h
ImplementationFile=Upgrade.cpp
Filter=N

[CLS:CUpgradeDlg]
Type=0
HeaderFile=UpgradeDlg.h
ImplementationFile=UpgradeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CUpgradeDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=UpgradeDlg.h
ImplementationFile=UpgradeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_UPGRADE_DIALOG]
Type=1
Class=CUpgradeDlg
ControlCount=2
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_PROGRESS1,msctls_progress32,1350565889

