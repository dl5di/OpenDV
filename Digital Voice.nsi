;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "Digital Voice - 20130219"
  OutFile "Digital_Voice-20130219.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\Digital Voice"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\COPYING.txt"
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
  
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\DCSClient.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\DExtraClient.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\DVAPClient.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\DVRPTRClient.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\DVToolReader.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\GMSKClient.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\SoundCardClient.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\portaudio_x86.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\WindowsUSB\dvrptr_cdc.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\WindowsUSB\gmsk.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\WindowsUSB\gmsk.cat"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\WindowsUSB\xDVRPTR-32-64-2.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\DCSClient\DCS_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\DExtraClient\DExtra_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\CHANGES.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Digital Voice\USAGE.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\Digital Voice"
  CreateShortCut "$SMPROGRAMS\Digital Voice\DCS Client.lnk"            "$INSTDIR\DCSClient.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\DExtra Client.lnk"         "$INSTDIR\DExtraClient.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\DVAP Client.lnk"           "$INSTDIR\DVAPClient.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\DV-RPTR Client.lnk"        "$INSTDIR\DVRPTRClient.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\DVTool Reader.lnk"         "$INSTDIR\DVToolReader.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\GMSK Client.lnk"           "$INSTDIR\GMSKClient.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\Sound Card Client.lnk"     "$INSTDIR\SoundCardClient.exe"
  CreateShortCut "$SMPROGRAMS\Digital Voice\Licence.lnk"               "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\Digital Voice\Changes.lnk"               "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\Digital Voice\Usage.lnk"                 "$INSTDIR\USAGE.txt"
  CreateShortCut "$SMPROGRAMS\Digital Voice\Uninstall.lnk"             "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\Digital Voice\*.*"
  RMDir  "$SMPROGRAMS\Digital Voice"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\DCS Client"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DExtra Client"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DVAP Client"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DVRPTR Client"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DVTool Reader"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\GMSK Client"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Sound Card Client"

SectionEnd
