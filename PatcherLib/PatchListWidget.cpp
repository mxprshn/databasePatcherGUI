#include "PatchListWidget.h"
#include "ObjectType.h"

const QHash<int, QString> *PatchListWidget::typeNames = new QHash<int, QString>({ {script, "script"}, {table, "table"}
	, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
	, {index, "index"} });

const QHash<int, QString> *PatchListWidget::typeIcons = new QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
	, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
	, {trigger, ":/images/trigger.svg"}, {index, ":/images/index.svg"} });

PatchListWidget::PatchListWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	// Should it be here???
	setRootIsDecorated(false);
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

PatchListWidget::~PatchListWidget()
{
	delete typeNames;
	delete typeIcons;
}
