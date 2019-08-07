#pragma once

#include <QDialog>

namespace Ui
{
	class AboutWindow;
}

class AboutWindow : public QDialog
{
	Q_OBJECT

public:
	AboutWindow(QWidget *parent = Q_NULLPTR);
	~AboutWindow();

private:
	Ui::AboutWindow *ui;
};
