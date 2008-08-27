XPStyle on
SetCompress off

!define PRODUCTNAME "UltraStar Manager"
!define PRODUCTVERSION "1.7.0"
Name "${PRODUCTNAME} ${PRODUCTVERSION}"

!include "MUI.nsh"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "img\uman_installer_top.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "img\uman_installer_top.bmp"

!define MUI_WELCOMEFINISHPAGE_BITMAP "img\uman_installer_side.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "img\uman_installer_side.bmp"

!define BASE_REGKEY "Software\HPI\${PRODUCTNAME}"
!define UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCTNAME}"

OutFile "_files\uman-${PRODUCTVERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${PRODUCTNAME}"
InstallDirRegKey HKCU "Software\HPI\${PRODUCTNAME}" ""

;; Vista...
RequestExecutionLevel admin

!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCTNAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${BASE_REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu_Folder"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\changes.txt"
!define MUI_FINISHPAGE_RUN "$INSTDIR\uman.exe"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

Var MUI_TEMP
Var STARTMENU_FOLDER

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\doc\gpl.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

!cd "..\bin\${PRODUCTVERSION}"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "${PRODUCTNAME}"

Section "Application" SecCopyUI
    WriteRegStr HKCU "${BASE_REGKEY}" "" "$INSTDIR"

    ;; Files
    SetOutPath "$INSTDIR"
    File "uman.exe"
    File "changes.txt"
    File "libtag.dll"
    File "mingwm10.dll"
    File "QtCore4.dll"
    File "QtGui4.dll"
    File "QtXml4.dll"
    SetOutPath "$INSTDIR\imageformats"
    File "imageformats\QJpeg4.dll"
    
    ;; Start Menu
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${PRODUCTNAME}.lnk" "$INSTDIR\uman.exe"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${PRODUCTNAME}.lnk" "$INSTDIR\Uninstall.exe"
    !insertmacro MUI_STARTMENU_WRITE_END

    ;; Registry for Windows Uninstall
    WriteRegStr HKLM "${UNINST_KEY}" "DisplayName" "${PRODUCTNAME}"
    WriteRegStr HKLM "${UNINST_KEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegStr HKLM "${UNINST_KEY}" "InstallLocation" $INSTDIR
    WriteRegStr HKLM "${UNINST_KEY}" "DisplayIcon" "$INSTDIR\uman.exe,0"
    WriteRegStr HKLM "${UNINST_KEY}" "Publisher" "uman Community"
    WriteRegStr HKLM "${UNINST_KEY}" "DisplayVersion" "${PRODUCTVERSION}"
    WriteRegDWORD HKLM "${UNINST_KEY}" "NoModify" 1
    WriteRegDWORD HKLM "${UNINST_KEY}" "NoRepair" 1
    WriteRegStr HKLM "${UNINST_KEY}" "Comment" "${PRODUCTNAME} ${PRODUCTVERSION}"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
    ;; Files
    Delete "$INSTDIR\uman.exe"
    Delete "$INSTDIR\changes.txt"
    Delete "$INSTDIR\libtag.dll"
    Delete "$INSTDIR\mingwm10.dll"
    Delete "$INSTDIR\QtCore4.dll"
    Delete "$INSTDIR\QtGui4.dll"
    Delete "$INSTDIR\QtXml4.dll"
    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\imageformats\QJpeg4.dll"
    RMDir "$INSTDIR\imageformats"
    RMDir "$INSTDIR"
    
    ;; Start Menu
    !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall ${PRODUCTNAME}.lnk"
    Delete "$SMPROGRAMS\$MUI_TEMP\${PRODUCTNAME}.lnk"
    ;Delete empty start menu parent diretories
    StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
    startMenuDeleteLoop:
        ClearErrors
        RMDir $MUI_TEMP
        GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
        IfErrors startMenuDeleteLoopDone
        StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
    startMenuDeleteLoopDone:
    
    MessageBox MB_YESNO|MB_ICONQUESTION "Do you want to keep your settings?" /SD IDYES IDYES skipKillReg
    DeleteRegKey HKCU "${BASE_REGKEY}"
    skipKillReg:

    ;; Registry for Windows Uninstall
    DeleteRegKey HKLM "${UNINST_KEY}"
SectionEnd
