TEMPLATE = app
TARGET = uman
QT += core \
    gui \
    xml \
    network
CONFIG += debug
HEADERS += tasks/QULyricTask.h \
    QUSongLine.h \
    monty/QUMontyArea.h \
    QUAboutDialog.h \
    tasks/tasks.h \
    filter/QUFilterArea.h \
    metaphone/QUMetaphoneString.h \
    amazon/QUCoverItemDelegate.h \
    amazon/QUCoverModel.h \
    amazon/QUAmazonResponse.h \
    amazon/QUAmazonRequestUrl.h \
    amazon/QUAmazonDialog.h \
    amazon/QUCoverList.h \
    amazon/QUCoverGroup.h \
    tasks/taskDialog/QUDefaultDelegate.h \
    tasks/taskDialog/QUAudioTagTaskDialog.h \
    tasks/taskDialog/QUTaskSourceDelegate.h \
    tasks/taskDialog/QUTaskTextDelegate.h \
    tasks/taskDialog/QUTaskConditionDelegate.h \
    tasks/taskDialog/QUTaskDataTable.h \
    tasks/taskDialog/QUTaskDialog.h \
    tasks/taskDialog/QURenameTaskDialog.h \
    tasks/QUScriptableTask.h \
    songtree/QUColumnAction.h \
    reports/QUSongFileData.h \
    QUCustomTagsDialog.h \
    playlist/QUPlaylistArea.h \
    playlist/QUPlaylistFile.h \
    playlist/QUPlaylistEntry.h \
    playlist/QUPlayListItem.h \
    playlist/QUPlayList.h \
    details/QUDropDownDelegate.h \
    preview/QUPreviewTree.h \
    QUPictureDialog.h \
    tasks/QUCleanTask.h \
    QUMessageBox.h \
    reports/QUPlainTextReport.h \
    reports/QUBooleanSongData.h \
    reports/QUAbstractReport.h \
    reports/QUHtmlReport.h \
    reports/QUReportItem.h \
    reports/QUSongTagData.h \
    reports/QUAbstractReportData.h \
    QUReportDialog.h \
    songtree/QUSongTree.h \
    songtree/QUSongItem.h \
    QUProgressDialog.h \
    details/QUTagItem.h \
    details/QUDetailsTable.h \
    details/QUDetailItem.h \
    tasks/QUPreparatoryTask.h \
    tasks/QURenameTask.h \
    tasks/QUTaskList.h \
    tasks/QUTaskItem.h \
    tasks/QUAudioTagTask.h \
    tasks/QUAbstractTask.h \
    QU.h \
    QUTextDialog.h \
    main.h \
    QUMonty.h \
    QUTagOrderDialog.h \
    QUSongFile.h \
    QUMainWindow.h
SOURCES += tasks/QULyricTask.cpp \
    QUSongLine.cpp \
    monty/QUMontyArea.cpp \
    QUAboutDialog.cpp \
    filter/QUFilterArea.cpp \
    metaphone/QUMetaphoneString.cpp \
    amazon/QUCoverItemDelegate.cpp \
    amazon/QUCoverModel.cpp \
    amazon/QUAmazonResponse.cpp \
    amazon/QUAmazonRequestUrl.cpp \
    amazon/QUAmazonDialog.cpp \
    amazon/QUCoverList.cpp \
    amazon/QUCoverGroup.cpp \
    tasks/taskDialog/QUDefaultDelegate.cpp \
    tasks/taskDialog/QUAudioTagTaskDialog.cpp \
    tasks/taskDialog/QUTaskSourceDelegate.cpp \
    tasks/taskDialog/QUTaskTextDelegate.cpp \
    tasks/taskDialog/QUTaskConditionDelegate.cpp \
    tasks/taskDialog/QUTaskDataTable.cpp \
    tasks/taskDialog/QUTaskDialog.cpp \
    tasks/taskDialog/QURenameTaskDialog.cpp \
    QU.cpp \
    tasks/QUScriptableTask.cpp \
    songtree/QUColumnAction.cpp \
    reports/QUSongFileData.cpp \
    QUCustomTagsDialog.cpp \
    playlist/QUPlaylistArea.cpp \
    playlist/QUPlaylistFile.cpp \
    playlist/QUPlaylistEntry.cpp \
    playlist/QUPlayListItem.cpp \
    playlist/QUPlayList.cpp \
    details/QUDropDownDelegate.cpp \
    preview/QUPreviewTree.cpp \
    QUPictureDialog.cpp \
    tasks/QUCleanTask.cpp \
    QUMessageBox.cpp \
    reports/QUPlainTextReport.cpp \
    reports/QUBooleanSongData.cpp \
    reports/QUAbstractReport.cpp \
    reports/QUHtmlReport.cpp \
    reports/QUReportItem.cpp \
    reports/QUSongTagData.cpp \
    reports/QUAbstractReportData.cpp \
    QUReportDialog.cpp \
    songtree/QUSongTree.cpp \
    songtree/QUSongItem.cpp \
    QUProgressDialog.cpp \
    details/QUTagItem.cpp \
    details/QUDetailsTable.cpp \
    details/QUDetailItem.cpp \
    tasks/QUPreparatoryTask.cpp \
    tasks/QURenameTask.cpp \
    tasks/QUAudioTagTask.cpp \
    tasks/QUTaskList.cpp \
    tasks/QUTaskItem.cpp \
    tasks/QUAbstractTask.cpp \
    QUTextDialog.cpp \
    QUMonty.cpp \
    QUTagOrderDialog.cpp \
    QUSongFile.cpp \
    QUMainWindow.cpp \
    main.cpp
FORMS += QUMontyArea.ui \
    QUAboutDialog.ui \
    QUFilterArea.ui \
    QUCoverGroup.ui \
    QUAmazonArea.ui \
    QUTaskDialog.ui \
    QUCustomTagsDialog.ui \
    QUPlaylistArea.ui \
    QUPictureDialog.ui \
    QUMessageBox.ui \
    QUReportDialog.ui \
    QUProgressDialog.ui \
    QUTextDialog.ui \
    QUTagOrderDialog.ui \
    QUMainWindow.ui
RESOURCES += images/uman.qrc
TRANSLATIONS = uman_de.ts \
    uman_pl.ts
INCLUDEPATH += tasks \
    details \
    songtree \
    reports \
    tasks/taskDialog \
    preview \
    playlist \
    amazon \
    metaphone \
    filter \
    monty
win32 { 
    RC_FILE = uman.rc
    INCLUDEPATH += ../include/taglib
    LIBS += -L"../lib" \
        -ltag
}
unix { 
    INCLUDEPATH += /usr/include/taglib
    LIBS += -L"/usr/lib" \
        -ltag
}
QMAKE_EXTRA_TARGETS += revtarget
PRE_TARGETDEPS += version.h
revtarget.target = version.h
revtarget.commands = @echo \
    "const char *revision = \"r$(shell svnversion .)\"; \
    const \
    char \
    *date_time \
    = \
    \"$(shell date /T)$(shell time /T)\";" > $$revtarget.target \
&&python getTaskText.py
revtarget.depends = $$SOURCES \
    $$HEADERS \
    $$FORMS
