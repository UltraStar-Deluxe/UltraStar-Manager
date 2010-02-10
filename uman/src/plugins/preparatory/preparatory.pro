SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QUPreparatoryTaskFactory.cpp \
    QUPreparatoryTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartInputField.cpp \
	../../QUSongSupport.cpp
HEADERS = ../shared/QUMultilingualTaskFactory.h \
	QUPreparatoryTaskFactory.h \
    QUPreparatoryTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../shared/QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartInputField.h \
	../../QUSongSupport.h \
	../QUSmartSettingInterface.h
TRANSLATIONS = preparatory.de.ts \
	preparatory.pl.ts \
	preparatory.es.ts
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared
TARGET = 1-preparatory
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/preparatory
MOC_DIR = ../tmp/preparatory
