#pragma once

#include <QCompleter>

class QSqlQueryModel;

class ObjectNameCompleter : public QCompleter
{
	Q_OBJECT

public:
	ObjectNameCompleter(QObject *parent);
	~ObjectNameCompleter();
	void setSchema(const QString &schema);
	void clear();
private:
	QSqlQueryModel *model;
	void setQuery(const QString &schema);
};
