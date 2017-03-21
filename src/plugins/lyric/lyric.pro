SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QULyricTaskFactory.cpp \
	QULyricTask.cpp \
	../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartInputField.cpp
HEADERS = ../shared/QUMultilingualTaskFactory.h \
	QULyricTaskFactory.h \
	QULyricTask.h \
	../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../shared/QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartInputField.h \
	../QUSmartSettingInterface.h
OTHER_FILES += lyric.json
TRANSLATIONS = lyric.de.ts \
	lyric.pl.ts \
	lyric.es.ts \
	lyric.fr.ts \
	lyric.pt.ts
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../shared

TEMPLATE = lib
CONFIG += plugin
QT += widgets

CONFIG(release, debug|release) {
	TARGET = 2-lyric
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/lyric/release
	MOC_DIR = ../tmp/lyric/release
}

CONFIG(debug, debug|release) {
	TARGET = 2-lyric_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/lyric/debug
	MOC_DIR = ../tmp/lyric/debug
}
unix {
	QMAKE_POST_LINK += $${QMAKE_MKDIR} $${DESTDIR}/languages/ $$escape_expand(\n\t)
	QMAKE_POST_LINK += $${QMAKE_COPY} $${PWD}/*.qm $${DESTDIR}/languages/
}
