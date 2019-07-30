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
#include <QMessageBox>
#include "BuilderWidget.h"
#include "PatchListWidget.h"
#include "PatchList.h"
#include "DatabaseProvider.h"
#include "ObjectType.h"
#include "ui_BuilderWidget.h"

// Some freezes in patch list?
// Rename layouts in Ui

BuilderWidget::BuilderWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BuilderWidget)
	, patchList(new PatchList)
	, wrongFunctionInputMessage("Invalid signature.")
	, functionInputRegex("[^,\\(\\) ]+\\((([^,\\(\\) ]+,)*([^, \\(\\)]+)+)?\\)")
	, functionInputValidator(new QRegExpValidator(functionInputRegex, this))
{
	ui->setupUi(this);
	ui->typeComboBox->addItem(QIcon(":/images/script.svg"), "script", script);
	ui->typeComboBox->addItem(QIcon(":/images/table.svg"), "table", table);
	ui->typeComboBox->addItem(QIcon(":/images/sequence.svg"), "sequence", sequence);
	ui->typeComboBox->addItem(QIcon(":/images/function.svg"), "function", function);
	ui->typeComboBox->addItem(QIcon(":/images/view.svg"), "view", view);
	ui->typeComboBox->addItem(QIcon(":/images/trigger.svg"), "trigger", trigger);
	ui->typeComboBox->addItem(QIcon(":/images/index.svg"), "index", index);


	ui->nameEdit->setValidator(functionInputValidator);
	ui->nameEdit->setPlaceholderText("function_name ([arg_name_1, ][arg_name_2, ] ... )");

	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClicked()));
	connect(ui->buildButton, SIGNAL(clicked()), this, SLOT(onBuildButtonClicked()));
	connect(ui->nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onWrongFunctionInput()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(onRemoveButtonClicked()));
	connect(ui->buildListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
	connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentTypeChanged(int)));
	connect(ui->moveUpButton, SIGNAL(clicked()), this, SLOT(onMoveUpButtonClicked()));
	connect(ui->moveDownButton, SIGNAL(clicked()), this, SLOT(onMoveDownButtonClicked()));
}

BuilderWidget::~BuilderWidget()
{
	delete ui;
}

