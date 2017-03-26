XPStyle on

!define PRODUCTNAME "UltraStarManager"
!define PRODUCTVERSION "1.8.4"
Name "${PRODUCTNAME} ${PRODUCTVERSION}"

!include "MUI.nsh"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "img\uman_installer_top_2.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "img\uman_installer_top_2.bmp"

!define MUI_WELCOMEFINISHPAGE_BITMAP "img\uman_installer_side_2.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "img\uman_installer_side_2.bmp"

!define BASE_REGKEY "Software\HPI\${PRODUCTNAME}"
!define UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCTNAME}"

OutFile "_files\uman-${PRODUCTVERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${PRODUCTNAME}"
InstallDirRegKey HKCU "Software\HPI\${PRODUCTNAME}" ""

;; Vista UAC
RequestExecutionLevel admin

!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCTNAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${BASE_REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu_Folder"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\changes.txt"
!define MUI_FINISHPAGE_RUN "$INSTDIR\UltraStarManager.exe"
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
	File "bass.dll"
	File "changes.txt"
	File "libgcc_s_dw2-1.dll"
	File "libstdc++-6.dll"
	File "libtag.dll"
	File "libwinpthread-1.dll"
	File "Qt5Core.dll"
	File "Qt5Gui.dll"
	File "Qt5Network.dll"
	File "Qt5PrintSupport.dll"
	File "Qt5Widgets.dll"
	File "Qt5Xml.dll"
	File "UltraStarManager.exe"
	SetOutPath "$INSTDIR\imageformats"
	File "imageformats\qgif.dll"
	File "imageformats\qjpeg.dll"
	SetOutPath "$INSTDIR\platforms"
	File "platforms\qwindows.dll"
	SetOutPath "$INSTDIR\plugins"
	File "plugins\1-preparatory.dll"
	File "plugins\2-lyric.dll"
	File "plugins\3-audiotag.dll"
	File "plugins\4-rename.dll"
	File "plugins\5-cleanup.dll"
	;;File "plugins\albumartex.dll"
	;;File "plugins\amazon.dll"
	;;File "plugins\freecovers.dll"
	SetOutPath "$INSTDIR\plugins\languages"
	;;File "plugins\languages\albumartex.de.qm"
	;;File "plugins\languages\albumartex.es.qm"
	;;File "plugins\languages\albumartex.fr.qm"
	;;File "plugins\languages\albumartex.pl.qm"
	;;File "plugins\languages\albumartex.pt.qm"
	;;File "plugins\languages\amazon.de.qm"
	;;File "plugins\languages\amazon.es.qm"
	;;File "plugins\languages\amazon.fr.qm"
	;;File "plugins\languages\amazon.pl.qm"
	;;File "plugins\languages\amazon.pt.qm"
	File "plugins\languages\audiotag.de.qm"
	File "plugins\languages\audiotag.es.qm"
	File "plugins\languages\audiotag.fr.qm"
	File "plugins\languages\audiotag.pl.qm"
	File "plugins\languages\audiotag.pt.qm"
	File "plugins\languages\cleanup.de.qm"
	File "plugins\languages\cleanup.es.qm"
	File "plugins\languages\cleanup.fr.qm"
	File "plugins\languages\cleanup.pl.qm"
	File "plugins\languages\cleanup.pt.qm"
	;;File "plugins\languages\freecovers.de.qm"
	;;File "plugins\languages\freecovers.es.qm"
	;;File "plugins\languages\freecovers.fr.qm"
	;;File "plugins\languages\freecovers.pl.qm"
	;;File "plugins\languages\freecovers.pt.qm"
	File "plugins\languages\lyric.de.qm"
	File "plugins\languages\lyric.es.qm"
	File "plugins\languages\lyric.fr.qm"
	File "plugins\languages\lyric.pl.qm"
	File "plugins\languages\lyric.pt.qm"
	File "plugins\languages\preparatory.de.qm"
	File "plugins\languages\preparatory.es.qm"
	File "plugins\languages\preparatory.fr.qm"
	File "plugins\languages\preparatory.pl.qm"
	File "plugins\languages\preparatory.pt.qm"
	File "plugins\languages\rename.de.qm"
	File "plugins\languages\rename.es.qm"
	File "plugins\languages\rename.fr.qm"
	File "plugins\languages\rename.pl.qm"
	File "plugins\languages\rename.pt.qm"
	SetOutPath "$INSTDIR\plugins\config\audiotag"
	File "plugins\config\audiotag\id3-010-useArtist.xml"
	File "plugins\config\audiotag\id3-020-useTitle.xml"
	File "plugins\config\audiotag\id3-021-useTitleVideo.xml"
	File "plugins\config\audiotag\id3-022-appendVideo.xml"
	File "plugins\config\audiotag\id3-023-removeVideoTitle.xml"
	File "plugins\config\audiotag\id3-024-appendKaraoke.xml"
	File "plugins\config\audiotag\id3-030-useGenre.xml"
	File "plugins\config\audiotag\id3-040-useYear.xml"
	File "plugins\config\audiotag\id3-050-useAlbum.xml"
	File "plugins\config\audiotag\id3-060-writeCO.xml"
	File "plugins\config\audiotag\id3-070-writeBG.xml"
	File "plugins\config\audiotag\id3-080-writeVIDEO.xml"
	SetOutPath "$INSTDIR\plugins\config\rename"
	File "plugins\config\rename\001-changePathArtistTitle.xml"
	File "plugins\config\rename\002-changePathGenreArtistTitle.xml"
	File "plugins\config\rename\003-changePathEditionArtistTitle.xml"
	File "plugins\config\rename\004-changePathArtistTitle2.xml"
	File "plugins\config\rename\005-changePathLanguageArtistTitle.xml"
	File "plugins\config\rename\006-changePathAlbum.xml"
	File "plugins\config\rename\010-renameSongDir.xml"
	File "plugins\config\rename\020-renameSongDirSpecial.xml"
	File "plugins\config\rename\030-renameSongTxt.xml"
	File "plugins\config\rename\035-renameSongTxtSpecial.xml"
	File "plugins\config\rename\040-renameSongMp3.xml"
	File "plugins\config\rename\050-renameSongCover.xml"
	File "plugins\config\rename\060-renameSongBackground.xml"
	File "plugins\config\rename\070-renameSongVideo.xml"
	File "plugins\config\rename\080-renameSongVideoSpecial.xml"
	SetOutPath "$INSTDIR\styles"
	File "styles\blue.css"
	File "styles\fabian.css"

	;; setup initial reg values
	;;WriteRegStr HKCU "Software\HPI\${PRODUCTNAME}" "customTags" "Comment Album"

	;; remove old values
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedAudioFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedKaraokeFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedLicenseFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedMidiFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedImageFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedPlaylistFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedScoreFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedSongFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "allowedVideoFiles"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "songTreeState"
	DeleteRegValue HKCU "Software\HPI\${PRODUCTNAME}" "windowState"

	;; Start Menu
	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${PRODUCTNAME}.lnk" "$INSTDIR\UltraStarManager.exe"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${PRODUCTNAME}.lnk" "$INSTDIR\Uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END

	;; Registry for Windows Uninstall
	WriteRegStr HKLM "${UNINST_KEY}" "DisplayName" "${PRODUCTNAME}"
	WriteRegStr HKLM "${UNINST_KEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKLM "${UNINST_KEY}" "InstallLocation" $INSTDIR
	WriteRegStr HKLM "${UNINST_KEY}" "DisplayIcon" "$INSTDIR\UltraStarManager.exe,0"
	WriteRegStr HKLM "${UNINST_KEY}" "Publisher" "uman Community"
	WriteRegStr HKLM "${UNINST_KEY}" "DisplayVersion" "${PRODUCTVERSION}"
	WriteRegDWORD HKLM "${UNINST_KEY}" "NoModify" 1
	WriteRegDWORD HKLM "${UNINST_KEY}" "NoRepair" 1
	WriteRegStr HKLM "${UNINST_KEY}" "Comment" "${PRODUCTNAME} ${PRODUCTVERSION}"
	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
	;; Files
	RMDir /r "$INSTDIR\covers"
	
	Delete "$INSTDIR\imageformats\qgif.dll"
	Delete "$INSTDIR\imageformats\qjpeg.dll"
	RMDir "$INSTDIR\imageformats"
	
	RMDir /r "$INSTDIR\logs"
	
	Delete "$INSTDIR\platforms\qwindows.dll"
	RMDir "$INSTDIR\platforms"
	
	;;Delete "$INSTDIR\plugins\languages\albumartex.de.qm"
	;;Delete "$INSTDIR\plugins\languages\albumartex.es.qm"
	;;Delete "$INSTDIR\plugins\languages\albumartex.fr.qm"
	;;Delete "$INSTDIR\plugins\languages\albumartex.pl.qm"
	;;Delete "$INSTDIR\plugins\languages\albumartex.pt.qm"
	;;Delete "$INSTDIR\plugins\languages\amazon.de.qm"
	;;Delete "$INSTDIR\plugins\languages\amazon.es.qm"
	;;Delete "$INSTDIR\plugins\languages\amazon.fr.qm"
	;;Delete "$INSTDIR\plugins\languages\amazon.pl.qm"
	;;Delete "$INSTDIR\plugins\languages\amazon.pt.qm"
	Delete "$INSTDIR\plugins\languages\audiotag.de.qm"
	Delete "$INSTDIR\plugins\languages\audiotag.es.qm"
	Delete "$INSTDIR\plugins\languages\audiotag.fr.qm"
	Delete "$INSTDIR\plugins\languages\audiotag.pl.qm"
	Delete "$INSTDIR\plugins\languages\audiotag.pt.qm"
	Delete "$INSTDIR\plugins\languages\cleanup.de.qm"
	Delete "$INSTDIR\plugins\languages\cleanup.es.qm"
	Delete "$INSTDIR\plugins\languages\cleanup.fr.qm"
	Delete "$INSTDIR\plugins\languages\cleanup.pl.qm"
	Delete "$INSTDIR\plugins\languages\cleanup.pt.qm"
	;;Delete "$INSTDIR\plugins\languages\freecovers.de.qm"
	;;Delete "$INSTDIR\plugins\languages\freecovers.es.qm"
	;;Delete "$INSTDIR\plugins\languages\freecovers.fr.qm"
	;;Delete "$INSTDIR\plugins\languages\freecovers.pl.qm"
	;;Delete "$INSTDIR\plugins\languages\freecovers.pt.qm"
	Delete "$INSTDIR\plugins\languages\lyric.de.qm"
	Delete "$INSTDIR\plugins\languages\lyric.es.qm"
	Delete "$INSTDIR\plugins\languages\lyric.fr.qm"
	Delete "$INSTDIR\plugins\languages\lyric.pl.qm"
	Delete "$INSTDIR\plugins\languages\lyric.pt.qm"
	Delete "$INSTDIR\plugins\languages\preparatory.de.qm"
	Delete "$INSTDIR\plugins\languages\preparatory.es.qm"
	Delete "$INSTDIR\plugins\languages\preparatory.fr.qm"
	Delete "$INSTDIR\plugins\languages\preparatory.pl.qm"
	Delete "$INSTDIR\plugins\languages\preparatory.pt.qm"
	Delete "$INSTDIR\plugins\languages\rename.de.qm"
	Delete "$INSTDIR\plugins\languages\rename.es.qm"
	Delete "$INSTDIR\plugins\languages\rename.fr.qm"
	Delete "$INSTDIR\plugins\languages\rename.pl.qm"
	Delete "$INSTDIR\plugins\languages\rename.pt.qm"
	RMDir "$INSTDIR\plugins\languages"
	Delete "$INSTDIR\plugins\config\audiotag\id3-010-useArtist.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-020-useTitle.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-021-useTitleVideo.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-022-appendVideo.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-023-removeVideoTitle.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-024-appendKaraoke.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-030-useGenre.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-040-useYear.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-050-useAlbum.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-060-writeCO.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-070-writeBG.xml"
	Delete "$INSTDIR\plugins\config\audiotag\id3-080-writeVIDEO.xml"
	RMDir "$INSTDIR\plugins\config\audiotag"
	Delete "$INSTDIR\plugins\config\rename\001-changePathArtistTitle.xml"
	Delete "$INSTDIR\plugins\config\rename\002-changePathGenreArtistTitle.xml"
	Delete "$INSTDIR\plugins\config\rename\003-changePathEditionArtistTitle.xml"
	Delete "$INSTDIR\plugins\config\rename\004-changePathArtistTitle2.xml"
	Delete "$INSTDIR\plugins\config\rename\005-changePathLanguageArtistTitle.xml"
	Delete "$INSTDIR\plugins\config\rename\006-changePathAlbum.xml"
	Delete "$INSTDIR\plugins\config\rename\010-renameSongDir.xml"
	Delete "$INSTDIR\plugins\config\rename\020-renameSongDirSpecial.xml"
	Delete "$INSTDIR\plugins\config\rename\030-renameSongTxt.xml"
	Delete "$INSTDIR\plugins\config\rename\035-renameSongTxtSpecial.xml"
	Delete "$INSTDIR\plugins\config\rename\040-renameSongMp3.xml"
	Delete "$INSTDIR\plugins\config\rename\050-renameSongCover.xml"
	Delete "$INSTDIR\plugins\config\rename\060-renameSongBackground.xml"
	Delete "$INSTDIR\plugins\config\rename\070-renameSongVideo.xml"
	Delete "$INSTDIR\plugins\config\rename\080-renameSongVideoSpecial.xml"
	RMDir "$INSTDIR\plugins\config\rename"
	RMDir "$INSTDIR\plugins\config"
	Delete "$INSTDIR\plugins\1-preparatory.dll"
	Delete "$INSTDIR\plugins\2-lyric.dll"
	Delete "$INSTDIR\plugins\3-audiotag.dll"
	Delete "$INSTDIR\plugins\4-rename.dll"
	Delete "$INSTDIR\plugins\5-cleanup.dll"
	;;Delete "$INSTDIR\plugins\albumartex.dll"
	;;Delete "$INSTDIR\plugins\amazon.dll"
	;;Delete "$INSTDIR\plugins\freecovers.dll"
	RMDir "$INSTDIR\plugins"

	Delete "$INSTDIR\styles\blue.css"
	Delete "$INSTDIR\styles\fabian.css"
	RMDir "$INSTDIR\styles"
	
	Delete "$INSTDIR\bass.dll"
	Delete "$INSTDIR\changes.txt"
	Delete "$INSTDIR\libgcc_s_dw2-1.dll"
	Delete "$INSTDIR\libstdc++-6.dll"
	Delete "$INSTDIR\libtag.dll"
	Delete "$INSTDIR\libwinpthread-1.dll"
	Delete "$INSTDIR\Qt5Core.dll"
	Delete "$INSTDIR\Qt5Gui.dll"
	Delete "$INSTDIR\Qt5Network.dll"
	Delete "$INSTDIR\Qt5PrintSupport.dll"
	Delete "$INSTDIR\Qt5Widgets.dll"
	Delete "$INSTDIR\Qt5Xml.dll"
	Delete "$INSTDIR\UltraStarManager.exe"

	Delete "$INSTDIR\Uninstall.exe"

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
