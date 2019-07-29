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
class PatchListWidget;
class PatchList;

class BuilderWidget : public QWidget, public Ui::BuilderWidget
{
	Q_OBJECT

public:

	BuilderWidget(QWidget *parent = Q_NULLPTR);
	~BuilderWidget();
	void setSchemaComboBoxModel(QAbstractItemModel *model);
	QString getItemNameInput();
	QString getCurrentSchemaName();
	int getObjectTypeIndex();
private:
	// feature
	PatchListWidget *buildListWidget;
	PatchList *patchList;
	void addToPatchListWidget(int type, const QString &schemaName, const QString &itemName);
	void addScripts();
	//
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

private slots:
	void onWrongFunctionInput();
	void onAddButtonClicked();
	void onBuildButtonClicked();
	void onRemoveButtonClicked();
	void buildPatch();
};