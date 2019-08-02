#include <QDropEvent>

#include "PatchListWidget.h"
#include "ObjectType.h"
#include "PatchList.h"

// Multiple selection?

const QHash<int, QString> *PatchListWidget::typeIcons = new QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
	, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
	, {trigger, ":/images/trigger.svg"}, {index, ":/images/index.svg"} });

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
		if (foundItems.at(i)->text(typeColumn) == PatchList::typeName(typeIndex) && foundItems.at(i)->text(schemaColumn) == schema)
		{
			return true;
		}
	}

	return false;
}

QString PatchListWidget::typeIcon(int typeIndex)
{
	// Add invalid index handling
	return typeIcons->value(typeIndex);
}

void PatchListWidget::dropEvent(QDropEvent *event)
{
	QTreeWidget::dropEvent(event);
	setCurrentItem(itemAt(event->pos()));
}

PatchListWidget::~PatchListWidget()
{
	delete typeIcons;
}
