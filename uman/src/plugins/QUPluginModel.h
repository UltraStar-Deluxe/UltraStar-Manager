#ifndef QUPLUGINMODEL_H
#define QUPLUGINMODEL_H

#include <QAbstractListModel>

class QUPluginModel: public QAbstractListModel {
	Q_OBJECT

public:
	enum CustomRole {
		NameRole = Qt::DisplayRole,
		VersionRole = Qt::UserRole,
		AuthorRole = Qt::UserRole + 1,
		DescriptionRole = Qt::UserRole + 2,
		IconRole = Qt::DecorationRole
	};

	QUPluginModel(QObject *parent = 0);

	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

private slots:
	void invalidateData();
};

#endif // QUPLUGINMODEL_H
