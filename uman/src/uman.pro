TEMPLATE = app
TARGET = uman
QT += core \
    gui \
    xml
HEADERS += reports/QUBooleanSongData.h \
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
    QUDropDownDelegate.h \
    main.h \
    QUMonty.h \
    QUTagOrderDialog.h \
    QUSongFile.h \
    QUMainWindow.h
SOURCES += reports/QUBooleanSongData.cpp \
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
    QUDropDownDelegate.cpp \
    QUMonty.cpp \
    QUTagOrderDialog.cpp \
    QUSongFile.cpp \
    QUMainWindow.cpp \
    main.cpp
FORMS += QUReportDialog.ui \
    QUProgressDialog.ui \
    QUTextDialog.ui \
    QUTagOrderDialog.ui \
    QUMainWindow.ui
RESOURCES += images/uman.qrc
win32 { 
    RC_FILE = uman.rc
    LIBS += -L"../lib" \
        -ltag
}
INCLUDEPATH += ../include/taglib \
    tasks \
    details \
    songtree \
    reports
CONFIG += debug
QMAKE_EXTRA_TARGETS += revtarget
PRE_TARGETDEPS += version.h
revtarget.target = version.h
revtarget.commands = @echo \
    "const char *revision = \"r$(shell svnversion .)\";" > $$revtarget.target
revtarget.depends = $$SOURCES \
    $$HEADERS \
    $$FORMS
