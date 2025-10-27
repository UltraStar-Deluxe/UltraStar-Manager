SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QUPreparatoryTaskFactory.cpp \
	QUPreparatoryTask.cpp \
	../shared/QUSimpleTask.cpp \
	../shared/QUSimpleTaskFactory.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUSmartCheckBox.cpp \
	../shared/QUSmartComboBox.cpp \
	../shared/QUSmartInputField.cpp \
	../../QUSongSupport.cpp
HEADERS = ../shared/QUMultilingualTaskFactory.h \
	QUPreparatoryTaskFactory.h \
	QUPreparatoryTask.h \
	../shared/QUSimpleTask.h \
	../shared/QUSimpleTaskFactory.h \
	../shared/QUSmartSetting.h \
	../shared/QUSmartCheckBox.h \
	../shared/QUSmartComboBox.h \
	../shared/QUSmartInputField.h \
	../../QUSongSupport.h \
	../QUSmartSettingInterface.h
OTHER_FILES += preparatory.json
TRANSLATIONS = preparatory.de.ts \
	preparatory.pl.ts \
	preparatory.es.ts \
	preparatory.fr.ts \
	preparatory.pt.ts
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../shared
TARGET = 1-preparatory
TEMPLATE = lib
CONFIG += plugin
QT += widgets

CONFIG(release, debug|release) {
	TARGET = 1-preparatory
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/preparatory/release
	MOC_DIR = ../tmp/preparatory/release
}

CONFIG(debug, debug|release) {
	TARGET = 1-preparatory_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/preparatory/debug
	MOC_DIR = ../tmp/preparatory/debug
}

LANG_SRC_DIR = $$shell_path(./)
LANG_DEST_DIR = $$shell_path($${DESTDIR}/languages/)

QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, "$${LANG_DEST_DIR}") $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $${QMAKE_COPY} $${LANG_SRC_DIR}*.qm $${LANG_DEST_DIR}
