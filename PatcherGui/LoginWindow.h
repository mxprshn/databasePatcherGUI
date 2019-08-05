#pragma once
#include <QDialog>

namespace Ui
{
	class LoginWindow;
}

class LoginWindow : public QDialog
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = Q_NULLPTR);
	~LoginWindow();

	QString getHostInput() const;
	int getPortInput() const;
	QString getDatabaseInput() const;
	QString getUsernameInput() const;
	QString getPasswordInput() const;
	void clear();

private:
	Ui::LoginWindow *ui;
signals:
	void connectButtonClicked();
};
