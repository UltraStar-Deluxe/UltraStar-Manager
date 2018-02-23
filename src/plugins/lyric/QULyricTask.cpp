#include "QULyricTask.h"
#include "QUSmartInputField.h"
#include "QUSmartCheckBox.h"

#include <QVariant>
#include <QRegExpValidator>
#include <QList>

QULyricTask::QULyricTask(TaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case FixTimeStamps:
		this->setIcon(QIcon(":/control/zero.png"));
		this->setDescription(tr("Set first timestamp to zero"));
		break;
	case FixOverlappingNotes:
		this->setIcon(QIcon(":/control/fix_overlapping_notes.png"));
		this->setDescription(tr("Fix overlapping notes"));
		this->setToolTip(tr("Shortens adjacent notes to remove overlapping."));
		break;
	case FixLineBreaks:
		this->setIcon(QIcon(":/control/edit_direction.png"));
		this->setDescription(tr("Fix line break timings"));
		this->setToolTip(tr("Moves line break timing to the middle of one line's end and the next line's beginning."));
		break;
	case FixSpaces:
		this->setIcon(QIcon(":/control/space.png"));
		this->setDescription(tr("Fix spaces"));
		this->setToolTip(tr("Moves spaces from the end of a syllable to the beginning of the next one. Trim the whole song line."));
		break;
	case FixApostrophes:
		this->setIcon(QIcon(":/control/edit_apostrophe.png"));
		this->setDescription(tr("Fix apostrophes"));
		this->setToolTip(tr("Replaces wrongfully used apostrophe symbols `, ´, ’ by the ASCII apostrophe '"));
		break;
	case FixLowBPM:
		this->setIcon(QIcon(":/control/bpm_increase.png"));
		this->setDescription(tr("Increase low BPM values"));
		this->setToolTip(tr("Doubles BPM value and all note timings while it is below a certain threshold."));
		break;
	case SetMedleyTags:
		this->setIcon(QIcon(":/types/medley.png"));
		this->setDescription(tr("Set medley and preview tags if not present"));
		this->setToolTip(tr("Tries to determine a suitable medley section and sets medley tags accordingly, if successful."));
		break;
	case FixLineCapitalization:
		this->setIcon(QIcon(":/control/edit_uppercase.png"));
		this->setDescription(tr("Capitalize first word of each song line"));
		break;
	case NormalizePitches:
		this->setIcon(QIcon(":/control/normalize_pitches.png"));
		this->setDescription(tr("Normalize pitch values"));
		this->setToolTip(tr("Shifts all note pitches octavewise towards a zero mean."));
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
	case FixOverlappingNotes:
		fixOverlappingNotes(song);
		break;
	case FixLineBreaks:
		fixLineBreaks(song);
		break;
	case FixSpaces:
		fixSpaces(song);
		break;
	case FixApostrophes:
		fixApostrophes(song);
		break;
	case FixLowBPM:
		fixLowBPM(song, smartSettings().first()->value().toInt());
		break;
	case SetMedleyTags:
		setMedleyTags(song, smartSettings().first()->value().toInt(), smartSettings().at(1)->value().toBool());
		break;
	case FixLineCapitalization:
		fixLineCapitalization(song);
		break;
	case NormalizePitches:
		normalizePitches(song);
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
	if(_smartSettings.isEmpty()) {
		if(_mode == FixTimeStamps) {
			_smartSettings.append(new QUSmartInputField("lyric/fixTimeStamps", "0", new QRegExpValidator(QRegExp("-?\\d*"), 0), tr("Start:"), ""));
		}
		if(_mode == FixLowBPM) {
			_smartSettings.append(new QUSmartInputField("lyric/fixLowBPM", "200", new QRegExpValidator(QRegExp("\\d*"), 0), tr("if BPM less than:"), ""));
		}
		if(_mode == SetMedleyTags) {
			_smartSettings.append(new QUSmartInputField("lyric/medleyMinDuration", "30", new QRegExpValidator(QRegExp("\\d*"), 0), tr("Minimum length:"), tr("seconds")));
			_smartSettings.append(new QUSmartCheckBox("lyric/overwriteExisting", tr("Overwrite existing values"), false));
		}
	}
	return _smartSettings;
}

/*!
 * Let the song start with timestamp 0.
 */
