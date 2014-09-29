;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "XReflector 20130223"
  OutFile "XReflector-20130223.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\XReflector"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\COPYING.txt"
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
  
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\Release\XReflector.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\CHANGES.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\XReflector\COPYING.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\XReflector"
  CreateShortCut "$SMPROGRAMS\XReflector\XReflector.lnk"   "$INSTDIR\XReflector.exe"
  CreateShortCut "$SMPROGRAMS\XReflector\Changes.lnk"      "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\XReflector\Licence.lnk"      "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\XReflector\Uninstall.lnk"    "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\XReflector\*.*"
  RMDir  "$SMPROGRAMS\XReflector"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\XReflector"

SectionEnd
