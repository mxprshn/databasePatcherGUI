#pragma once

#include <QTreeWidget>

class PatchListWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PatchListWidget(QObject *parent);
	~PatchListWidget();
};