void QULyricTask::fixTimeStamps(QUSongInterface *song, int start) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics in file \"%1\"")).arg(song->txt()), QU::Warning);
		return;
	}

	if(song->loadMelody().first()->notes().first()->timestamp() == start)
		return;

	// the diff value has to be subtracted from each timestamp
	int begin = start;
	int diff = 0;
	if(!song->isDuet()) {
		diff = song->loadMelody().first()->notes().first()->timestamp() - begin;
	} else {
		int firstTimestampP1 = song->melodyForSinger(QUSongLineInterface::first).first()->notes().first()->timestamp();
		int firstTimestampP2 = song->melodyForSinger(QUSongLineInterface::second).first()->notes().first()->timestamp();
		diff = qMin(firstTimestampP1, firstTimestampP2) - begin;
	}

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

	//adjust medley tags if (both) present
	if(song->hasMedley()) {
		song->setInfo(MEDLEYSTARTBEAT_TAG, QString::number(song->medleystartbeat().toInt() - diff));
		song->setInfo(MEDLEYENDBEAT_TAG, QString::number(song->medleyendbeat().toInt() - diff));
	}

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
 * Fixes overlapping notes be either
 *  - shortening the end of the first note
 *  - shortening the beginning of the second note
 *  - shortening both the end of the first and the beginning of the second note
 */
void QULyricTask::fixOverlappingNotes(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	// check for overlapping notes
	int overlappingNotesCnt = 0;
	int overlappingNotesFixed = 0;

	for(int i = 0; i < song->loadMelody().length() - 1; i++) {
		QUSongLineInterface *currentLine = song->loadMelody().at(i);
		for(int j = 0; j < currentLine->notes().size(); j++) {
			QUSongNoteInterface *currentNote = currentLine->notes().at(j);

			// the very last note of a song can't overlap
			if(currentLine == song->loadMelody().last() && currentNote == currentLine->notes().last())
				break;

			// the very last note of singer 1 of a duet can't overlap either
			if(song->isDuet() && currentNote == song->melodyForSinger(QUSongLineInterface::first).last()->notes().last())
				break;

			// relative songs can't have overlaps across lines
			if(song->isRelative() && currentNote == currentLine->notes().last())
				break;

			QUSongNoteInterface *nextNote;
			if(currentNote == currentLine->notes().last() && currentLine != song->loadMelody().last()) {
				if(!song->loadMelody().at(i+1)->notes().isEmpty())
					nextNote = song->loadMelody().at(i+1)->notes().first();
			} else {
				nextNote = currentLine->notes().at(j+1);
			}

			int currentNoteStartBeat = currentNote->timestamp();
			int currentNoteEndBeat   = currentNote->timestamp() + currentNote->duration();
			int nextNoteStartBeat    = nextNote->timestamp();
			int nextNoteEndBeat      = nextNote->timestamp() + nextNote->duration();

			if(currentNoteEndBeat > nextNoteStartBeat) {
				overlappingNotesCnt++;
				if(currentNoteStartBeat < nextNoteStartBeat) {
					currentNote->setDuration(nextNoteStartBeat - currentNoteStartBeat);
					overlappingNotesFixed++;
				}
				else { // next note starts before current note - out of sequence
					if(currentNoteEndBeat < nextNoteEndBeat) {
						nextNote->setTimestamp(currentNoteEndBeat);
						nextNote->setDuration(nextNoteEndBeat - currentNoteEndBeat);
						overlappingNotesFixed++;
					} else { // next note ends even before current note, something must be seriously wrong
						song->log(QString(tr("Unable to fix out of sequence notes at beats %1 to %2 for \"%3 - %4\".")).arg(currentNoteStartBeat).arg(nextNoteStartBeat).arg(song->artist()).arg(song->title()), QU::Information);
					}
				}
			}

			if(currentNoteEndBeat == song->medleyendbeat().toInt()) {
				song->setInfo(MEDLEYENDBEAT_TAG, QString::number(currentNote->timestamp() + currentNote->duration()));
				song->log(QString(tr("Adjusted medley end beat from %1 to %2 for \"%3 - %4\".")).arg(currentNoteEndBeat).arg(song->medleyendbeat()).arg(song->artist()).arg(song->title()), QU::Information);
			}
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	if(overlappingNotesCnt > 0)
		song->log(QString(tr("%1 of %2 overlapping notes were fixed successfully for \"%3 - %4\".")).arg(overlappingNotesFixed).arg(overlappingNotesCnt).arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * Recalculate line break timings to be after the last beat of the current line and the first beat of the subsequent line.
 */
void QULyricTask::fixLineBreaks(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics in file \"%1\"")).arg(song->txt()), QU::Warning);
		return;
	}

	if(song->isRelative()) {
		return;
	}

	// fix all line breaks
	for(int i = 0; i < song->loadMelody().length() - 2; i++) {
		QUSongLineInterface *line = song->loadMelody().at(i);

		if(line->outTime() != 0) {
			QUSongLineInterface *nextLine = song->loadMelody().at(i+1);
			int lineEnd = line->notes().last()->timestamp() + line->notes().last()->duration();
			int nextLineStart = nextLine->notes().first()->timestamp();
			int lineGap = nextLineStart - lineEnd;


			if(lineGap < 0) { // overlapping lines
				line->setOutTime(nextLineStart);
				line->removeInTime();
				song->log(QString("Lines %1 and %2 are overlapping from beat %3 to beat %4 for \"%5 - %6\".")
						.arg(QString::number(i+1))
						.arg(QString::number(i+2))
						.arg(QString::number(nextLineStart))
						.arg(QString::number(lineEnd))
						.arg(song->artist())
						.arg(song->title()), QU::Warning);
			} else if(lineGap == 0) { // touching lines
				line->setOutTime(nextLineStart);
				line->removeInTime();
				song->log(QString("Lines %1 and %2 are touching at beat %3 for \"%4 - %5\".")
						.arg(QString::number(i+1))
						.arg(QString::number(i+2))
						.arg(QString::number(nextLineStart))
						.arg(song->artist())
						.arg(song->title()), QU::Information);
			} else { // gap between lines
				line->setOutTime(lineEnd + lineGap/2);
				line->removeInTime();
			}
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Line break timings adjusted for \"%1 - %2\"."))
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);
}

