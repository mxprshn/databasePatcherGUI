#pragma once

#include <QWidget>
#include "ui_BuilderWidget.h"

class QListWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QAction;
class QPushButton;
class QComboBox;
class QLineEdit;
class QToolButton;

class BuilderWidget : public QWidget, public Ui::BuilderWidget
{
	Q_OBJECT

public:
	BuilderWidget(QWidget *parent = Q_NULLPTR);
	~BuilderWidget();

private:
	QHBoxLayout *mainLayout;
	QHBoxLayout *addItemLayout;
	QVBoxLayout *itemListLayout;
	QVBoxLayout *toolsLayout;

	QListWidget *itemListWidget;

	QGroupBox *addItemGroupBox;
	QComboBox *typeComboBox;
	QLineEdit *itemNameEdit;

	QPushButton *addButton;
	QToolButton *removeButton;
	QToolButton *moveUpButton;
	QToolButton *moveDownButton;
	QToolButton *buildButton;
};