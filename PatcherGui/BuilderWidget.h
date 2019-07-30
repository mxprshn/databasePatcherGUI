#pragma once

#include <QWidget>
#include "ui_BuilderWidget.h"

class PatchList;

namespace Ui
{
	class BuilderWidget;
}

class BuilderWidget : public QWidget
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
	Ui::BuilderWidget *ui;

	PatchList *patchList;
	void addToPatchListWidget(int type, const QString &schemaName, const QString &itemName);
	void addScripts();

	const QString wrongFunctionInputMessage;

	QRegExp functionInputRegex;;
	QValidator *functionInputValidator;

signals:
	void addButtonClicked();

private slots:
	void onWrongFunctionInput();
	void onAddButtonClicked();
	void onBuildButtonClicked();
	void onMoveUpButtonClicked();
	void onMoveDownButtonClicked();
	void onRemoveButtonClicked();
	void onItemSelectionChanged();
	void onCurrentTypeChanged(int type);
	void buildPatch();
};