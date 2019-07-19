#pragma once

#include <QAbstractListModel>
#include "ObjectType.h"
#include "PatchListElement.h"

class BuildListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	BuildListModel(QObject *parent);
	~BuildListModel();
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	void addObject(ObjectType type, const QString &name, const QString &scheme = ""
		, const QList<QPair<QString, QString>> &parameters = {});
	QStringList getObjectList();
private:
	QHash<int, QString> *typeIcons;
	QHash<int, QString> *typeNames;
	QList<PatchListElement*> *elements;
	static QString getParametersString(const QList<QPair<QString, QString>> &parameters);
	static const int columnAmount = 3;
};
