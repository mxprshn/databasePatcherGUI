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
	bool setCheckStatus(const QBitArray &checkResult);
	void add(int typeIndex, const QString &schema, const QString &name);
private:
	static const QHash<int, QString> statusIcons;
};
