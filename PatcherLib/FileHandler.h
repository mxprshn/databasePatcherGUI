#pragma once

#include <QObject>

class BuildListModel;
class DependenciesListModel;

class FileHandler : public QObject
{
public:
	FileHandler(QObject *parent);
	static bool makePatchList(const BuildListModel &model, const QString &path);
	// static bool parseDependenciesList(const DependenciesListModel &model, const QString &path);
private:
	static QString getParametersString(const QVector<QPair<QString, QString>> &parameters);
};
