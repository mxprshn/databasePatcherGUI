#pragma once

#include <QTreeWidget>

class PatchListWidget : public QTreeWidget
{
	Q_OBJECT

public:

	enum ColumnIndexes
	{
		typeColumn,
		schemaColumn,
		nameColumn
	};

	PatchListWidget(QWidget *parent);
	~PatchListWidget();
	QStringList itemList() const;
	static QString typeIcon(int typeIndex);
	bool itemExists(int typeIndex, const QString &schema, const QString &name);
	void add(int typeIndex, const QString &schema, const QString &name, bool isDraggable);
private:
	void dropEvent(QDropEvent *event) override;
	static const QHash<int, QString> *typeIcons;
};