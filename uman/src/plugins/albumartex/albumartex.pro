SOURCES = ../shared2/QUMultilingualImageSource.cpp \
    ../../QUStringSupport.cpp \
    ../../QUSongSupport.cpp \
    QUAlbumArtExImageSource.cpp \
    ../shared2/QURequestUrl.cpp \
    ../shared2/QUHttpCollector.cpp \
    QUAlbumArtExCollector.cpp \
    QUAlbumArtExRequestUrl.cpp
HEADERS = ../shared2/QUMultilingualImageSource.h \
    ../../QUStringSupport.h \
    ../../QUSongSupport.h \
    QUAlbumArtExImageSource.h \
    ../shared2/QURequestUrl.h \
    ../shared2/QUHttpCollector.h \
    QUAlbumArtExCollector.h \
    QUAlbumArtExRequestUrl.h
TRANSLATIONS = albumartex.de.ts \
	albumartex.pl.ts \
	albumartex.es.ts
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared2
TARGET = albumartex
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/albumartex
MOC_DIR = ../tmp/albumartex
UI_DIR = ../ui
QT += xml \
    network
