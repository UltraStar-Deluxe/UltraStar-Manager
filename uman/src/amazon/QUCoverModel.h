#ifndef QUCOVERMODEL_H_
#define QUCOVERMODEL_H_

#include <QAbstractListModel>

#include <QIcon>
#include <QString>

class QUCoverModel: public QAbstractListModel {
	Q_OBJECT

public:
	QUCoverModel(QObject *parent = 0);

	virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;

private:

};

#endif /* QUCOVERMODEL_H_ */
