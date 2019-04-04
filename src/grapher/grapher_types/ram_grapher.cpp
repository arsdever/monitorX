#include "ram_grapher.h"

#include <QPainter>
#include "../client/data.h"

RAMUsageGrapher::RAMUsageGrapher(QWidget *parent)
	: Grapher(parent)
	, __v_range(qInf(), -qInf())
	, __is_range_fixed(false)
	, __is_user(false)
{
	setFixedRange(0, 1);
}

void RAMUsageGrapher::paintEvent(QPaintEvent *event)
{
	Grapher::paintEvent(event);
}

qreal RAMUsageGrapher::getValueToShow() const
{
	return __used.back();
}

QVector<qreal> const& RAMUsageGrapher::getData() const
{
	return __used;
}

QPair<qreal, qreal> const& RAMUsageGrapher::getVRange() const
{
	return __v_range;
}

void RAMUsageGrapher::setFixedRange(qreal min, qreal max)
{
	if (min < 0 || max < 0 || min > max)
	{
		__is_range_fixed = false;
		return;
	}

	__is_range_fixed = true;
	__v_range.first = min;
	__v_range.second = max;
}

void RAMUsageGrapher::addData(qreal used)
{
	__used.push_back(used);

	emit dataChanged(used * 100);
	
	if (__is_range_fixed)
		return;

	if (used < __v_range.first)
		__v_range.first = used;

	if (used > __v_range.second)
		__v_range.second = used;
}

InfoConsumerInterface & RAMUsageGrapher::InfoConsumerInterface_impl::operator << (INFO * const &data)
{
	if (__this->count() > __this->getMaxDataCount())
	{
		__this->__used.pop_front();
	}
	
	__this->addData((float)memScaleUp(data->memoryFree, data->memoryFreePrescaler) / (float)memScaleUp(data->memoryTotal, data->memoryTotalPrescaler));
	return *this;
}
