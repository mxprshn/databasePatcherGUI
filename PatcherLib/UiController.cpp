#include <QBitArray>
#include <QFile>
#include <QSqlQueryModel>
#include "UiController.h"
#include "DatabaseProvider.h"
#include "BuilderHandler.h"
#include "InstallerHandler.h"
#include "InstallListModel.h"
#include "DependenciesListModel.h"

UiController::UiController(QObject* parent)
	: builderHandler(new BuilderHandler(this, "builder.exe"))
	, installerHandler(new InstallerHandler(this, "PatchInstaller_exe.exe"))
	, installListModel(new InstallListModel(this))
	, dependenciesListModel(new DependenciesListModel(this))
	, schemaListModel(new QSqlQueryModel)
{
}

QAbstractItemModel* UiController::getInstallListModel() const
{
	return installListModel;
}

QAbstractItemModel* UiController::getDependenciesListModel() const
{
	return dependenciesListModel;
}

void UiController::connectToDatabase(const QString &database, const QString &user, const QString &password,
	const QString &server, const int port)
{
	QString errorMessage = "";

	if (databaseProvider->connect(database, user, password, server, port, errorMessage))
	{
		emit connectedToDatabase(database, user, password, server, port);
	}
	else
	{
		emit notConnectedToDatabase(errorMessage);
	}
}

void UiController::disconnectFromDatabase()
{
	schemaListModel->clear();
	databaseProvider->disconnect();
	emit disconnectedFromDatabase();
}

void UiController::testDependencies()
{
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(databaseProvider->server())
		.arg(databaseProvider->port()).arg(databaseProvider->database()).arg(databaseProvider->user())
		.arg(databaseProvider->password());
	const QStringList installerArguments = { connectionInfo, "check" };
	dependenciesListModel->getTestResult(installerHandler->testDependencies(installerArguments));
	emit testPassed();
}

QString UiController::installPatch()
{
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(databaseProvider->database())
		.arg(databaseProvider->user()).arg(databaseProvider->password()).arg(databaseProvider->server())
		.arg(databaseProvider->port());
	const QStringList installerArguments = { connectionInfo, "install", "C:\\Users\\mxprshn\\Desktop\\test" };
	return(installerHandler->install(installerArguments));
}


void UiController::openPatchFile(const QString &filePath)
{
	
}

QAbstractItemModel* UiController::getSchemaListModel() const
{
	databaseProvider->initSchemaListModel(*schemaListModel);
	return schemaListModel;
}

UiController::~UiController()
{
	disconnectFromDatabase();
}

