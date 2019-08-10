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

PatchList::PatchList(const PatchList &other)
	: elements(new QList<PatchListElement*>)
{
	for (const auto current : *other.elements)
	{
		elements->append(new PatchListElement(*current));
	}
}

//PatchList& PatchList::operator=(const PatchList &other)
//{
//	if (this != &other)
//	{
//		PatchList temp(other);
//		for ()
//	}
//
//	return *this;
//}

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

PatchList::~PatchList()
{
	clear();
	delete elements;
}