TEMPLATE = app
TARGET = uman
QT += core \
    gui
HEADERS += details/QUTagItem.h \
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
    QUSongItem.h \
    QUSongFile.h \
    QUMainWindow.h
SOURCES += details/QUTagItem.cpp \
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
    QUSongItem.cpp \
    QUSongFile.cpp \
    QUMainWindow.cpp \
    main.cpp
FORMS += QUTextDialog.ui \
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
    details
