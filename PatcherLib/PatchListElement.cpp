#include "PatchListElement.h"

PatchListElement::PatchListElement(const QString &type, const QString &name, const QString &schema
	, const QStringList &parameters)
	: type(type)
	, name(name)
	, scheme(schema)
	, parameters(parameters)
{
}

QString PatchListElement::getType() const
{
	return type;
}

QString PatchListElement::getName() const
{
	return name;
}

QString PatchListElement::getScheme() const
{
	return scheme;
}

QStringList PatchListElement::getParameters() const
{
	return parameters;
}