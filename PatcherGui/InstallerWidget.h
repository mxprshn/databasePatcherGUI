#pragma once

#include <QWidget>
#include "ui_InstallerWidget.h"

class PatchList;

namespace Ui
{
	class InstallerWidget;
}

class InstallerWidget : public QWidget
{
	Q_OBJECT

public:
	InstallerWidget(QWidget *parent = Q_NULLPTR);
	~InstallerWidget();
	QAction* getTestAction() const;
private:
	Ui::InstallerWidget *ui;
	PatchList *patchList;
	PatchList *dependenciesList;
	void initPatchList();
	void initDependenciesList();
	QAction *testDependenciesAction;
private slots:
	void onCheckButtonClicked();
	void onInstallButtonClicked();
};