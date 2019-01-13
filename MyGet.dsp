# Microsoft Developer Studio Project File - Name="MyGet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MyGet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MyGet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyGet.mak" CFG="MyGet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyGet - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MyGet - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MyGet - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MyGet - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_ANSIUNI_ONLY" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib Rasapi32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile /incremental:no

!ENDIF 

# Begin Target

# Name "MyGet - Win32 Release"
# Name "MyGet - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddBatchDownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRegs.cpp
# End Source File
# Begin Source File

SOURCE=.\CategoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CategoryTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\CommFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadFtpJet.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadHttpJet.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadJet.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadScheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadSchedulerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDrivedSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\FileName.cpp
# End Source File
# Begin Source File

SOURCE=.\FileName.h
# End Source File
# Begin Source File

SOURCE=.\FlashGetToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashGetTray.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Ftp.cpp
# End Source File
# Begin Source File

SOURCE=.\HistogramCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemListView.cpp
# End Source File
# Begin Source File

SOURCE=.\JCDFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ListItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyGet.cpp
# End Source File
# Begin Source File

SOURCE=.\MyGet.rc
# End Source File
# Begin Source File

SOURCE=.\MyGetDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MyGetView.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsConnectionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDialUpPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsFileManagementPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsGeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsGraphLogPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsLoginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsMirrorPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsMonitorPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsOtherPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsProtocolPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsProxyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsSchedulePage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsSoundPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressChart.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyData.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxySettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxySettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetupAlternativeURLsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellPidl.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SocksPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabView.cpp
# End Source File
# Begin Source File

SOURCE=.\TE_Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeItem.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddBatchDownloadDlg.h
# End Source File
# Begin Source File

SOURCE=.\AppRegs.h
# End Source File
# Begin Source File

SOURCE=.\CategoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\CategoryTree.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\CommonUtils.h
# End Source File
# Begin Source File

SOURCE=.\ContentListView.h
# End Source File
# Begin Source File

SOURCE=.\DownloadDlg.h
# End Source File
# Begin Source File

SOURCE=.\DownloadFtpJet.h
# End Source File
# Begin Source File

SOURCE=.\DownloadHttpJet.h
# End Source File
# Begin Source File

SOURCE=.\DownloadItemManager.h
# End Source File
# Begin Source File

SOURCE=.\DownloadJet.h
# End Source File
# Begin Source File

SOURCE=.\DownloadPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadScheduler.h
# End Source File
# Begin Source File

SOURCE=.\DownloadSchedulerThread.h
# End Source File
# Begin Source File

SOURCE=.\EventDrivedSocket.h
# End Source File
# Begin Source File

SOURCE=.\FlashGetToolBar.h
# End Source File
# Begin Source File

SOURCE=.\FlashGetTray.h
# End Source File
# Begin Source File

SOURCE=.\FolderSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Ftp.h
# End Source File
# Begin Source File

SOURCE=.\HistogramCtrl.h
# End Source File
# Begin Source File

SOURCE=.\IndexTreeView.h
# End Source File
# Begin Source File

SOURCE=.\InputDlg.h
# End Source File
# Begin Source File

SOURCE=.\ItemHeader.h
# End Source File
# Begin Source File

SOURCE=.\ItemListView.h
# End Source File
# Begin Source File

SOURCE=.\JCDFile.h
# End Source File
# Begin Source File

SOURCE=.\ListItem.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyGet.h
# End Source File
# Begin Source File

SOURCE=.\MyGetDoc.h
# End Source File
# Begin Source File

SOURCE=.\MyGetView.h
# End Source File
# Begin Source File

SOURCE=.\NewFolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsConnectionPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDialUpPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsFileManagementPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsGeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsGraphLogPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.h
# End Source File
# Begin Source File

SOURCE=.\OptionsLoginPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsMirrorPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsMonitorPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsOtherPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsProtocolPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsProxyPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsSchedulePage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsSoundPage.h
# End Source File
# Begin Source File

SOURCE=.\ProgressChart.h
# End Source File
# Begin Source File

SOURCE=.\ProxyData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetupAlternativeURLsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShellPidl.h
# End Source File
# Begin Source File

SOURCE=.\ShellTree.h
# End Source File
# Begin Source File

SOURCE=.\Sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\SocksPacket.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TabDef.h
# End Source File
# Begin Source File

SOURCE=.\Tabview.h
# End Source File
# Begin Source File

SOURCE=.\TE_Socket.h
# End Source File
# Begin Source File

SOURCE=.\TreeItem.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp133.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp145.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp159.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp160.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp199.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp200.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check_bo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\choice_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\content_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_ite.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MyGet.ico
# End Source File
# Begin Source File

SOURCE=.\res\MyGet.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MyGetDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\others.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tray1.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray2.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray3.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MyGet : {66833FE7-8583-11D1-B16A-00C0F0283628}
# 	2:5:Class:CButtons
# 	2:10:HeaderFile:buttons.h
# 	2:8:ImplFile:buttons.cpp
# End Section
# Section MyGet : {66833FE6-8583-11D1-B16A-00C0F0283628}
# 	2:21:DefaultSinkHeaderFile:toolbar.h
# 	2:16:DefaultSinkClass:CToolbar
# End Section
# Section MyGet : {66833FED-8583-11D1-B16A-00C0F0283628}
# 	2:5:Class:CButtonMenu
# 	2:10:HeaderFile:buttonmenu.h
# 	2:8:ImplFile:buttonmenu.cpp
# End Section
# Section MyGet : {C8A3DC00-8593-11D1-B16A-00C0F0283628}
# 	2:5:Class:CControls
# 	2:10:HeaderFile:controls.h
# 	2:8:ImplFile:controls.cpp
# End Section
# Section MyGet : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section MyGet : {66833FE9-8583-11D1-B16A-00C0F0283628}
# 	2:5:Class:CButton1
# 	2:10:HeaderFile:button.h
# 	2:8:ImplFile:button.cpp
# End Section
# Section MyGet : {66833FE4-8583-11D1-B16A-00C0F0283628}
# 	2:5:Class:CToolbar
# 	2:10:HeaderFile:toolbar.h
# 	2:8:ImplFile:toolbar.cpp
# End Section
# Section MyGet : {66833FEB-8583-11D1-B16A-00C0F0283628}
# 	2:5:Class:CButtonMenus
# 	2:10:HeaderFile:buttonmenus.h
# 	2:8:ImplFile:buttonmenus.cpp
# End Section
