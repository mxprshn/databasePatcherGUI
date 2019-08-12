#pragma once

#include <QWidget>
#include "ui_BuilderWidget.h"

class QSqlQueryModel;
class ObjectNameCompleter;

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
private:
	Ui::BuilderWidget *ui;
	QSqlQueryModel *schemaListModel;
	ObjectNameCompleter *nameCompleter;
	void addScripts(const QString &input);
	QRegExp functionInputRegExp;
	bool checkConnection();
	void initScriptInput();
	void initCompleter();
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
	void onCurrentSchemaChanged(const QString &schema);
	void onNameTextChanged(const QString &input);
	void onItemCountChanged();
	void onConnected();
	void onDisconnectionStarted();
};