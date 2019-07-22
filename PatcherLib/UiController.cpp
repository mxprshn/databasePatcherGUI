#include <QBitArray>
#include <QFile>
#include <QSqlQueryModel>
#include "UiController.h"
#include "DatabaseProvider.h"
#include "BuilderHandler.h"
#include "InstallerHandler.h"
#include "FileHandler.h"
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
	, schemaListModel(new QSqlQueryModel)
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

void UiController::installPatch()
{
	
}

void UiController::buildPatch()
{
	FileHandler::makePatchList(*this->buildListModel, "BuildList.txt");
}

void UiController::openPatchFile(const QString &filePath)
{
	
}

void UiController::addObject(const int typeIndex, const QString &schema, const QString &input)
{
	if (typeIndex != script)
	{
		auto exists = false;
		auto objectName = input;
		ObjectType type = typeCount;
		QStringList parameters = {};

		switch (typeIndex)
		{
			case table:
			{
				exists = databaseProvider->tableExists(input);
				type = table;
				break;
			}
			case sequence:
			{
				exists = databaseProvider->sequenceExists(input);
				type = sequence;
				break;
			}
			case function:
			{
				exists = databaseProvider->functionExists(input);
				auto splitResult = input.split(QRegExp("(\\ |\\,|\\(|\\))"), QString::SkipEmptyParts);
				objectName = splitResult.first();
				splitResult.pop_front();
				parameters = splitResult;
				type = function;
				break;
			}
			case view:
			{
				exists = databaseProvider->viewExists(input);
				type = view;
				break;
			}
			case trigger:
			{
				exists = databaseProvider->triggerExists(input);
				type = trigger;
				break;
			}
			case index:
			{
				exists = databaseProvider->indexExists(input);
				type = index;
				break;
			}
		}

		if (exists)
		{
			buildListModel->addObject(type, objectName, schema, parameters);
		}
		else
		{
			emit objectNotFound();
		}
	}
	else
	{
		if (QFile::exists(input))
		{
			buildListModel->addObject(script, input);
		}
		else
		{
			emit scriptNotFound();
		}
	}
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

