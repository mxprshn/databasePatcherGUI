#include <QAction>
#include <QBitArray>
#include <QFileDialog>
#include <QMessageBox>

#include "InstallerWidget.h"
#include "InstallerHandler.h"
#include "PatchListWidget.h"
#include "DependenciesListWidget.h"
#include "PatchList.h"
#include "PatchListElement.h"
#include "ObjectTypes.h"
#include "DatabaseProvider.h"
#include "ui_InstallerWidget.h"

InstallerWidget::InstallerWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::InstallerWidget)
	, patchList(new PatchList)
	, dependenciesList(new PatchList)
	, isPatchOpened(false)
{
	ui->setupUi(this);
	ui->installInfoLabel->setText("");
	ui->checkButton->setDisabled(true);
	ui->installButton->setDisabled(true);
	setReadyToOpen();

	connect(ui->checkButton, SIGNAL(clicked()), this, SLOT(onCheckButtonClicked()));
	connect(ui->installButton, SIGNAL(clicked()), this, SLOT(onInstallButtonClicked()));
	connect(ui->openPatchButton, SIGNAL(clicked()), this, SLOT(onOpenButtonClicked()));
	connect(ui->dependenciesListWidget, SIGNAL(itemCheckChanged()), this, SLOT(onItemCheckChanged()));
}

InstallerWidget::~InstallerWidget()
{
	delete patchList;
	delete dependenciesList;
	delete ui;
}

bool InstallerWidget::checkConnection()
{
	if (!DatabaseProvider::isConnected())
	{
		QApplication::beep();
		QMessageBox::warning(this, "Database error"
			, "Not connected to database."
			, QMessageBox::Ok, QMessageBox::Ok);
		emit connectionRequested();
		return false;
	}

	return true;
}


void InstallerWidget::setReadyToOpen()
{
	ui->patchPathEdit->setPlaceholderText("Patch folder path (leave empty to open in explorer)");
	ui->openPatchButton->setText("Open");
	ui->openPatchButton->setIcon(QIcon(":/images/box.svg"));
	ui->openPatchButton->setIconSize(QSize(20, 20));
}

bool InstallerWidget::initPatchList(const QString &filePath)
{
	if (!patchList->importPatchListFile(filePath))
	{
		patchList->clear();
		return false;		
	}

	for (auto i = 0; i < patchList->count(); ++i)
	{
		const auto type = patchList->at(i).getType();
		ui->patchListWidget->add(type, patchList->at(i).getSchema(), patchList->at(i).getName()
			+ QString(type == ObjectTypes::function ? "(" + patchList->at(i).getParameters().join(",") + ")" : ""), false);
	}

	ui->patchListWidget->scrollToTop();
	return true;
}

bool InstallerWidget::initDependenciesList(const QString &filePath)
{
	if(!dependenciesList->importDependenciesListFile(filePath))
	{
		dependenciesList->clear();
		return false;
	}

	for (auto i = 0; i < dependenciesList->count(); ++i)
	{
		const auto type = dependenciesList->at(i).getType();
		ui->dependenciesListWidget->add(type, dependenciesList->at(i).getSchema(), dependenciesList->at(i).getName());
	}

	return true;
}

void InstallerWidget::clearCurrentPatch()
{
	dependenciesList->clear();
	patchList->clear();
	patchDir = QDir();
	ui->dependenciesListWidget->clear();
	ui->patchListWidget->clear();
	ui->patchPathEdit->setPlaceholderText("Patch folder path");
	ui->patchPathEdit->setEnabled(true);
	ui->checkButton->setDisabled(true);
	ui->installButton->setDisabled(true);
	ui->installInfoLabel->setText("");
	setReadyToOpen();
	isPatchOpened = false;
}

