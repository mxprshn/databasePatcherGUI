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
private:
	Ui::BuilderWidget *ui;
	PatchList *patchList;
	void addScripts(const QString &input);
	QValidator *functionInputValidator;
private slots:
	void onAddButtonClicked();
	void onBuildButtonClicked();
	void onExplorerButtonClicked();
	void onMoveUpButtonClicked();
	void onMoveDownButtonClicked();
	void onRemoveButtonClicked();
	void onItemSelectionChanged();
	void onCurrentTypeChanged(int type);
	void onNameTextChanged(const QString &input);
	bool startPatchBuild(const QString &path);
};