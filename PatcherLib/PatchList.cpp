#include "PatchList.h"
#include "PatchListElement.h"

#include <QList>
#include <QFile>
#include <QTextStream>
#include <algorithm>

// Constructor
PatchList::PatchList()
	: elements(new QList<PatchListElement*>)
{
}

// Copy constructor
PatchList::PatchList(const PatchList &other)
	: elements(new QList<PatchListElement*>)
{
	for (const auto current : *other.elements)
	{
		elements->append(new PatchListElement(*current));
	}
}

// Assignment operator
PatchList& PatchList::operator=(const PatchList &other)
{
	if (this != &other)
	{
		PatchList(other).swap(*this);
	}
	return *this;
}

// Swap method used in assignment operator implementation
void PatchList::swap(PatchList &other)
{
	std::swap(elements, other.elements);
}

// Adds a new object to the list
void PatchList::add(int typeIndex, const QString &schemaName, const QString &name, const QStringList &parameters)
{	
	elements->append(new PatchListElement(typeIndex, name, schemaName, parameters));
}

// Returns amount of objects in the list
int PatchList::count() const
{
	return elements->count();
}

// Iterators used in range-based 'for' loop
QList<class PatchListElement*>::const_iterator PatchList::begin() const
{
	return elements->constBegin();
}

QList<class PatchListElement*>::const_iterator PatchList::end() const
{
	return elements->constEnd();
}

// Clears the list
void PatchList::clear()
{
	for (const auto current : *elements)
	{
		delete current;
	}

	elements->clear();
}

// Destructor
PatchList::~PatchList()
{
	clear();
	delete elements;
}