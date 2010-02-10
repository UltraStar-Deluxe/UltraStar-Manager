#ifndef QUPLAYLISTDBMODEL_H
#define QUPLAYLISTDBMODEL_H

#include <QAbstractListModel>

class QUPlaylistDBModel: public QAbstractListModel {
	Q_OBJECT

public:
	QUPlaylistDBModel(QObject *parent = 0);

	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

protected slots:
	void reload();
};


#endif // QUPLAYLISTDBMODEL_H
