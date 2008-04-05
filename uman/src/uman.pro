TEMPLATE = app
TARGET = uman
QT += core \
    gui
HEADERS += QUDropDownDelegate.h \
    main.h \
    QUMonty.h \
    QUTagOrderDialog.h \
    QUDetailItem.h \
    QUSongItem.h \
    QUSongFile.h \
    QUMainWindow.h
SOURCES += QUDropDownDelegate.cpp \
    QUMonty.cpp \
    QUTagOrderDialog.cpp \
    QUDetailItem.cpp \
    QUSongItem.cpp \
    QUSongFile.cpp \
    QUMainWindow.cpp \
    main.cpp
FORMS += QUTagOrderDialog.ui \
    QUMainWindow.ui
RESOURCES += images/uman.qrc
win32 { 
    RC_FILE = uman.rc
    LIBS += -L"../lib" \
        -ltag
}
INCLUDEPATH += ../include/taglib
