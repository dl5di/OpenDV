;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "Analogue Repeater 20150308"
  OutFile "AnalogueRepeater-20150308.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\Analogue Repeater"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\COPYING.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Analogue Repeater Program Files" SecProgram

  SetOutPath "$INSTDIR"

  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\Release\AnalogueRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\Release\portaudio_x86.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\AnalogueRepeater\CHANGES.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\Analogue Repeater"
  CreateShortCut "$SMPROGRAMS\Analogue Repeater\Analogue Repeater.lnk"        "$INSTDIR\AnalogueRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Analogue Repeater\Licence.lnk"                  "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\Analogue Repeater\Changes.lnk"                  "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\Analogue Repeater\Uninstall.lnk"                "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\Analogue Repeater\*.*"
  RMDir  "$SMPROGRAMS\Analogue Repeater"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\Analogue Repeater"

SectionEnd
