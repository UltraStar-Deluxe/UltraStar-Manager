TEMPLATE = app
UI_DIR = ui
QT += core \
	gui \
	widgets \
	multimedia \
	xml \
	network \
	printsupport

#CONFIG += release

CONFIG(release, debug|release) {
	TARGET = "UltraStarManager"
	DESTDIR = ../bin/release
	MOC_DIR = tmp/release
	OBJECTS_DIR = tmp/release
	RCC_DIR = tmp/release
}

CONFIG(debug, debug|release) {
	TARGET = UltraStarManager_debug
	DESTDIR = ../bin/debug
	MOC_DIR = tmp/debug
	OBJECTS_DIR = tmp/debug
	RCC_DIR = tmp/debug
}

HEADERS += plugins/QUPluginManager.h \
	plugins/QUPluginDialog.h \
	plugins/QUPluginModel.h \
	plugins/QUPluginDelegate.h \
	plugins/QUTaskPlugin.h \
	plugins/QUTaskFactoryProxy.h \
	plugins/QUSmartSettingInterface.h \
	plugins/QURemoteImageSourcePlugin.h \
	plugins/QUCommunicatorInterface.h \
	plugins/QUSimpleCommunicator.h \
	main.h \
	QU.h \
	QUMonty.h \
	QUAboutDialog.h \
	QUMainWindow.h \
	QUStringSupport.h \
	QUSongSupport.h \
	song/QUSongInterface.h \
	song/QUSongFile.h \
	song/QUSongLine.h \
	song/QUSongDatabase.h \
	mediaplayer/QUAutoCue.h \
	mediaplayer/QUMediaPlayer.h \
	support/QULogService.h \
	support/QUMessageBox.h \
	support/QUProgressDialog.h \
	support/QUTextDialog.h \
	support/QUPictureDialog.h \
	monty/QUMontyArea.h \
	filter/QUFilterArea.h \
	filter/QUMetaphoneString.h \
	remoteimages/QUCoverItemDelegate.h \
	remoteimages/QUCoverModel.h \
	remoteimages/QUCoverList.h \
	remoteimages/QUCoverGroup.h \
	remoteimages/QURemoteImageDialog.h \
	remoteimages/QUPropertyTable.h \
	remoteimages/QUFieldDelegate.h \
	remoteimages/QUHelpButton.h \
	preferences/QUCustomTagsDialog.h \
	preferences/QUTagOrderDialog.h \
	playlist/QUPlaylistFile.h \
	playlist/QUPlaylistEntry.h \
	playlist/QUPlaylistDatabase.h \
	playlistdetails/QUPlaylistArea.h \
	playlistdetails/QUPlayListItem.h \
	playlistdetails/QUPlayList.h \
	playlistdetails/QUPlaylistModel.h \
	playlistdetails/QUPlaylistDBModel.h \
	preview/QUPreviewTree.h \
	report/QUSongFileData.h \
	report/QUPlainTextReport.h \
	report/QUBooleanSongData.h \
	report/QUAbstractReport.h \
	report/QUHtmlReport.h \
	report/QUReportItem.h \
	report/QUSongTagData.h \
	report/QUAbstractReportData.h \
	report/QUReportDialog.h \
	report/QUPdfReport.h \
	songtree/QUColumnAction.h \
	songtree/QUSongTree.h \
	songtree/QUSongItem.h \
	songdetails/QUTagItem.h \
	songdetails/QUDetailsTable.h \
	songdetails/QUDetailItem.h \
	songdetails/QUDropDownDelegate.h \
	tasks/QUTaskList.h \
	tasks/QUTaskItem.h \
	ribbon/QURibbonBar.h \
	slideshow/pictureflow.h \
	slideshow/QUSlideShowDialog.h \
	lyricseditor/QULyricsEdit.h \
	lyricseditor/QUSongLineDelegate.h \
	lyricseditor/QULyricsEditorDialog.h \
	lyricseditor/QUSyllableDelegate.h \
	preferences/QUPathsDialog.h \
	score/QUScoreFile.h \
	preferences/QUEncodingsDialog.h \
	report/QUCSVReport.h
