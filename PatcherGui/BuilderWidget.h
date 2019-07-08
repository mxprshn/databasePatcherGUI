#pragma once

#include <QWidget>
#include "ui_BuilderWidget.h"

class QListWidget;
class QHBoxLayout;

class BuilderWidget : public QWidget, public Ui::BuilderWidget
{
	Q_OBJECT

public:
	BuilderWidget(QWidget *parent = Q_NULLPTR);
	~BuilderWidget();

private:
	QHBoxLayout *mainLayout;
	QListWidget *objectsListWidget;


};
