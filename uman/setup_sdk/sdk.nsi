!include "dialogs\defines.nsh"

XPStyle on
SetCompress off

!define PRODUCTNAME "UltraStar Manager"
!define PRODUCTNAME_SUFFIX "Plugin SDK"
!define FULL_NAME "${PRODUCTNAME} - ${PRODUCTNAME_SUFFIX}"
!define PRODUCTVERSION "1.0.0"
Name "${FULL_NAME} ${PRODUCTVERSION}"

!include "MUI.nsh"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "img\uman_sdk_top.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "img\uman_sdk_top.bmp"

!define MUI_WELCOMEFINISHPAGE_BITMAP "img\uman_sdk_side.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "img\uman_sdk_side.bmp"

!define BASE_REGKEY "Software\HPI\${FULL_NAME}"
!define UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FULL_NAME}"

OutFile "..\setup_sdk\_files\uman-sdk-${PRODUCTVERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${FULL_NAME}"
InstallDirRegKey HKCU "Software\HPI\${FULL_NAME}" ""

;; Vista UAC
RequestExecutionLevel admin

!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCTNAME}\${PRODUCTNAME_SUFFIX}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${BASE_REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu_Folder"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
;; !define MUI_FINISHPAGE_RUN "$INSTDIR\uman.exe"
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

!cd "..\src"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "${PRODUCTNAME}"

Section "Application" SecCopyUI
	;; Dialogs::Folder "Custom Title" "Custom String" "D:\" ${VAR_0}
	;; general support files - not necessary, just for convenience
	SetOutPath "$INSTDIR\support"
	File "QU.h"
	File "QU.cpp"
	File "QUSongSupport.h"
	File "QUSongSupport.cpp"
	File "QUStringSupport.h"
	File "QUStringSupport.cpp"
	;; general plugin interface - most important
	SetOutPath "$INSTDIR"
	File "..\setup_sdk\umanenv.bat"
	File "plugins\QUPlugin.h"
	File "song\QUSongInterface.h"
	;; task plugins
	SetOutPath "$INSTDIR\task"
	File "plugins\QUTaskPlugin.h"
	File "plugins\QUSmartSettingInterface.h"
	;; task plugins - convenience classes (for reusage)
	SetOutPath "$INSTDIR\task\base"
	File "plugins\shared\QUMultilingualTaskFactory.cpp"
	File "plugins\shared\QUMultilingualTaskFactory.h"
	File "plugins\shared\QUSimpleTask.cpp"
	File "plugins\shared\QUSimpleTask.h"
	File "plugins\shared\QUSimpleTaskFactory.cpp"
	File "plugins\shared\QUSimpleTaskFactory.h"
	File "plugins\shared\QUSmartSetting.cpp"
	File "plugins\shared\QUSmartSetting.h"
	File "plugins\shared\QUScriptableTask.cpp"
	File "plugins\shared\QUScriptableTask.h"
	File "plugins\shared\QUScriptableTaskFactory.cpp"
	File "plugins\shared\QUScriptableTaskFactory.h"
	SetOutPath "$INSTDIR\task\extensions"
	File "plugins\shared\QUSmartCheckBox.cpp"
	File "plugins\shared\QUSmartCheckBox.h"
	File "plugins\shared\QUSmartInputCheck.cpp"
	File "plugins\shared\QUSmartInputCheck.h"
	File "plugins\shared\QUSmartInputField.cpp"
	File "plugins\shared\QUSmartInputField.h"
	File "plugins\shared\QUTaskConditionDelegate.cpp"
	File "plugins\shared\QUTaskConditionDelegate.h"
	File "plugins\shared\QUTaskDataTable.cpp"
	File "plugins\shared\QUTaskDataTable.h"
	File "plugins\shared\QUTaskDialog.cpp"
	File "plugins\shared\QUTaskDialog.h"
	File "plugins\shared\QUTaskDialog.ui"
	File "plugins\shared\QUTaskSourceDelegate.cpp"
	File "plugins\shared\QUTaskSourceDelegate.h"
	File "plugins\shared\QUTaskTextDelegate.cpp"
	File "plugins\shared\QUTaskTextDelegate.h"
	;; remote image source (RIS) plugins
	SetOutPath "$INSTDIR\ris"
	File "plugins\QURemoteImageSourcePlugin.h"
	File "plugins\QUCommunicatorInterface.h"
	;; RIS plugins - convenience classes
	SetOutPath "$INSTDIR\ris\base"
	File "plugins\shared2\QUHttpCollector.cpp"
	File "plugins\shared2\QUHttpCollector.h"
	File "plugins\shared2\QUMultilingualImageSource.cpp"
	File "plugins\shared2\QUMultilingualImageSource.h"
	File "plugins\shared2\QURequestUrl.cpp"
	File "plugins\shared2\QURequestUrl.h"
    
    ;; Start Menu
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Open ${PRODUCTNAME_SUFFIX} Folder.lnk" "$INSTDIR"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\UMan SDK Command Prompt.lnk" '$%ComSpec% /K "$INSTDIR\umanenv.bat"'
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall ${PRODUCTNAME_SUFFIX}.lnk" "$INSTDIR\Uninstall.exe"
    !insertmacro MUI_STARTMENU_WRITE_END

    ;; Registry for Windows Uninstall
	WriteRegStr HKLM "${UNINST_KEY}" "DisplayName" "${FULL_NAME}"
    WriteRegStr HKLM "${UNINST_KEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegStr HKLM "${UNINST_KEY}" "InstallLocation" $INSTDIR
	WriteRegStr HKLM "${UNINST_KEY}" "DisplayIcon" ""
    WriteRegStr HKLM "${UNINST_KEY}" "Publisher" "uman Community"
    WriteRegStr HKLM "${UNINST_KEY}" "DisplayVersion" "${PRODUCTVERSION}"
    WriteRegDWORD HKLM "${UNINST_KEY}" "NoModify" 1
    WriteRegDWORD HKLM "${UNINST_KEY}" "NoRepair" 1
	WriteRegStr HKLM "${UNINST_KEY}" "Comment" "${FULL_NAME} ${PRODUCTVERSION}"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
	;; general support files - not necessary, just for convenience
	Delete "$INSTDIR\support\QU.h"
	Delete "$INSTDIR\support\QU.cpp"
	Delete "$INSTDIR\support\QUSongSupport.h"
	Delete "$INSTDIR\support\QUSongSupport.cpp"
	Delete "$INSTDIR\support\QUStringSupport.h"
	Delete "$INSTDIR\support\QUStringSupport.cpp"
	;; general plugin interface - most important
	Delete "$INSTDIR\QUPlugin.h"
	Delete "$INSTDIR\QUSongInterface.h"
	;; task plugins
	Delete "$INSTDIR\task\QUTaskPlugin.h"
	Delete "$INSTDIR\task\QUSmartSettingInterface.h"
	;; task plugins - convenience classes (for reusage)
	Delete "$INSTDIR\task\base\QUMultilingualTaskFactory.cpp"
	Delete "$INSTDIR\task\base\QUMultilingualTaskFactory.h"
	Delete "$INSTDIR\task\base\QUSimpleTask.cpp"
	Delete "$INSTDIR\task\base\QUSimpleTask.h"
	Delete "$INSTDIR\task\base\QUSimpleTaskFactory.cpp"
	Delete "$INSTDIR\task\base\QUSimpleTaskFactory.h"
	Delete "$INSTDIR\task\base\QUSmartSetting.cpp"
	Delete "$INSTDIR\task\base\QUSmartSetting.h"
	Delete "$INSTDIR\task\base\QUScriptableTask.cpp"
	Delete "$INSTDIR\task\base\QUScriptableTask.h"
	Delete "$INSTDIR\task\base\QUScriptableTaskFactory.cpp"
	Delete "$INSTDIR\task\base\QUScriptableTaskFactory.h"
	Delete "$INSTDIR\task\extensions\QUSmartCheckBox.cpp"
	Delete "$INSTDIR\task\extensions\QUSmartCheckBox.h"
	Delete "$INSTDIR\task\extensions\QUSmartInputCheck.cpp"
	Delete "$INSTDIR\task\extensions\QUSmartInputCheck.h"
	Delete "$INSTDIR\task\extensions\QUSmartInputField.cpp"
	Delete "$INSTDIR\task\extensions\QUSmartInputField.h"
	Delete "$INSTDIR\task\extensions\QUTaskConditionDelegate.cpp"
	Delete "$INSTDIR\task\extensions\QUTaskConditionDelegate.h"
	Delete "$INSTDIR\task\extensions\QUTaskDataTable.cpp"
	Delete "$INSTDIR\task\extensions\QUTaskDataTable.h"
	Delete "$INSTDIR\task\extensions\QUTaskDialog.cpp"
	Delete "$INSTDIR\task\extensions\QUTaskDialog.h"
	Delete "$INSTDIR\task\extensions\QUTaskDialog.ui"
	Delete "$INSTDIR\task\extensions\QUTaskSourceDelegate.cpp"
	Delete "$INSTDIR\task\extensions\QUTaskSourceDelegate.h"
	Delete "$INSTDIR\task\extensions\QUTaskTextDelegate.cpp"
	Delete "$INSTDIR\task\extensions\QUTaskTextDelegate.h"
	;; remote image source (RIS) plugins
	Delete "$INSTDIR\ris\QURemoteImageSourcePlugin.h"
	Delete "$INSTDIR\ris\QUCommunicatorInterface.h"
	;; RIS plugins - convenience classes
	Delete "$INSTDIR\ris\base\QUHttpCollector.cpp"
	Delete "$INSTDIR\ris\base\QUHttpCollector.h"
	Delete "$INSTDIR\ris\base\QUMultilingualImageSource.cpp"
	Delete "$INSTDIR\ris\base\QUMultilingualImageSource.h"
	Delete "$INSTDIR\ris\base\QURequestUrl.cpp"
	Delete "$INSTDIR\ris\base\QURequestUrl.h"
	;; all folders
	RMDir "$INSTDIR\support"
	RMDir "$INSTDIR\task\base"
	RMDir "$INSTDIR\task\extensions"
	RMDir "$INSTDIR\task"
	RMDir "$INSTDIR\ris\base"
	RMDir "$INSTDIR\ris"
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\umanenv.bat"
	RMDir "$INSTDIR"

    ;; Start Menu
    !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
	Delete "$SMPROGRAMS\$MUI_TEMP\Open ${PRODUCTNAME_SUFFIX} Folder.lnk"
	Delete "$SMPROGRAMS\$MUI_TEMP\UMan SDK Command Prompt.lnk"
	Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall ${PRODUCTNAME_SUFFIX}.lnk"
    ;Delete empty start menu parent diretories
    StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
    startMenuDeleteLoop:
        ClearErrors
        RMDir $MUI_TEMP
        GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
        IfErrors startMenuDeleteLoopDone
        StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
    startMenuDeleteLoopDone:
    
	DeleteRegKey HKCU "${BASE_REGKEY}"

    ;; Registry for Windows Uninstall
    DeleteRegKey HKLM "${UNINST_KEY}"
SectionEnd
