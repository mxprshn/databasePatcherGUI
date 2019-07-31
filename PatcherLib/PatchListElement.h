#pragma once

#include <QStringList>

class QString;

// Think about pointer to parameters

class PatchListElement
{
public:
	PatchListElement(int type, const QString &typeName, const QString &name, const QString &schema, const QStringList &parameters);
	int getType() const;
	QString getTypeName() const;
	QString getName() const;
	QString getSchema() const;
	QStringList getParameters() const;
private:
	int type;
	QString typeName;
	QString name;
	QString schema;
	QStringList parameters;
};
