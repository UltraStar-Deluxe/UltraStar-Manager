#ifndef QUPLUGINMODEL_H
#define QUPLUGINMODEL_H

#include <QAbstractListModel>

class QUPluginModel: public QAbstractListModel {
	Q_OBJECT

public:
	enum CustomRoles {
		VersionRole = 7
	};

	QUPluginModel(QObject *parent = 0);

	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

private slots:
	void invalidateData();
};

#endif // QUPLUGINMODEL_H
