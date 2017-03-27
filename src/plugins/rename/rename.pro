SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QURenameTaskFactory.cpp \
	QURenameTask.cpp \
	../../QUStringSupport.cpp \
	../../QUSongSupport.cpp \
	../shared/QUSimpleTask.cpp \
	../shared/QUScriptableTask.cpp \
	../shared/QUScriptableTaskFactory.cpp \
	QURenameTaskDialog.cpp \
	QURenameTaskSourceDelegate.cpp \
	../shared/QUTaskDialog.cpp \
	../shared/QUTaskDataTable.cpp \
	../shared/QUTaskConditionDelegate.cpp \
	../shared/QUTaskSourceDelegate.cpp \
	../shared/QUTaskTextDelegate.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUTaskModifierDelegate.cpp
HEADERS = language.h \
	../shared/QUMultilingualTaskFactory.h \
	QURenameTaskFactory.h \
	QURenameTask.h \
	../../QUStringSupport.h \
	../../QUSongSupport.h \
	../shared/QUSimpleTask.h \
	../shared/QUScriptableTask.h \
	../shared/QUScriptableTaskFactory.h \
	QURenameTaskDialog.h \
	QURenameTaskSourceDelegate.h \
	../shared/QUTaskDialog.h \
	../shared/QUTaskDataTable.h \
	../shared/QUTaskConditionDelegate.h \
	../shared/QUTaskSourceDelegate.h \
	../shared/QUTaskTextDelegate.h \
	../shared/QUSmartSetting.h \
	../QUSmartSettingInterface.h \
	../shared/QUTaskModifierDelegate.h
FORMS += ../shared/QUTaskDialog.ui
OTHER_FILES += rename.json
TRANSLATIONS = rename.de.ts \
	rename.pl.ts \
	rename.es.ts \
	rename.fr.ts \
	rename.pt.ts
INCLUDEPATH += . \
	ui \
	.. \
	../.. \
	../../song \
	../shared
TEMPLATE = lib
CONFIG += plugin
UI_DIR = ../ui
QT += xml \
	widgets
QMAKE_EXTRA_TARGETS += langtarget
PRE_TARGETDEPS += language.h
langtarget.target = language.h
langtarget.commands = python getTaskText.py

CONFIG(release, debug|release) {
	TARGET = 4-rename
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/rename/release
	MOC_DIR = ../tmp/rename/release
}

CONFIG(debug, debug|release) {
	TARGET = 4-rename_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/rename/debug
	MOC_DIR = ../tmp/rename/debug
}
CONFIG_SRC_DIR = $$IN_PWD/config/
CONFIG_DEST_DIR = $${DESTDIR}/config/rename/
LANG_SRC_DIR = $$IN_PWD/
LANG_DEST_DIR = $${DESTDIR}/languages/
win32 {
	CONFIG_SRC_DIR ~= s,/,\\,g
	CONFIG_DEST_DIR ~= s,/,\\,g
	LANG_SRC_DIR ~= s,/,\\,g
	LANG_DEST_DIR ~= s,/,\\,g
}

QMAKE_POST_LINK += $$system($$sprintf($${QMAKE_MKDIR_CMD}, "$${CONFIG_DEST_DIR}")) $$escape_expand(\n\t)
QMAKE_POST_LINK += $$system($${QMAKE_COPY} $${CONFIG_SRC_DIR}* $${CONFIG_DEST_DIR}) $$escape_expand(\n\t)
QMAKE_POST_LINK += $$system($$sprintf($${QMAKE_MKDIR_CMD}, "$${LANG_DEST_DIR}")) $$escape_expand(\n\t)
QMAKE_POST_LINK += $$system($${QMAKE_COPY} $${LANG_SRC_DIR}*.qm $${LANG_DEST_DIR})

