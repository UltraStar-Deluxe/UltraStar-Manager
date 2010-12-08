#include "QULyricTask.h"
#include "QUSmartInputField.h"
#include "QUSmartCheckBox.h"

#include <QVariant>
#include <QRegExpValidator>

QULyricTask::QULyricTask(TaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case FixTimeStamps:
		this->setIcon(QIcon(":/control/zero.png"));
		this->setDescription(tr("Set first timestamp to zero"));
		break;
	case FixSpaces:
		this->setIcon(QIcon(":/control/space.png"));
		this->setDescription(tr("Fix spaces"));
		this->setToolTip(tr("Moves spaces from the end of a syllable to the beginning of the next one. Trim the whole song line."));
		break;
	case RemoveEmptySyllables:
		this->setIcon(QIcon(":/control/empty_syllable.png"));
		this->setDescription(tr("Remove empty syllables"));
		this->setToolTip(tr("Example:<br><br>: 230 6 9 be<br><b>: 236 5 10</b><br>: 241 7 3 not.<br><br>Remove the bold one."));
		break;
	case ConvertSyllablePlaceholder1:
		this->setDescription(tr("Convert syllable placeholders from \"-\" to \"~\""));
		this->setIcon(QIcon(":/control/convert-2.png"));
		this->setGroup(999); // hopefully this group is free ^^
		break;
	case ConvertSyllablePlaceholder2:
		this->setDescription(tr("Convert syllable placeholders from \"~\" to \"-\""));
		this->setIcon(QIcon(":/control/convert-1.png"));
		this->setGroup(999); // hopefully this group is free ^^
		break;
	case ConvertRelativeToAbsolute:
		this->setDescription(tr("Convert relative to absolute timestamps"));
		this->setIcon(QIcon(":/control/convert-r.png"));
		this->setGroup(998); // hopefully this group is free ^^
		break;
	case ConvertAbsoluteToRelative:
		this->setDescription(tr("Convert absolute to relative timestamps"));
		this->setIcon(QIcon(":/control/convert-a.png"));
		this->setGroup(998); // hopefully this group is free ^^
		break;
	}
}

void QULyricTask::startOn(QUSongInterface *song) {
	switch(_mode) {
	case FixTimeStamps:
		fixTimeStamps(song, smartSettings().first()->value().toInt());
		break;
	case FixSpaces:
		fixSpaces(song);
		break;
	case RemoveEmptySyllables:
		removeEmptySyllables(song);
		break;
	case ConvertSyllablePlaceholder1:
		convertSyllablePlaceholder(song, "-", "~");
		break;
	case ConvertSyllablePlaceholder2:
		convertSyllablePlaceholder(song, "~", "-");
		break;
	case ConvertRelativeToAbsolute:
		convertRelativeToAbsolute(song);
		break;
	case ConvertAbsoluteToRelative:
		convertAbsoluteToRelative(song);
		break;
	}
}

QList<QUSmartSettingInterface*> QULyricTask::smartSettings() const {
	if(_smartSettings.isEmpty())
		if(_mode == FixTimeStamps)
			_smartSettings.append(new QUSmartInputField("lyric/fixTimeStamps", "0", new QRegExpValidator(QRegExp("-?\\d*"), 0), "Start:", ""));
	return _smartSettings;
}

/*!
 * Let the song start with timestamp 0.
 */
