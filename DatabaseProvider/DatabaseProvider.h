#pragma once

#include <QObject>

class DatabaseProvider : public QObject
{
	Q_OBJECT

public:
	DatabaseProvider(QObject *parent);
	//QSqlDatabase database();
	//bool isConnected();
	bool connect(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port, const QString &name);
	//bool exists(const QString &objectType, const QString &objectName, const QString &connectionName);
private:
	QString connection;
};