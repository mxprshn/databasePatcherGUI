#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include "DatabaseProvider.h"

DatabaseProvider::DatabaseProvider(QObject *parent)
	: QObject(parent)
{
}

QString DatabaseProvider::connect(const QString &database, const QString &user, const QString &password,
	const QString &server, const int port, const QString &name)
{
	auto connection = QSqlDatabase::addDatabase("QPSQL", name);
	connection.setDatabaseName(database);
	connection.setUserName(user);
	connection.setPassword(password);
	connection.setHostName(server);
	connection.setPort(port);

	auto isConnectionSet = connection.open();

	if (!isConnectionSet)
	{
		return connection.lastError().text();
	}

		return "Yea!";
}

DatabaseProvider::~DatabaseProvider()
{
}
