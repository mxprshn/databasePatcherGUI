#pragma once

#include <QObject>

class QSqlDatabase;

class DatabaseProvider : public QObject
{
	Q_OBJECT

public:
	DatabaseProvider(QObject *parent);
	~DatabaseProvider();
	QSqlDatabase database();
	bool isConnected();
	QString connect(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port, const QString &name);
	bool exists(const QString &objectType, const QString &objectName, const QString &connectionName);
private:
	QString connection;
};
