#pragma once

#include <QWidget>
#include "ui_InstallerWidget.h"

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QToolButton;
class QListWidget;
class QTreeView;
class QTableView;
class QAbstractItemModel;
class PatchListWidget;
class PatchList;
class DependenciesListWidget;

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
signals:
	void installButtonClicked();
private slots:
	void onCheckButtonClicked();
	void onInstallButtonClicked();
};