#include "BuildListModel.h"

BuildListModel::BuildListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	typeNames = new QHash<int, QString>({ {script, "script"}, {table, "table"}
		, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
		, {ObjectType::index, "index"} });

	typeIcons = new QHash<int, QString>({ {script, ":/images/table.svg"}, {table, ":/images/table.svg"}
		, {sequence, ":/images/table.svg"}, {function, ":/images/table.svg"}, {view, ":/images/table.svg"}
		, {trigger, ":/images/table.svg"}, {ObjectType::index, ":/images/table.svg"} });

	elements = new QList<PatchListElement*>;
}

int BuildListModel::rowCount(const QModelIndex& parent) const
{
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
				return typeIcons->value(elements->at(index.row())->getType());
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

QString BuildListModel::getParametersString(const QList<QPair<QString, QString>> &parameters)
{
	QString result = "( ";

	for (auto i = 0; i < parameters.count(); ++i)
	{
		result += parameters[i].second + " ";
	}

	return result + ")";
}

void BuildListModel::addObject(ObjectType type, const QString &name, const QString &scheme
	, const QList<QPair<QString, QString>> &parameters)
{
	elements->append(new PatchListElement(this, type, name, scheme, parameters));
	const QModelIndex topLeft = createIndex(elements->count() - 1, 0);
	const QModelIndex bottomRight = createIndex(elements->count() - 1, columnAmount);
	emit dataChanged(topLeft, bottomRight);
}

QStringList BuildListModel::getObjectList()
{
	QStringList result;

	for (auto i = 0; i < elements->count(); ++i)
	{
		result.append(elements->at(i)->getScheme() + " " + elements->at(i)->getName() + " "
			+ typeNames->value(elements->at(i)->getType()) +  " "
			+ getParametersString(elements->at(i)->getParameters()));
	}

	return result;
}


BuildListModel::~BuildListModel()
{
	delete typeNames;
	delete elements;
	delete typeIcons;
}

