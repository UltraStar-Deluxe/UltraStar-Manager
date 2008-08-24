#ifndef QU_H_
#define QU_H_

#include <QObject>
#include <QString>

#define CHAR_UTF8_APPROX "\xe2\x89\x88"
#define CHAR_UTF8_NEQUAL "\xe2\x89\xa0"

//! This class provides global flags and enumerations.
class QU: QObject {
	Q_OBJECT
public:
	enum EventMessageType {
		information,
		warning,
		help,
		saving
	};
	Q_DECLARE_FLAGS(EventMessageTypes, EventMessageType)

	enum AudioTagTaskMode {
		useArtist,
		useTitle,
		useGenre,
		useYear
	};
	Q_DECLARE_FLAGS(AudioTagTaskModes, AudioTagTaskMode)

	enum RenameTaskMode {
		renameDirectory,
		renameDirectorySpecial, // use and preserve folder tags ([VIDEO], [SC], ...)
		renameSongFile,
		renameAudioFile,
		renameCoverFile,
		renameBackgroundFile,
		renameVideoFile,
		renameVideoFileSpecial // use videogap [VD#0]
	};
	Q_DECLARE_FLAGS(RenameTaskModes, RenameTaskMode)

	enum PreparatoryTaskMode {
		autoAssignFiles,
		removeUnsupportedTags
	};
	Q_DECLARE_FLAGS(PreparatoryTaskModes, PreparatoryTaskMode)

	enum CleanTaskMode {
		unusedFiles,
		invalidFileTags
	};
	Q_DECLARE_FLAGS(CleanTaskModes, CleanTaskMode)

	enum FilterMode {
		informationTags = 0x01,
		fileTags = 0x02,
		controlTags = 0x04,
		customTags = 0x08,
		negateFilter = 0x10
	};
	Q_DECLARE_FLAGS(FilterModes, FilterMode)

	static QString withoutFolderTags(const QString &text);
};

#endif /*QU_H_*/
