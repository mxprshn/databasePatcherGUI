#pragma once

#include <QTreeWidget>

class PatchListWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PatchListWidget(QWidget *parent);
	~PatchListWidget();
	static QString typeIcon(int typeIndex);
	static QString typeName(int typeIndex);
private:
	static const QHash<int, QString> *typeIcons;
	static const QHash<int, QString> *typeNames;
};