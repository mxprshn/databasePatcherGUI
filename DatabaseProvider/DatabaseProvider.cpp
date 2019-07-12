#include <QSqlDatabase>
#include "DatabaseProvider.h"

DatabaseProvider::DatabaseProvider(QObject *parent)
	: QObject(parent)
{
}

bool DatabaseProvider::connect(const QString &database, const QString &user, const QString &password,
	const QString &server, const int port, const QString &name)
{
	auto connection = QSqlDatabase::addDatabase("QPSQL", name);
	connection.setDatabaseName(database);
	connection.setUserName(user);
	connection.setPassword(password);
	connection.setHostName(server);
	connection.setPort(port);

	auto isConnectionSet = connection.open();

	return !isConnectionSet;
}
