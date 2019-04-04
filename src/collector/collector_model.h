#pragma once

#include <QAbstractItemModel>

class Collector;
class DeviceInformator;
class INFO;

class CollectorModel : public QAbstractItemModel
{

	Q_OBJECT

public:
	CollectorModel();
	~CollectorModel();

	int columnCount(QModelIndex const &parent = QModelIndex()) const override;
	int rowCount(QModelIndex const &parent = QModelIndex()) const override;

	QVariant data(QModelIndex const &index, int role) const override;
	QModelIndex index(int row, int col, QModelIndex const &parent = QModelIndex()) const override;

	QModelIndex parent(QModelIndex const &index) const override;

private slots:
	void dataChanged();

private:
	DeviceInformator *__informator;
	Collector *__collector;
	INFO *__last_info;
};

