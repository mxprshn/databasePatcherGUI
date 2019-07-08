#include <QListWidget>
#include <QLayout>
#include <QPushButton>
#include "BuilderWidget.h"

BuilderWidget::BuilderWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	objectsListWidget = new QListWidget;
	mainLayout = new QHBoxLayout;

	mainLayout->addWidget(objectsListWidget);

	//setMinimumSize(600, 400);
	setLayout(mainLayout);
}

BuilderWidget::~BuilderWidget()
{
}
