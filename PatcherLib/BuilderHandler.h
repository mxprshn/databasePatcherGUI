#pragma once

#include <QObject>

class BuilderHandler : public QObject
{
	Q_OBJECT

public:
	BuilderHandler(QObject *parent, const QString &builderPath);
};
