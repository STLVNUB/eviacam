;  Copyright (C) 2008 - 2010 Cesar Mauri Loba
;
;   Permission is hereby granted, free of charge, to any person obtaining a
;   copy of this software and associated documentation files (the "Software"),
;   to deal in the Software without restriction, including without limitation
;   the rights to use, copy, modify, merge, publish, distribute, sublicense,
;   and/or sell copies of the Software, and to permit persons to whom the
;   Software is furnished to do so, subject to the following conditions:
;
;   The above copyright notice and this permission notice shall be included in
;   all copies or substantial portions of the Software.
;
;   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
;   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
;   DEALINGS IN THE SOFTWARE.
;
;   Inno Setup installer script
;

#define APPNAME			"Enable Viacam"
#define APPNAME_SHORT	"eViacam"
#define VERSION         "1.2.1"
#define PUBLISHER		"Cesar Mauri - CREA Software Systems"
#define VERTEXT			"v1.2.1"
#define APPBIN			"eViacam.exe"
#define COPYRIGHTY		"� 2008-10"

[Setup]
AppName={#APPNAME}
AppVerName={#APPNAME} {#VERSION}
AppPublisher={#PUBLISHER}
AppCopyright={#COPYRIGHTY} {#PUBLISHER}
AppVersion={#VERSION}
AppUpdatesURL=http://viacam.org
AppPublisherURL=http://www.crea-si.com

VersionInfoTextVersion={#VERTEXT}
VersionInfoCopyright={#COPYRIGHTY} {#PUBLISHER}
VersionInfoVersion={#VERSION}
VersionInfoCompany={#PUBLISHER}
VersionInfoDescription=Installer for {#APPNAME}
VersionInfoProductName={#APPNAME}
VersionInfoProductVersion={#VERSION}

SourceDir=..
DefaultDirName={pf}\{#APPNAME}
UninstallDisplayIcon={app}\bin\{#APPBIN}
DefaultGroupName={#APPNAME}
LicenseFile=COPYING
OutputBaseFilename={#APPNAME_SHORT}-{#VERSION}
PrivilegesRequired=poweruser

MinVersion=0,5.0.2195sp3
UsePreviousSetupType=true
UsePreviousGroup=false
DirExistsWarning=yes
Compression=zip/9
ShowLanguageDialog=yes

AppID={{BF2FC5F6-EC88-4CA5-BD83-DC6613FD077D}
OutputDir=win32
InfoAfterFile=win32\Info\InfoAfter.rtf

[Files]
Source: win32\Release\bin\*; DestDir: {app}\bin; Flags: recursesubdirs ignoreversion
Source: doc\*.png; DestDir: {app}\bin; Flags: ignoreversion
Source: doc\*.wav; DestDir: {app}\bin; Flags: ignoreversion
Source: THANKS; DestDir: {app}\bin; DestName: THANKS
Source: COPYING; DestDir: {app}\doc; DestName: license.txt
Source: doc\*.txt; DestDir: {app}\doc; Flags: recursesubdirs ignoreversion
Source: doc\*.png; DestDir: {app}\doc; Flags: recursesubdirs ignoreversion
Source: README; DestDir: {app}\doc; Flags: ignoreversion; DestName: README.txt
Source: README.ca; DestDir: {app}\doc; Flags: ignoreversion; DestName: README.ca.txt

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}

[Icons]
Name: {group}\{#APPNAME}; Filename: {app}\bin\{#APPBIN}; WorkingDir: {app}\bin
Name: {group}\{cm:UninstallProgram,{#APPNAME}}; Filename: {uninstallexe}
Name: {group}\README; Filename: {app}\doc\README.txt
Name: {group}\README.Catal�; Filename: {app}\doc\README.ca.txt
Name: {userdesktop}\eViacam; Filename: {app}\bin\eViacam.exe; WorkingDir: {app}\bin; Tasks: desktopicon

[Run]
Filename: {app}\doc\README.txt; Flags: postinstall shellexec
;Filename: {app}\doc\README.ca.txt; Flags: postinstall shellexec; Tasks: ; Languages: Catala; Verb: open
;Filename: {app}\bin\{#APPBIN}; WorkingDir: {app}\bin; Flags: postinstall nowait; Tasks: ; Languages: 
;Filename: {app}\bin\fixvfwfps.exe; Flags: runminimized; Tasks: 

[Components]

[Languages]
Name: Catala; MessagesFile: compiler:Languages\Catalan.isl; InfoAfterFile: win32\Info\InfoAfter-ca.rtf
Name: Castellano; MessagesFile: compiler:Languages\Spanish.isl; InfoAfterFile: win32\Info\InfoAfter-es.rtf
Name: Deutsch; MessagesFile: compiler:Languages\German.isl
Name: English; MessagesFile: compiler:Default.isl
Name: Galego; MessagesFile: compiler:Languages\Galician.isl
Name: Italiano; MessagesFile: compiler:Languages\Italian.isl
Name: Turkish; MessagesFile: compiler:Languages\Turkish.isl
Name: Occitan; MessagesFile: compiler:Languages\Occitan.isl
Name: French; MessagesFile: compiler:Languages\French.isl

[Registry]
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: english; Languages: English
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: spanish; Languages: Castellano
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: catalan; Languages: Catala
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: gl; Languages: Galego
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: it; Languages: Italiano
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: tr; Languages: Turkish
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: de; Languages: Deutsch
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: oc; Languages: Occitan
Root: HKCU; Subkey: Software\eviacam; ValueType: string; ValueName: language; ValueData: fr; Languages: French

[_ISTool]
Use7zip=false
UseAbsolutePaths=false