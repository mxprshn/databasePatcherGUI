#pragma once

#include <QStringList>

class QString;

// Think about pointer to parameters

class PatchListElement
{
public:
	PatchListElement(const QString &type, const QString &name, const QString &schema, const QStringList &parameters);
	QString getType() const;
	QString getName() const;
	QString getScheme() const;
	QStringList getParameters() const;
private:
	QString type;
	QString name;
	QString scheme;
	QStringList parameters;
};
