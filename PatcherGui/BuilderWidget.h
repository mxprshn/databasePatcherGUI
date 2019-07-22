#pragma once

#include <QWidget>
#include "ObjectType.h"
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
class QValidator;
class QLabel;

class BuilderWidget : public QWidget, public Ui::BuilderWidget
{
	Q_OBJECT

public:

	BuilderWidget(QWidget *parent = Q_NULLPTR);
	~BuilderWidget();
	void setBuildListModel(QAbstractItemModel *model);
	QString getItemNameInput();
	int getObjectTypeIndex();
private:
	void initializeItemList();
	void initializeToolButtons();
	void initializeAddItemBox();

	static const int smallIconSize = 15;
	const QString wrongFunctionInputMessage;

	const QSize toolButtonSize;
	const QSize toolButtonIconSize;

	QGridLayout *mainLayout;
	QGridLayout *addItemLayout;
	QVBoxLayout *itemListLayout;
	QVBoxLayout *toolsLayout;

	QLabel *inputStatusLabel;

	QGroupBox *itemListGroupBox;
	QTreeView *itemListView;

	QGroupBox *addItemGroupBox;
	QComboBox *typeComboBox;
	QComboBox *schemeComboBox;
	QListWidget *schemeListWidget;
	QLineEdit *itemNameEdit;

	QPushButton *addButton;
	QToolButton *removeButton;
	QToolButton *moveUpButton;
	QToolButton *moveDownButton;
	QToolButton *buildButton;

	QRegExp functionInputRegex;;
	QValidator *functionInputValidator;

signals:
	void addButtonClicked();
	void buildButtonClicked();

private slots:
	void onWrongFunctionInput();
};