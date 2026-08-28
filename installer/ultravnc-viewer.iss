; Minimal, viewer-only Inno Setup script for UltraVNC's vncviewer.
;
; Deliberately NOT derived from upstream's UltraVNC_installer_x64.iss/_x86.iss — those
; package the entire suite (server, drivers, repeater, MS-Logon helpers) via a
; Components-gated installer. This one installs nothing but the unmodified vncviewer.exe
; produced by this repo's build, so it stays scoped to "ultra-vncviewer" only.
;
; All build-specific inputs are passed in via /D command-line defines so this script
; doesn't need to know the CI working-directory layout:
;   iscc /DMyAppVersion=1.9.0.0 /DSourceExe="C:\path\to\vncviewer.exe" ^
;        /DSourceIcon="C:\path\to\UltraVNC.ico" /O"." /F"ultra-vncviewer" ^
;        installer\ultravnc-viewer.iss

#ifndef MyAppVersion
  #define MyAppVersion "0.0.0.0"
#endif
#ifndef SourceExe
  #define SourceExe "ultra-vncviewer.exe"
#endif
#ifndef SourceIcon
  #define SourceIcon ""
#endif

#define MyAppName "UltraVNC Viewer"
#define MyAppPublisher "UltraVNC Team"
#define MyAppURL "https://uvnc.com/"
#define MyAppExeName "ultra-vncviewer.exe"

[Setup]
AppId={{6F2D9C2E-6C3B-4B8A-9C2E-ULTRAVNCVIEW}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
DefaultDirName={autopf}\UltraVNC Viewer
DefaultGroupName=UltraVNC Viewer
DisableProgramGroupPage=yes
OutputBaseFilename=ultra-vncviewer
Compression=lzma2/ultra
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
#if SourceIcon != ""
SetupIconFile={#SourceIcon}
#endif
WizardStyle=modern
DisableWelcomePage=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#SourceExe}"; DestDir: "{app}"; DestName: "{#MyAppExeName}"; Flags: ignoreversion

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch {#MyAppName}"; Flags: nowait postinstall skipifsilent
