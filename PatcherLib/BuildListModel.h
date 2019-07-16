#pragma once

#include <QAbstractListModel>

class BuildListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	BuildListModel(QObject *parent);
	~BuildListModel();
	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
};
