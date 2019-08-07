#include <QSqlQueryModel>
#include <QSqlQuery>

#include "ObjectNameCompleter.h"

ObjectNameCompleter::ObjectNameCompleter(QObject *parent)
	: QCompleter(parent)
	, model(new QSqlQueryModel)
{
	setModel(model);
	setCompletionRole(Qt::DisplayRole);
	setCompletionMode(PopupCompletion);
}

void ObjectNameCompleter::setSchema(const QString &schema)
{
	clear();
	setQuery(schema);
}

void ObjectNameCompleter::setQuery(const QString &schema)
{
	QSqlQuery fetch;
	fetch.prepare("SELECT r.routine_name || '(' || COALESCE(array_to_string(p.proargnames, ',', '*'), '') || ')' "
		"FROM information_schema.routines r, pg_catalog.pg_proc p WHERE r.specific_schema = ? and r.external_language = 'PLPGSQL' "
		"and r.routine_name = p.proname and r.specific_name = p.proname || '_' || p.oid;");
	fetch.addBindValue(schema);
	fetch.exec();
	model->setQuery(fetch);
}

void ObjectNameCompleter::clear()
{
	model->clear();
}

ObjectNameCompleter::~ObjectNameCompleter()
{
	clear();
	delete model;
}
