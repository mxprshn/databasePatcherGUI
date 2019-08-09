#include <QList>
#include <QFile>
#include <QTextStream>

#include "PatchList.h"
#include "PatchListElement.h"
#include "ObjectTypes.h"

PatchList::PatchList()
	: elements(new QList<PatchListElement*>)
{
}

//void PatchList::add(int type, const QString &schemaName, const QString &fullName)
//{
//	auto splitResult = fullName.split(QRegExp("(\\ |\\,|\\(|\\))"), QString::SkipEmptyParts);
//	const auto itemName = splitResult.first();
//	splitResult.pop_front();
//	elements->append(new PatchListElement(type, itemName, schemaName, splitResult));
//}

PatchList::PatchList(const PatchList &other)
{
	elements 
}


void PatchList::add(int typeIndex, const QString &schemaName, const QString &name, const QStringList &parameters)
{	
	elements->append(new PatchListElement(typeIndex, name, schemaName, parameters));
}

int PatchList::count() const
{
	return elements->count();
}

QList<class PatchListElement*>::const_iterator PatchList::begin() const
{
	return elements->constBegin();
}

QList<class PatchListElement*>::const_iterator PatchList::end() const
{
	return elements->constEnd();
}


PatchListElement PatchList::at(int position) const
{
	// Add invalid index processing
	return *elements->at(position);
}

void PatchList::clear()
{
	for (const auto current : *elements)
	{
		delete current;
	}

	elements->clear();
}

bool PatchList::importPatchListFile(const QString &path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream input(&file);

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
			return false;
		}

		elements->append(new PatchListElement(type, name, schemaName, parameters));
	}

	file.close();
	return true;
}

bool PatchList::importDependenciesListFile(const class QString &path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream input(&file);

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
			return false;
		}

		elements->append(new PatchListElement(type, name, schemaName, QStringList()));
	}

	file.close();
	return true;
}


PatchList::~PatchList()
{
	clear();
	delete elements;
}