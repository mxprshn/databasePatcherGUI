#include "DependenciesListElement.h"

DependenciesListElement::DependenciesListElement(QObject *parent, ObjectType type, const QString &scheme, const QString &name)
	: QObject(parent)
	, type(type)
	, name(name)
	, status(waitingForCheck)
	, isReadyToInstall(false)
	, scheme(scheme)
{
}

void DependenciesListElement::setSatisfied()
{
	status = satisfied;
}

void DependenciesListElement::setNotSatisfied()
{
	status = notSatisfied;
}

void DependenciesListElement::setReadyToInstall()
{
	isReadyToInstall = true;
}

ObjectType DependenciesListElement::getType() const
{
	return type;
}

bool DependenciesListElement::getIsReadyToInstall() const
{
	return isReadyToInstall;
}

DependenciesListElement::CheckStatus DependenciesListElement::getStatus() const
{
	return status;
}


QString DependenciesListElement::getName() const
{
	return name;
}

QString DependenciesListElement::getScheme() const
{
	return scheme;
}

DependenciesListElement::~DependenciesListElement()
{
}
