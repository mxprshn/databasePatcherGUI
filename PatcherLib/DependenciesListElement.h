#pragma once

#include <QObject>
#include "ObjectType.h"

class DependenciesListElement : public QObject
{
	Q_OBJECT

public:

	enum CheckStatus
	{
		waitingForCheck,
		satisfied,
		notSatisfied
	};

	DependenciesListElement(QObject *parent, ObjectType type, const QString &scheme, const QString &name);
	~DependenciesListElement();

	void setSatisfied();
	void setNotSatisfied();
	void setReadyToInstall();

	ObjectType getType() const;
	QString getName() const;
	QString getScheme() const;
	CheckStatus getStatus() const;
	bool getIsReadyToInstall() const;

private:

	ObjectType type;
	QString name;
	QString scheme;
	CheckStatus status;
	bool isReadyToInstall;
};
