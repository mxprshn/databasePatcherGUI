#pragma once

#include <QAbstractListModel>
#include <QList>
#include "DependenciesListElement.h"

class DependenciesListModel : public QAbstractListModel
{
	Q_OBJECT

public:

	DependenciesListModel(QObject *parent);
	~DependenciesListModel();

	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	bool parseDependenciesFromFile(const QString &filePath);
	bool parseCheckResultsFromFile(const QString &filePath);

private:

	QList<DependenciesListElement*> elements;
	QHash<int, QString> typeIcons;
	QHash<int, QString> statusIcons;
	QHash<int, QString> typeNames;
	static const int columnAmount = 3;
	static const int statusColumnIndex = 2;
};
