#include <QLineEdit>
#include "LoginWindow.h"
#include "ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
	clear();

	connect(ui->buttonBox, SIGNAL(accepted()), this, SIGNAL(connectButtonClicked()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(clear()));
}

QString LoginWindow::getHostInput() const
{
	return ui->hostLineEdit->text();
}

int LoginWindow::getPortInput() const
{
	return ui->portLineEdit->text().toInt();
}

QString LoginWindow::getDatabaseInput() const
{
	return ui->databaseLineEdit->text();
}

QString LoginWindow::getUsernameInput() const
{
	return ui->usernameLineEdit->text();
}

QString LoginWindow::getPasswordInput() const
{
	return ui->passwordLineEdit->text();
}

void LoginWindow::clear()
{
	ui->hostLineEdit->clear();
	ui->portLineEdit->setText("5432");
	ui->databaseLineEdit->clear();
	ui->usernameLineEdit->clear();
	ui->passwordLineEdit->clear();
}

LoginWindow::~LoginWindow()
{
	delete ui;
}