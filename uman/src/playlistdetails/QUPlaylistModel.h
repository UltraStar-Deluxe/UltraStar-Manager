#ifndef QUPLAYLISTMODEL_H
#define QUPLAYLISTMODEL_H

#include <QAbstractListModel>

class QUPlaylistFile;

class QUPlaylistModel: public QAbstractListModel {
	Q_OBJECT

public:
	QUPlaylistModel(QObject *parent = 0);

	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

	void setIndex(int);

protected slots:
	void reload();

private:
	int _playlistIndex;

	QUPlaylistFile* playlist() const;
};

#endif // QUPLAYLISTMODEL_H
