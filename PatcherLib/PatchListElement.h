#pragma once

#include <QObject>
#include "ObjectType.h"

class PatchListElement : public QObject
{
	Q_OBJECT

public:
	PatchListElement(QObject *parent, ObjectType type, const QString &name, const QString &scheme = ""
		, const QStringList &parameters = {});
	ObjectType getType();
	QString getName();
	QString getScheme();
	QStringList getParameters();
private:
	ObjectType type;
	QString name;
	QString scheme;
	QStringList parameters;
};
