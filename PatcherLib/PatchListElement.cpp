#include "PatchListElement.h"

PatchListElement::PatchListElement(int type, const QString &typeName, const QString &name, const QString &schema
	, const QStringList &parameters)
	: type(type)
	, typeName(typeName)
	, name(name)
	, schema(schema)
	, parameters(parameters)
{
}

int PatchListElement::getType() const
{
	return type;
}

QString PatchListElement::getTypeName() const
{
	return typeName;
}

QString PatchListElement::getName() const
{
	return name;
}

QString PatchListElement::getSchema() const
{
	return schema;
}

QStringList PatchListElement::getParameters() const
{
	return parameters;
}