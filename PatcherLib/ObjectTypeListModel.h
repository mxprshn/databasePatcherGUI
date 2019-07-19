#pragma once

#include <QAbstractListModel>

class ObjectTypeListModel : public QAbstractListModel
{
public:

	ObjectTypeListModel(QObject *parent);
	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

private:

	QHash<int, QString> *typeNames;
};
