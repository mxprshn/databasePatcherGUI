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
#include <QToolTip>
#include <QDir>
#include <QFileDialog>
#include "BuilderWidget.h"
#include "PatchListWidget.h"
#include "PatchList.h"
#include "DatabaseProvider.h"

BuilderWidget::BuilderWidget(QWidget *parent)
	: QWidget(parent)
	, buildListWidget(new PatchListWidget(this))
	, patchList(new PatchList)
	, wrongFunctionInputMessage("Invalid signature.")
	, toolButtonSize(QSize(90, 70))
	, toolButtonIconSize(QSize(35, 35))
	, functionInputRegex("[^,\\(\\) ]+\\((([^,\\(\\) ]+,)*([^, \\(\\)]+)+)?\\)")
	, functionInputValidator(new QRegExpValidator(functionInputRegex, this))
{
	setupUi(this);
	mainLayout = new QGridLayout;

	initializeItemList();
	initializeToolButtons();
	initializeAddItemBox();

	setLayout(mainLayout);

	connect(this->addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClicked()));
	connect(this->buildButton, SIGNAL(clicked()), this, SLOT(onBuildButtonClicked()));
	connect(this->itemNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onWrongFunctionInput()));
}

BuilderWidget::~BuilderWidget()
{
	delete patchList;
}

void BuilderWidget::initializeItemList()
{
	itemListLayout = new QVBoxLayout;
	itemListGroupBox = new QGroupBox("Build list");

	itemListLayout->addWidget(buildListWidget);
	
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
	typeComboBox->setIconSize(QSize(smallIconSize, smallIconSize));
	typeComboBox->addItem(QIcon(":/images/script.svg"), "script", script);
	typeComboBox->addItem(QIcon(":/images/table.svg"), "table", table);
	typeComboBox->addItem(QIcon(":/images/sequence.svg"), "sequence", sequence);
	typeComboBox->addItem(QIcon(":/images/function.svg"), "function", function);
	typeComboBox->addItem(QIcon(":/images/view.svg"), "view", view);
	typeComboBox->addItem(QIcon(":/images/trigger.svg"), "trigger", trigger);
	typeComboBox->addItem(QIcon(":/images/index.svg"), "index", index);

	schemeComboBox = new QComboBox;

	itemNameEdit = new QLineEdit;
	itemNameEdit->setValidator(functionInputValidator);
	itemNameEdit->setPlaceholderText("function_name ([arg_name_1, ][arg_name_2, ] ... )");

	addButton = new QPushButton(QIcon(":/images/addFile.svg"), "Add");

	inputStatusLabel = new QLabel(wrongFunctionInputMessage);

	addItemLayout->addWidget(typeComboBox, 0, 0);
	addItemLayout->addWidget(schemeComboBox, 0, 1);
	addItemLayout->addWidget(itemNameEdit, 0, 2);
	addItemLayout->addWidget(addButton, 0, 3);
	addItemLayout->addWidget(inputStatusLabel, 1, 2);

	addItemGroupBox->setLayout(addItemLayout);

	mainLayout->addWidget(addItemGroupBox, 0, 0);
}

void BuilderWidget::onAddButtonClicked()
{
	auto exists = false;

	switch (typeComboBox->currentData().toInt())
	{
		case table:
		{
			exists = DatabaseProvider::tableExists(schemeComboBox->currentText(), itemNameEdit->text());
			break;
		}
		case sequence:
		{
			exists = DatabaseProvider::sequenceExists(schemeComboBox->currentText(), itemNameEdit->text());
			break;
		}
		case view:
		{
			exists = DatabaseProvider::viewExists(schemeComboBox->currentText(), itemNameEdit->text());
			break;
		}
		case trigger:
		{
			exists = DatabaseProvider::triggerExists(schemeComboBox->currentText(), itemNameEdit->text());
			break;
		}
		case function:
		{
			exists = DatabaseProvider::functionExists(schemeComboBox->currentText(), itemNameEdit->text());
			break;
		}
		case index:
		{
			exists = DatabaseProvider::indexExists(schemeComboBox->currentText(), itemNameEdit->text());
			break;
		}
		case script:
		{
			addScripts();
			return;
		}
	}

	if (exists)
	{
		addToPatchListWidget(typeComboBox->currentData().toInt(), schemeComboBox->currentText(), itemNameEdit->text());
	}
}

void BuilderWidget::addScripts()
{
	QStringList fileList;

	if (itemNameEdit->text().isEmpty())
	{
		fileList = QFileDialog::getOpenFileNames(this, "Open script file", "", "*.sql");
	}
	else if (QFile::exists(itemNameEdit->text()))
	{
		fileList.append(itemNameEdit->text());
	}

	for (auto i = 0; i < fileList.count(); ++i)
	{
		addToPatchListWidget(script, "", fileList[i]);
	}
}


void BuilderWidget::addToPatchListWidget(int type, const QString &schemaName, const QString &itemName)
{
	auto *newItem = new QTreeWidgetItem(buildListWidget);
	newItem->setIcon(PatchListWidget::ColumnIndexes::TypeColumn, QIcon(PatchListWidget::typeIcon(type)));
	newItem->setText(PatchListWidget::ColumnIndexes::TypeColumn, PatchListWidget::typeName(type));
	newItem->setText(PatchListWidget::ColumnIndexes::SchemaColumn, schemaName);
	newItem->setText(PatchListWidget::ColumnIndexes::NameColumn, itemName);
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
	buildListWidget->addTopLevelItem(newItem);
}


void BuilderWidget::onBuildButtonClicked()
{
	for (auto i = 0; i < buildListWidget->topLevelItemCount(); ++i)
	{
		patchList->add(buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::TypeColumn)
			, buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::SchemaColumn)
			, buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::NameColumn));
	}

	patchList->exportFile("PatchList.txt");
	patchList->clear();
}

void BuilderWidget::setSchemaComboBoxModel(QAbstractItemModel* model)
{
	schemeComboBox->setModel(model);
}

QString BuilderWidget::getItemNameInput()
{
	return itemNameEdit->text();
}

int BuilderWidget::getObjectTypeIndex()
{
	return typeComboBox->currentData().toInt();
}

QString BuilderWidget::getCurrentSchemaName()
{
	return schemeComboBox->currentText();
}

void BuilderWidget::onWrongFunctionInput()
{
	if (itemNameEdit->hasAcceptableInput())
	{
		inputStatusLabel->setText("Valid signature.");
	}
	else
	{
		inputStatusLabel->setText(wrongFunctionInputMessage);
	}
}