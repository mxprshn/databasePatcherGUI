#include <QDropEvent>

#include "PatchListWidget.h"
#include "ObjectType.h"

// Multiple selection?

const QHash<int, QString> *PatchListWidget::typeNames = new QHash<int, QString>({ {script, "script"}, {table, "table"}
	, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
	, {index, "index"} });

const QHash<int, QString> *PatchListWidget::typeIcons = new QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
	, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
	, {trigger, ":/images/trigger.svg"}, {index, ":/images/index.svg"} });

PatchListWidget::PatchListWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	setColumnCount(3);
	QStringList headerLabels;
	headerLabels.insert(TypeColumn, "Type");
	headerLabels.insert(SchemaColumn, "Schema");
	headerLabels.insert(NameColumn, "Name");
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

QString PatchListWidget::typeIcon(int typeIndex)
{
	// Add invalid index handling
	return typeIcons->value(typeIndex);
}

QString PatchListWidget::typeName(int typeIndex)
{
	return typeNames->value(typeIndex);
}

void PatchListWidget::dropEvent(QDropEvent *event)
{
	QTreeWidget::dropEvent(event);
	setCurrentItem(itemAt(event->pos()));
}

PatchListWidget::~PatchListWidget()
{
	delete typeNames;
	delete typeIcons;
}
