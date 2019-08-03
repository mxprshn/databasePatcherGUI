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

void PatchList::add(int type, const QString &schemaName, const QString &fullName)
{
	auto splitResult = fullName.split(QRegExp("(\\ |\\,|\\(|\\))"), QString::SkipEmptyParts);
	const auto itemName = splitResult.first();
	splitResult.pop_front();
	elements->append(new PatchListElement(type, itemName, schemaName, splitResult));
}

int PatchList::count() const
{
	return elements->count();
}

PatchListElement PatchList::at(int position) const
{
	// Add invalid index processing
	return *elements->at(position);
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
		const auto typeName = ObjectTypes::typeNames.value(type);
		const auto parameters = elements->at(i)->getParameters();

		if (type ==ObjectTypes::script)
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

		if (i != elements->count() - 1)
		{
			patchFileStream << endl;
		}
	}

	file.close();
	return true;
}

bool PatchList::importFile(const QString &path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream input(&file);

	while (!input.atEnd())
	{
		int type = ObjectTypes::typeCount;
		QString schemaName = "";
		QString name = "";
		QStringList parameters;

		const auto readString = input.readLine();

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
		else if (QRegExp("([^ ])+ ([^ ])+ function \\( (([^,() ])+ )+\\)").exactMatch(readString))
		{
			auto splitResult = readString.split(QRegExp("(\\ |\\(|\\))"), QString::SkipEmptyParts);
			schemaName = splitResult.first();
			splitResult.pop_front();
			name = splitResult.first();
			splitResult.pop_front();
			type = ObjectTypes::function;
			splitResult.pop_front();
			parameters = splitResult;
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