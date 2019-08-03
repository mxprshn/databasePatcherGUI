#include <QBitArray>
#include <QFile>
#include <QSqlQueryModel>
#include "UiController.h"
#include "DatabaseProvider.h"
#include "BuilderHandler.h"
#include "InstallerHandler.h"
#include "InstallListModel.h"

UiController::UiController(QObject* parent)
	: schemaListModel(new QSqlQueryModel)
{
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

QString UiController::installPatch()
{
	InstallerHandler::installPatch(DatabaseProvider::database()
		, DatabaseProvider::user(), DatabaseProvider::password(), DatabaseProvider::server(), DatabaseProvider::port(), "C:\\Users\\mxprshn\\Desktop\\test");
	return "";
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

