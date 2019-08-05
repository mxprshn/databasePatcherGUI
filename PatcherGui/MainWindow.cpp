#include <QPlainTextEdit>
#include <QDockWidget>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
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
	ui->viewMenu->addAction("Build", [=]() {ui->tabWidget->setCurrentWidget(ui->builderTab); });
	ui->viewMenu->addAction("Install", [=]() {ui->tabWidget->setCurrentWidget(ui->installerTab); });

	ui->mainToolBar->addAction(loginAction);
	ui->mainToolBar->addWidget(databaseInformation);

	connect(loginWindow, SIGNAL(connectButtonClicked()), this, SLOT(onDialogConnectButtonClicked()));
	connect(loginAction, SIGNAL(triggered()), this, SLOT(onConnectionRequested()));
	connect(logoutAction, SIGNAL(triggered()), this, SLOT(onDisconnectButtonClicked()));
	connect(ui->builderTab, SIGNAL(connectionRequested()), this, SLOT(onConnectionRequested()));
	connect(ui->installerTab, SIGNAL(connectionRequested()), this, SLOT(onConnectionRequested()));
}

MainWindow::~MainWindow()
{
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
		loginAction->setDisabled(true);
		logoutAction->setEnabled(true);
		ui->builderTab->initSchemaComboBox();
	}
	else
	{
		ui->logTextEdit->append(errorMessage);
		QApplication::beep();
		QMessageBox::warning(this, "Connection error"
				, "Connection error. See logs for detailed information.", QMessageBox::Ok, QMessageBox::Ok);
	}

	loginWindow->clear();
	loginWindow->close();
}

void MainWindow::onConnectionRequested()
{
	loginWindow->show();
}

void MainWindow::onDisconnectButtonClicked()
{
	ui->builderTab->clearSchemaComboBox();
	DatabaseProvider::disconnect();
	databaseInformation->setText("Connect to database!");
	loginAction->setEnabled(true);
	logoutAction->setDisabled(true);
}