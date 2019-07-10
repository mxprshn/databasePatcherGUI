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

class InstallerWidget : public QWidget, public Ui::InstallerWidget
{
	Q_OBJECT

public:
	InstallerWidget(QWidget *parent = Q_NULLPTR);
	~InstallerWidget();

private:
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

	QListWidget *itemListWidget;
	QListWidget *dependenciesListWidget;

	QLineEdit *patchPathLineEdit;

	QPushButton *openPatchButton;
	QPushButton *openExplorerButton;

	QToolButton *testButton;
	QToolButton *installButton;
};
