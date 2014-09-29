;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "AMBE Tools 20140519"
  OutFile "AMBETools-20140519.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\DV Tools"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\COPYING.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Repeater Program Files" SecProgram

  SetOutPath "$INSTDIR"
  
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\ambe2wav.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\DVToolReader.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\DVToolWriter.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\dvtool2wav.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\wav2ambe.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\wav2dvtool.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\portaudio_x86.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\AMBETools\CHANGES.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\DV Tools"
  CreateShortCut "$SMPROGRAMS\DV Tools\DVTool Reader.lnk"         "$INSTDIR\DVToolReader.exe"
  CreateShortCut "$SMPROGRAMS\DV Tools\DVTool Writer.lnk"         "$INSTDIR\DVToolWriter.exe"
  CreateShortCut "$SMPROGRAMS\DV Tools\Licence.lnk"               "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\DV Tools\Changes.lnk"               "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\DV Tools\Uninstall.lnk"             "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\DV Tools\*.*"
  RMDir  "$SMPROGRAMS\DV Tools"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\DVTool Reader"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DVTool Writer"

SectionEnd
