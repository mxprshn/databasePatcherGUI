#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

class QAction;
class QLabel;
class LoginWindow;
class UiController;
class LogOutputDevice;

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
private:
	Ui::MainWindow *ui;
	LogOutputDevice *logOutputDevice;
	UiController *mainController;
	LoginWindow *loginWindow;

	//void initializeDocks();
	//QTextEdit *logOutput;
	//QDockWidget *logOutputDock;

	//void initializeTabs();
	//QTabWidget *modeTab;
	//BuilderWidget *builderWidget;
	//InstallerWidget *installerWidget;

	//void initializeActions();
	QAction *loginAction;
	QAction *logoutAction;

	//void initializeMainMenu();	
	//QMenu *databaseMenu;

	//void initializeToolBars();
	QLabel *databaseInformation;

private slots:
	void requestConnection();
	void setConnectionInfo(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
	void setDefaultConnectionInfo();
	void showConnectionError(const QString &errorMessage);
signals:
	void connectionRequested(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
};
