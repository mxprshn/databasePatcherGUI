#include <QListWidget>
#include <QLayout>
#include <QAction>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>
#include "BuilderWidget.h"

BuilderWidget::BuilderWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	addItemLayout = new QHBoxLayout;
	typeComboBox = new QComboBox;
	itemNameEdit = new QLineEdit;
	addButton = new QPushButton(QIcon(":/images/addFile.svg"),"Add");

	addItemLayout->addWidget(typeComboBox);
	addItemLayout->addWidget(itemNameEdit);
	addItemLayout->addWidget(addButton);

	addItemGroupBox = new QGroupBox("Add item");
	addItemGroupBox->setLayout(addItemLayout);

	itemListLayout = new QVBoxLayout;
	itemListWidget = new QListWidget;

	itemListLayout->addWidget(addItemGroupBox);
	itemListLayout->addWidget(itemListWidget);

	toolsLayout = new QVBoxLayout;

	const auto buttonIconSize = QSize(35, 35);

	removeButton = new QToolButton;
	removeButton->setIcon(QIcon(":/images/removeFile.svg"));
	removeButton->setText("Remove");
	removeButton->setFixedSize(80, 70);
	removeButton->setIconSize(buttonIconSize);
	removeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	moveUpButton = new QToolButton;
	moveUpButton->setIcon(QIcon(":/images/upArrow.svg"));
	moveUpButton->setText("Move up");
	moveUpButton->setFixedSize(80, 70);
	moveUpButton->setIconSize(buttonIconSize);
	moveUpButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	moveDownButton = new QToolButton;
	moveDownButton->setIcon(QIcon(":/images/downArrow.svg"));
	moveDownButton->setText("Move down");
	moveDownButton->setFixedSize(80, 70);
	moveDownButton->setIconSize(buttonIconSize);
	moveDownButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	buildButton = new QToolButton;
	buildButton->setIcon(QIcon(":/images/hammer.svg"));
	buildButton->setText("Build");
	buildButton->setFixedSize(80, 70);
	buildButton->setIconSize(buttonIconSize);
	buildButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	toolsLayout->addWidget(removeButton);
	toolsLayout->addWidget(moveUpButton);
	toolsLayout->addWidget(moveDownButton);
	toolsLayout->addStretch(1);
	toolsLayout->addWidget(buildButton);

	mainLayout = new QHBoxLayout;

	mainLayout->addLayout(itemListLayout);
	mainLayout->addLayout(toolsLayout);
	setLayout(mainLayout);
}

BuilderWidget::~BuilderWidget()
{
}

