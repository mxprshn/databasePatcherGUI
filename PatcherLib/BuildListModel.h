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
		, const QStringList &parameters = {});

	int count() const;
	ObjectType getType(int index) const;
	QString getTypeName(int index) const;
	QString getSchemeName(int index) const;
	QString getName(int index) const;
	QStringList getParameters(int index) const;
private:
	QHash<int, QString> *typeIcons;
	QHash<int, QString> *typeNames;
	QVector<PatchListElement*> *elements;
	static const int columnAmount = 3;
};
