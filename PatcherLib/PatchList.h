#pragma once

class PatchListElement;

class PatchList
{
public:
	PatchList();
	~PatchList();
	void add(int type, const QString &schemaName, const QString &fullName);
	int count() const;
	PatchListElement at(int position) const;
	bool exportFile(const QString &path) const;
	bool importFile(const QString &path);
	void clear();
	static QString typeName(int typeIndex);
private:
	QList<PatchListElement*> *elements;
	static QString getParametersString(const QStringList &parameters);
	static const QHash<int, QString> *typeNames;
};