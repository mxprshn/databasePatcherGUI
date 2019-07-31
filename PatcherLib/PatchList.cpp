#include <QList>
#include <QFile>
#include <QTextStream>

#include "PatchList.h"
#include "PatchListElement.h"
#include "ObjectType.h"

PatchList::PatchList()
	: elements(new QList<PatchListElement*>)
{
}

void PatchList::add(int type, const QString &typeName, const QString &schemaName, const QString &fullName)
{
	auto splitResult = fullName.split(QRegExp("(\\ |\\,|\\(|\\))"), QString::SkipEmptyParts);
	const auto itemName = splitResult.first();
	splitResult.pop_front();
	elements->append(new PatchListElement(type, typeName, itemName, schemaName, splitResult));
}

void PatchList::clear()
{
	for (auto i = 0; i < elements->count(); ++i)
	{
		delete elements->at(i);
	}

	elements->clear();
}

bool PatchList::exportFile(const QString &path) const
{
	QFile file(path);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::NewOnly))
	{
		return false;
	}

	QTextStream patchFileStream(&file);

	for (auto i = 0; i < elements->count(); ++i)
	{
		const auto schema = elements->at(i)->getSchema();
		const auto name = elements->at(i)->getName();
		const auto type = elements->at(i)->getType();
		const auto typeName = elements->at(i)->getTypeName();
		const auto parameters = elements->at(i)->getParameters();

		if (schema != "")
		{
			patchFileStream << schema << " ";
		}

		patchFileStream << name << " " << typeName;

		if (type == function)
		{
			patchFileStream << " " << getParametersString(parameters);
		}

		if (i != elements->count() - 1)
		{
			patchFileStream << endl;
		}
	}

	file.close();
	return true;
}

QString PatchList::getParametersString(const QStringList &parameters)
{
	QString result = "( ";

	for (auto i = 0; i < parameters.count(); ++i)
	{
		result += parameters[i] + " ";
	}

	return result + ")";
}

PatchList::~PatchList()
{
	clear();
	delete elements;
}
