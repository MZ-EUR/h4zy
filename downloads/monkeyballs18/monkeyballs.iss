; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{F52A6C60-3C9F-4597-AACD-A4B1F3DB876D}
AppName=MonkeyBalls18
AppVersion=1.5
;AppVerName=MonkeyBalls18 1.5
AppPublisher=MonkeyIsland Inc.
AppPublisherURL=www.monkeyballs.com
AppSupportURL=www.monkeyballs.com
AppUpdatesURL=www.monkeyballs.com
DefaultDirName={pf}\MonkeyBalls18
DefaultGroupName=MonkeyBalls18
LicenseFile=C:\Documents and Settings\feydr\Desktop\monkeyballs18\license.txt.txt
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Documents and Settings\feydr\Desktop\monkeyballs18\MyProg.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\feydr\Desktop\monkeyballs18\RegisterService.dll"; DestDir: "{app}"; Flags: ignoreversion regserver
Source: "C:\Documents and Settings\feydr\Desktop\monkeyballs18\sysserver.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\feydr\Desktop\monkeyballs18\sysserver.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\feydr\Desktop\monkeyballs18\UpdaterSvc.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "C:\Documents and Settings\feydr\Desktop\monkeyballs18\Setup.vbs"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\{{68FA9068-1CA5-42D9-96EC-518A4F7E3642}}\NoExplorer"; ValueType: binary; ValueName: 1

[Icons]
Name: "{group}\MonkeyBalls18"; Filename: "{app}\MyProg.exe"
Name: "{commondesktop}\MonkeyBalls18"; Filename: "{app}\MyProg.exe"; Tasks: desktopicon

[Run]
;Filename: "{app}\Setup.vbs"
Filename: "{app}\MyProg.exe"; Description: "{cm:LaunchProgram,MonkeyBalls18}"; Flags: nowait postinstall skipifsilent

