#include <QListWidget>
#include <QLayout>
#include <QAction>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>
#include <QGridLayout>
#include <QTreeView>
#include <QRegExp>
#include <QValidator>
#include <QLabel>
#include "BuilderWidget.h"

BuilderWidget::BuilderWidget(QWidget *parent)
	: QWidget(parent)
	, toolButtonSize(QSize(90, 70))
	, toolButtonIconSize(QSize(35, 35))
	, functionInputRegex("\\S+ \\((([^,\\(\\) ]+, )*([^, \\(\\)]+)+)?\\)")
	, functionInputValidator(new QRegExpValidator(functionInputRegex, this))
{
	setupUi(this);

	mainLayout = new QGridLayout;

	initializeItemList();
	initializeToolButtons();
	initializeAddItemBox();

	setLayout(mainLayout);

	connect(this->addButton, SIGNAL(clicked()), this, SIGNAL(addButtonClicked()));
	connect(this->buildButton, SIGNAL(clicked()), this, SIGNAL(buildButtonClicked()));
}

BuilderWidget::~BuilderWidget()
{
}

void BuilderWidget::initializeItemList()
{
	itemListLayout = new QVBoxLayout;
	itemListGroupBox = new QGroupBox("Build list");
	itemListView = new QTreeView;

	itemListLayout->addWidget(itemListView);
	
	itemListGroupBox->setLayout(itemListLayout);
	mainLayout->addWidget(itemListGroupBox, 1, 0);
}

void BuilderWidget::initializeToolButtons()
{
	toolsLayout = new QVBoxLayout;

	removeButton = new QToolButton;
	removeButton->setIcon(QIcon(":/images/removeFile.svg"));
	removeButton->setText("Remove");
	removeButton->setFixedSize(toolButtonSize);
	removeButton->setIconSize(toolButtonIconSize);
	removeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	moveUpButton = new QToolButton;
	moveUpButton->setIcon(QIcon(":/images/upArrow.svg"));
	moveUpButton->setText("Move up");
	moveUpButton->setFixedSize(toolButtonSize);
	moveUpButton->setIconSize(toolButtonIconSize);
	moveUpButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	moveDownButton = new QToolButton;
	moveDownButton->setIcon(QIcon(":/images/downArrow.svg"));
	moveDownButton->setText("Move down");
	moveDownButton->setFixedSize(toolButtonSize);
	moveDownButton->setIconSize(toolButtonIconSize);
	moveDownButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	buildButton = new QToolButton;
	buildButton->setIcon(QIcon(":/images/hammer.svg"));
	buildButton->setText("Build");
	buildButton->setFixedSize(toolButtonSize);
	buildButton->setIconSize(toolButtonIconSize);
	buildButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	toolsLayout->addWidget(removeButton);
	toolsLayout->addWidget(moveUpButton);
	toolsLayout->addWidget(moveDownButton);
	toolsLayout->addStretch(1);
	toolsLayout->addWidget(buildButton);

	mainLayout->addLayout(toolsLayout, 0, 1, 2, 1);
}

void BuilderWidget::initializeAddItemBox()
{
	addItemLayout = new QGridLayout;
	addItemGroupBox = new QGroupBox("Add item");

	typeComboBox = new QComboBox;
	schemeComboBox = new QComboBox;
	itemNameEdit = new QLineEdit;
	itemNameEdit->setValidator(functionInputValidator);

	addButton = new QPushButton(QIcon(":/images/addFile.svg"), "Add");

	inputStatusLabel = new QLabel("");

	addItemLayout->addWidget(typeComboBox, 0, 0);
	addItemLayout->addWidget(schemeComboBox, 0, 1);
	addItemLayout->addWidget(itemNameEdit, 0, 2);
	addItemLayout->addWidget(addButton, 0, 3);
	addItemLayout->addWidget(inputStatusLabel, 1, 2);

	addItemGroupBox->setLayout(addItemLayout);

	mainLayout->addWidget(addItemGroupBox, 0, 0);
}

void BuilderWidget::setObjectTypeListModel(QAbstractItemModel* model)
{
	typeComboBox->setModel(model);
}

void BuilderWidget::setBuildListModel(QAbstractItemModel* model)
{
	itemListView->setModel(model);
}

QString BuilderWidget::getItemNameInput()
{
	return itemNameEdit->text();
}

int BuilderWidget::getObjectTypeIndex()
{
	return typeComboBox->currentIndex();
}

//void BuilderWidget::validateFunctionInput()
//{
//	if (typeComboBox->currentIndex() == function)
//	{
//		QCursor cursor = itemNameEdit->cursor();
//		if (functionInputValidator->validate(itemNameEdit->text(), cursor.pos()) == QValidator::Acceptable)
//		{
//			
//		}
//	}
//}