/*!
 * Avoid a space at the beginning of a syllable.
 * ": 200 5 20 'cause"		-> ": 200 5 20 'cause␣"
 * ": 205 2 10 ␣I"			-> ": 205 2 10 I"
 */
void QULyricTask::fixSpaces(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	// modify all lyrics
	foreach(QUSongLineInterface *line, song->loadMelody()) {
		// first syllable should not start with a space
		if(line->notes().size() > 0) {
			QUSongNoteInterface *first = line->notes().first();
			if(first->syllable().startsWith(" "))
				first->resetTrailingSpaces(0, -1);
		}

		// if current syllable starts with a space, shift it to the end of the previous syllable
		for(int i = 1; i < line->notes().size(); i++) {
			QUSongNoteInterface *previous = line->notes()[i-1];
			QUSongNoteInterface *current = line->notes()[i];

			if(current->syllable().startsWith(" ")) {
				previous->resetTrailingSpaces(-1, 1);
				current->resetTrailingSpaces(0, -1);
			}
		}

		// ensure last syllable ends with a space as well (so that syllable highlighting is always complete when singing)
		if(line->notes().size() > 0) {
			QUSongNoteInterface *last = line->notes().last();
			if(!last->syllable().endsWith(" "))
				last->resetTrailingSpaces(-1, 1);
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Spaces were fixed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * Avoid a space at the end of a syllable.
 * ": 200 5 20 'cause␣"		-> ": 200 5 20 'cause"
 * ": 205 2 10 I"			-> ": 205 2 10 ␣I"
 */
/*void QULyricTask::fixSpaces(QUSongInterface *song) {
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
}*/

/*!
 * Replace wrongfully used apostrophe symbols `, ´ and ’ by ASCII apostroph '.
 */
void QULyricTask::fixApostrophes(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	int count = 0;

	foreach(QUSongLineInterface *line, song->loadMelody()) {
		foreach(QUSongNoteInterface *note, line->notes()) {
			if(note->syllable().contains("´")) { // acute accent (U+00B4)
				note->setSyllable(note->syllable().replace("´", "'"));
				count++;
			}
			if(note->syllable().contains("`")) { // grave accent (U+0060)
				note->setSyllable(note->syllable().replace("`", "'"));
				count++;
			}
			if(note->syllable().contains("′")) { // prime (U+2032)
				note->setSyllable(note->syllable().replace("′", "'"));
				count++;
			}
			if(note->syllable().contains("’")) { // typographically correct apostrophe (U+2019) (not available in ASCII/Latin-9/ISO 8859-15)
				note->setSyllable(note->syllable().replace("’", "'"));
				count++;
			}
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("%1 wrong apostrophe symbols replaced successfully for \"%2 - %3\"."))
			  .arg(count)
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);
}

/*!
 * Doubles BPM value while below given threshold and adjust all note timings accordingly
 */
void QULyricTask::fixLowBPM(QUSongInterface *song, int threshold) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics in file \"%1\"")).arg(song->txt()), QU::Warning);
		return;
	}

	double BPM = QVariant(song->bpm().replace(",", ".")).toDouble();

	if(BPM >= threshold)
		return;
	else {
		// calculate and set new BPM
		double newBPM = BPM;
		while(newBPM < threshold) {
			newBPM *=2;
		}
		song->setInfo(BPM_TAG, QString::number(newBPM));

		int multiplicator = newBPM / BPM;

		// modify medley tags, if present
		if(song->hasMedley()) {
			int test = song->medleystartbeat().toInt() * multiplicator;
			song->log(QString::number(test), QU::Warning);
			song->setInfo(MEDLEYSTARTBEAT_TAG, QString::number(song->medleystartbeat().toInt() * multiplicator));
			song->setInfo(MEDLEYENDBEAT_TAG, QString::number(song->medleyendbeat().toInt() * multiplicator));
		}

		// modify all timestamps
		foreach(QUSongLineInterface *line, song->loadMelody()) {
			foreach(QUSongNoteInterface *note, line->notes()) {
				note->setTimestamp(note->timestamp() * multiplicator);
				note->setDuration(note->duration() * multiplicator);
			}

			if(line->useOutTime()) {
				line->setOutTime(line->outTime() * multiplicator);

				if(line->useInTime())
					line->setInTime(line->inTime() * multiplicator);
			}
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("BPM changed from %1 to %2 for \"%3 - %4\"."))
			  .arg(BPM)
			  .arg(song->bpm())
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);
}

