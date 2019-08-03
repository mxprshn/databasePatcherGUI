#pragma once

#include <QStringList>

class QString;

// Think about pointer to parameters

class PatchListElement
{
public:
	PatchListElement(int type, const QString &name, const QString &schema, const QStringList &parameters);
	int getType() const;
	QString getName() const;
	QString getSchema() const;
	QStringList getParameters() const;
private:
	int type;
	QString name;
	QString schema;
	QStringList parameters;
};
