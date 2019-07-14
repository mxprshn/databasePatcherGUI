#include "DependenciesListElement.h"

DependenciesListElement::DependenciesListElement(QObject *parent, ObjectType type, const QString &name)
	: QObject(parent)
	, type(type)
	, name(name)
	, status(waitingForCheck)
	, isReadyToInstall(false)
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

DependenciesListElement::ObjectType DependenciesListElement::getType() const
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

DependenciesListElement::~DependenciesListElement()
{
}
