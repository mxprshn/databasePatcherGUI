#pragma once

class QSqlDatabase;
class QSqlQueryModel;

class DatabaseProvider
{
public:
	static QString database();
	static QString user();
	static QString password();
	static QString server();
	static int port();
	static bool isConnected();

	// Maybe it should return some kind of error index
	static bool connect(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port, QString &errorMessage);

	static void disconnect();

	static bool tableExists(const QString &schema, const QString &name);
	static bool sequenceExists(const QString &schema, const QString &name);
	static bool functionExists(const QString &schema, const QString &signature);
	static bool viewExists(const QString &schema, const QString &name);
	static bool triggerExists(const QString &schema, const QString &name);
	static bool indexExists(const QString &schema, const QString &name);

	static void initSchemaListModel(QSqlQueryModel &model);
};