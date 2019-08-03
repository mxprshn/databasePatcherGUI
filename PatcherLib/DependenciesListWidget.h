#pragma once

#include <QTreeWidget>

class DependenciesListWidget : public QTreeWidget
{
	Q_OBJECT

public:

	enum ColumnIndexes
	{
		typeColumn,
		schemaColumn,
		nameColumn,
		statusColumn
	};

	enum CheckStatus
	{
		waitingForCheck,
		satisfied,
		notSatisfied
	};

	DependenciesListWidget(QWidget *parent);
	~DependenciesListWidget();
	bool setCheckStatus(const QBitArray &checkResult);
	static QString typeIcon(int typeIndex);
	static QString statusIcon(int status);
private:
	static const QHash<int, QString> *typeIcons;
	static const QHash<int, QString> *statusIcons;
};
