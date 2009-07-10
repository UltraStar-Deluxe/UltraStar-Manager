SOURCES = QUPreparatoryTaskFactory.cpp \
    QUPreparatoryTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp
HEADERS = QUPreparatoryTaskFactory.h \
    QUPreparatoryTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../shared/QUSmartSetting.h \
	../shared/QUSmartCheckBox.h
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
