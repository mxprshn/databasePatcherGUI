#include <QDropEvent>

#include "PatchListWidget.h"
#include "ObjectTypes.h"
#include "PatchList.h"

// Multiple selection?

PatchListWidget::PatchListWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	setColumnCount(3);
	QStringList headerLabels;
	headerLabels.insert(typeColumn, "Type");
	headerLabels.insert(schemaColumn, "Schema");
	headerLabels.insert(nameColumn, "Name");
	setHeaderLabels(headerLabels);
	setRootIsDecorated(false);
	setSelectionMode(SingleSelection);
	setDragEnabled(true);
	viewport()->setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(InternalMove);
}

QStringList PatchListWidget::itemList() const
{
	QStringList result;

	for (auto i = 0; i < topLevelItemCount(); ++i)
	{
		QString currentString = "";

		for (auto j = 0; j < columnCount(); ++j)
		{
			currentString += topLevelItem(i)->text(j);
		}

		result.append(currentString);
	}

	return result;
}

bool PatchListWidget::itemExists(int typeIndex, const class QString &schema, const class QString &name)
{
	const auto foundItems = findItems(name, Qt::MatchFixedString, nameColumn);

	for (auto i = 0; i < foundItems.count(); ++i)
	{
		if (foundItems.at(i)->text(typeColumn) == ObjectTypes::typeNames.value(typeIndex) && foundItems.at(i)->text(schemaColumn) == schema)
		{
			return true;
		}
	}

	return false;
}

void PatchListWidget::add(int typeIndex, const class QString& schema, const class QString& name, bool isDraggable)
{
	auto *newItem = new QTreeWidgetItem(this);

	newItem->setIcon(typeColumn, QIcon(ObjectTypes::typeIcons.value(typeIndex)));
	newItem->setText(typeColumn, ObjectTypes::typeNames.value(typeIndex));
	newItem->setData(typeColumn, Qt::UserRole, typeIndex);
	newItem->setText(schemaColumn, schema);
	newItem->setText(nameColumn, name);

	if (isDraggable)
	{
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
	}
	else
	{
		newItem->setFlags(Qt::ItemIsEnabled);
	}

	addTopLevelItem(newItem);
}

void PatchListWidget::dropEvent(QDropEvent *event)
{
	QTreeWidget::dropEvent(event);
	setCurrentItem(itemAt(event->pos()));
}