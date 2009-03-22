#include "QULyricTask.h"

#include <QVariant>

QULyricTask::QULyricTask(QU::LyricTaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::fixTimeStamps:
		this->setIcon(QIcon(":/control/zero.png"));
		this->setDescription(tr("Set first timestamp to zero"));
		break;
	case QU::fixSpaces:
		this->setIcon(QIcon(":/control/space.png"));
		this->setDescription(tr("Fix spaces"));
		this->setToolTip(tr("Moves spaces from the end of a syllable to the beginning of the next one. Trim the whole song line."));
		break;
	case QU::removeEmptySyllables:
		this->setIcon(QIcon(":/control/empty_syllable.png"));
		this->setDescription(tr("Remove empty syllables"));
		break;
	}
}

void QULyricTask::startOn(QUSongInterface *song) {
	switch(_mode) {
	case QU::fixTimeStamps:
		fixTimeStamps(song);
		break;
	case QU::fixSpaces:
		fixSpaces(song);
		break;
	case QU::removeEmptySyllables:
		removeEmptySyllables(song);
		break;
	}
}

/*!
 * Let the song start with timestamp 0.
 */
void QULyricTask::fixTimeStamps(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::warning);
		return;
	}

	// the diff value has to be subtracted from each timestamp
	int begin = 0;
	int diff = song->loadMelody().first()->notes().first()->timestamp() - begin;

	double gap = QVariant(song->gap().replace(",", ".")).toDouble();
	double bpm = QVariant(song->bpm().replace(",", ".")).toDouble();

	// calculate and set the new gap
	double oldGap = gap;
	gap = gap + (diff * 15000) / bpm;
	song->setInfo(GAP_TAG, QVariant(qRound(gap)).toString());
	song->log(QString(tr("#GAP changed from %1 to %2 for \"%3 - %4\"."))
			  .arg(oldGap)
			  .arg(song->gap())
			  .arg(song->artist())
			  .arg(song->title()), QU::information);

	// modify all timestamps
	if(song->relative() == N_A) { // simple way: not relative
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

		// process next lines
		for(int i = 1; i < song->loadMelody().size(); i++) {
			QUSongLineInterface *currentLine = song->loadMelody()[i];
			QUSongLineInterface *prevLine    = song->loadMelody()[i - 1];

			if(currentLine->notes().isEmpty())
				continue;

			int diff2 = currentLine->notes().first()->timestamp() - begin;

			prevLine->setInTime(prevLine->inTime() + diff2); // every line has to have "in" and "out" timestamps! -> relative

			foreach(QUSongNoteInterface *note, currentLine->notes()) {
				note->setTimestamp(note->timestamp() - diff2);
			}

			if(currentLine->useOutTime()) {
				currentLine->setOutTime(currentLine->outTime() - diff2);

				if(currentLine->useInTime())
					currentLine->setInTime(currentLine->inTime() - diff2);
			}
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Timestamps were changed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::information);
}

/*!
 * Avoid a space at the end of a syllable.
 * ": 200 5 20 'cause " -> ": 200 5 20 'cause"
 * ": 205 2 10 I"       -> ": 205 2 10  I"
 */
void QULyricTask::fixSpaces(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::warning);
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

	song->log(QString(tr("Spaces were fixed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::information);
}

/*!
 * Removes empty syllables.
 */
void QULyricTask::removeEmptySyllables(QUSongInterface *song) {
	if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
		song->log(QString(tr("Invalid lyrics: %1 - %2")).arg(song->artist()).arg(song->title()), QU::warning);
		return;
	}

	foreach(QUSongLineInterface *line, song->loadMelody()) {
		QList<QUSongNoteInterface*> emptyNotes;
		foreach(QUSongNoteInterface *note, line->notes()) {
			if(note->syllable().trimmed().isEmpty())
				emptyNotes.append(note);
		}

		foreach(QUSongNoteInterface *note, emptyNotes) {
			line->notes().removeAll(note);
		}
	}

	song->saveMelody();
	song->clearMelody(); // save memory

	song->log(QString(tr("Empty syllables were removed successfully for \"%1 - %2\".")).arg(song->artist()).arg(song->title()), QU::information);
}
