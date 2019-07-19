#include "PatchListElement.h"

PatchListElement::PatchListElement(QObject *parent, ObjectType type, const QString &name, const QString &scheme
	, const QList<QPair<QString, QString>> &parameters)
	: QObject(parent)
	, type(type)
	, name(name)
	, scheme(scheme)
	, parameters(parameters)
{
}

ObjectType PatchListElement::getType()
{
	return type;
}

QString PatchListElement::getName()
{
	return name;
}

QString PatchListElement::getScheme()
{
	return scheme;
}

QList<QPair<QString, QString>> PatchListElement::getParameters()
{
	return parameters;
}