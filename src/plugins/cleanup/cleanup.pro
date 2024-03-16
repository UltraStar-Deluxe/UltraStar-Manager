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
OTHER_FILES += cleanup.json
TRANSLATIONS = cleanup.de.ts \
	cleanup.pl.ts \
	cleanup.es.ts \
	cleanup.fr.ts \
	cleanup.pt.ts
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../../../include/taglib \
	../shared
TEMPLATE = lib
CONFIG += plugin
QT += widgets

win32 {
	LIBS += -L"../../../lib/win64" \
		-llibtag
}
mac {
	LIBS += -L"/usr/local/lib" \
		-ltag
}
unix:!macx {
	LIBS += -ltag
}

CONFIG(release, debug|release) {
	TARGET = 5-cleanup
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/cleanup/release
	MOC_DIR = ../tmp/cleanup/release
}

CONFIG(debug, debug|release) {
	TARGET = 5-cleanup_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/cleanup/debug
	MOC_DIR = ../tmp/cleanup/debug
}

LANG_SRC_DIR = $$shell_path(./)
LANG_DEST_DIR = $$shell_path($${DESTDIR}/languages/)

QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, "$${LANG_DEST_DIR}") $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $${QMAKE_COPY} $${LANG_SRC_DIR}*.qm $${LANG_DEST_DIR}
