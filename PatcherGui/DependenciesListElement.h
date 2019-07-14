#pragma once

#include <QObject>

class DependenciesListElement : public QObject
{
	Q_OBJECT

public:

	enum ObjectType
	{
		unidentified,
		table,
		sequence,
		function,
		view,
		trigger,
		index
	};

	enum CheckStatus
	{
		waitingForCheck,
		satisfied,
		notSatisfied
	};

	DependenciesListElement(QObject *parent, ObjectType type, const QString &name);
	~DependenciesListElement();

	void setSatisfied();
	void setNotSatisfied();

	ObjectType getType() const;
	QString getName() const;
	CheckStatus getStatus() const;
	bool getIsReadyToInstall() const;

private:

	ObjectType type;
	QString name;
	CheckStatus status;
	bool isReadyToInstall;
};
