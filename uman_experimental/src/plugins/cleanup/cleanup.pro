SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QUCleanUpTaskFactory.cpp \
    QUCleanUpTask.cpp \
    ../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartInputCheck.cpp \
	../../QUSongSupport.cpp
HEADERS = ../shared/QUMultilingualTaskFactory.h \
	QUCleanUpTaskFactory.h \
    QUCleanUpTask.h \
    ../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../shared/QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartInputCheck.h \
	../../QUSongSupport.h \
	../QUSmartSettingInterface.h
TRANSLATIONS = cleanup.de.ts \
	cleanup.pl.ts \
	cleanup.es.ts
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared
TEMPLATE = lib
CONFIG += plugin

CONFIG(release, debug|release) {
	TARGET = 5-cleanup
	DESTDIR = ../../../bin/wip/plugins
	OBJECTS_DIR = ../tmp/cleanup/release
	MOC_DIR = ../tmp/cleanup/release
}

CONFIG(debug, debug|release) {
	TARGET = 5-cleanup_debug
	DESTDIR = ../../../bin/wip_debug/plugins
	OBJECTS_DIR = ../tmp/cleanup/debug
	MOC_DIR = ../tmp/cleanup/debug
}
