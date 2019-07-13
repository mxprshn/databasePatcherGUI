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
	QAction *logoutAction;

	void initializeMainMenu();	
	QMenu *databaseMenu;

	void initializeToolBars();
	QLabel *databaseInformation;

private slots:	

	void login(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port) const;

	void logout();
};
