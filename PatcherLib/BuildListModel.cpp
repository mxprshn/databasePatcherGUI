#include "BuildListModel.h"

BuildListModel::BuildListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int BuildListModel::rowCount(const QModelIndex& parent) const
{
	return 0;
}

QVariant BuildListModel::data(const QModelIndex& index, int role) const
{
	return QVariant();
}

BuildListModel::~BuildListModel()
{
}
