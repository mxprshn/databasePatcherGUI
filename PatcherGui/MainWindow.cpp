#include <QPlainTextEdit>
#include <QDockWidget>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QDialogButtonBox>
#include <QAbstractItemModel>
#include "MainWindow.h"
#include "BuilderWidget.h"
#include "InstallerWidget.h"
#include "LoginWindow.h"
#include "UiController.h"
#include "LogOutputDevice.h"
#include "InstallerHandler.h"
#include "BuilderHandler.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, logOutputDevice(new LogOutputDevice(this))
	, mainController(new UiController(this))
	, loginWindow(new LoginWindow(this))
{
	ui->setupUi(this);
	setWindowTitle("Database patcher");

	logOutputDevice->setTextEdit(ui->logTextEdit);
	logOutputDevice->open(QIODevice::WriteOnly);
	InstallerHandler::setOutputDevice(*logOutputDevice);
	BuilderHandler::setOutputDevice(*logOutputDevice);

	loginAction = new QAction(QIcon(":/images/addDatabase.svg"), "Connect to database...", this);
	logoutAction = new QAction(QIcon(":/images/removeDatabase.svg"), "Disconnect", this);
	logoutAction->setDisabled(true);
	databaseInformation = new QLabel("Connect to database!");
	ui->databaseMenu->addAction(loginAction);
	ui->databaseMenu->addAction(logoutAction);
	ui->mainToolBar->addAction(loginAction);
	ui->mainToolBar->addWidget(databaseInformation);

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
}

MainWindow::~MainWindow()
{
	delete ui;
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
	ui->builderTab->setSchemaComboBoxModel(mainController->getSchemaListModel());
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