/*!
 * Tries to detect the beginning of a chorus to use a starting point for a medley
 * and sets medley tags accordingly (thanks to brunzel's CMD mod code from USong.pas)
 */
void QULyricTask::setMedleyTags(QUSongInterface *song, int medleyMinDuration, bool overwriteExisting) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics in file \"%1\"")).arg(song->txt()), QU::Warning);
		return;
	}

	if(song->hasMedley() && !overwriteExisting) {
		song->log(QString(tr("Skipping medley search: medley tags already set for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	if(song->isDuet()) {
		song->log(QString(tr("Skipping medley search: medley tags not supported for duet \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	if(song->isRelative()) {
		song->log(QString(tr("Skipping medley search: relative format not supported for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	if(song->calcmedley().toLower() == "off") {
		song->log(QString(tr("Skipping medley search: search disabled by CALCMEDLEY tag for song \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	bool medleyFound = false;

	// find all medley candidates
	QPair<int, int> tempMedley;
	QList<QPair<int, int> > medleyCandidates;

	for(int i = 0; i <= song->loadMelody().size() - 2; i++) {
		QString firstLine = song->loadMelody().at(i)->toString().toLower().remove(QRegExp("[,\\.!\\?~ ]"));
		for(int j = i + 4; j <= song->loadMelody().size() - 1; j++) {
			QString secondLine = song->loadMelody().at(j)->toString().toLower().remove(QRegExp("[,\\.!\\?~ ]"));
			if(firstLine == secondLine) {
				tempMedley.first = i;
				tempMedley.second = i;

				int max = 0;
				if(j + (j - i) - 1 > song->loadMelody().size() - 1) {
					max = song->loadMelody().size() - j - 1;
				} else {
					max = j - i - 1;
				}

				for(int k = 1; k <= max; k++) {
					firstLine = song->loadMelody().at(i+k)->toString().toLower().remove(QRegExp("[,\\.!\\?~ ]"));
					secondLine = song->loadMelody().at(j+k)->toString().toLower().remove(QRegExp("[,\\.!\\?~ ]"));
					if(firstLine == secondLine) {
						tempMedley.second = i + k;
					} else {
						break;
					}
				}
				medleyCandidates.append(tempMedley);
			}
		}
	}

	// find longest medley candidate
	int longestMedleyIndex = 0;
	if(medleyCandidates.size() == 0) { // no medley candidates found
		song->log(QString(tr("No medley candidates found for song \"%1 - %2\"")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	} else { // find longest of all medley candidates (backwards to take first longest, not last longest
		for (int l = medleyCandidates.size() - 1; l >= 0; l--) {
			if( (medleyCandidates.at(l).second - medleyCandidates.at(l).first) >= (medleyCandidates.at(longestMedleyIndex).second - medleyCandidates.at(longestMedleyIndex).first) ) {
				longestMedleyIndex = l;
			}
		}
	}

	double bpm = song->bpm().replace(',','.').toDouble();
	int medleyMinBeats = QVariant(medleyMinDuration * bpm / 15).toInt();
	double medleyDuration = 0;
	int medleyStartBeat = 0;
	int medleyEndBeat = 0;

	int medleyStartLine = medleyCandidates.at(longestMedleyIndex).first;
	int medleyEndLine   = medleyCandidates.at(longestMedleyIndex).second;
	int medleyLineCount = medleyEndLine - medleyStartLine + 1;

	if( medleyLineCount > 3 ) {
		medleyStartBeat = song->loadMelody().at(medleyStartLine)->notes().first()->timestamp();
		medleyEndBeat   = song->loadMelody().at(medleyStartLine)->notes().last()->timestamp() + song->loadMelody().at(medleyEndLine)->notes().last()->duration();

		medleyDuration = (medleyEndBeat - medleyStartBeat + 1) * 15 / bpm;
		if( medleyDuration >= medleyMinDuration) {
			medleyFound = true;
		} else {
			int approximateEndBeat = medleyStartBeat + medleyMinBeats - 1;
			// find line that contains approximateEnd, set medleyendbeat to last beat of this line
			foreach(QUSongLineInterface *line, song->loadMelody()) {
				if(medleyFound)
					break;
				foreach(QUSongNoteInterface *note, line->notes()) {
					if(note->timestamp() > approximateEndBeat) {
						medleyEndBeat = line->notes().last()->timestamp() + line->notes().last()->duration();
						medleyDuration = (medleyEndBeat - medleyStartBeat + 1) * 15 / bpm;
						medleyFound = true;
						break;
					}
				}
			}
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	if(medleyFound) {
		song->setInfo(MEDLEYSTARTBEAT_TAG, QString::number(medleyStartBeat));
		song->setInfo(MEDLEYENDBEAT_TAG, QString::number(medleyEndBeat));
		song->log(QString(tr("Medley with a duration of %1 seconds set for \"%2 - %3\"."))
				  .arg(medleyDuration, 0, 'f', 1)
				  .arg(song->artist())
				  .arg(song->title()), QU::Information);
		if(song->previewstart() == N_A || overwriteExisting) {
			song->setInfo(PREVIEWSTART_TAG, QString::number(song->medleystartbeat().toInt() * 15 / bpm + song->gap().toFloat() / 1000, 'f', 3));
		}
	} else {
		song->log(QString(tr("No suitable medley section found for \"%1 - %2\"."))
				  .arg(song->artist())
				  .arg(song->title()), QU::Warning);
	}
}

/*!
 * Capitalizes each first word of a song line
 */
void QULyricTask::fixLineCapitalization(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	foreach(QUSongLineInterface *line, song->loadMelody()) {
		if(!line->notes().isEmpty())
			line->notes().first()->setSyllable(line->notes().first()->syllable().at(0).toUpper() + line->notes().first()->syllable().mid(1));
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Line capitalization fixed for \"%1 - %2\"."))
			  .arg(song->artist())
			  .arg(song->title()), QU::Information);
}

/*!
 * Shifts all note pitch values to a sensible range around zero.
 */
void QULyricTask::normalizePitches(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	// MB TODO: calculate a sensible amount of octaves to shift by (median probably better than mean)
	double meanPitch = 0;
	int numNotes = 0;
	int octaves = 0;
	foreach(QUSongLineInterface *line, song->loadMelody()) {
		foreach(QUSongNoteInterface *note, line->notes()) {
			numNotes++;
			meanPitch += note->pitch();
		}
	}

	song->clearMelody(); // save memory

	meanPitch = meanPitch/numNotes;
	if (meanPitch < 0)
		octaves = meanPitch/12 - 1;
	else
		octaves = meanPitch/12;

	if(octaves != 0) {
		// modify all note pitches
		foreach(QUSongLineInterface *line, song->loadMelody()) {
			foreach(QUSongNoteInterface *note, line->notes()) {
				note->setPitch(note->pitch() - (octaves*12));
			}
		}

		song->saveMelody();
		song->clearMelody(); // save memory

		song->log(QString(tr("Mean note pitch changed from %1 to %2 for \"%3 - %4\"."))
				  .arg(QString::number(meanPitch))
				  .arg(QString::number(meanPitch - (octaves*12)))
				  .arg(song->artist())
				  .arg(song->title()), QU::Information);
	}
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
