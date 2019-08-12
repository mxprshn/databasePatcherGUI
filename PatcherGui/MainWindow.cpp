#include <QMessageBox>

#include "MainWindow.h"
#include "BuilderWidget.h"
#include "InstallerWidget.h"
#include "LoginWindow.h"
#include "LogOutputDevice.h"
#include "InstallerHandler.h"
#include "BuilderHandler.h"
#include "DatabaseProvider.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, logOutputDevice(new LogOutputDevice(this))
	, loginWindow(new LoginWindow(this))
{
	ui->setupUi(this);
	logOutputDevice->setTextEdit(ui->logTextEdit);
	logOutputDevice->open(QIODevice::WriteOnly);
	InstallerHandler::setOutputDevice(*logOutputDevice);
	BuilderHandler::setOutputDevice(*logOutputDevice);

	connectAction = new QAction(QIcon(":/images/addDatabase.svg"), "Connect to database...", this);
	disconnectAction = new QAction(QIcon(":/images/removeDatabase.svg"), "Disconnect", this);
	disconnectAction->setDisabled(true);
	databaseInformation = new QLabel("Connect to database!");

	ui->databaseMenu->addAction(connectAction);
	ui->databaseMenu->addAction(disconnectAction);

	ui->viewMenu->addAction(QIcon(":/images/hammer.svg"),"Build", [=]() {ui->tabWidget->setCurrentWidget(ui->builderTab); });
	ui->viewMenu->addAction(QIcon(":/images/install.svg"), "Install", [=]() {ui->tabWidget->setCurrentWidget(ui->installerTab); });

	ui->mainToolBar->addAction(connectAction);
	ui->mainToolBar->addWidget(databaseInformation);

	connect(loginWindow, SIGNAL(connectButtonClicked()), this, SLOT(onDialogConnectButtonClicked()));
	connect(connectAction, SIGNAL(triggered()), this, SLOT(onConnectionRequested()));
	connect(disconnectAction, SIGNAL(triggered()), this, SLOT(onDisconnectButtonClicked()));
	connect(ui->builderTab, SIGNAL(connectionRequested()), this, SLOT(onConnectionRequested()));
	connect(ui->installerTab, SIGNAL(connectionRequested()), this, SLOT(onConnectionRequested()));
	connect(this, SIGNAL(connected()), ui->builderTab, SLOT(onConnected()));
	connect(this, SIGNAL(disconnectionStarted()), ui->builderTab, SLOT(onDisconnectionStarted()));
}

MainWindow::~MainWindow()
{
	if (DatabaseProvider::isConnected())
	{
		emit disconnectionStarted();
		DatabaseProvider::disconnect();
	}

	delete ui;
}

void MainWindow::onDialogConnectButtonClicked()
{
	QString errorMessage = "";

	if (DatabaseProvider::connect(loginWindow->getDatabaseInput(), loginWindow->getUsernameInput()
		, loginWindow->getPasswordInput(), loginWindow->getHostInput(), loginWindow->getPortInput(), errorMessage))
	{
		databaseInformation->setText("Connected to \"" + DatabaseProvider::database() + "\" as \""
			+ DatabaseProvider::user() + "\"");
		connectAction->setDisabled(true);
		disconnectAction->setEnabled(true);
		loginWindow->clear();
		loginWindow->close();
		emit connected();
	}
	else
	{
		ui->logTextEdit->append(errorMessage);
		QApplication::beep();
		QMessageBox::warning(this, "Connection error"
				, "Connection error. See logs for detailed information.", QMessageBox::Ok, QMessageBox::Ok);
	}
}

void MainWindow::onConnectionRequested()
{
	loginWindow->show();
}

void MainWindow::onDisconnectButtonClicked()
{
	emit disconnectionStarted();
	DatabaseProvider::disconnect();
	databaseInformation->setText("Connect to database!");
	connectAction->setEnabled(true);
	disconnectAction->setDisabled(true);
}