void QULyricTask::fixTimeStamps(QUSongInterface *song, int start) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	// the diff value has to be subtracted from each timestamp
	int begin = start;
	int diff = song->loadMelody().first()->notes().first()->timestamp() - begin;

	double gap = QVariant(song->gap().replace(",", ".")).toDouble();
	double bpm = QVariant(song->bpm().replace(",", ".")).toDouble();

	// calculate and set the new gap
	double oldGap = gap;
	gap = gap + (diff * 15000) / bpm;
	song->setInfo(GAP_TAG, QString::number(qRound(gap)));
	song->log(QString(tr("#GAP changed from %1 to %2 for \"%3 - %4\"."))
			  .arg(oldGap)
			  .arg(song->gap())
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);

	// modify all timestamps
	if(!song->isRelative()) { // simple way: not relative
		foreach(QUSongLineInterface *line, song->loadMelody()) {
			foreach(QUSongNoteInterface *note, line->notes()) {
				note->setTimestamp(note->timestamp() - diff);
			}

			if(line->useOutTime()) {
				line->setOutTime(line->outTime() - diff);

				if(line->useInTime())
					line->setInTime(line->inTime() - diff);
			}
		}
	} else { // hard way: relative timestamps
		// the first line is like the "simple way" :)
		foreach(QUSongNoteInterface *note, song->loadMelody().first()->notes()) {
			note->setTimestamp(note->timestamp() - diff);
		}

		if(song->loadMelody().first()->useOutTime()) {
			song->loadMelody().first()->setOutTime(song->loadMelody().first()->outTime() - diff);

			if(song->loadMelody().first()->useInTime())
				song->loadMelody().first()->setInTime(song->loadMelody().first()->inTime() - diff);
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Timestamps were changed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * Avoid a space at the end of a syllable.
 * ": 200 5 20 'cause " -> ": 200 5 20 'cause"
 * ": 205 2 10 I"       -> ": 205 2 10  I"
 */
void QULyricTask::fixSpaces(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	// modify all lyrics
	foreach(QUSongLineInterface *line, song->loadMelody()) {
		if(line->notes().size() > 0) {
			QUSongNoteInterface *first = line->notes().first();
			if(first->syllable().startsWith(" "))
				first->resetTrailingSpaces(0, -1);
		}

		for(int i = 0; i < line->notes().size() - 1; i++) {
			QUSongNoteInterface *current = line->notes()[i];
			QUSongNoteInterface *next = line->notes()[i+1];

			if(current->syllable().endsWith(" ")) {
				current->resetTrailingSpaces(-1, 0);
				next->resetTrailingSpaces(1, -1);
			}
		}

		if(line->notes().size() > 0) {
			QUSongNoteInterface *last = line->notes().last();
			if(last->syllable().endsWith(" "))
				last->resetTrailingSpaces(-1, 0);
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Spaces were fixed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * Removes empty syllables.
 */
void QULyricTask::removeEmptySyllables(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	foreach(QUSongLineInterface *line, song->loadMelody()) {
		QList<QUSongNoteInterface*> emptyNotes;
		foreach(QUSongNoteInterface *note, line->notes()) {
			if(note->syllable().trimmed().isEmpty()) // what about relative songs?
				emptyNotes.append(note);
		}

		foreach(QUSongNoteInterface *note, emptyNotes) {
			line->notes().removeAll(note);
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Empty syllables were removed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Information);
}

void QULyricTask::convertSyllablePlaceholder(QUSongInterface *song, const QString &before, const QString &after) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	foreach(QUSongLineInterface *line, song->loadMelody()) {
		foreach(QUSongNoteInterface *note, line->notes()) {
			if(note->syllable().trimmed().startsWith(before) || note->syllable().trimmed().endsWith(before)) // to enable "~." and "~,"
				note->setSyllable(note->syllable().replace(before, after));
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Syllable placeholders were converted successfully from '%3' to '%4' for \"%1 - %2\"."))
			  .arg(song->artist())
			  .arg(song->title())
			  .arg(before)
			  .arg(after), QU::Information);
}

void QULyricTask::convertRelativeToAbsolute(QUSongInterface *song) {
	if(!song->isRelative())
		return;

	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	int diff = 0;
	foreach(QUSongLineInterface *line, song->loadMelody()) {
		if(!line->useInTime())
			line->setInTime(line->outTime()); // fix missing in-time

//		if(line == song->loadMelody().first()) {
//			diff = line->inTime();
////			line->removeInTime(); // analog to ultrastar conversion in editor
//			continue;
//		}

		foreach(QUSongNoteInterface *note, line->notes()) {
			note->setTimestamp(note->timestamp() + diff);
		}

		if(line->useOutTime()) {
			line->setOutTime(line->outTime() + diff);
			line->setInTime(line->inTime() + diff);

			diff = line->inTime();

			if(line->outTime() > line->inTime()) { // is possible for strange line start timestamps

			}

			if(line->outTime() == line->inTime())
				line->removeInTime();
		}
	}

	song->setInfo("RELATIVE", "");
	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(tr("Relative timestamps converted successfully to absolute timestamps for \"%1 - %2\".")
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);
}

void QULyricTask::convertAbsoluteToRelative(QUSongInterface *song) {
	if(song->isRelative())
		return;

	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	int diff = 0;
	foreach(QUSongLineInterface *line, song->loadMelody()) {
		if(!line->useInTime())
			line->setInTime(line->outTime()); // fix missing in-time

//		if(line == song->loadMelody().first()) {
//			diff = line->inTime();
////			line->removeInTime(); // analog to ultrastar conversion in editor
//			continue;
//		}

		foreach(QUSongNoteInterface *note, line->notes()) {
			note->setTimestamp(note->timestamp() - diff);
		}

		if(line->useOutTime()) {
			line->setOutTime(line->outTime() - diff);
			line->setInTime(line->inTime() - diff);

			diff = diff + line->inTime();
		}
	}

	song->setInfo("RELATIVE", "yes");
	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(tr("Absolute timestamps converted successfully to relative timestamps for \"%1 - %2\".")
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);
}
