#pragma once

#include <QWidget>
#include "ui_InstallerWidget.h"
#include "DependenciesListModel.h"

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
class QStringArray;
class InstallerHandler;

class InstallerWidget : public QWidget, public Ui::InstallerWidget
{
	Q_OBJECT

public:

	InstallerWidget(QWidget *parent = Q_NULLPTR);
	~InstallerWidget();
	void setDependenciesListModel(QAbstractItemModel *model);
	void setInstallListModel(QAbstractItemModel *model);

private:

	const QString installerProgram;

	void initializeOpenPatchBox();
	void initializeItemLists();
	void initializeToolButtons();
	void initializeActions();

	const QSize toolButtonSize;
	const QSize toolButtonIconSize;

	QGridLayout *mainLayout;
	QHBoxLayout *openPatchLayout;
	QVBoxLayout *itemListLayout;
	QVBoxLayout *dependenciesListLayout;
	QVBoxLayout *toolsLayout;

	QGroupBox *openPatchGroupBox;
	QGroupBox *itemListGroupBox;
	QGroupBox *dependenciesListGroupBox;

	QListWidget *itemListWidget;
	QTreeView *dependenciesListView;

	QLineEdit *patchPathLineEdit;

	QPushButton *openPatchButton;
	QPushButton *openExplorerButton;

	QToolButton *testButton;
	QToolButton *installButton;

	QAction *testDependenciesAction;

	//DependenciesListModel *dependenciesListModel;

	InstallerHandler *installerHandler;

signals:
	void testButtonClicked();

private slots:

	//void requestTest();
};
