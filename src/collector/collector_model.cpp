#include "collector_model.h"

#include "../collector/collector.h"
#include "../collector/device_informator.h"
#include "../client/data.h"

#include <QVariant>
#include <QModelIndex>

static QList<QString> data_categories = {"Signature", "Date", "Time", "CPU", "RAM", "HDD", "Power"};

CollectorModel::CollectorModel()
	: __informator(new DeviceInformator())
	, __collector(new Collector(__informator->GetInterface_DataRequestInterface(), 500))
	, __last_info(nullptr)
{
	connect(__collector, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
}

CollectorModel::~CollectorModel()
{
	delete __collector;
	delete __informator;
}

int CollectorModel::columnCount(QModelIndex const &index) const
{
	return 2;
}

int CollectorModel::rowCount(QModelIndex const &index) const
{
	return data_categories.size();
}

QVariant CollectorModel::data(QModelIndex const &index, int role) const
{
	if (index.isValid())
	{
		return (uint64_t)__collector->getAll().at(index.row());
	}

	return (uint64_t)__collector->getLast();
}

QModelIndex CollectorModel::index(int row, int col, QModelIndex const &parent = QModelIndex()) const
{
	return createIndex(row, col, __collector->getLast());
}

QModelIndex CollectorModel::parent(QModelIndex const &index) const
{
	return QModelIndex();
}

void CollectorModel::dataChanged()
{
	emit dataChanged(QModelIndex(), QModelIndex());
}