void InstallerWidget::onOpenButtonClicked()
{
	if (isPatchOpened)
	{
		const auto dialogResult = QMessageBox::question(this, "Close", "Are you sure to close current patch?"
			, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

		if (dialogResult == QMessageBox::Ok)
		{
			clearCurrentPatch();
		}
		
		return;
	}

	if (ui->patchPathEdit->text().isEmpty())
	{
		patchDir.setPath(QFileDialog::getExistingDirectory(this, "Choose patch directory"));

		if (patchDir.path().isEmpty())
		{
			clearCurrentPatch();
			return;
		}
	}
	else
	{
		patchDir.setPath(ui->patchPathEdit->text());

		if (!patchDir.exists())
		{
			QApplication::beep();
			QMessageBox::warning(this, "Open error", "Patch directory does not exist."
				, QMessageBox::Ok, QMessageBox::Ok);
			clearCurrentPatch();
			return;
		}
	}

	const QString patchListFileName = "PatchList.txt";
	const QString dependenciesListFileName = "DependencyList.dpn";

	if (!patchDir.exists(patchListFileName))
	{
		QApplication::beep();
		QMessageBox::warning(this, "Open error", patchListFileName + " does not exist in patch directory."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	if (!patchDir.exists(dependenciesListFileName))
	{
		QApplication::beep();
		QMessageBox::warning(this, "Open error", dependenciesListFileName + " does not exist in patch directory."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	if (!initPatchList(patchDir.filePath(patchListFileName)))
	{
		QApplication::beep();
		QMessageBox::warning(this, "Open error", "Incorrect file " + patchListFileName + " ."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	if (!initDependenciesList(patchDir.filePath(dependenciesListFileName)))
	{
		QApplication::beep();
		QMessageBox::warning(this, "Open error", "Incorrect file " + dependenciesListFileName + " ."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	ui->patchPathEdit->clear();
	ui->patchPathEdit->setPlaceholderText("Opened patch: " + patchDir.absolutePath());
	ui->patchPathEdit->setDisabled(true);
	ui->openPatchButton->setText("Close");
	ui->openPatchButton->setIcon(QIcon(":/images/close.svg"));
	ui->openPatchButton->setIconSize(QSize(12, 12));
	ui->checkButton->setEnabled(true);
	isPatchOpened = true;
}

void InstallerWidget::onCheckButtonClicked()
{
	if (!checkConnection())
	{
		return;
	}

	auto isSuccessful = false;
	const auto checkResult = InstallerHandler::checkDependencies(DatabaseProvider::database(), DatabaseProvider::user(), DatabaseProvider::password()
		, DatabaseProvider::server(), DatabaseProvider::port(), patchDir.absolutePath(), isSuccessful);

	if (isSuccessful && ui->dependenciesListWidget->setCheckStatus(checkResult))
	{
		ui->checkButton->setEnabled(false);

		QApplication::beep();

		if (!ui->dependenciesListWidget->getAreAllSatisfied())
		{
			QMessageBox::warning(this, "Check completed"
				, "Check completed. Not all dependencies are satisfied. If you want to install patch anyway, "
				"mark all not satisfied dependencies manually in the list."
				, QMessageBox::Ok, QMessageBox::Ok);
		}
		else
		{
			QMessageBox::information(this, "Check completed"
				, "Check completed. All dependencies are satisfied. Patch may be installed safely."
				, QMessageBox::Ok, QMessageBox::Ok);
		}
	}
	else
	{
		QApplication::beep();
		QMessageBox::warning(this, "Check error"
			, "Error occured. See logs for detailed information."
			, QMessageBox::Ok, QMessageBox::Ok);
	}
}

void InstallerWidget::onInstallButtonClicked()
{
	if (!checkConnection())
	{
		return;
	}

	if (!ui->dependenciesListWidget->getAreAllSatisfied())
	{
		QApplication::beep();
		const auto dialogResult = QMessageBox::warning(this, "Unsafe installation"
			,"WARNING: Not all dependencies are satisfied. Installation may cause database errors. "
			"Are you sure to continue?"
			, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

		if (dialogResult == QMessageBox::Cancel)
		{
			return;
		}
	}

	if (InstallerHandler::installPatch(DatabaseProvider::database()
		, DatabaseProvider::user(), DatabaseProvider::password(), DatabaseProvider::server()
		, DatabaseProvider::port(), patchDir.absolutePath()))
	{
		QApplication::beep();
		QMessageBox::information(this, "Installation completed"
			, "Installation completed. See logs for detailed information."
			, QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		QApplication::beep();
		QMessageBox::warning(this, "Installation error"
			, "Error occured. See logs for detailed information."
			, QMessageBox::Ok, QMessageBox::Ok);
	}
}

void InstallerWidget::onItemCheckChanged()
{
	if (ui->dependenciesListWidget->getCheckedCount() == ui->dependenciesListWidget->topLevelItemCount())
	{
		if (!ui->dependenciesListWidget->getAreAllSatisfied())
		{
			ui->installInfoLabel->setText("Warning: Some dependencies are not satisfied!");
		}
		else
		{
			ui->installInfoLabel->setText("All dependencies are satisfied.");
		}
		
		ui->installButton->setEnabled(true);
	}
	else
	{
		ui->installInfoLabel->setText("To enable installation, mark not satisfied dependencies manually.");
		ui->installButton->setEnabled(false);
	}
}