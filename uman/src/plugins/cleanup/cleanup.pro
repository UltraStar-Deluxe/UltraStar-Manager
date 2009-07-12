SOURCES = QUCleanUpTaskFactory.cpp \
    QUCleanUpTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartInputCheck.cpp \
	../../QUSongSupport.cpp
HEADERS = QUCleanUpTaskFactory.h \
    QUCleanUpTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartInputCheck.h \
	../../QUSongSupport.h
TRANSLATIONS = cleanup.de.ts \
	cleanup.pl.ts
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared
TARGET = 5-cleanup
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/cleanup
MOC_DIR = ../tmp/cleanup
