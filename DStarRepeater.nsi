;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "DStarRepeater 20151012"
  OutFile "DStarRepeater-20151012.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\D-Star Repeater"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\COPYING.txt"
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

  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Release\DStarRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Release\DStarRepeaterConfig.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Release\portaudio_x86.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\WindowsUSB\xDVRPTR-32-64-2.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\WindowsUSB\dvrptr_cdc.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\WindowsUSB\gmsk.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\WindowsUSB\gmsk.cat"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\dk_DK.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\dk_DK.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\es_ES.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\es_ES.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\fr_FR.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\fr_FR.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\it_IT.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\it_IT.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\no_NO.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\no_NO.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\pl_PL.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\pl_PL.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\Data\se_SE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\DStarRepeater\CHANGES.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\D-Star Repeater"
  CreateShortCut "$SMPROGRAMS\D-Star Repeater\D-Star Repeater.lnk"          "$INSTDIR\DStarRepeater.exe"
  CreateShortCut "$SMPROGRAMS\D-Star Repeater\D-Star Repeater Config.lnk"   "$INSTDIR\DStarRepeaterConfig.exe"
  CreateShortCut "$SMPROGRAMS\D-Star Repeater\Licence.lnk"                  "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\D-Star Repeater\Changes.lnk"                  "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\D-Star Repeater\Uninstall.lnk"                "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\D-Star Repeater\*.*"
  RMDir  "$SMPROGRAMS\D-Star Repeater"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\D-Star Repeater"

SectionEnd
