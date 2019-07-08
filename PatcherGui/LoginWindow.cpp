#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include "LoginWindow.h"

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
	setWindowTitle("Login");
	setLayout(mainLayout);
}

LoginWindow::~LoginWindow()
{
}
