#include "DependenciesListWidget.h"
#include "ObjectTypes.h"
#include "PatchList.h"
#include <QHeaderView>
#include <QBitArray>
 
const QHash<int, QString> DependenciesListWidget::statusIcons = QHash<int, QString>({ {waitingForCheck, ":/images/unchecked.svg"}
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
	
	for (auto i = 0; i < checkResult.count() - 1; ++i)
	{
		if (checkResult[i + 1])
		{
			topLevelItem(i)->setCheckState(statusColumn, Qt::Checked);
			topLevelItem(i)->setIcon(statusColumn, QIcon(statusIcons.value(satisfied)));
		}
		else
		{
			topLevelItem(i)->setIcon(statusColumn, QIcon(statusIcons.value(notSatisfied)));
		}
	}

	return true;
}

void DependenciesListWidget::add(int typeIndex, const class QString& schema, const class QString& name)
{
	auto *newItem = new QTreeWidgetItem(this);
	newItem->setIcon(typeColumn, QIcon(ObjectTypes::typeIcons.value(typeIndex)));
	newItem->setText(typeColumn, ObjectTypes::typeNames.value(typeIndex));
	newItem->setData(typeColumn, Qt::UserRole, typeIndex);
	newItem->setText(schemaColumn, schema);
	newItem->setText(nameColumn, name);
	newItem->setIcon(statusColumn, QIcon(statusIcons.value(waitingForCheck)));
	newItem->setCheckState(statusColumn, Qt::Unchecked);
	newItem->setFlags(Qt::ItemIsEnabled);
	addTopLevelItem(newItem);
}