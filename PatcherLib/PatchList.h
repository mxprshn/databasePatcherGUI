#pragma once

class PatchListElement;

class PatchList
{
public:
	PatchList();
	~PatchList();
	PatchList(const PatchList &other);
	// void add(int type, const QString &schemaName, const QString &fullName);
	void add(int typeIndex, const QString &schemaName, const QString &name, const QStringList &parameters = {});
	int count() const;
	QList<PatchListElement*>::const_iterator begin() const;
	QList<PatchListElement*>::const_iterator end() const;
	PatchListElement at(int position) const;
	bool importPatchListFile(const QString &path);
	bool importDependenciesListFile(const QString &path);
	void clear();
private:
	QList<PatchListElement*> *elements;
};