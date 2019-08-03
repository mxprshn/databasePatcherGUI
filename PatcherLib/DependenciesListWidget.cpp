#include "DependenciesListWidget.h"
#include "ObjectType.h"
#include <QHeaderView>
#include <QBitArray>

// I don't like the way it is deleted
const QHash<int, QString> *DependenciesListWidget::typeIcons = new QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
	, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
	, {trigger, ":/images/trigger.svg"}, {index, ":/images/index.svg"} });

const QHash<int, QString> *DependenciesListWidget::statusIcons = new QHash<int, QString>({ {waitingForCheck, ":/images/unchecked.svg"}
	, {satisfied, ":/images/checked.svg"}, {notSatisfied, ":/images/error.svg"} });

DependenciesListWidget::DependenciesListWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	setColumnCount(4);
	QStringList headerLabels;
	headerLabels.insert(typeColumn, "Type");
	headerLabels.insert(schemaColumn, "Schema");
	headerLabels.insert(nameColumn, "Name");
	headerLabels.insert(statusColumn, "Status");
	setHeaderLabels(headerLabels);
	setRootIsDecorated(false);
	setSelectionMode(NoSelection);
	header()->setStretchLastSection(false);
	header()->setSectionResizeMode(typeColumn, QHeaderView::ResizeMode::ResizeToContents);
	header()->setSectionResizeMode(schemaColumn, QHeaderView::ResizeMode::ResizeToContents);
	header()->setSectionResizeMode(nameColumn, QHeaderView::ResizeMode::Stretch);
	header()->setSectionResizeMode(statusColumn, QHeaderView::ResizeMode::ResizeToContents);
}

bool DependenciesListWidget::setCheckStatus(const QBitArray& checkResult)
{
	if (checkResult.count() - 1 != topLevelItemCount())
	{
		return false;
	}
	
	// Ugly indexes
	for (auto i = 0; i < checkResult.count() - 1; ++i)
	{
		if (checkResult[i + 1])
		{
			topLevelItem(i)->setCheckState(statusColumn, Qt::Checked);
			topLevelItem(i)->setIcon(statusColumn, QIcon(statusIcons->value(satisfied)));
		}
		else
		{
			topLevelItem(i)->setIcon(statusColumn, QIcon(statusIcons->value(notSatisfied)));
		}
	}

	return true;
}

QString DependenciesListWidget::typeIcon(int typeIndex)
{
	// Add invalid index handling
	return typeIcons->value(typeIndex);
}

QString DependenciesListWidget::statusIcon(int status)
{
	return statusIcons->value(status);
}

DependenciesListWidget::~DependenciesListWidget()
{
	delete typeIcons;
	delete statusIcons;
}