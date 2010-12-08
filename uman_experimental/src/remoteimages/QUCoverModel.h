#ifndef QUCOVERMODEL_H_
#define QUCOVERMODEL_H_

#define COVER_ICON_WIDTH  64
#define COVER_ICON_HEIGHT 64

#include <QAbstractListModel>

#include <QIcon>
#include <QString>
#include <QSize>
#include <QStringList>
#include <QList>

/*!
 * Stores the result of a cover request.
 */
class QUCoverModel: public QAbstractListModel {
	Q_OBJECT

public:
	QUCoverModel(QObject *parent = 0);

	virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;

	virtual Qt::ItemFlags flags (const QModelIndex &index) const;

	void addCover(const QString &filePath);
	void clear();
private:
	QList<QIcon>   _iconList;
	QList<QSize>   _dimensionsList;
	QStringList    _filePathList;

};

#endif /* QUCOVERMODEL_H_ */
