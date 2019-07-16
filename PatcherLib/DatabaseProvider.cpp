#include <QSqlDatabase>
#include <QSqlError>
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


DatabaseProvider::~DatabaseProvider()
{
	disconnect();
}
