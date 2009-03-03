SOURCES = QUAudioTagTaskFactory.cpp \
    QUAudioTagTask.cpp \
    ../../QUStringSupport.cpp \
    ../../QUSongSupport.cpp \
    ../shared/QUSimpleTask.cpp \
    ../shared/QUScriptableTask.cpp \
    ../shared/QUScriptableTaskFactory.cpp
HEADERS = QUAudioTagTaskFactory.h \
    QUAudioTagTask.h \
    ../../QUStringSupport.h \
    ../../QUSongSupport.h \
    ../shared/QUSimpleTask.h \
    ../shared/QUScriptableTask.h \
    ../shared/QUScriptableTaskFactory.h
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared \
    ../../../include/taglib
TARGET = audiotag
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = tmp
MOC_DIR = tmp
QT += xml
win32 { 
    INCLUDEPATH += ../../../include/taglib
    LIBS += -L"../../../lib" \
        -ltag
}
unix { 
    INCLUDEPATH += /usr/include/taglib
    LIBS += -L"/usr/lib" \
        -ltag
}
