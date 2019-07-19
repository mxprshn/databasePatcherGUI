#include "ObjectTypeListModel.h"
#include "ObjectType.h"

ObjectTypeListModel::ObjectTypeListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	typeNames = new QHash<int, QString>({ {script, "script"}, {table, "table"}
		, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
		, {ObjectType::index, "index"}});
}

int ObjectTypeListModel::rowCount(const QModelIndex& parent) const
{
	return typeCount;
}

QVariant ObjectTypeListModel::data(const QModelIndex& index, int role) const
{
	return role == Qt::DisplayRole ? typeNames->value(index.row()) : QVariant();
}