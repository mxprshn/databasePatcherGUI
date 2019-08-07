#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

class QAction;
class QLabel;
class LoginWindow;
class LogOutputDevice;

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
private:
	Ui::MainWindow *ui;
	LogOutputDevice *logOutputDevice;
	LoginWindow *loginWindow;
	QAction *connectAction;
	QAction *disconnectAction;
	QAction *buildAction;
	QLabel *databaseInformation;
signals:
	void connected();
	void disconnected();
private slots:
	void onDialogConnectButtonClicked();
	void onConnectionRequested();
	void onDisconnectButtonClicked();
};
