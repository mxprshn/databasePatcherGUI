#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QTabWidget;
class QPlainTextEdit;
class QMenu;
class QAction;
class QLabel;
class BuilderWidget;
class InstallerWidget;
class LoginWindow;
class DatabaseProvider;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private:
	QString databaseName;
	QString username;
	QString serverName;
	int port;
	QString password;

	LoginWindow *loginWindow;
	DatabaseProvider *databaseProvider;

	void initializeDocks();
	QPlainTextEdit *logOutput;
	QDockWidget *logOutputDock;

	void initializeModeTabs();
	QTabWidget *modeTab;
	BuilderWidget *builderWidget;
	InstallerWidget *installerWidget;

	void initializeActions();
	QAction *loginAction;

	void initializeMainMenu();	
	QMenu *databaseMenu;

	void initializeToolBars();
	QLabel *databaseInformation;

private slots:
	bool login();
};
