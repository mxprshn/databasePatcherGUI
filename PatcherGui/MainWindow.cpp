#include <QPlainTextEdit>
#include <QDockWidget>
#include <QLabel>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QAbstractItemModel>
#include "MainWindow.h"
#include "BuilderWidget.h"
#include "InstallerWidget.h"
#include "LoginWindow.h"
#include "UiController.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mainController(new UiController(this))
	, loginWindow(new LoginWindow(this))
	, builderWidget(new BuilderWidget)
	, installerWidget(new InstallerWidget)
{
	setupUi(this);
	setWindowTitle("Database patcher");

	initializeActions();
	initializeMainMenu();
	initializeTabs();
	initializeDocks();
	initializeToolBars();

	installerWidget->setDependenciesListModel(mainController->getDependenciesListModel());

	connect(this->loginWindow, SIGNAL(connectButtonClicked()), this, SLOT(requestConnection()));
	connect(this, SIGNAL(connectionRequested(const QString&, const QString&, const QString&, const QString&, const int)),
		this->mainController, SLOT(connectToDatabase(const QString&, const QString&, const QString&, const QString&, const int)));
	connect(this->mainController, SIGNAL(connectedToDatabase(const QString&, const QString&, const QString&, const QString&, const int)),
		this, SLOT(setConnectionInfo(const QString&, const QString&, const QString&, const QString&, const int)));
	connect(this->mainController, SIGNAL(connectedToDatabase(const QString&, const QString&, const QString&, const QString&, const int)),
		this->loginWindow, SLOT(clear()));
	connect(this->mainController, SIGNAL(connectedToDatabase(const QString&, const QString&, const QString&, const QString&, const int)),
		this->loginWindow, SLOT(close()));
	connect(this->mainController, SIGNAL(notConnectedToDatabase(const QString&)), this, SLOT(showConnectionError(const QString&)));
	connect(this->mainController, SIGNAL(disconnectedFromDatabase()), this, SLOT(setDefaultConnectionInfo()));
	connect(loginAction, SIGNAL(triggered()), loginWindow, SLOT(show()));
	connect(logoutAction, SIGNAL(triggered()), this->mainController, SLOT(disconnectFromDatabase()));
	connect(installerWidget->getTestAction(), SIGNAL(triggered()), mainController, SLOT(testDependencies()));

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
}

void MainWindow::initializeTabs()
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

void MainWindow::requestConnection()
{
	emit connectionRequested(loginWindow->getDatabaseInput(), loginWindow->getUsernameInput(),
		loginWindow->getPasswordInput(), loginWindow->getHostInput(), loginWindow->getPortInput());
}

void MainWindow::setConnectionInfo(const QString& database, const QString& user, const QString& password, const QString& server, const int port)
{
	databaseInformation->setText("Connected to \"" + database + "\" as \"" + user + "\"");
	loginAction->setDisabled(true);
	logoutAction->setEnabled(true);
}

void MainWindow::showConnectionError(const QString &errorMessage)
{
	QMessageBox::warning(loginWindow, "Connection error", errorMessage,
		QMessageBox::Ok,	QMessageBox::Ok);
}

void MainWindow::setDefaultConnectionInfo()
{
	databaseInformation->setText("Connect to database!");
	loginAction->setEnabled(true);
	logoutAction->setDisabled(true);
}