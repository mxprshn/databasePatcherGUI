#include <QBitArray>
#include "UiController.h"
#include "DatabaseProvider.h"
#include "BuilderHandler.h"
#include "InstallerHandler.h"
#include "BuildListModel.h"
#include "InstallListModel.h"
#include "DependenciesListModel.h"

UiController::UiController(QObject* parent)
	: databaseProvider(new DatabaseProvider(this))
	, builderHandler(new BuilderHandler(this, "builder.exe"))
	, installerHandler(new InstallerHandler(this, "PatchInstaller_exe.exe"))
	, buildListModel(new BuildListModel(this))
	, installListModel(new InstallListModel(this))
	, dependenciesListModel(new DependenciesListModel(this))
{
}

BuildListModel* UiController::getBuildListModel() const
{
	return buildListModel;
}

InstallListModel* UiController::getInstallListModel() const
{
	return installListModel;
}

DependenciesListModel* UiController::getDependenciesListModel() const
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
	databaseProvider->disconnect();
	emit disconnectedFromDatabase();
}

void UiController::checkDependencies()
{
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(databaseProvider->server())
		.arg(databaseProvider->port()).arg(databaseProvider->database()).arg(databaseProvider->user())
		.arg(databaseProvider->password());
	const QStringList installerArguments = { connectionInfo, "check" };
	dependenciesListModel->getTestResult(installerHandler->testDependencies(installerArguments));
	emit checkPassed();
}

void UiController::installPatch()
{
	
}

void UiController::buildPatch()
{
	
}

void UiController::openPatchFile(const QString &filePath)
{
	
}