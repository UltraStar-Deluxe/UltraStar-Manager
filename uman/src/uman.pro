TEMPLATE = app
TARGET = uman
QT += core \
    gui
HEADERS += QUDetailItem.h \
    QUSongItem.h \
    QUSongFile.h \
    QUMainWindow.h
SOURCES += QUDetailItem.cpp \
    QUSongItem.cpp \
    QUSongFile.cpp \
    QUMainWindow.cpp \
    main.cpp
FORMS += QUMainWindow.ui
RESOURCES += images/uman.qrc
win32 { 
    RC_FILE = uman.rc
    LIBS += -L"../lib" \
        -ltag
}
INCLUDEPATH += taglib
