#pragma once

#include <QWidget>
#include "ui_BuilderWidget.h"

class PatchList;
class QSqlQueryModel;

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
	void initSchemaComboBox();
	void clearSchemaComboBox();
private:
	Ui::BuilderWidget *ui;
	PatchList *patchList;
	QSqlQueryModel *schemaListModel;
	void addScripts(const QString &input);
	QRegExp functionInputRegExp;
	bool checkConnection();
	void initScriptInput();
	bool startPatchBuild(const QString &path);
signals:
	void connectionRequested();
	void itemCountChanged();
private slots:
	void onAddButtonClicked();
	void onBuildButtonClicked();
	void onExplorerButtonClicked();
	void onMoveUpButtonClicked();
	void onMoveDownButtonClicked();
	void onRemoveButtonClicked();
	void onClearButtonClicked();
	void onItemSelectionChanged();
	void onCurrentTypeChanged(int type);
	void onNameTextChanged(const QString &input);
	void onItemCountChanged();
};