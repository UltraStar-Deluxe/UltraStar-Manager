SOURCES = ../shared2/QUMultilingualImageSource.cpp \
    ../../QUStringSupport.cpp \
    ../../QUSongSupport.cpp \
    QUFreeCoversSource.cpp \
    ../shared2/QURequestUrl.cpp \
    ../shared2/QUHttpCollector.cpp \
    QUFreeCoversCollector.cpp \
    QUFreeCoversRequestUrl.cpp \
    QUFreeCoversResponse.cpp
HEADERS = ../shared2/QUMultilingualImageSource.h \
    ../../QUStringSupport.h \
    ../../QUSongSupport.h \
    QUFreeCoversSource.h \
    ../shared2/QURequestUrl.h \
    ../shared2/QUHttpCollector.h \
    QUFreeCoversCollector.h \
    QUFreeCoversRequestUrl.h \
    QUFreeCoversResponse.h
TRANSLATIONS = freecovers.de.ts \
    freecovers.pl.ts
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared2
TARGET = freecovers
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/freecovers
MOC_DIR = ../tmp/freecovers
UI_DIR = ../ui
QT += xml \
    network
