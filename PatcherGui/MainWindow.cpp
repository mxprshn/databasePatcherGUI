#include <QPlainTextEdit>
#include <QDockWidget>
#include <QLabel>
#include "MainWindow.h"
#include "BuilderWidget.h"
#include "InstallerWidget.h"
#include "LoginWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, loginWindow(new LoginWindow(this))
{
	setupUi(this);

	initializeActions();
	initializeMainMenu();
	initializeModeTabs();
	initializeLogOutput();
	initializeToolBars();

	setCentralWidget(modeTab);
	addDockWidget(Qt::BottomDockWidgetArea, logOutputDock);
	addToolBar(Qt::TopToolBarArea, mainToolBar);
	setMinimumSize(800, 600);
}

void MainWindow::initializeActions()
{
	loginAction = new QAction(QIcon(":/images/addDatabase.svg"),"Connect to database...", this);
	connect(loginAction, SIGNAL(triggered()), this, SLOT(login()));
}

void MainWindow::initializeModeTabs()
{
	builderWidget = new BuilderWidget;
	installerWidget = new InstallerWidget;

	modeTab = new QTabWidget;
	modeTab->addTab(builderWidget, "Build");
	modeTab->addTab(installerWidget, "Install");
	modeTab->setMinimumHeight(400);
}

void MainWindow::initializeLogOutput()
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

	QMainWindow::menuBar()->addMenu(databaseMenu);
}

void MainWindow::initializeToolBars()
{
	databaseInformation = new QLabel("Connect to database!");
	mainToolBar->addAction(loginAction);
	mainToolBar->addWidget(databaseInformation);
}

bool MainWindow::login()
{
	loginWindow->show();
	return true;
}

MainWindow::~MainWindow()
{
}
