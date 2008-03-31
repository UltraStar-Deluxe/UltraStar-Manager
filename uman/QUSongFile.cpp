#include "QUSongFile.h"

#include <QByteArray>
#include <QVariant>
#include <QDir>

QUSongFile::QUSongFile(const QString &file, QObject *parent): QObject(parent) {
	_fi.setFile(file);
	updateCache();
}

QUSongFile::~QUSongFile() {
}

bool QUSongFile::updateCache() {
	_file.setFileName(_fi.filePath());
	
	if(!_file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	
	QString line;
	while( !(line.startsWith(":") || _file.atEnd()) ) {
		line = QString(_file.readLine());
		
		foreach(QString tag, tags()) {
			if(line.startsWith("#" + tag + ":"))
				setInfo(tag, line.section("#" + tag + ":", 0, 0, QString::SectionSkipEmpty));		
		}
	}
	
	_lyrics.clear();
	_lyrics << line;
	
	while(!_file.atEnd()) {
		line = QString(_file.readLine());
		_lyrics << line;
	}
	
	_file.close();
	return true;
}

void QUSongFile::setInfo(const QString &key, const QString &value) {
	if(value == "") {
		_info.take(key);
		return;
	}
	
	_info[key] = value.trimmed();
}

QStringList QUSongFile::tags() {
	QStringList result;
	
	result << "ARTIST";
	result << "TITLE";
	result << "MP3";
	result << "BPM";
	result << "GAP";
	result << "VIDEO";
	result << "VIDEOGAP";
	result << "COVER";
	result << "BACKGROUND";
	result << "START";
	result << "RELATIVE";
	result << "EDITION";
	result << "GENRE";
	result << "LANGUAGE";
	
	return result;
}

bool QUSongFile::hasMp3() const {
	return mp3FileInfo().exists();
}

bool QUSongFile::hasCover() const {
	return coverFileInfo().exists();
}

bool QUSongFile::hasBackground() const {
	return backgroundFileInfo().exists();
}

bool QUSongFile::hasVideo() const {
	return videoFileInfo().exists();
}

bool QUSongFile::save() {
	QFile::remove(_fi.filePath());
	
	_file.setFileName(_fi.filePath());
	
	if(!_file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
		
	foreach(QString key, _info.keys()) {
		_file.write("#");
		_file.write(key.toLocal8Bit());
		_file.write(":");
		_file.write(_info.value(key).toLocal8Bit());
		_file.write("\n");
	}
	
	foreach(QString line, _lyrics) {
		_file.write(line.toLocal8Bit());
	}
	
	_file.close();
	return true;
}

bool QUSongFile::renameSongDir(const QString &newName) {
	QDir dir(_fi.dir());	
	dir.cdUp();
	
	bool result = dir.rename(_fi.dir().dirName(), newName);
	
	if(result) {
		dir.cd(newName);
		_fi.setFile(dir.path() + "/" + _fi.fileName());
	}
	
	return result;
}

bool QUSongFile::renameSongTxt(const QString &newName) {
	bool result = _fi.dir().rename(_fi.fileName(), newName);
	
	if(result) {
		_fi.setFile(_fi.dir().path() + "/" + newName);
	}
	
	return result;	
}

bool QUSongFile::renameSongMp3(const QString &newName) {
	bool result = _fi.dir().rename(mp3(), newName);
	
	if(result) {
		setInfo("MP3", newName);
	}
	
	return result;
}

bool QUSongFile::renameSongCover(const QString &newName) {
	bool result = _fi.dir().rename(cover(), newName);
	
	if(result) {
		setInfo("COVER", newName);
	}
	
	return result;
}

bool QUSongFile::renameSongBackground(const QString &newName) {
	bool result = _fi.dir().rename(background(), newName);
	
	if(result) {
		setInfo("BACKGROUND", newName);
	}
	
	return result;
}

bool QUSongFile::renameSongVideo(const QString &newName) {
	bool result = _fi.dir().rename(video(), newName);
	
	if(result) {
		setInfo("VIDEO", newName);
	}
	
	return result;
}
