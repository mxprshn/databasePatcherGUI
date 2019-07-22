#include <QLineEdit>
#include "LoginWindow.h"
#include "ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::LoginWindow)
{
	ui->setupUi(this);

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &LoginWindow::connectButtonClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &LoginWindow::close);
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

void LoginWindow::clear() const
{
	foreach(QLineEdit* lineEdit, findChildren<QLineEdit*>())
	{
		lineEdit->clear();
	}
}

LoginWindow::~LoginWindow()
{
	delete ui;
}