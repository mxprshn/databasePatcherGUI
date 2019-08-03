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
#include "ObjectType.h"
#include "DatabaseProvider.h"

InstallerWidget::InstallerWidget(QWidget *parent)
	: QWidget(parent)
	, patchList(new PatchList)
	, dependenciesList(new PatchList)
	, toolButtonSize(QSize(90, 70))
	, toolButtonIconSize(QSize(35, 35))
{
	setupUi(this);
	mainLayout = new QGridLayout;

	initializeItemLists();
	initializeOpenPatchBox();
	initializeToolButtons();
	initPatchList();
	initDependenciesList();

	testDependenciesAction = new QAction(QIcon(":/images/test.svg"), "Connect to database...", this);
	connect(this->testDependenciesAction, SIGNAL(triggered()), this,
		SLOT(onCheckButtonClicked()));
	connect(this->testButton, SIGNAL(clicked()), this->testDependenciesAction, SLOT(trigger()));
	connect(this->installButton, SIGNAL(clicked()), this, SIGNAL(installButtonClicked()));

	setLayout(mainLayout);
}

InstallerWidget::~InstallerWidget()
{
	delete patchList;
	delete dependenciesList;
}

void InstallerWidget::initializeItemLists()
{
	itemListLayout = new QVBoxLayout;
	dependenciesListLayout = new QVBoxLayout;

	itemListGroupBox = new QGroupBox("Patch");
	dependenciesListGroupBox = new QGroupBox("Dependencies");

	itemListWidget = new PatchListWidget(this);
	dependenciesListWidget = new DependenciesListWidget(this);

	itemListLayout->addWidget(itemListWidget);
	dependenciesListLayout->addWidget(dependenciesListWidget);

	itemListGroupBox->setLayout(itemListLayout);
	dependenciesListGroupBox->setLayout(dependenciesListLayout);

	mainLayout->addWidget(itemListGroupBox, 1, 0);
	mainLayout->addWidget(dependenciesListGroupBox, 1, 1);
}

void InstallerWidget::initializeOpenPatchBox()
{
	openPatchLayout = new QHBoxLayout;
	openPatchGroupBox = new QGroupBox("Open patch file");

	patchPathLineEdit = new QLineEdit;
	openExplorerButton = new QPushButton("Open file...");
	openPatchButton = new QPushButton("Open");

	openPatchLayout->addWidget(patchPathLineEdit);
	openPatchLayout->addWidget(openExplorerButton);
	openPatchLayout->addWidget(openPatchButton);

	openPatchGroupBox->setLayout(openPatchLayout);

	mainLayout->addWidget(openPatchGroupBox, 0, 0, 1, 2);
}

void InstallerWidget::initializeToolButtons()
{
	toolsLayout = new QVBoxLayout;

	testButton = new QToolButton;
	testButton->setIcon(QIcon(":/images/test.svg"));
	testButton->setText("Test \n dependencies");
	testButton->setFixedSize(toolButtonSize);
	testButton->setIconSize(toolButtonIconSize);
	testButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	installButton = new QToolButton;
	installButton->setIcon(QIcon(":/images/install.svg"));
	installButton->setText("Install");
	installButton->setFixedSize(toolButtonSize);
	installButton->setIconSize(toolButtonIconSize);
	installButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	toolsLayout->addStretch(1);
	toolsLayout->addWidget(testButton);
	toolsLayout->addWidget(installButton);

	mainLayout->addLayout(toolsLayout, 0, 2, 2, 1);
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
		itemListWidget->add(type, patchList->at(i).getSchema(), patchList->at(i).getName()
			+ QString(type == function ? "(" + patchList->at(i).getParameters().join(",") + ")" : ""), false);
	}
}

void InstallerWidget::initDependenciesList()
{
	dependenciesList->importFile("C:\\Users\\mxprshn\\Desktop\\test\\DependencyList.dpn");

	for (auto i = 0; i < dependenciesList->count(); ++i)
	{
		auto *newItem = new QTreeWidgetItem(dependenciesListWidget);
		const auto type = dependenciesList->at(i).getType();

		// Maybe make all this code as a method in widget?
		newItem->setIcon(DependenciesListWidget::ColumnIndexes::typeColumn, QIcon(DependenciesListWidget::typeIcon(type)));
		newItem->setText(DependenciesListWidget::ColumnIndexes::typeColumn, PatchList::typeName(type));
		newItem->setData(DependenciesListWidget::ColumnIndexes::typeColumn, Qt::UserRole, type);
		newItem->setText(DependenciesListWidget::ColumnIndexes::schemaColumn, dependenciesList->at(i).getSchema());
		newItem->setText(DependenciesListWidget::ColumnIndexes::nameColumn, dependenciesList->at(i).getName()
			+ QString(type == function ? "(" + patchList->at(i).getParameters().join(",") + ")" : ""));
		newItem->setFlags(Qt::ItemIsEnabled);
		newItem->setIcon(DependenciesListWidget::ColumnIndexes::statusColumn,
			QIcon(DependenciesListWidget::statusIcon(DependenciesListWidget::CheckStatus::waitingForCheck)));
		newItem->setCheckState(DependenciesListWidget::ColumnIndexes::statusColumn, Qt::Unchecked);
		dependenciesListWidget->addTopLevelItem(newItem);
	}
}

void InstallerWidget::onCheckButtonClicked()
{
	dependenciesListWidget->setCheckStatus(InstallerHandler::checkDependencies(DatabaseProvider::database(), DatabaseProvider::user(), DatabaseProvider::password()
		, DatabaseProvider::server(), DatabaseProvider::port(), "C:\\Users\\mxprshn\\Desktop\\test"));
}
