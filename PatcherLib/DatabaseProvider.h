#pragma once

#include <QObject>

class QSqlDatabase;
class QSqlQueryModel;

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

	bool tableExists(const QString &name);
	bool sequenceExists(const QString &name);
	bool functionExists(const QString &name);
	bool viewExists(const QString &name);
	bool triggerExists(const QString &name);
	bool indexExists(const QString &name);

	void initSchemaListModel(QSqlQueryModel &model);
};