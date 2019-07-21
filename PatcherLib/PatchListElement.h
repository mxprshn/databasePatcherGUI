#pragma once

#include <QObject>
#include "ObjectType.h"

class PatchListElement : public QObject
{
	Q_OBJECT

public:
	PatchListElement(QObject *parent, ObjectType type, const QString &name, const QString &scheme = ""
		, const QVector<QPair<QString, QString>> &parameters = {});
	ObjectType getType();
	QString getName();
	QString getScheme();
	QVector<QPair<QString, QString>> getParameters();
private:
	ObjectType type;
	QString name;
	QString scheme;
	QVector<QPair<QString, QString>> parameters;
};
