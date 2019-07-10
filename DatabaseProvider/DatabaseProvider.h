#pragma once

#include <QObject>
#include "databaseprovider_global.h"

class QSqlDatabase;

class DATABASEPROVIDER_EXPORT DatabaseProvider : public QObject
{
	Q_OBJECT

public:
	DatabaseProvider();
	QSqlDatabase database();
	bool isConnected();
	bool connect(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port, const QString &name);
	bool exists(const QString &objectType, const QString &objectName, const QString &connectionName);
private:
	QString connection;
};
