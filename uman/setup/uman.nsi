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
    File "QtNetwork4.dll"
    SetOutPath "$INSTDIR\imageformats"
    File "imageformats\QJpeg4.dll"
    SetOutPath "$INSTDIR\styles"
    File "styles\blue.css"
    SetOutPath "$INSTDIR\task-def"
    File "task-def\001-changePathArtistTitle.xml"
    File "task-def\002-changePathGenreArtistTitle.xml"
    File "task-def\003-changePathEditionArtistTitle.xml"
    File "task-def\004-changePathArtistTitle2.xml"
    File "task-def\005-changePathLanguageArtistTitle.xml"
    File "task-def\006-changePathAlbum.xml"
    File "task-def\010-renameSongDir.xml"
    File "task-def\020-renameSongDirSpecial.xml"
    File "task-def\030-renameSongTxt.xml"
    File "task-def\040-renameSongMp3.xml"
    File "task-def\050-renameSongCover.xml"
    File "task-def\060-renameSongBackground.xml"
    File "task-def\070-renameSongVideo.xml"
    File "task-def\080-renameSongVideoSpecial.xml"
    File "task-def\id3-010-useArtist.xml"
    File "task-def\id3-020-useTitle.xml"
    File "task-def\id3-021-useTitleVideo.xml"
    File "task-def\id3-022-appendVideo.xml"
    File "task-def\id3-023-removeVideoTitle.xml"
    File "task-def\id3-030-useGenre.xml"
    File "task-def\id3-040-useYear.xml"
    File "task-def\id3-050-useAlbum.xml"
    
    ;; setup initial reg values
    WriteRegStr HKCU "Software\HPI\${PRODUCTNAME}" "customTags" "Comment Album"
    WriteRegBin HKCU "Software\HPI\${PRODUCTNAME}" "windowState" 40004200790074006500410072007200610079002800000000000000ff000000000000000000fd0000000000000003000000000000000000000000000000e500000000000100b200fc0002000000000000000100fc000000000000002f00000000000100b2000000000000007a000100000000001500fa00000000000000000001000000000000000200fb000000000000001600000064000000650000007400000061000000690000006c00000073000000440000006f000000630000006b0001000000000000000000ff00ff00ff00ff0000000000000064000000ff00ff00ff00fb000000000000001600000070000000720000006500000076000000690000006500000077000000440000006f000000630000006b0001000000000000000000ff00ff00ff00ff0000000000000046000000ff00ff00ff000000000000000100000000000000e900000000000100b200fc0002000000000000000100fc000000000000002f00000000000100b200000000000000d1000100000000001500fa00000000000000000001000000000000000200fb00000000000000120000007400000061000000730000006b00000073000000440000006f000000630000006b0001000000000000000000ff00ff00ff00ff00000000000000bc000000ff00ff00ff00fb0000000000000018000000700000006c00000061000000790000006c000000690000007300000074000000440000006f000000630000006b000100000000000200e8000000000001000000000000000000e9000000ff00ff00ff000000000000000200000000000300e800ff00ff00ff00fa00fc0001000000000000000100fb00000000000000140000006500000076000000650000006e0000007400000073000000440000006f000000630000006b0002000000000000009c00000000000200ec00000000000300e800000000000000e3000000000002000e00000000000100b2000000000000000400000000000000040000000000000008000000000000000800fc0000000000000005000000000000000000000000000000000000000000000002000000000000000000000000000000020000000000000000000000000000000200000000000000030000000000000010000000730000006f0000006e00000067000000730000004200000061000000720001000000000000000000000000000000c10000000000000000000000000000000000000000000000140000006f0000007000000074000000690000006f0000006e00000073000000420000006100000072000000000000000000c100000000000000a400000000000000000000000000000000000000000000000e00000076000000690000006500000077000000420000006100000072000100000000000000c100000000000300270000000000000000000000000000000000000000000000030000000000000000002900

    ;; remove invalid values
    ;; DeleteRegKey HKCU "Software\HPI\${PRODUCTNAME}\allowedVideoFiles"

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
    Delete "$INSTDIR\QtNetwork4.dll"
    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\imageformats\QJpeg4.dll"
    RMDir "$INSTDIR\imageformats"
    Delete "$INSTDIR\styles\blue.css"
    RMDir "$INSTDIR\styles"
    Delete "$INSTDIR\task-def\001-changePathArtistTitle.xml"
    Delete "$INSTDIR\task-def\002-changePathGenreArtistTitle.xml"
    Delete "$INSTDIR\task-def\003-changePathEditionArtistTitle.xml"
    Delete "$INSTDIR\task-def\004-changePathArtistTitle2.xml"
    Delete "$INSTDIR\task-def\005-changePathLanguageArtistTitle.xml"
    Delete "$INSTDIR\task-def\006-changePathAlbum.xml"
    Delete "$INSTDIR\task-def\010-renameSongDir.xml"
    Delete "$INSTDIR\task-def\020-renameSongDirSpecial.xml"
    Delete "$INSTDIR\task-def\030-renameSongTxt.xml"
    Delete "$INSTDIR\task-def\040-renameSongMp3.xml"
    Delete "$INSTDIR\task-def\050-renameSongCover.xml"
    Delete "$INSTDIR\task-def\060-renameSongBackground.xml"
    Delete "$INSTDIR\task-def\070-renameSongVideo.xml"
    Delete "$INSTDIR\task-def\080-renameSongVideoSpecial.xml"
    Delete "$INSTDIR\task-def\id3-010-useArtist.xml"
    Delete "$INSTDIR\task-def\id3-020-useTitle.xml"
    Delete "$INSTDIR\task-def\id3-021-useTitleVideo.xml"
    Delete "$INSTDIR\task-def\id3-022-appendVideo.xml"
    Delete "$INSTDIR\task-def\id3-023-removeVideoTitle.xml"
    Delete "$INSTDIR\task-def\id3-030-useGenre.xml"
    Delete "$INSTDIR\task-def\id3-040-useYear.xml"
    Delete "$INSTDIR\task-def\id3-050-useAlbum.xml"
    RMDir "$INSTDIR\task-def"
    RMDir /r "$INSTDIR\logs"
    RMDir /r "$INSTDIR\covers"
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
