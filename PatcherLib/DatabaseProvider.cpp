#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "DatabaseProvider.h"

QString DatabaseProvider::database()
{
	return QSqlDatabase::database().databaseName();
}

QString DatabaseProvider::user()
{
	return QSqlDatabase::database().userName();
}

QString DatabaseProvider::password()
{
	return QSqlDatabase::database().password();
}

QString DatabaseProvider::server()
{
	return QSqlDatabase::database().hostName();
}

int DatabaseProvider::port()
{
	return QSqlDatabase::database().port();
}

bool DatabaseProvider::connect(const QString &database, const QString &user, const QString &password,
	const QString &server, const int port, QString &errorMessage)
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

bool DatabaseProvider::tableExists(const QString &schema, const QString &name)
{
	QSqlQuery check;
	check.prepare("SELECT EXISTS (SELECT * FROM information_schema.tables WHERE table_schema = ?"
		"AND table_name = ?)");
	check.addBindValue(schema);
	check.addBindValue(name);
	// Add check maybe and remove smth?
	check.exec();
	check.next();
	return check.value("exists").toBool();
}

bool DatabaseProvider::sequenceExists(const QString &schema, const QString &name)
{
	QSqlQuery check;
	check.prepare("SELECT EXISTS (SELECT * FROM information_schema.sequences WHERE sequence_schema = ?"
		"AND sequence_name = ?)");
	check.addBindValue(schema);
	check.addBindValue(name);
	// Add check maybe and remove smth?
	check.exec();
	check.next();
	return check.value("exists").toBool();
}

bool DatabaseProvider::functionExists(const QString &schema, const QString &signature)
{
	// It knows only foo(bar_1,bar_2) signature
	QSqlQuery check;
	check.prepare("SELECT EXISTS (SELECT * FROM information_schema.routines r, pg_catalog.pg_proc p WHERE"
		" r.specific_schema = ? and r.routine_name||'('||COALESCE(array_to_string(p.proargnames, ',', '*'),'')||')' = ?"
		" and r.external_language = 'PLPGSQL' and r.routine_name = p.proname and"
		" r.specific_name = p.proname || '_' || p.oid);");
	check.addBindValue(schema);
	check.addBindValue(signature);
	// Add check maybe and remove smth?
	check.exec();
	check.next();
	return check.value("exists").toBool();
}

bool DatabaseProvider::viewExists(const QString &schema, const QString &name)
{
	QSqlQuery check;
	check.prepare("SELECT EXISTS (SELECT * FROM information_schema.views WHERE table_schema = ?"
		"AND table_name = ?)");
	check.addBindValue(schema);
	check.addBindValue(name);
	// Add check maybe and remove smth?
	check.exec();
	check.next();
	return check.value("exists").toBool();
}

bool DatabaseProvider::triggerExists(const QString &schema, const QString &name)
{
	QSqlQuery check;
	check.prepare("SELECT EXISTS (SELECT * FROM information_schema.triggers WHERE trigger_schema = ?"
		"AND trigger_name = ?)");
	check.addBindValue(schema);
	check.addBindValue(name);
	// Add check maybe and remove smth?
	check.exec();
	check.next();
	return check.value("exists").toBool();
}

bool DatabaseProvider::indexExists(const QString &schema, const QString &name)
{
	QSqlQuery check;
	check.prepare("SELECT EXISTS (SELECT * FROM pg_indexes WHERE schemaname = ? and indexname = ?);");
	check.addBindValue(schema);
	check.addBindValue(name);
	// Add check maybe and remove smth?
	check.exec();
	check.next();
	return check.value("exists").toBool();
}

void DatabaseProvider::initSchemaListModel(QSqlQueryModel &model)
{
	model.setQuery("SELECT schema_name FROM information_schema.schemata WHERE"
		" schema_name NOT IN ('pg_catalog', 'information_schema');");
	// Check something
}