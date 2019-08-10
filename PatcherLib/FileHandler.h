#pragma once
#include "PatchList.h"

#include <QString>

class FileHandler
{
public:
	static QDir makePatchDir(const QString &path, bool &isSuccessful);
	static bool makePatchList(const QString &path, const PatchList &patchList);
	// Replace isSuccessful with exceptions
	static PatchList parseObjectList(const QString &path, bool &isSuccessful);
	static PatchList parseDependencyList(const QString &path, bool &isSuccessful);
	static QString getPatchListName();
	static QString getDependencyListName();
	static QString getObjectListName();
private:
	static const QString patchListName;
	static const QString dependencyListName;
	static const QString objectListName;
	static QString getParametersString(const QStringList &parameters);
};