#	support/qkFileWatcher.h
SOURCES += plugins/QUPluginManager.cpp \
	plugins/QUPluginDialog.cpp \
	plugins/QUPluginModel.cpp \
	plugins/QUPluginDelegate.cpp \
	plugins/QUTaskFactoryProxy.cpp \
	plugins/QUSimpleCommunicator.cpp \
	main.cpp \
	QU.cpp \
	QUAboutDialog.cpp \
	QUMainWindow.cpp \
	QUMonty.cpp \
	QUStringSupport.cpp \
	QUSongSupport.cpp \
	mediaplayer/QUAutoCue.cpp \
	mediaplayer/QUMediaPlayer.cpp \
	support/QULogService.cpp \
	support/QUProgressDialog.cpp \
	support/QUTextDialog.cpp \
	support/QUPictureDialog.cpp \
	support/QUMessageBox.cpp \
	monty/QUMontyArea.cpp \
	filter/QUFilterArea.cpp \
	filter/QUMetaphoneString.cpp \
	remoteimages/QUCoverItemDelegate.cpp \
	remoteimages/QUCoverModel.cpp \
	remoteimages/QUCoverList.cpp \
	remoteimages/QUCoverGroup.cpp \
	remoteimages/QURemoteImageDialog.cpp \
	remoteimages/QUPropertyTable.cpp \
	remoteimages/QUFieldDelegate.cpp \
	remoteimages/QUHelpButton.cpp \
	preferences/QUCustomTagsDialog.cpp \
	preferences/QUTagOrderDialog.cpp \
	playlist/QUPlaylistFile.cpp \
	playlist/QUPlaylistEntry.cpp \
	playlist/QUPlaylistDatabase.cpp \
	playlistdetails/QUPlaylistArea.cpp \
	playlistdetails/QUPlayListItem.cpp \
	playlistdetails/QUPlayList.cpp \
	playlistdetails/QUPlaylistModel.cpp \
	playlistdetails/QUPlaylistDBModel.cpp \
	preview/QUPreviewTree.cpp \
	report/QUSongFileData.cpp \
	report/QUPlainTextReport.cpp \
	report/QUBooleanSongData.cpp \
	report/QUAbstractReport.cpp \
	report/QUHtmlReport.cpp \
	report/QUReportItem.cpp \
	report/QUSongTagData.cpp \
	report/QUAbstractReportData.cpp \
	report/QUReportDialog.cpp \
	report/QUPdfReport.cpp \
	songtree/QUColumnAction.cpp \
	songtree/QUSongTree.cpp \
	songtree/QUSongItem.cpp \
	songdetails/QUDropDownDelegate.cpp \
	songdetails/QUTagItem.cpp \
	songdetails/QUDetailsTable.cpp \
	songdetails/QUDetailItem.cpp \
	song/QUSongLine.cpp \
	song/QUSongFile.cpp \
	song/QUSongDatabase.cpp \
	tasks/QUTaskList.cpp \
	tasks/QUTaskItem.cpp \
	ribbon/QURibbonBar.cpp \
	slideshow/pictureflow.cpp \
	slideshow/QUSlideShowDialog.cpp \
	lyricseditor/QULyricsEdit.cpp \
	lyricseditor/QUSongLineDelegate.cpp \
	lyricseditor/QULyricsEditorDialog.cpp \
	lyricseditor/QUSyllableDelegate.cpp \
	preferences/QUPathsDialog.cpp \
	score/QUScoreFile.cpp \
	preferences/QUEncodingsDialog.cpp \
	report/QUCSVReport.cpp
	#support/qkFileWatcher.cpp
FORMS += plugins/QUPluginDialog.ui \
	QUMainWindow.ui \
	QUAboutDialog.ui \
	mediaplayer/QUMediaPlayer.ui \
	monty/QUMontyArea.ui \
	filter/QUFilterArea.ui \
	playlistdetails/QUPlaylistArea.ui \
	report/QUReportDialog.ui \
	support/QUPictureDialog.ui \
	support/QUMessageBox.ui \
	support/QUProgressDialog.ui \
	support/QUTextDialog.ui \
	preferences/QUTagOrderDialog.ui \
	preferences/QUCustomTagsDialog.ui \
	ribbon/QURibbonBar.ui \
	slideshow/QUSlideShowDialog.ui \
	lyricseditor/QULyricsEditorDialog.ui \
	preferences/QUPathsDialog.ui \
	remoteimages/QURemoteImageDialog.ui \
	remoteimages/QUCoverGroup.ui \
	preferences/QUEncodingsDialog.ui
RESOURCES += resources/uman.qrc
TRANSLATIONS = resources/lang/uman.de.ts \
	resources/lang/uman.pl.ts \
	resources/lang/uman.es.ts \
	resources/lang/uman.fr.ts \
	resources/lang/uman.pt.ts
INCLUDEPATH += . \
	song \
	songdetails \
	songtree \
	report \
	tasks \
	tasks/taskDialog \
	preview \
	playlist \
	playlistdetails \
	filter \
	monty \
	mediaplayer \
	support \
	preferences \
	ui \
	plugins \
	ribbon \
	slideshow \
	lyricseditor \
	score \
	remoteimages
INCLUDEPATH += ../include/bass

win32 {
	INCLUDEPATH += ../include/taglib \
		../include/mediainfo

	LIBS += -L"../lib/win32" \
		-lbass \
		-ltag \
		-lmediainfo \
		-lzen \
		-lzlibstatic

	RC_FILE = uman.rc
}

