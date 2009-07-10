SOURCES = QULyricTaskFactory.cpp \
    QULyricTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../shared/QUSmartSetting.cpp
HEADERS = QULyricTaskFactory.h \
    QULyricTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../shared/QUSmartSetting.h
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared
TARGET = 2-lyric
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/lyric
MOC_DIR = ../tmp/lyric
