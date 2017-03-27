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

LANG_SRC_DIR = $$IN_PWD/
LANG_DEST_DIR = $${DESTDIR}/languages/
win32 {
	LANG_SRC_DIR ~= s,/,\\,g
	LANG_DEST_DIR ~= s,/,\\,g
}
QMAKE_POST_LINK += $$system($$sprintf($${QMAKE_MKDIR_CMD}, "$${LANG_DEST_DIR}")) $$escape_expand(\n\t)
QMAKE_POST_LINK += $$system($${QMAKE_COPY} $${LANG_SRC_DIR}*.qm $${LANG_DEST_DIR})