macx {
	LIBS += -L"../lib/macx" -lbass
	CONFIG += link_pkgconfig
	PKGCONFIG += taglib
	PKGCONFIG += libmediainfo

	CONFIG += app_bundle
	QMAKE_RPATHDIR += @executable_path/../Frameworks
}

unix:!macx {
	# the INCLUDEPATH += ../include/mediainfo should be removed since it refers to the local copy of the header file
	# which should only be used for Windows. However, for some reason mediainfo on Unix installed via
	# 'apt-get install libmediainfo-dev' does NOT have MediaInfoDLL/MediaInfoDLL_Static.h, but instead only
	# MediaInfoDLL/MediaInfoDLL.h, and currently compilation fails with that header file. Help wanted.
	INCLUDEPATH += ../include/mediainfo

	LIBS += -L"../lib/unix" -lbass
	CONFIG += link_pkgconfig
	PKGCONFIG += taglib
	PKGCONFIG += libmediainfo
}

QMAKE_EXTRA_TARGETS += revtarget
PRE_TARGETDEPS += version.h
revtarget.target = version.h

win32 {
	revtarget.commands = "$$system(echo 'const char *revision = \"$$system(git describe --always)\";' > $$revtarget.target)"
	revtarget.commands += "$$system(echo 'const char *date_time = \"$$system(date /T) $$system(time /T)\";' >> $$revtarget.target)"
}

unix {
revtarget.commands = @echo \
	"const char *revision = \\\"`git describe --always`\\\"\\; \
	const char *date_time = \\\"`date +'%d.%m.%Y %H:%M'`\\\"\\;" \
	> $${PWD}/$$revtarget.target
}

revtarget.depends = $$SOURCES \
	$$HEADERS \
	$$FORMS

unix:!macx {
	QMAKE_POST_LINK += $${QMAKE_COPY} $$IN_PWD/../lib/unix/libbass.so $$IN_PWD/../bin/release
}

win32 {
	QMAKE_POST_LINK += windeployqt --release --no-translations --no-system-d3d-compiler --compiler-runtime --no-angle --no-opengl-sw ..\bin\release\UltraStarManager.exe $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/Qt5Svg.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/iconengines/qsvgicon.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_DIR} $$shell_path($${DESTDIR}/iconengines) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qicns.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qico.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qsvg.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qtga.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qtiff.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qwbmp.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qwebp.dll) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path(../lib/win32/bass.dll) $$shell_path($${DESTDIR}) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path(../lib/win32/libtag.dll) $$shell_path($${DESTDIR}) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, $$shell_path($${DESTDIR}/styles/)) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path(styles/*.css) $$shell_path($${DESTDIR}/styles/) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path(../doc/changes.txt) $$shell_path($${DESTDIR}) $$escape_expand(\\n\\t)
	#QMAKE_POST_LINK += $$shell_quote(C:\Program Files (x86)\NSIS\makensis.exe) $$shell_path(../setup/win32/uman.nsi) $$escape_expand(\\n\\t)
}

macx {
	plugins.files = ../bin/release/plugins
	plugins.path = Contents/MacOS
	QMAKE_BUNDLE_DATA += plugins
	dylibs.files = ../lib/macx/libbass.dylib
	dylibs.path = Contents/Frameworks
	QMAKE_BUNDLE_DATA += dylibs
	ICON = resources/uman.icns

	QMAKE_POST_LINK += macdeployqt ../bin/release/UltraStarManager.app $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change @loader_path/libbass.dylib @executable_path/../Frameworks/libbass.dylib ../bin/release/UltraStarManager.app/Contents/MacOS/UltraStarManager $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/media-info/0.7.94/lib/libzen.0.dylib @executable_path/../Frameworks/libzen.0.dylib ../bin/release/UltraStarManager.app/Contents/Frameworks/libmediainfo.0.dylib $$escape_expand(\\n\\t)	
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/qt5/5.8.0_1/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../bin/release/UltraStarManager.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/qt5/5.8.0_1/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../bin/release/UltraStarManager.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/qt5/5.8.0_1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../bin/release/UltraStarManager.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/qt5/5.8.0_1/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../bin/release/UltraStarManager.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/qt5/5.8.0_1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../bin/release/UltraStarManager.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += install_name_tool -change /usr/local/Cellar/qt5/5.8.0_1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../bin/release/UltraStarManager.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += ../setup/macx/create-dmg --volname UltraStarManager --volicon resources/uman.icns --app-drop-link 350 170 --background ../setup/macx/img/uman_bg.png --hide-extension UltraStarManager.app --window-size 500 300 --text-size 14 --icon-size 64 --icon UltraStarManager.app 150 170 "../bin/release/UltraStarManager.dmg" ../bin/release/UltraStarManager.app/
}
