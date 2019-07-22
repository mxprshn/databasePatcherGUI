#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "DatabaseProvider.h"

DatabaseProvider::DatabaseProvider(QObject *parent)
	: QObject(parent)
{
}

QString DatabaseProvider::database() const
{
	return QSqlDatabase::database().databaseName();
}

QString DatabaseProvider::user() const
{
	return QSqlDatabase::database().userName();
}

QString DatabaseProvider::password() const
{
	return QSqlDatabase::database().password();
}

QString DatabaseProvider::server() const
{
	return QSqlDatabase::database().hostName();
}

int DatabaseProvider::port() const
{
	return QSqlDatabase::database().port();
}

bool DatabaseProvider::connect(const QString &database, const QString &user, const QString &password,
	const QString &server, const int port, QString &errorMessage) const
{
	auto connection = QSqlDatabase::addDatabase("QPSQL");
	connection.setDatabaseName(database);
	connection.setUserName(user);
	connection.setPassword(password);
	connection.setHostName(server);
	connection.setPort(port);

	const auto isConnectionSet = connection.open();

	if (!isConnectionSet)
	{
		errorMessage = connection.lastError().text();
	}

	return isConnectionSet;
}

void DatabaseProvider::disconnect()
{
	const auto connectionName = QSqlDatabase::database().connectionName();
	auto connection = QSqlDatabase::database(connectionName, false);

	if (connection.isOpen())
	{
		connection.close();
	}

	QSqlDatabase::removeDatabase(connectionName);
}

bool DatabaseProvider::tableExists(const QString& name)
{
	return true;
}

bool DatabaseProvider::sequenceExists(const QString& name)
{
	return true;
}

bool DatabaseProvider::functionExists(const QString& name)
{
	return true;
}

bool DatabaseProvider::viewExists(const QString& name)
{
	return true;
}

bool DatabaseProvider::triggerExists(const QString& name)
{
	return true;
}

bool DatabaseProvider::indexExists(const QString& name)
{
	return true;
}

void DatabaseProvider::initSchemaListModel(QSqlQueryModel &model)
{
	model.setQuery("SELECT schema_name FROM information_schema.schemata");
	// Check something
}

DatabaseProvider::~DatabaseProvider()
{
	disconnect();
}
