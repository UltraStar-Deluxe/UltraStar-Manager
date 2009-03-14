TEMPLATE = app
TARGET = uman
DESTDIR = ../bin/wip
MOC_DIR = tmp
UI_DIR = ui
OBJECTS_DIR = tmp
RCC_DIR = tmp
QT += core \
    gui \
    xml \
    network
CONFIG -= debug
HEADERS += plugins/QUPluginDialog.h \
    plugins/QUTaskPlugin.h \
    plugins/QUTaskFactoryProxy.h \
    main.h \
    QU.h \
    QUMonty.h \
    QUAboutDialog.h \
    QUMainWindow.h \
    QUStringSupport.h \
    QUSongSupport.h \
    song/QUSongInterface.h \
    song/QUSongFile.h \
    song/QUSongLine.h \
    mediaplayer/QUAutoCue.h \
    mediaplayer/QUMediaPlayer.h \
    support/QULogService.h \
    support/QUMessageBox.h \
    support/QUProgressDialog.h \
    support/QUTextDialog.h \
    support/QUPictureDialog.h \
    monty/QUMontyArea.h \
    filter/QUFilterArea.h \
    filter/QUMetaphoneString.h \
    amazon/QUCoverItemDelegate.h \
    amazon/QUCoverModel.h \
    amazon/QUAmazonResponse.h \
    amazon/QUAmazonRequestUrl.h \
    amazon/QUAmazonDialog.h \
    amazon/QUCoverList.h \
    amazon/QUCoverGroup.h \
    preferences/QUCustomTagsDialog.h \
    preferences/QUTagOrderDialog.h \
    playlist/QUPlaylistArea.h \
    playlist/QUPlaylistFile.h \
    playlist/QUPlaylistEntry.h \
    playlist/QUPlayListItem.h \
    playlist/QUPlayList.h \
    preview/QUPreviewTree.h \
    report/QUSongFileData.h \
    report/QUPlainTextReport.h \
    report/QUBooleanSongData.h \
    report/QUAbstractReport.h \
    report/QUHtmlReport.h \
    report/QUReportItem.h \
    report/QUSongTagData.h \
    report/QUAbstractReportData.h \
    report/QUReportDialog.h \
    songtree/QUColumnAction.h \
    songtree/QUSongTree.h \
    songtree/QUSongItem.h \
    songdetails/QUTagItem.h \
    songdetails/QUDetailsTable.h \
    songdetails/QUDetailItem.h \
    songdetails/QUDropDownDelegate.h \
    tasks/QUTaskList.h \
    tasks/QUTaskItem.h
SOURCES += plugins/QUPluginDialog.cpp \
    plugins/QUTaskFactoryProxy.cpp \
    main.cpp \
    QU.cpp \
    QUAboutDialog.cpp \
    QUMainWindow.cpp \
    QUMonty.cpp \
    QUStringSupport.cpp \
    QUSongSupport.cpp \
    mediaplayer/QUAutoCue.cpp \
    mediaplayer/QUMediaPlayer.cpp \
    support/QULogService.cpp \
    support/QUProgressDialog.cpp \
    support/QUTextDialog.cpp \
    support/QUPictureDialog.cpp \
    support/QUMessageBox.cpp \
    monty/QUMontyArea.cpp \
    filter/QUFilterArea.cpp \
    filter/QUMetaphoneString.cpp \
    amazon/QUCoverItemDelegate.cpp \
    amazon/QUCoverModel.cpp \
    amazon/QUAmazonResponse.cpp \
    amazon/QUAmazonRequestUrl.cpp \
    amazon/QUAmazonDialog.cpp \
    amazon/QUCoverList.cpp \
    amazon/QUCoverGroup.cpp \
    preferences/QUCustomTagsDialog.cpp \
    preferences/QUTagOrderDialog.cpp \
    playlist/QUPlaylistArea.cpp \
    playlist/QUPlaylistFile.cpp \
    playlist/QUPlaylistEntry.cpp \
    playlist/QUPlayListItem.cpp \
    playlist/QUPlayList.cpp \
    preview/QUPreviewTree.cpp \
    report/QUSongFileData.cpp \
    report/QUPlainTextReport.cpp \
    report/QUBooleanSongData.cpp \
    report/QUAbstractReport.cpp \
    report/QUHtmlReport.cpp \
    report/QUReportItem.cpp \
    report/QUSongTagData.cpp \
    report/QUAbstractReportData.cpp \
    report/QUReportDialog.cpp \
    songtree/QUColumnAction.cpp \
    songtree/QUSongTree.cpp \
    songtree/QUSongItem.cpp \
    songdetails/QUDropDownDelegate.cpp \
    songdetails/QUTagItem.cpp \
    songdetails/QUDetailsTable.cpp \
    songdetails/QUDetailItem.cpp \
    song/QUSongLine.cpp \
    song/QUSongFile.cpp \
    tasks/QUTaskList.cpp \
    tasks/QUTaskItem.cpp
FORMS += plugins/QUPluginDialog.ui \
    QUMainWindow.ui \
    QUAboutDialog.ui \
    mediaplayer/QUMediaPlayer.ui \
    monty/QUMontyArea.ui \
    filter/QUFilterArea.ui \
    amazon/QUCoverGroup.ui \
    amazon/QUAmazonArea.ui \
    playlist/QUPlaylistArea.ui \
    report/QUReportDialog.ui \
    support/QUPictureDialog.ui \
    support/QUMessageBox.ui \
    support/QUProgressDialog.ui \
    support/QUTextDialog.ui \
    preferences/QUTagOrderDialog.ui \
    preferences/QUCustomTagsDialog.ui
RESOURCES += resources/uman.qrc
TRANSLATIONS = resources/lang/uman_de.ts \
    resources/lang/uman_pl.ts
INCLUDEPATH += . \
    song \
    songdetails \
    songtree \
    report \
    tasks \
    tasks/taskDialog \
    preview \
    playlist \
    amazon \
    filter \
    monty \
    mediaplayer \
    support \
    preferences \
    ui \
    plugins
win32 { 
    RC_FILE = uman.rc
    INCLUDEPATH += ../include/taglib \
        ../include/bass
    LIBS += -L"../lib" \
        -ltag \
        -lbass
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
