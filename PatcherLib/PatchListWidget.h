#pragma once

#include <QTreeWidget>

class PatchListWidget : public QTreeWidget
{
	Q_OBJECT

public:
	enum ColumnIndexes
	{
		TypeColumn,
		SchemaColumn,
		NameColumn
	};

	PatchListWidget(QWidget *parent);
	~PatchListWidget();
	QStringList itemList() const;
	static QString typeIcon(int typeIndex);
	static QString typeName(int typeIndex);
	bool itemExists(int typeIndex, const QString &schema, const QString &name);
private:
	void dropEvent(QDropEvent *event) override;
	static const QHash<int, QString> *typeIcons;
	static const QHash<int, QString> *typeNames;
};