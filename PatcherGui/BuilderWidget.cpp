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
#include "BuilderHandler.h"
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

	// ui->nameEdit->setValidator(functionInputValidator);

	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClicked()));
	connect(ui->buildButton, SIGNAL(clicked()), this, SLOT(onBuildButtonClicked()));
	connect(ui->nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onWrongFunctionInput()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(onRemoveButtonClicked()));
	connect(ui->buildListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
	connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentTypeChanged(int)));
	connect(ui->moveUpButton, SIGNAL(clicked()), this, SLOT(onMoveUpButtonClicked()));
	connect(ui->moveDownButton, SIGNAL(clicked()), this, SLOT(onMoveDownButtonClicked()));
	connect(ui->explorerButton, SIGNAL(clicked()), this, SLOT(onExplorerButtonClicked()));
	connect(ui->nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onNameTextChanged(const QString&)));
}

BuilderWidget::~BuilderWidget()
{
	delete patchList;
	delete ui;
}

void BuilderWidget::onAddButtonClicked()
{
	if (ui->typeComboBox->currentData().toInt() == script)
	{
		addScripts();
		return;
	}

	if (!DatabaseProvider::isConnected())
	{
		// Add opening login window
		QMessageBox::warning(this, "Database error"
			, "Not connected to database."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	if (ui->buildListWidget->itemExists(ui->typeComboBox->currentData().toInt(), ui->schemaComboBox->currentText()
		, ui->nameEdit->text()))
	{
		QMessageBox::warning(this, "Item not added"
			, ui->typeComboBox->currentText().replace(0, 1, ui->typeComboBox->currentText()[0].toUpper())
			+ " " + ui->nameEdit->text() + " already exists in patch list."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

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
	}

	if (exists)
	{
		addToPatchListWidget(ui->typeComboBox->currentData().toInt(), ui->schemaComboBox->currentText(), ui->nameEdit->text());
		ui->nameEdit->clear();
	}
	else
	{
		QMessageBox::warning(this, "Item not added"
			, ui->typeComboBox->currentText().replace(0, 1, ui->typeComboBox->currentText()[0].toUpper())
				+ " " + ui->nameEdit->text() + " does not exist in current database."
			, QMessageBox::Ok, QMessageBox::Ok);
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
			fileList.append(fileInfo.filePath());
		}
		else
		{
			QMessageBox::warning(this, "File not opened"
				, "File " + fileInfo.filePath() + " does not exist or is not a SQL script file (*.sql)."
				, QMessageBox::Ok, QMessageBox::Ok);
		}
	}
	
	auto someScriptsExist = false;

	for (auto i = 0; i < fileList.count(); ++i)
	{
		const auto currentFileName = fileList[i];

		if (!ui->buildListWidget->itemExists(script, "", currentFileName))
		{
			addToPatchListWidget(script, "", currentFileName);
		}
		else
		{
			someScriptsExist = true;
		}		
	}

	if (someScriptsExist)
	{
		QMessageBox::warning(this, "Some scripts not added"
			, "Some script files already exist in patch list and were not added."
			, QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		ui->nameEdit->clear();
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

void BuilderWidget::onExplorerButtonClicked()
{
	ui->patchPathEdit->setText(QFileDialog::getExistingDirectory(this, "Choose build directory"));
}

void BuilderWidget::onBuildButtonClicked()
{
	QDir patchDir;
	patchDir.setPath(ui->patchPathEdit->text());

	if (!patchDir.exists())
	{
		QMessageBox::warning(this, "Build error"
			, "Target directory does not exist."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

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
		if (buildPatch(ui->patchPathEdit->text()))
		{
			QApplication::beep();
			QMessageBox::information(this, "Build completed"
				, "Build completed. Watch logs for detailed information."
				, QMessageBox::Ok, QMessageBox::Ok);
		}
		else
		{
			QApplication::beep();
			QMessageBox::warning(this, "Build error"
				, "Error occured. Watch logs for detailed information."
				, QMessageBox::Ok, QMessageBox::Ok);
			
		}
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
		ui->nameEdit->setPlaceholderText("SQL script file path (leave empty to open in explorer)");
		ui->nameLabel->setText("Path");
	}
	else if (!ui->schemaComboBox->isEnabled())
	{
		ui->schemaComboBox->setEnabled(true);
	}

	if (type != function && type != script)
	{
		ui->nameEdit->setPlaceholderText(ui->typeComboBox->currentText().replace(0, 1, ui->typeComboBox->currentText()[0].toUpper())
			+ " name");
		ui->nameLabel->setText("Name");
	}

	if (type == function)
	{
		ui->nameEdit->setPlaceholderText("Function signature (e.g. function(arg_1,arg_2))");
		ui->nameLabel->setText("Signature (invalid, function may not be found))");
		emit ui->nameEdit->textChanged(ui->nameEdit->text());
	}
}

void BuilderWidget::onNameTextChanged(const QString &input)
{
	if (ui->typeComboBox->currentData().toInt() != function)
	{
		return;
	}

	// Validate method wants lvalues
	auto position = 0;
	auto text = input;

	if (functionInputValidator->validate(text, position) == QValidator::Acceptable)
	{
		ui->nameLabel->setText("Signature (Valid)");
	}
	else
	{
		ui->nameLabel->setText("Signature (Invalid, function may not be found)");
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

bool BuilderWidget::buildPatch(const QString &path)
{
	for (auto i = 0; i < ui->buildListWidget->topLevelItemCount(); ++i)
	{
		patchList->add(ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::TypeColumn)
			, ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::SchemaColumn)
			, ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::NameColumn));
	}

	// Where exactly should it be saved? FIX IT!

	const QDir patchDir(path);

	if (!patchList->exportFile(patchDir.filePath("PatchList.txt")))
	{
		patchList->clear();
		return false;
	}

	patchList->clear();

	return BuilderHandler::buildPatch(DatabaseProvider::database(), DatabaseProvider::user(), DatabaseProvider::password()
		, DatabaseProvider::server(), DatabaseProvider::port(), path);
}