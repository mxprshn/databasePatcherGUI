#pragma once
#include "PatchList.h"

#include <QString>

class FileHandler
{
public:
	QString makePatchDir(const QString &path, bool &isSuccessful);
	static bool makePatchList(const QString &path, const PatchList &patchList);
	// Replace isSuccessful with exceptions
	static PatchList parseObjectList(const QString &path, bool &isSuccessful);
	static PatchList parseDependencyList(const QString &path, bool &isSuccessful);
private:
	static const QString patchListName;
	static const QString dependencyListName;
	static const QString objectListName;
	static QString getParametersString(const QStringList &parameters);
};
