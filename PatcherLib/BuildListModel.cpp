#include <QIcon>
#include "BuildListModel.h"

BuildListModel::BuildListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	typeNames = new QHash<int, QString>({ {script, "script"}, {table, "table"}
		, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
		, {ObjectType::index, "index"} });

	typeIcons = new QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
		, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
		, {trigger, ":/images/trigger.svg"}, {ObjectType::index, ":/images/index.svg"} });

	elements = new QVector<PatchListElement*>;
}

int BuildListModel::rowCount(const QModelIndex& parent) const
{
	// check if valid index?
	return elements->count();
}

int BuildListModel::columnCount(const QModelIndex& parent) const
{
	return columnAmount;
}

QVariant BuildListModel::data(const QModelIndex& index, int role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
		{
			switch (index.column())
			{
				case 0:
				{
					return typeNames->value(elements->at(index.row())->getType());
				}
				case 1:
				{
					return elements->at(index.row())->getScheme();
				}
				case 2:
				{
					return elements->at(index.row())->getName();
				}
			}
		}
		case Qt::DecorationRole:
		{
			if (index.column() == 0)
			{
				return QIcon(typeIcons->value(elements->at(index.row())->getType()));
			}
		}
	}

	return QVariant();
}

QVariant BuildListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
				case 0:
				{
					return QString("Type");
				}
				case 1:
				{
					return QString("Scheme");
				}
				case 2:
				{
					return QString("Name");
				}
			}
		}
	}

	return QVariant();
}

void BuildListModel::addObject(ObjectType type, const QString &name, const QString &scheme
	, const QVector<QPair<QString, QString>> &parameters)
{
	beginInsertRows(QModelIndex(), elements->count(), elements->count() + 1);
	elements->append(new PatchListElement(this, type, name, scheme, parameters));
	endInsertRows();
}

int BuildListModel::count() const
{
	return elements->count();
}

ObjectType BuildListModel::getType(int index) const
{
	return elements->at(index)->getType();
}

QString BuildListModel::getTypeName(int index) const
{
	return typeNames->value(elements->at(index)->getType());
}

QString BuildListModel::getSchemeName(int index) const
{
	return elements->at(index)->getScheme();
}

QString BuildListModel::getName(int index) const
{
	return elements->at(index)->getName();
}

QVector<QPair<QString, QString>> BuildListModel::getParameters(int index) const
{
	return elements->at(index)->getParameters();
}

BuildListModel::~BuildListModel()
{
	delete typeNames;
	delete elements;
	delete typeIcons;
}