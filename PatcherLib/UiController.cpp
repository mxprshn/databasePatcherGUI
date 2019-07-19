#include <QBitArray>
#include <QFile>
#include "UiController.h"
#include "DatabaseProvider.h"
#include "BuilderHandler.h"
#include "InstallerHandler.h"
#include "BuildListModel.h"
#include "InstallListModel.h"
#include "DependenciesListModel.h"
#include "ObjectTypeListModel.h"

UiController::UiController(QObject* parent)
	: databaseProvider(new DatabaseProvider(this))
	, builderHandler(new BuilderHandler(this, "builder.exe"))
	, installerHandler(new InstallerHandler(this, "PatchInstaller_exe.exe"))
	, buildListModel(new BuildListModel(this))
	, installListModel(new InstallListModel(this))
	, dependenciesListModel(new DependenciesListModel(this))
	, objectTypeListModel(new ObjectTypeListModel(this))
{
}

QAbstractItemModel* UiController::getBuildListModel() const
{
	return buildListModel;
}

QAbstractItemModel* UiController::getInstallListModel() const
{
	return installListModel;
}

QAbstractItemModel* UiController::getDependenciesListModel() const
{
	return dependenciesListModel;
}

QAbstractItemModel* UiController::getObjectTypeListModel() const
{
	return objectTypeListModel;
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

void UiController::testDependencies()
{
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(databaseProvider->server())
		.arg(databaseProvider->port()).arg(databaseProvider->database()).arg(databaseProvider->user())
		.arg(databaseProvider->password());
	const QStringList installerArguments = { connectionInfo, "check" };
	dependenciesListModel->getTestResult(installerHandler->testDependencies(installerArguments));
	emit testPassed();
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

void UiController::addObject(const int modelIndex, const QString& name)
{
	if (modelIndex != script)
	{
		auto exists = false;
		ObjectType type = typeCount;

		switch (modelIndex)
		{
			case table:
			{
				exists = databaseProvider->tableExists(name);
				type = table;
				break;
			}
			case sequence:
			{
				exists = databaseProvider->sequenceExists(name);
				type = sequence;
				break;
			}
			case function:
			{
				exists = databaseProvider->functionExists(name);
				type = function;
				break;
			}
			case view:
			{
				exists = databaseProvider->viewExists(name);
				type = view;
				break;
			}
			case trigger:
			{
				exists = databaseProvider->triggerExists(name);
				type = trigger;
				break;
			}
			case index:
			{
				exists = databaseProvider->indexExists(name);
				type = index;
				break;
			}
		}

		if (exists)
		{
			buildListModel->addObject(type, name);
		}
		else
		{
			emit objectNotFound();
		}
	}
	else
	{
		if (QFile::exists(name))
		{
			buildListModel->addObject(script, name);
		}
		else
		{
			emit scriptNotFound();
		}
	}
}
