#pragma once

#include <QWidget>
#include "ui_InstallerWidget.h"



class InstallerWidget : public QWidget, public Ui::InstallerWidget
{
	Q_OBJECT

public:
	InstallerWidget(QWidget *parent = Q_NULLPTR);
	~InstallerWidget();
};
