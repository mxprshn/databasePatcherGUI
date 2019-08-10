#pragma once

#include <QWidget>
#include <QDir>
#include "ui_InstallerWidget.h"

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

private:
	Ui::InstallerWidget *ui;
	QDir patchDir;

	bool initPatchList(const QString &path);
	bool initDependencyList(const QString &path);
	void clearCurrentPatch();
	void setReadyToOpen();
	bool isPatchOpened;
	bool checkConnection();
signals:
	void connectionRequested();
private slots:
	void onOpenButtonClicked();
	void onCheckButtonClicked();
	void onInstallButtonClicked();
	void onItemCheckChanged();
};