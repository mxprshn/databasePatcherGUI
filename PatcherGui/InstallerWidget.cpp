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
{
	ui->setupUi(this);

	initPatchList();
	initDependenciesList();

	testDependenciesAction = new QAction(QIcon(":/images/test.svg"), "Connect to database...", this);
	connect(this->testDependenciesAction, SIGNAL(triggered()), this,
		SLOT(onCheckButtonClicked()));
	connect(ui->checkButton, SIGNAL(clicked()), this->testDependenciesAction, SLOT(trigger()));
	connect(ui->installButton, SIGNAL(clicked()), this, SLOT(onInstallButtonClicked()));
}

InstallerWidget::~InstallerWidget()
{
	delete patchList;
	delete dependenciesList;
	delete ui;
}

QAction* InstallerWidget::getTestAction() const
{
	return testDependenciesAction;
}

void InstallerWidget::initPatchList()
{
	patchList->importFile("PatchList.txt");

	for (auto i = 0; i < patchList->count(); ++i)
	{
		const auto type = patchList->at(i).getType();
		ui->patchListWidget->add(type, patchList->at(i).getSchema(), patchList->at(i).getName()
			+ QString(type == ObjectTypes::function ? "(" + patchList->at(i).getParameters().join(",") + ")" : ""), false);
	}
}

void InstallerWidget::initDependenciesList()
{
	dependenciesList->importFile("C:\\Users\\mxprshn\\Desktop\\test\\DependencyList.dpn");

	for (auto i = 0; i < dependenciesList->count(); ++i)
	{
		const auto type = dependenciesList->at(i).getType();
		ui->dependenciesListWidget->add(type, dependenciesList->at(i).getSchema(), dependenciesList->at(i).getName()
			+ QString(type == ObjectTypes::function ? "(" + patchList->at(i).getParameters().join(",") + ")" : ""));
	}
}

void InstallerWidget::onCheckButtonClicked()
{
	ui->dependenciesListWidget->setCheckStatus(InstallerHandler::checkDependencies(DatabaseProvider::database(), DatabaseProvider::user(), DatabaseProvider::password()
		, DatabaseProvider::server(), DatabaseProvider::port(), "C:\\Users\\mxprshn\\Desktop\\test"));
}

void InstallerWidget::onInstallButtonClicked()
{
	InstallerHandler::installPatch(DatabaseProvider::database()
		, DatabaseProvider::user(), DatabaseProvider::password(), DatabaseProvider::server()
		, DatabaseProvider::port(), "C:\\Users\\mxprshn\\Desktop\\test");
}

