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
class UiController;
class QDialogButtonBox;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
private:
	UiController *mainController;
	LoginWindow *loginWindow;

	void initializeDocks();
	QPlainTextEdit *logOutput;
	QDockWidget *logOutputDock;

	void initializeTabs();
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
	void requestConnection();
	void requestAddition();
	void setConnectionInfo(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
	void setDefaultConnectionInfo();
	void showConnectionError(const QString &errorMessage);
	void install();
signals:
	void connectionRequested(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
	void additionRequested(const int typeIndex, const QString &schema, const QString &input);
};
