#include <QLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAction>
#include <QHeaderView>
#include <QProcess>
#include <QBitArray>
#include <QStringList>
#include <QFile>
#include <QDir>
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
	setReadyToOpen();

	testDependenciesAction = new QAction(QIcon(":/images/test.svg"), "Connect to database...", this);
	connect(this->testDependenciesAction, SIGNAL(triggered()), this,
		SLOT(onCheckButtonClicked()));
	connect(ui->checkButton, SIGNAL(clicked()), this->testDependenciesAction, SLOT(trigger()));
	connect(ui->installButton, SIGNAL(clicked()), this, SLOT(onInstallButtonClicked()));
	connect(ui->openPatchButton, SIGNAL(clicked()), this, SLOT(onOpenButtonClicked()));
}

InstallerWidget::~InstallerWidget()
{
	delete patchList;
	delete dependenciesList;
	delete ui;
}

void InstallerWidget::setReadyToOpen()
{
	ui->patchPathEdit->setPlaceholderText("Patch folder path (leave empty to open in explorer)");
	ui->openPatchButton->setText("Open");
	ui->openPatchButton->setIcon(QIcon(":/images/box.svg"));
	ui->openPatchButton->setIconSize(QSize(20, 20));
}

QAction* InstallerWidget::getTestAction() const
{
	return testDependenciesAction;
}

bool InstallerWidget::initPatchList(const QString &filePath)
{
	if (!patchList->importFile(filePath))
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

	return true;
}

bool InstallerWidget::initDependenciesList(const QString &filePath)
{
	if(!dependenciesList->importFile(filePath))
	{
		dependenciesList->clear();
		return false;
	}

	for (auto i = 0; i < dependenciesList->count(); ++i)
	{
		const auto type = dependenciesList->at(i).getType();
		ui->dependenciesListWidget->add(type, dependenciesList->at(i).getSchema(), dependenciesList->at(i).getName()
			+ QString(type == ObjectTypes::function ? "(" + dependenciesList->at(i).getParameters().join(",") + ")" : ""));
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
	setReadyToOpen();
	isPatchOpened = false;
}

void InstallerWidget::onOpenButtonClicked()
{
	if (isPatchOpened)
	{
		const auto dialogResult = QMessageBox::warning(this, "Close", "Are you sure to close current patch?"
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
		QMessageBox::warning(this, "Open error", patchListFileName + " does not exist in patch directory."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	if (!patchDir.exists(dependenciesListFileName))
	{
		QMessageBox::warning(this, "Open error", dependenciesListFileName + " does not exist in patch directory."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	if (!initPatchList(patchDir.filePath(patchListFileName)))
	{
		QMessageBox::warning(this, "Open error", "Incorrect file " + patchListFileName + " ."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	if (!initDependenciesList(patchDir.filePath(dependenciesListFileName)))
	{
		QMessageBox::warning(this, "Open error", "Incorrect file " + dependenciesListFileName + " ."
			, QMessageBox::Ok, QMessageBox::Ok);
		clearCurrentPatch();
		return;
	}

	ui->patchPathEdit->clear();
	ui->patchPathEdit->setPlaceholderText("Opened patch: " + patchDir.absolutePath());
	ui->patchPathEdit->setEnabled(false);
	ui->openPatchButton->setText("Close");
	ui->openPatchButton->setIcon(QIcon(":/images/close.svg"));
	ui->openPatchButton->setIconSize(QSize(12, 12));
	isPatchOpened = true;
}


void InstallerWidget::onCheckButtonClicked()
{
	ui->dependenciesListWidget->setCheckStatus(InstallerHandler::checkDependencies(DatabaseProvider::database(), DatabaseProvider::user(), DatabaseProvider::password()
		, DatabaseProvider::server(), DatabaseProvider::port(), patchDir.absolutePath()));
}

void InstallerWidget::onInstallButtonClicked()
{
	InstallerHandler::installPatch(DatabaseProvider::database()
		, DatabaseProvider::user(), DatabaseProvider::password(), DatabaseProvider::server()
		, DatabaseProvider::port(), patchDir.absolutePath());
}

