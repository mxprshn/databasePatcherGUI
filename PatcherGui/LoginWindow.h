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
	QString getHostInput()const;
	int getPortInput() const;
	QString getDatabaseInput() const;
	QString getUsernameInput() const;
	QString getPasswordInput() const;
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
	void clear() const;
signals:
	void loginButtonClicked();
};
