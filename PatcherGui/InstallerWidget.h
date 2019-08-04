#pragma once

#include <QWidget>
#include <QDir>
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
	QDir patchDir;
	PatchList *patchList;
	PatchList *dependenciesList;
	bool initPatchList(const QString &filePath);
	bool initDependenciesList(const QString &filePath);
	void clearCurrentPatch();
	void setReadyToOpen();
	bool isPatchOpened;
	QAction *testDependenciesAction;
private slots:
	void onOpenButtonClicked();
	void onCheckButtonClicked();
	void onInstallButtonClicked();
};