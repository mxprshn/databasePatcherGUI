#include <QPlainTextEdit>
#include <QDockWidget>
#include <QLabel>
#include <QMessageBox>
#include "MainWindow.h"
#include "BuilderWidget.h"
#include "InstallerWidget.h"
#include "LoginWindow.h"
#include "DatabaseProvider.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, databaseProvider(new DatabaseProvider(this))
	, loginWindow(new LoginWindow(this))
	, builderWidget(new BuilderWidget)
	, installerWidget(new InstallerWidget)
{
	setupUi(this);
	setWindowTitle("Database patcher");

	initializeActions();
	initializeMainMenu();
	initializeModeTabs();
	initializeDocks();
	initializeToolBars();

	setCentralWidget(modeTab);
	addDockWidget(Qt::BottomDockWidgetArea, logOutputDock);
	addToolBar(Qt::TopToolBarArea, mainToolBar);
	setMinimumSize(800, 600);
}

void MainWindow::initializeActions()
{
	loginAction = new QAction(QIcon(":/images/addDatabase.svg"),"Connect to database...", this);
	logoutAction = new QAction(QIcon(":/images/removeDatabase.svg"), "Disconnect", this);
	logoutAction->setDisabled(true);

	connect(loginAction, SIGNAL(triggered()), loginWindow, SLOT(showLoginWindow()));
	connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));
}

void MainWindow::initializeModeTabs()
{
	modeTab = new QTabWidget;
	modeTab->addTab(builderWidget, "Build");
	modeTab->addTab(installerWidget, "Install");
	modeTab->setMinimumHeight(400);
}

void MainWindow::initializeDocks()
{
	logOutputDock = new QDockWidget("Log output", this);
	logOutput = new QPlainTextEdit;
	logOutputDock->setWidget(logOutput);
	logOutputDock->setAllowedAreas(Qt::BottomDockWidgetArea);	
}

void MainWindow::initializeMainMenu()
{
	databaseMenu = new QMenu("Database");
	databaseMenu->addAction(loginAction);
	databaseMenu->addAction(logoutAction);

	QMainWindow::menuBar()->addMenu(databaseMenu);
}

void MainWindow::initializeToolBars()
{
	databaseInformation = new QLabel("Connect to database!");
	mainToolBar->addAction(loginAction);
	mainToolBar->addWidget(databaseInformation);
	mainToolBar->setMovable(false);
}

void MainWindow::login(const QString &database, const QString &user, const QString &password,
	const QString &server, const int port) const
{
	auto connectionMessage = "Connected to \"" + database + "\" as \"" + user + "\"";
	const auto isConnected = databaseProvider->connect(database, user, password, server, port, connectionMessage);

	if (isConnected)
	{
		databaseInformation->setText(connectionMessage);
		loginWindow->close();
		loginAction->setDisabled(true);
		logoutAction->setEnabled(true);
	}
	else
	{
		QMessageBox::warning(loginWindow, "Connection error", connectionMessage, QMessageBox::Ok,
			QMessageBox::Ok);
	}
}

void MainWindow::logout()
{
	databaseProvider->disconnect();
	databaseInformation->setText("Connect to database!");
	loginAction->setEnabled(true);
	logoutAction->setDisabled(true);
}


MainWindow::~MainWindow()
{
	delete databaseProvider;
}
