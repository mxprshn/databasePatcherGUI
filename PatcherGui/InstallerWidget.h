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

class InstallerWidget : public QWidget, public Ui::InstallerWidget
{
	Q_OBJECT

public:
	InstallerWidget(QWidget *parent = Q_NULLPTR);
	~InstallerWidget();
	QAction* getTestAction() const;
private:
	PatchList *patchList;
	PatchList *dependenciesList;
	void initPatchList();
	void initDependenciesList();
	void initializeOpenPatchBox();
	void initializeItemLists();
	void initializeToolButtons();

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
	PatchListWidget *itemListWidget;
	DependenciesListWidget *dependenciesListWidget;
	QLineEdit *patchPathLineEdit;
	QPushButton *openPatchButton;
	QPushButton *openExplorerButton;
	QToolButton *testButton;
	QToolButton *installButton;
	QAction *testDependenciesAction;
signals:
	void installButtonClicked();
private slots:
	void onCheckButtonClicked();
};