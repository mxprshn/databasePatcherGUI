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
#include "InstallerWidget.h"
#include "InstallerHandler.h"

InstallerWidget::InstallerWidget(QWidget *parent)
	: QWidget(parent)
	, toolButtonSize(QSize(90, 70))
	, toolButtonIconSize(QSize(35, 35))
{
	setupUi(this);
	mainLayout = new QGridLayout;

	initializeItemLists();
	initializeOpenPatchBox();
	initializeToolButtons();

	testDependenciesAction = new QAction(QIcon(":/images/test.svg"), "Connect to database...", this);
	connect(this->testDependenciesAction, SIGNAL(triggered()), this,
		SIGNAL(testButtonClicked()));
	connect(this->testButton, SIGNAL(clicked()), this->testDependenciesAction, SLOT(trigger()));

	setLayout(mainLayout);
}

void InstallerWidget::initializeItemLists()
{
	itemListLayout = new QVBoxLayout;
	dependenciesListLayout = new QVBoxLayout;

	itemListGroupBox = new QGroupBox("Patch");
	dependenciesListGroupBox = new QGroupBox("Dependencies");

	itemListWidget = new QListWidget;
	dependenciesListView = new QTreeView;

	itemListLayout->addWidget(itemListWidget);
	dependenciesListLayout->addWidget(dependenciesListView);

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


void InstallerWidget::setDependenciesListModel(QAbstractItemModel* model)
{
	dependenciesListView->setModel(model);
	dependenciesListView->setRootIsDecorated(false);
	dependenciesListView->header()->setStretchLastSection(false);
	dependenciesListView->header()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
	dependenciesListView->header()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
}

void InstallerWidget::setInstallListModel(QAbstractItemModel* model)
{
}



