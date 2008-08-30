#include "QUTaskSourceDelegate.h"

#include <QString>
#include <QVariant>
#include <QLineEdit>

QUTaskSourceDelegate::QUTaskSourceDelegate(QU::ScriptableTaskTypes type, QObject *parent): QItemDelegate(parent), _type(type) {
}

QWidget* QUTaskSourceDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	QComboBox *editor = new QComboBox(parent);
	this->setItems(editor);
	editor->setEditable(false);

	return editor;
}

void QUTaskSourceDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	comboBox->setCurrentIndex(comboBox->findText(value, Qt::MatchContains));
}

void QUTaskSourceDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	QString value = comboBox->currentText();

	model->setData(index, QVariant(value), Qt::EditRole);
}

void QUTaskSourceDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}

void QUTaskSourceDelegate::setItems(QComboBox *comboBox) const {
	if(_type == QU::renameTask) {
		comboBox->addItems(QURenameTask::availableSpecialSources());
		for(int i = 0; i < comboBox->count(); i++)
			comboBox->setItemData(i, Qt::darkGray, Qt::ForegroundRole);

		comboBox->addItems(QURenameTask::availableCommonSources());
		int i = comboBox->count();
		comboBox->addItems(QUScriptableTask::availableCustomSources());
		for(; i < comboBox->count(); i++)
			comboBox->setItemData(i, Qt::blue, Qt::ForegroundRole);

	} else if(_type == QU::audioTagTask) {
		comboBox->addItems(QUScriptableTask::availableSources() + QUAudioTagTask::availableSpecialSources());
		for(int i = 0; i < comboBox->count(); i++)
			comboBox->setItemData(i, Qt::darkGray, Qt::ForegroundRole);

		foreach(QString commonSource, QUAudioTagTask::availableCommonSources()) {
			comboBox->addItem(QIcon(":/control/id3.png"), commonSource);
		}

	}
}
