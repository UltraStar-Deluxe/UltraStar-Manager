#ifndef QU_H_
#define QU_H_

#include <QObject>
#include <QFileInfo>

#define N_A					"-"
#define NONE				"(none)"
#define GAMEMODE_NORMAL		"TR_GAMEMODE_NORMAL"
#define GAMEMODE_SHORT		"TR_GAMEMODE_SHORTSONG"
#define GAMEMODE_MEDLEY		"TR_GAMEMODE_MEDLEY"
#define GAMEMODE_DUET		"TR_GAMEMODE_DUET"

#define CHAR_UTF8_APPROX	"\xe2\x89\x88"
#define CHAR_UTF8_NEQUAL	"\xe2\x89\xa0"
#define CHAR_UTF8_DOT		"\xc2\xb7"
#define CHAR_UTF8_CHECK		"\xe2\x9c\x93"
#define CHAR_UTF8_BALLOT	"\xe2\x9c\x97"

// used for time comparison and fixing
#define LOWER_TIME_BOUND_DEFAULT 15
#define UPPER_TIME_BOUND_DEFAULT 30

// used for encodings
#define ENCODING_CP1250	"CP1250"
#define ENCODING_CP1252	"CP1252"
#define ENCODING_UTF8	"UTF8"

//! This class provides global flags and enumerations.
class QU: QObject {
	Q_OBJECT
public:
	QU(QObject *parent = 0);

	static QDir BaseDir; // UltraStar song folder

	enum MessageType {
		Information,
		Warning,
		Help,
		Saving,
		Error
	};
	Q_DECLARE_FLAGS(MessageTypes, MessageType)

	enum FilterMode {
		informationTags = 0x01,
		fileTags = 0x02,
		controlTags = 0x04,
		customTags = 0x08,
		negateFilter = 0x10
	};
	Q_DECLARE_FLAGS(FilterModes, FilterMode)

	enum ImageSize {
		mediumImage,
		largeImage
	};
	Q_DECLARE_FLAGS(ImageSizes, ImageSize)

	enum SpellState {
		spellingOk,
		spellingWarning,
		spellingError
	};
	Q_DECLARE_FLAGS(SpellStates, SpellState)

	enum ReportOption {
		reportPrependCurrentPath = 0x01,
		reportPrependUserData = 0x02,
		reportAppendLyrics = 0x04,
		reportLinkLyrics = 0x08,
		reportUseStyleSheet = 0x10
	};
	Q_DECLARE_FLAGS(ReportOptions, ReportOption)

	static bool fileTypeLessThan(const QFileInfo &fi1, const QFileInfo &fi2);
};

#endif /*QU_H_*/