void BuilderWidget::onAddButtonClicked()
{
	auto exists = false;

	switch (ui->typeComboBox->currentData().toInt())
	{
		case table:
		{
			exists = DatabaseProvider::tableExists(ui->schemaComboBox->currentText(), ui->nameEdit->text());
			break;
		}
		case sequence:
		{
			exists = DatabaseProvider::sequenceExists(ui->schemaComboBox->currentText(), ui->nameEdit->text());
			break;
		}
		case view:
		{
			exists = DatabaseProvider::viewExists(ui->schemaComboBox->currentText(), ui->nameEdit->text());
			break;
		}
		case trigger:
		{
			exists = DatabaseProvider::triggerExists(ui->schemaComboBox->currentText(), ui->nameEdit->text());
			break;
		}
		case function:
		{
			exists = DatabaseProvider::functionExists(ui->schemaComboBox->currentText(), ui->nameEdit->text());
			break;
		}
		case index:
		{
			exists = DatabaseProvider::indexExists(ui->schemaComboBox->currentText(), ui->nameEdit->text());
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
		addToPatchListWidget(ui->typeComboBox->currentData().toInt(), ui->schemaComboBox->currentText(), ui->nameEdit->text());
	}
}

void BuilderWidget::addScripts()
{
	QStringList fileList;

	if (ui->nameEdit->text().isEmpty())
	{
		// Anyway, it opens aligned to top left corner, they say it's because qt uses Windows settings
		fileList = QFileDialog::getOpenFileNames(this, "Open script file", "", "SQL Script Files (*.sql)");
	}
	else
	{
		const QFileInfo fileInfo(ui->nameEdit->text());

		if (fileInfo.exists() && fileInfo.suffix() == "sql")
		{
			fileList.append(ui->nameEdit->text());
		}
		else
		{
			QMessageBox::warning(this, "File not opened"
				, "File does not exist or is not a SQL script file (*.sql)."
				, QMessageBox::Cancel, QMessageBox::Cancel);
		}
	}

	for (auto i = 0; i < fileList.count(); ++i)
	{
		addToPatchListWidget(script, "", fileList[i]);
	}
}

void BuilderWidget::addToPatchListWidget(int type, const QString &schemaName, const QString &itemName)
{
	auto *newItem = new QTreeWidgetItem(ui->buildListWidget);
	newItem->setIcon(PatchListWidget::ColumnIndexes::TypeColumn, QIcon(PatchListWidget::typeIcon(type)));
	newItem->setText(PatchListWidget::ColumnIndexes::TypeColumn, PatchListWidget::typeName(type));
	newItem->setText(PatchListWidget::ColumnIndexes::SchemaColumn, schemaName);
	newItem->setText(PatchListWidget::ColumnIndexes::NameColumn, itemName);
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
	ui->buildListWidget->addTopLevelItem(newItem);
}


void BuilderWidget::onBuildButtonClicked()
{
	const auto dialogResult = QMessageBox::information(this, "Patch list order information"
		, "For successful further installation of the built script it is recommended to keep the following"
		" order of patch list:\n\n"
		"- sequences\n"
		"- tables\n"
		"- views\n"
		"- indexes\n"
		"- functions\n"
		"- triggers\n"
		"- scripts\n\n"
		"Are you sure to continue?"
		, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

	if (dialogResult == QMessageBox::Ok)
	{
		buildPatch();
	}
}

void BuilderWidget::onRemoveButtonClicked()
{
	const auto dialogResult = QMessageBox::warning(this, "Remove item", "Are you sure to remove " +
		ui->buildListWidget->currentItem()->text(PatchListWidget::ColumnIndexes::NameColumn) +
		" from patch list?"
		, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

	if (dialogResult == QMessageBox::Ok && ui->buildListWidget->topLevelItemCount() != 0)
	{
		ui->buildListWidget->takeTopLevelItem(ui->buildListWidget->currentIndex().row());
	}
}

void BuilderWidget::onMoveUpButtonClicked()
{
	if (ui->buildListWidget->topLevelItemCount() > 1 && ui->buildListWidget->currentIndex().row() > 0)
	{
		const auto selectedRow = ui->buildListWidget->currentIndex().row();
		const auto selectedItem = ui->buildListWidget->takeTopLevelItem(ui->buildListWidget->currentIndex().row());
		ui->buildListWidget->insertTopLevelItem(selectedRow - 1, selectedItem);
		ui->buildListWidget->setCurrentItem(selectedItem);
	}	
}

void BuilderWidget::onMoveDownButtonClicked()
{
	if (ui->buildListWidget->topLevelItemCount() > 1 && ui->buildListWidget->currentIndex().row() != ui->buildListWidget->topLevelItemCount() - 1)
	{
		const auto selectedRow = ui->buildListWidget->currentIndex().row();
		const auto selectedItem = ui->buildListWidget->takeTopLevelItem(selectedRow);
		ui->buildListWidget->insertTopLevelItem(selectedRow + 1, selectedItem);
		ui->buildListWidget->setCurrentItem(selectedItem);
	}	
}

void BuilderWidget::onItemSelectionChanged()
{
	if (ui->buildListWidget->selectedItems().isEmpty())
	{
		ui->moveUpButton->setDisabled(true);
		ui->moveDownButton->setDisabled(true);
		ui->removeButton->setDisabled(true);
	}
	else
	{
		ui->moveUpButton->setEnabled(true);
		ui->moveDownButton->setEnabled(true);
		ui->removeButton->setEnabled(true);
	}
}

void BuilderWidget::onCurrentTypeChanged(int type)
{
	if (type == script)
	{
		ui->schemaComboBox->setDisabled(true);
	}
	else if (!ui->schemaComboBox->isEnabled())
	{
		ui->schemaComboBox->setEnabled(true);
	}
}


void BuilderWidget::setSchemaComboBoxModel(QAbstractItemModel* model)
{
	ui->schemaComboBox->setModel(model);
}

QString BuilderWidget::getItemNameInput()
{
	return ui->nameEdit->text();
}

int BuilderWidget::getObjectTypeIndex()
{
	return ui->typeComboBox->currentData().toInt();
}

QString BuilderWidget::getCurrentSchemaName()
{
	return ui->schemaComboBox->currentText();
}

void BuilderWidget::onWrongFunctionInput()
{
	if (ui->nameEdit->hasAcceptableInput())
	{
		ui->nameLabel->setText("Valid signature.");
	}
	else
	{
		ui->nameLabel->setText(wrongFunctionInputMessage);
	}
}

void BuilderWidget::buildPatch()
{
	for (auto i = 0; i < ui->buildListWidget->topLevelItemCount(); ++i)
	{
		patchList->add(ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::TypeColumn)
			, ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::SchemaColumn)
			, ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::NameColumn));
	}

	patchList->exportFile("PatchList.txt");
	patchList->clear();
}