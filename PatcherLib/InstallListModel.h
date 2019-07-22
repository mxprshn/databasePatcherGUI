#pragma once

#include <QAbstractListModel>

class InstallListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	InstallListModel(QObject *parent);
	~InstallListModel();
	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
};
