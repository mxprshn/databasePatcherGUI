#include "InstallListModel.h"

InstallListModel::InstallListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int InstallListModel::rowCount(const QModelIndex& parent) const
{
	return 0;
}

QVariant InstallListModel::data(const QModelIndex& index, int role) const
{
	return QVariant();
}

InstallListModel::~InstallListModel()
{
}
