#pragma once

#include <QObject>

class QSqlDatabase;
class QSqlQueryModel;

// MAKE IT STATIC!

class DatabaseProvider
{
public:
	static QString database();
	static QString user();
	static QString password();
	static QString server();
	static int port();

	// Maybe it should return some kind of error index
	static bool connect(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port, QString &errorMessage);

	static void disconnect();

	static bool tableExists(const QString &name);
	static bool sequenceExists(const QString &name);
	static bool functionExists(const QString &name);
	static bool viewExists(const QString &name);
	static bool triggerExists(const QString &name);
	static bool indexExists(const QString &name);

	static void initSchemaListModel(QSqlQueryModel &model);
};