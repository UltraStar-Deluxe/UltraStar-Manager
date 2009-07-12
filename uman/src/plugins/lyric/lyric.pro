SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QULyricTaskFactory.cpp \
    QULyricTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartInputField.cpp
HEADERS = ../shared/QUMultilingualTaskFactory.h \
	QULyricTaskFactory.h \
    QULyricTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartInputField.h
TRANSLATIONS = lyric.de.ts \
	lyric.pl.ts
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
