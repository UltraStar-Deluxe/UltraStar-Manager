SOURCES = ../../QUStringSupport.cpp \
	../../QUSongSupport.cpp \
	QUAmazonImageSource.cpp \
	QUAmazonImageCollector.cpp \
	QUAmazonRequestUrl.cpp \
	QUAmazonResponse.cpp \
	sha2/sha2.c \
	sha2/hmac_sha2.c \
	../shared2/QUMultilingualImageSource.cpp \
	../shared2/QURequestUrl.cpp \
	../shared2/QUHttpCollector.cpp
HEADERS = ../../QUStringSupport.h \
	../../QUSongSupport.h \
	QUAmazonImageSource.h \
	QUAmazonImageCollector.h \
	QUAmazonRequestUrl.h \
	QUAmazonResponse.h \
	sha2/sha2.h \
	sha2/hmac_sha2.h \
	../shared2/QUMultilingualImageSource.h \
	../shared2/QURequestUrl.h \
	../shared2/QUHttpCollector.h
TRANSLATIONS = amazon.de.ts \
	amazon.pl.ts \
	amazon.es.ts
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../shared2 \
	sha2
TEMPLATE = lib
CONFIG += plugin
UI_DIR = ../ui
QT += xml network

CONFIG(release, debug|release) {
	TARGET = amazon
	DESTDIR = ../../../bin/wip/plugins
	OBJECTS_DIR = ../tmp/amazon/release
	MOC_DIR = ../tmp/amazon/release
}

CONFIG(debug, debug|release) {
	TARGET = amazon_debug
	DESTDIR = ../../../bin/wip_debug/plugins
	OBJECTS_DIR = ../tmp/amazon/debug
	MOC_DIR = ../tmp/amazon/debug
}
