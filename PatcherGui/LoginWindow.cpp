#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include "LoginWindow.h"
#include "MainWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	mainLayout = new QVBoxLayout;
	inputLayout = new QFormLayout;
	buttonsLayout = new QHBoxLayout;
	serverInputLayout = new QHBoxLayout;

	serverInputBox = new QLineEdit;
	portInputBox = new QLineEdit;
	databaseInputBox = new QLineEdit;
	usernameInputBox = new QLineEdit;
	passwordInputBox = new QLineEdit;

	passwordInputBox->setEchoMode(QLineEdit::Password);

	buttons = new QDialogButtonBox;

	portLabel = new QLabel("Port:");

	serverInputLayout->addWidget(serverInputBox, 5);
	serverInputLayout->addStretch(1);
	serverInputLayout->addWidget(portLabel);
	serverInputLayout->addWidget(portInputBox, 2);

	inputLayout->addRow("Server:", serverInputLayout);
	inputLayout->addRow("Database:", databaseInputBox);
	inputLayout->addRow("Username:", usernameInputBox);
	inputLayout->addRow("Password:", passwordInputBox);

	buttons->addButton("Login", QDialogButtonBox::AcceptRole);
	buttons->addButton("Cancel", QDialogButtonBox::RejectRole);
	buttonsLayout->addWidget(buttons);

	mainLayout->addStretch(2);
	mainLayout->addLayout(inputLayout);
	mainLayout->addStretch(2);
	mainLayout->addLayout(buttonsLayout);
	mainLayout->addStretch(1);

	setFixedSize(450, 230);
	setWindowTitle("Connect to database");
	setLayout(mainLayout);

	setWindowIcon(QIcon(":/images/addDatabase.svg"));

	setWindowModality(Qt::ApplicationModal);

	connect(this->buttons, SIGNAL(accepted()), this, SIGNAL(loginButtonClicked()));
	connect(this->buttons, SIGNAL(rejected()), this, SLOT(close()));
}

QString LoginWindow::getHostInput() const
{
	return serverInputBox->text();
}

int LoginWindow::getPortInput() const
{
	return portInputBox->text().toInt();
}

QString LoginWindow::getDatabaseInput() const
{
	return databaseInputBox->text();
}

QString LoginWindow::getUsernameInput() const
{
	return usernameInputBox->text();
}

QString LoginWindow::getPasswordInput() const
{
	return passwordInputBox->text();
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
}
