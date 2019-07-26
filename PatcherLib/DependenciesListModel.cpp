#include "DependenciesListModel.h"
#include "DependenciesListElement.h"
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <QBitArray>

DependenciesListModel::DependenciesListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	statusIcons.insert(DependenciesListElement::CheckStatus::waitingForCheck, ":/images/unchecked.svg");
	statusIcons.insert(DependenciesListElement::CheckStatus::satisfied, ":/images/checked.svg");
	statusIcons.insert(DependenciesListElement::CheckStatus::notSatisfied, ":/images/error.svg");

	typeIcons.insert(ObjectType::table, ":/images/table.svg");
	typeIcons.insert(ObjectType::function, ":/images/function.svg");
	typeIcons.insert(ObjectType::sequence, ":/images/sequence.svg");
	typeIcons.insert(ObjectType::index, ":/images/function.svg");
	typeIcons.insert(ObjectType::trigger, ":/images/function.svg");
	typeIcons.insert(ObjectType::view, ":/images/view.svg");

	typeNames.insert(ObjectType::table, "table");
	typeNames.insert(ObjectType::function, "function");
	typeNames.insert(ObjectType::sequence, "sequence");
	typeNames.insert(ObjectType::index, "index");
	typeNames.insert(ObjectType::trigger, "trigger");
	typeNames.insert(ObjectType::view, "view");

	parseDependenciesFromFile("C:\\Users\\mxprshn\\Desktop\\test\\DependencyList.dpn");
}

DependenciesListModel::~DependenciesListModel()
{
}

int DependenciesListModel::rowCount(const QModelIndex & parent) const
{
	return elements.count();
}

int DependenciesListModel::columnCount(const QModelIndex& parent) const
{
	return columnAmount;
}


QVariant DependenciesListModel::data(const QModelIndex& index, int role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			{
				switch (index.column())
				{
					case 0:
						{
							return typeNames[elements[index.row()]->getType()];
						}
					case 1:
						{
							return elements[index.row()]->getScheme();
						}
					case 2:
						{
						return elements[index.row()]->getName();
						}
				}
			}
		case Qt::DecorationRole:
			{
				switch (index.column())
				{
					case 0:
					{
						return QIcon(typeIcons[elements[index.row()]->getType()]);
					}
					case 3:
					{
						return QIcon(statusIcons[elements[index.row()]->getStatus()]);
					}
				}
			}
		case Qt::CheckStateRole:
			{
				if (index.column() == 3)
				{
					return elements[index.row()]->getIsReadyToInstall() ? Qt::Checked : Qt::Unchecked;
				}				
			}
	}

	return QVariant();
}

QVariant DependenciesListModel::headerData(int section, Qt::Orientation orientation, int role) const
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
				case 3:
					{
					return QString("Status");
					}
			}

		}
	}

	return QVariant();
}

bool DependenciesListModel::parseDependenciesFromFile(const QString& filePath)
{
	QFile file(filePath);

	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream input(&file);

	while (!input.atEnd())
	{
		QString schemeName = "";
		QString name = "";
		QString typeName = "";
		ObjectType type = ObjectType::typeCount;

		input >> schemeName >> name >> typeName;

		if (typeName == "table")
		{
			type = ObjectType::table;
		}
		if (typeName == "function")
		{
			type = ObjectType::function;
		}
		if (typeName == "sequence")
		{
			type = ObjectType::sequence;
		}
		if (typeName == "view")
		{
			type = ObjectType::view;
		}
		if (typeName == "trigger")
		{
			type = ObjectType::trigger;
		}
		if (typeName == "index")
		{
			type = ObjectType::index;
		}

		elements.push_back(new DependenciesListElement(this, type, schemeName, name));
	}

	file.close();
	return true;
}

void DependenciesListModel::getTestResult(const QBitArray& testResult)
{
	for (auto i = 0; i < testResult.count() - 1; ++i)
	{
		if (testResult[i + 1])
		{
			elements[i]->setSatisfied();
			elements[i]->setReadyToInstall();
		}
		else
		{
			elements[i]->setNotSatisfied();
		}
	}

	const QModelIndex topLeft = createIndex(0, statusColumnIndex);
	const QModelIndex bottomRight = createIndex(elements.count(), statusColumnIndex);
	emit dataChanged(topLeft, bottomRight);
}
