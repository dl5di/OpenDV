;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "ircDDB Gateway 20151116"
  OutFile "ircDDBGateway-20151116.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\ircDDBGateway"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\COPYING.txt"
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
  
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\ircDDBGateway.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\ircDDBGatewayConfig.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\APRSTransmit.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\RemoteControl.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\StarNetServer.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\TextTransmit.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\TimerControl.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\TimeServer.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\VoiceTransmit.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\CHANGES.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\CCS_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\DCS_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\DExtra_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\DPlus_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_fr_FR.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_fr_FR.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\TIME_se_SE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\dk_DK.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\dk_DK.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\es_ES.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\es_ES.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\fr_FR.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\fr_FR.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\it_IT.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\it_IT.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\no_NO.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\no_NO.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\pl_PL.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\pl_PL.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv-git\OpenDV\ircDDBGateway\Data\se_SE.indx"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\ircDDBGateway"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\ircDDB Gateway.lnk"        "$INSTDIR\ircDDBGateway.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\ircDDB Gateway Config.lnk" "$INSTDIR\ircDDBGatewayConfig.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\APRS Transmit.lnk"         "$INSTDIR\APRSTransmit.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Remote Control.lnk"        "$INSTDIR\RemoteControl.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\StarNet Server.lnk"        "$INSTDIR\StarNetServer.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Timer Control.lnk"         "$INSTDIR\TimerControl.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Time Server.lnk"           "$INSTDIR\TimeServer.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Changes.lnk"               "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Licence.lnk"               "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Uninstall.lnk"             "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\ircDDBGateway\*.*"
  RMDir  "$SMPROGRAMS\ircDDBGateway"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\IRCDDB Gateway"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\APRS Transmit"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Remote Control"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\StarNet Server"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Timer Control"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Time Server"

SectionEnd
