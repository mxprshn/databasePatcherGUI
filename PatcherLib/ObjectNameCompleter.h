#pragma once

#include <QCompleter>

class QSqlQueryModel;

class ObjectNameCompleter : public QCompleter
{
	Q_OBJECT

public:
	ObjectNameCompleter(QObject *parent);
	~ObjectNameCompleter();
	void initialize(int typeIndex, const QString &schema);
	void clear();
private:
	QSqlQueryModel *model;
	static const QString tableQuery;
	static const QString sequenceQuery;
	static const QString functionQuery;
	static const QString viewQuery;
	static const QString triggerQuery;
	static const QString indexQuery;
};
