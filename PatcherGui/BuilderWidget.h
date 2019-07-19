#pragma once

#include <QWidget>
#include "ui_BuilderWidget.h"

class QListWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QGroupBox;
class QAction;
class QPushButton;
class QComboBox;
class QLineEdit;
class QToolButton;
class QSize;
class QAbstractItemModel;
class QTreeView;

class BuilderWidget : public QWidget, public Ui::BuilderWidget
{
	Q_OBJECT

public:

	BuilderWidget(QWidget *parent = Q_NULLPTR);
	~BuilderWidget();
	void setObjectTypeListModel(QAbstractItemModel *model);
	void setBuildListModel(QAbstractItemModel *model);
private:

	void initializeItemList();
	void initializeToolButtons();
	void initializeAddItemBox();

	const QSize toolButtonSize;
	const QSize toolButtonIconSize;

	QGridLayout *mainLayout;
	QHBoxLayout *addItemLayout;
	QVBoxLayout *itemListLayout;
	QVBoxLayout *toolsLayout;

	QGroupBox *itemListGroupBox;
	QTreeView *itemListView;

	QGroupBox *addItemGroupBox;
	QComboBox *typeComboBox;
	QLineEdit *itemNameEdit;

	QPushButton *addButton;
	QToolButton *removeButton;
	QToolButton *moveUpButton;
	QToolButton *moveDownButton;
	QToolButton *buildButton;
};