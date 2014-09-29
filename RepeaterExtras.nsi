;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "Repeater Extras 20130702"
  OutFile "RepeaterExtras-20130702.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\Repeater Extras"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\COPYING.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Repeater Extras Program Files" SecProgram

  SetOutPath "$INSTDIR"

  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Release\DCSGateway.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Release\DExtraGateway.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Release\ParrotController.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\DCS_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\DExtra_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\dk_DK.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\dk_DK.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\es_ES.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\es_ES.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\fr_FR.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\fr_FR.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\it_IT.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\it_IT.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\no_NO.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\no_NO.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\pl_PL.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\pl_PL.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\Data\se_SE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\RepeaterExtras\CHANGES.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\Repeater Extras"
  CreateShortCut "$SMPROGRAMS\Repeater Extras\DCS Gateway.lnk"              "$INSTDIR\DCSGateway.exe"
  CreateShortCut "$SMPROGRAMS\Repeater Extras\DExtra Gateway.lnk"           "$INSTDIR\DExtraGateway.exe"
  CreateShortCut "$SMPROGRAMS\Repeater Extras\Parrot Controller.lnk"        "$INSTDIR\ParrotController.exe"
  CreateShortCut "$SMPROGRAMS\Repeater Extras\Licence.lnk"                  "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\Repeater Extras\Changes.lnk"                  "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\Repeater Extras\Uninstall.lnk"                "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\Repeater Extras\*.*"
  RMDir  "$SMPROGRAMS\Repeater Extras"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\DCS Gateway"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DExtra Gateway"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Parrot Controller"

SectionEnd
