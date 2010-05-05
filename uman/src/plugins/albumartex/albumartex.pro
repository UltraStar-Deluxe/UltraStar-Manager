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
UI_DIR = ../ui
QT += xml \
    network

CONFIG(release, debug|release) {
	TARGET = albumartex
	DESTDIR = ../../../bin/wip/plugins
	OBJECTS_DIR = ../tmp/albumartex/release
	MOC_DIR = ../tmp/albumartex/release
}

CONFIG(debug, debug|release) {
	TARGET = albumartex_debug
	DESTDIR = ../../../bin/wip_debug/plugins
	OBJECTS_DIR = ../tmp/albumartex/debug
	MOC_DIR = ../tmp/albumartex/debug
}
