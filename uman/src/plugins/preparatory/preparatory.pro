SOURCES = QUPreparatoryTaskFactory.cpp \
    QUPreparatoryTask.cpp \
    ../shared/QUSimpleTask.cpp \
    ../shared/QUSimpleTaskFactory.cpp
HEADERS = QUPreparatoryTaskFactory.h \
    QUPreparatoryTask.h \
    ../shared/QUSimpleTask.h \
    ../shared/QUSimpleTaskFactory.h
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
OBJECTS_DIR = tmp
MOC_DIR = tmp
