#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

#include "BuilderWidget.h"
#include "PatchListWidget.h"
#include "PatchList.h"
#include "DatabaseProvider.h"
#include "ObjectTypes.h"
#include "BuilderHandler.h"
#include "ui_BuilderWidget.h"

// Some freezes in patch list?
// Rename layouts in Ui
// Fix headers coming to infinity

BuilderWidget::BuilderWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BuilderWidget)
	, patchList(new PatchList)
	, functionInputValidator(new QRegExpValidator(QRegExp("[^,\\(\\) ]+\\((([^,\\(\\) ]+,)*([^, \\(\\)]+)+)?\\)"), this))
{
	ui->setupUi(this);

	ui->typeComboBox->addItem(QIcon(":/images/script.svg"), "script", ObjectTypes::script);
	ui->typeComboBox->addItem(QIcon(":/images/table.svg"), "table", ObjectTypes::table);
	ui->typeComboBox->addItem(QIcon(":/images/sequence.svg"), "sequence", ObjectTypes::sequence);
	ui->typeComboBox->addItem(QIcon(":/images/function.svg"), "function", ObjectTypes::function);
	ui->typeComboBox->addItem(QIcon(":/images/view.svg"), "view", ObjectTypes::view);
	ui->typeComboBox->addItem(QIcon(":/images/trigger.svg"), "trigger", ObjectTypes::trigger);
	ui->typeComboBox->addItem(QIcon(":/images/index.svg"), "index", ObjectTypes::index);

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
	const auto nameInput = ui->nameEdit->text().remove(QRegExp("\\ "));

	if (!DatabaseProvider::isConnected())
	{
		QApplication::beep();
		// Add opening login window
		QMessageBox::warning(this, "Database error"
			, "Not connected to database."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	if (ui->typeComboBox->currentData().toInt() == ObjectTypes::script)
	{
		addScripts(nameInput);
		return;
	}

	if (ui->buildListWidget->itemExists(ui->typeComboBox->currentData().toInt(), ui->schemaComboBox->currentText(), nameInput))
	{
		QApplication::beep();
		QMessageBox::warning(this, "Item not added"
			, ui->typeComboBox->currentText().replace(0, 1, ui->typeComboBox->currentText()[0].toUpper())
			+ " " + ui->nameEdit->text() + " already exists in patch list."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	auto exists = false;

	switch (ui->typeComboBox->currentData().toInt())
	{
		case ObjectTypes::table:
		{
			exists = DatabaseProvider::tableExists(ui->schemaComboBox->currentText(), nameInput);
			break;
		}
		case ObjectTypes::sequence:
		{
			exists = DatabaseProvider::sequenceExists(ui->schemaComboBox->currentText(), nameInput);
			break;
		}
		case ObjectTypes::view:
		{
			exists = DatabaseProvider::viewExists(ui->schemaComboBox->currentText(), nameInput);
			break;
		}
		case ObjectTypes::trigger:
		{
			exists = DatabaseProvider::triggerExists(ui->schemaComboBox->currentText(), nameInput);
			break;
		}
		case ObjectTypes::function:
		{
			exists = DatabaseProvider::functionExists(ui->schemaComboBox->currentText(), nameInput);
			break;
		}
		case ObjectTypes::index:
		{
			exists = DatabaseProvider::indexExists(ui->schemaComboBox->currentText(), nameInput);
			break;
		}
	}

	if (exists)
	{
		ui->buildListWidget->add(ui->typeComboBox->currentData().toInt(), ui->schemaComboBox->currentText(), nameInput, true);
		ui->nameEdit->clear();
	}
	else
	{
		QApplication::beep();
		QMessageBox::warning(this, "Item not added"
			, ui->typeComboBox->currentText().replace(0, 1, ui->typeComboBox->currentText()[0].toUpper())
				+ " " + ui->nameEdit->text() + " does not exist in current database."
			, QMessageBox::Ok, QMessageBox::Ok);
	}
}

void BuilderWidget::addScripts(const QString &input)
{
	QStringList fileList;

	auto allScriptsExist = true;

	if (input.isEmpty())
	{
		// Anyway, it opens aligned to top left corner, they say it's because qt uses Windows settings
		fileList = QFileDialog::getOpenFileNames(this, "Open script files", "", "SQL Script Files (*.sql)");
	}
	else
	{
		const auto scriptPaths = input.split(QRegExp("\\,"), QString::SkipEmptyParts);

		for (auto i = 0; i < scriptPaths.count(); ++i)
		{
			const QFileInfo fileInfo(scriptPaths.at(i));

			if (fileInfo.exists() && fileInfo.suffix() == "sql")
			{
				fileList.append(fileInfo.filePath());
			}
			else
			{
				allScriptsExist = false;
			}
		}
	}	

	for (auto i = 0; i < fileList.count(); ++i)
	{
		const auto currentFileName = fileList[i];

		if (!ui->buildListWidget->itemExists(ObjectTypes::script, "", currentFileName))
		{
			ui->buildListWidget->add(ObjectTypes::script, "", currentFileName, true);
		}
		else
		{
			allScriptsExist = false;
		}		
	}

	if (!allScriptsExist)
	{
		QApplication::beep();
		QMessageBox::warning(this, "Some scripts not added"
			, "Some script files already exist in patch list, not found or are not SQL script files (*.sql) and were not added."
			, QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		ui->nameEdit->clear();
	}	
}

void BuilderWidget::onExplorerButtonClicked()
{
	ui->patchPathEdit->setText(QFileDialog::getExistingDirectory(this, "Choose build directory"));
}

void BuilderWidget::onBuildButtonClicked()
{
	// Separate method?
	if (!DatabaseProvider::isConnected())
	{
		QApplication::beep();
		// Add opening login window
		QMessageBox::warning(this, "Database error"
			, "Not connected to database."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	if (ui->patchPathEdit->text().isEmpty())
	{
		QMessageBox::information(this, "Build error", "Please, choose target directory."
			, QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	QDir patchDir;
	patchDir.setPath(ui->patchPathEdit->text());

	if (!patchDir.exists())
	{
		QApplication::beep();
		QMessageBox::warning(this, "Build error", "Target directory does not exist."
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
		if (startPatchBuild(ui->patchPathEdit->text()))
		{
			QApplication::beep();
			QMessageBox::information(this, "Build completed"
				, "Build completed. See logs for detailed information."
				, QMessageBox::Ok, QMessageBox::Ok);
		}
		else
		{
			QApplication::beep();
			QMessageBox::warning(this, "Build error"
				, "Error occured. See logs for detailed information."
				, QMessageBox::Ok, QMessageBox::Ok);			
		}
	}
}

void BuilderWidget::onRemoveButtonClicked()
{
	const auto dialogResult = QMessageBox::question(this, "Remove item", "Are you sure to remove " +
		ui->buildListWidget->currentItem()->text(PatchListWidget::ColumnIndexes::nameColumn) +
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
	if (type == ObjectTypes::script)
	{
		ui->schemaComboBox->setDisabled(true);
		ui->nameEdit->setPlaceholderText("SQL script file paths (leave empty to open in explorer)");
		ui->nameLabel->setText("Path");
	}
	else if (!ui->schemaComboBox->isEnabled())
	{
		ui->schemaComboBox->setEnabled(true);
	}

	if (type != ObjectTypes::function && type != ObjectTypes::script)
	{
		ui->nameEdit->setPlaceholderText(ui->typeComboBox->currentText().replace(0, 1, ui->typeComboBox->currentText()[0].toUpper())
			+ " name");
		ui->nameLabel->setText("Name");
	}

	if (type == ObjectTypes::function)
	{
		ui->nameEdit->setPlaceholderText("Function signature (e.g. function(arg_1,arg_2))");
		ui->nameLabel->setText("Signature (Invalid, function may not be found))");
		emit ui->nameEdit->textChanged(ui->nameEdit->text());
	}
}

void BuilderWidget::onNameTextChanged(const QString &input)
{
	if (ui->typeComboBox->currentData().toInt() != ObjectTypes::function)
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

bool BuilderWidget::startPatchBuild(const QString &path)
{
	for (auto i = 0; i < ui->buildListWidget->topLevelItemCount(); ++i)
	{
		patchList->add(ui->buildListWidget->topLevelItem(i)->data(PatchListWidget::ColumnIndexes::typeColumn, Qt::UserRole).toInt()
			, ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::schemaColumn)
			, ui->buildListWidget->topLevelItem(i)->text(PatchListWidget::ColumnIndexes::nameColumn));
	}

	QDir patchDir(path);
	// Can database have a name with dots?
	const auto patchDirName = DatabaseProvider::database() + "_build_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");

	if (!patchDir.mkdir(patchDirName))
	{
		patchList->clear();
		return false;
	}

	patchDir.cd(patchDirName);

	if (!patchList->exportFile(patchDir.absoluteFilePath("PatchList.txt")))
	{
		patchList->clear();
		return false;
	}

	patchList->clear();

	return BuilderHandler::buildPatch(DatabaseProvider::database(), DatabaseProvider::user(), DatabaseProvider::password()
		, DatabaseProvider::server(), DatabaseProvider::port(), patchDir.absolutePath(), patchDir.absoluteFilePath("PatchList.txt"));
}