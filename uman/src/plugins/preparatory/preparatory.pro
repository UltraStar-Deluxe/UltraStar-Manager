SOURCES = QUPreparatoryTaskFactory.cpp \
    QUPreparatoryTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartInputField.cpp
HEADERS = QUPreparatoryTaskFactory.h \
    QUPreparatoryTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartInputField.h
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
