#pragma once

#include <QDialog>
#include "ui_LoginWindow.h"

class QFormLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QDialogButtonBox;
class QPushButton;
class QLabel;

class LoginWindow : public QDialog, public Ui::LoginWindow
{
	Q_OBJECT

public:

	LoginWindow(QWidget *parent = Q_NULLPTR);
	~LoginWindow();

	void clear();

private:

	QDialogButtonBox *buttons;

	QVBoxLayout *mainLayout;
	QFormLayout *inputLayout;
	QHBoxLayout *buttonsLayout;
	QHBoxLayout *serverInputLayout;

	QLineEdit *serverInputBox;
	QLineEdit *portInputBox;
	QLineEdit *databaseInputBox;
	QLineEdit *usernameInputBox;
	QLineEdit *passwordInputBox;

	QLabel *portLabel;

private slots:

	void showLoginWindow();
	void connectionRequest();

signals:

	void connectionRequested(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
};
