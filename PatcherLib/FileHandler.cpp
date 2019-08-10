#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include "FileHandler.h"
#include "DatabaseProvider.h"
#include "PatchListElement.h"
#include "ObjectTypes.h"

const QString FileHandler::patchListName = "PatchList.txt";
const QString FileHandler::dependencyListName = "DependencyList.dpn";
const QString FileHandler::objectListName = "PatchList.txt";

QDir FileHandler::makePatchDir(const QString &path, bool &isSuccessful)
{
	QDir patchDir(path);
	// Can database have a name with dots?
	const auto patchDirName = DatabaseProvider::database() + "_build_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");

	if (!patchDir.mkdir(patchDirName) || !patchDir.cd(patchDirName))
	{
		isSuccessful = false;
		return QString();
	}

	isSuccessful = true;
	return patchDir;
}

bool FileHandler::makePatchList(const QString &path, const PatchList &patchList)
{
	const QDir patchDir(path);
	QFile file(patchDir.absoluteFilePath(patchListName));

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::NewOnly))
	{
		return false;
	}

	QTextStream patchFileStream(&file);

	for (const auto current : patchList)
	{
		const auto schema = current->getSchema();
		const auto name = current->getName();
		const auto type = current->getType();
		const auto typeName = ObjectTypes::typeNames.value(type);
		const auto parameters = current->getParameters();

		if (type == ObjectTypes::script)
		{
			patchFileStream << typeName << " " << name;
		}
		else
		{
			patchFileStream << schema << " " << name << " " << typeName;

			if (type == ObjectTypes::function)
			{
				patchFileStream << " " << getParametersString(parameters);
			}
		}

		patchFileStream << endl;
	}

	file.close();
	return true;
}

PatchList FileHandler::parseObjectList(const QString &path, bool &isSuccessful)
{
	const QDir patchDir(path);
	QFile file(patchDir.absoluteFilePath(objectListName));

	if (!file.open(QIODevice::ReadOnly))
	{
		isSuccessful = false;
		return PatchList();
	}

	QTextStream input(&file);
	PatchList objectList;

	while (!input.atEnd())
	{
		const QString readString = input.readLine();

		if (readString.isEmpty())
		{
			continue;
		}

		int type = ObjectTypes::typeCount;
		QString schemaName = "";
		QString name;
		QStringList parameters = QStringList("");

		if (QRegExp("([^ ])+ ([^ ])+ (table|sequence|view|trigger|index)").exactMatch(readString))
		{
			const auto splitResult = readString.split(" ", QString::SkipEmptyParts);
			schemaName = splitResult.at(0);
			name = splitResult.at(1);
			type = ObjectTypes::typeNames.key(splitResult.at(2));
		}
		else if (QRegExp("script ([^ ])+").exactMatch(readString))
		{
			const auto splitResult = readString.split(" ", QString::SkipEmptyParts);
			name = splitResult.at(1);
			type = ObjectTypes::script;
		}
		else if (QRegExp("([^ ])+ ([^ ])+ function \\( (([^,() ])+ )*\\)").exactMatch(readString))
		{
			auto splitResult = readString.split(QRegExp("(\\ |\\(|\\))"), QString::SkipEmptyParts);
			schemaName = splitResult.first();
			splitResult.pop_front();
			name = splitResult.first();
			splitResult.pop_front();
			type = ObjectTypes::function;
			splitResult.pop_front();

			if (!splitResult.isEmpty())
			{
				parameters = splitResult;
			}
		}
		else
		{
			file.close();
			isSuccessful = false;
			return PatchList();
		}

		objectList.add(type, name, schemaName, parameters);
	}

	file.close();
	isSuccessful = true;
	return objectList;
}

PatchList FileHandler::parseDependencyList(const QString &path, bool &isSuccessful)
{
	const QDir patchDir(path);
	QFile file(patchDir.absoluteFilePath(dependencyListName));

	if (!file.open(QIODevice::ReadOnly))
	{
		isSuccessful = false;
		return PatchList();
	}

	QTextStream input(&file);
	PatchList dependencyList;

	while (!input.atEnd())
	{
		const QString readString = input.readLine();

		if (readString.isEmpty())
		{
			continue;
		}

		int type = ObjectTypes::typeCount;
		QString schemaName = "";
		QString name;

		if (QRegExp("([^ ])+ ([^ ])+ (table|sequence|view|trigger|index|function)").exactMatch(readString))
		{
			const auto splitResult = readString.split(" ", QString::SkipEmptyParts);
			schemaName = splitResult.at(0);
			name = splitResult.at(1);
			type = ObjectTypes::typeNames.key(splitResult.at(2));
		}
		else
		{
			file.close();
			isSuccessful = false;
			return PatchList();
		}

		dependencyList.add(type, name, schemaName, QStringList());
	}

	file.close();
	isSuccessful = true;
	return dependencyList;
}

QString FileHandler::getPatchListName()
{
	return patchListName;
}

QString FileHandler::getDependencyListName()
{
	return dependencyListName;
}

QString FileHandler::getObjectListName()
{
	return objectListName;
}

QString FileHandler::getParametersString(const QStringList &parameters)
{
	QString result = "( ";

	for (const auto &current : parameters)
	{
		result += current + " ";
	}

	return result + ")";
}