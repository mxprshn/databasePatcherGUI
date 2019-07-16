#pragma once

#include <QObject>

class QSqlDatabase;

class DatabaseProvider : public QObject
{
	Q_OBJECT

public:

	DatabaseProvider(QObject *parent);
	~DatabaseProvider();

	QString database() const;
	QString user() const;
	QString password() const;
	QString server() const;
	int port() const;

	bool connect(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port, QString &errorMessage) const;

	void disconnect();

	//bool exists(const QString &objectType, const QString &objectName, const QString &connectionName);

};