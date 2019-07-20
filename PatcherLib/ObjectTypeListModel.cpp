#include <QIcon>
#include "ObjectTypeListModel.h"
#include "ObjectType.h"

ObjectTypeListModel::ObjectTypeListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	typeNames = new QHash<int, QString>({ {script, "script"}, {table, "table"}
		, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
		, {ObjectType::index, "index"}});

	typeIcons = new QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
		, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
		, {trigger, ":/images/trigger.svg"}, {ObjectType::index, ":/images/index.svg"} });
}

int ObjectTypeListModel::rowCount(const QModelIndex& parent) const
{
	return typeCount;
}

QVariant ObjectTypeListModel::data(const QModelIndex& index, int role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
		{
			return typeNames->value(index.row());
		}
		case Qt::DecorationRole:
		{
			return QIcon(typeIcons->value(index.row()));
		}
	}

	return QVariant();
}