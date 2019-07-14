#include "DependenciesListModel.h"
#include "DependenciesListElement.h"
#include <QFile>
#include <QTextStream>
#include <QIcon>

DependenciesListModel::DependenciesListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	statusIcons.insert(DependenciesListElement::CheckStatus::waitingForCheck, ":/images/unchecked.svg");
	statusIcons.insert(DependenciesListElement::CheckStatus::satisfied, ":/images/checked.svg");
	statusIcons.insert(DependenciesListElement::CheckStatus::notSatisfied, ":/images/error.svg");

	typeIcons.insert(DependenciesListElement::ObjectType::table, ":/images/table.svg");
	typeIcons.insert(DependenciesListElement::ObjectType::function, ":/images/function.svg");
	typeIcons.insert(DependenciesListElement::ObjectType::sequence, ":/images/sequence.svg");
	typeIcons.insert(DependenciesListElement::ObjectType::index, ":/images/function.svg");
	typeIcons.insert(DependenciesListElement::ObjectType::trigger, ":/images/function.svg");
	typeIcons.insert(DependenciesListElement::ObjectType::view, ":/images/view.svg");

	typeNames.insert(DependenciesListElement::ObjectType::table, "table");
	typeNames.insert(DependenciesListElement::ObjectType::function, "function");
	typeNames.insert(DependenciesListElement::ObjectType::sequence, "sequence");
	typeNames.insert(DependenciesListElement::ObjectType::index, "index");
	typeNames.insert(DependenciesListElement::ObjectType::trigger, "trigger");
	typeNames.insert(DependenciesListElement::ObjectType::view, "view");
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
					case 2:
					{
						return QIcon(statusIcons[elements[index.row()]->getStatus()]);
					}
				}
			}
		case Qt::CheckStateRole:
			{
				if (index.column() == 2)
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
						return QString("Name");
					}
				case 2:
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
		QString typeName = "";
		QString name = "";
		DependenciesListElement::ObjectType type = DependenciesListElement::ObjectType::unidentified;

		input >> typeName >> name;

		if (typeName == "table")
		{
			type = DependenciesListElement::ObjectType::table;
		}
		if (typeName == "function")
		{
			type = DependenciesListElement::ObjectType::function;
		}
		if (typeName == "sequence")
		{
			type = DependenciesListElement::ObjectType::sequence;
		}
		if (typeName == "view")
		{
			type = DependenciesListElement::ObjectType::view;
		}
		if (typeName == "trigger")
		{
			type = DependenciesListElement::ObjectType::trigger;
		}
		if (typeName == "index")
		{
			type = DependenciesListElement::ObjectType::index;
		}

		elements.push_back(new DependenciesListElement(this, type, name));
	}

	file.close();
	return true;
}