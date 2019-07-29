#pragma once

class PatchListElement;

class PatchList
{
public:
	PatchList();
	~PatchList();
	void add(const QString &typeName, const QString &schemaName, const QString &fullName);
	bool exportFile(const QString &path) const;
	void clear();
private:
	QList<PatchListElement*> *elements;
	static QString getParametersString(const QStringList &parameters);
};