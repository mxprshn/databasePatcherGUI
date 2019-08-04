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
	void clear();
	int getCheckedCount() const;
	bool getAreAllSatisfied() const;
private:
	int checkedCount;
	bool areAllSatisfied;
	static const QHash<int, QString> statusIcons;
signals:
	void itemCheckChanged();
private slots:
	void onItemClicked(QTreeWidgetItem *item, int column);
};
