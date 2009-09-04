SOURCES = ../../QUStringSupport.cpp \
	../../QUSongSupport.cpp \
	QUAmazonImageSource.cpp \
	QUAmazonImageCollector.cpp
HEADERS = ../../QUStringSupport.h \
	../../QUSongSupport.h \
	QUAmazonImageSource.h \
	QUAmazonImageCollector.h
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../shared
TARGET = amazon
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/amazon
MOC_DIR = ../tmp/amazon
UI_DIR = ../ui
QT += xml
