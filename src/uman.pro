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

win32 {
	INCLUDEPATH += ../include/taglib \
		../include/bass \
		../include/mediainfo

	RC_FILE = uman.rc

	LIBS += -L"../lib/Windows" \
		-ltag \
		-lbass \
		-lmediainfo \
		-lzen \
		-lzlibstatic
}

mac {
	INCLUDEPATH += ../include/bass

	CONFIG += link_pkgconfig
	PKGCONFIG += taglib
	PKGCONFIG += libmediainfo
	LIBS += -L"../lib/MacOS" \
		-lbass
	CONFIG += app_bundle
	QMAKE_RPATHDIR += @executable_path/../Frameworks
}

unix:!mac {
	# the INCLUDEPATH += ../include/mediainfo should be removed
	# because it refers to the local copy of the header file
	# which should only be used for Windows.
	# However, for some reason mediainfo on Unix installed via 'apt-get install libmediainfo-dev'
	# does NOT have MediaInfoDLL/MediaInfoDLL_Static.h, but instead only
	# MediaInfoDLL/MediaInfoDLL.h, and currently compilation fails with that
	# header file. Help wanted.
	INCLUDEPATH += ../include/mediainfo \
	INCLUDEPATH += ../include/bass

	CONFIG += link_pkgconfig
	PKGCONFIG += taglib
	PKGCONFIG += libmediainfo
	LIBS += -L"../lib/Unix" \
		-lbass
	#QMAKE_RPATHDIR += \$\$ORIGIN
	QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}

QMAKE_EXTRA_TARGETS += revtarget
PRE_TARGETDEPS += version.h
revtarget.target = version.h

win32 {
revtarget.commands = @echo \
	"const char *revision = \"$(shell git describe --always)\"; const char *date_time = \"$(shell date /T)$(shell time /T)\";" > $$revtarget.target
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

unix {
	QMAKE_POST_LINK += $${QMAKE_COPY} $$IN_PWD/../lib/Unix/libbass.so $$IN_PWD/../bin/release
}

mac {
	plugins.files = ../bin/release/plugins
	plugins.path = Contents/MacOS
	QMAKE_BUNDLE_DATA += plugins
	dylibs.files = ../lib/MacOS/libbass.dylib
	dylibs.path = Contents/MacOS
	QMAKE_BUNDLE_DATA += dylibs
	ICON = resources/uman.icns
}
