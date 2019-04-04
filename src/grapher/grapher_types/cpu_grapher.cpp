#include "cpu_grapher.h"

#include <QPainter>
#include "../client/data.h"

CPULoadGrapher::CPULoadGrapher(QWidget *parent)
	: Grapher(parent)
	  , __v_range(0, 1)
	  , __is_range_fixed(true)
	  , __is_user(false) {}

void CPULoadGrapher::paintEvent(QPaintEvent *event)
{
	if ( __total.isEmpty() )
		return;

	Grapher::paintEvent(event);

	if ( __kernel.isEmpty() )
		return;

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	QPainterPath path;
	path.moveTo(0, height());
	for ( int i = 0; i < __kernel.size(); ++i ) { path.lineTo(map(i, __kernel[i])); }
	path.lineTo(width(), height());
	painter.setPen(QPen(getGraphColor(), 1));
	painter.drawPath(path);
}

qreal CPULoadGrapher::getValueToShow() const { return __total.back(); }

QVector< qreal > const& CPULoadGrapher::getData() const { return __total; }

QPair< qreal, qreal > const& CPULoadGrapher::getVRange() const { return __v_range; }

void CPULoadGrapher::setFixedRange(qreal min, qreal max)
{
	if ( min < 0 || max < 0 || min > max )
	{
		__is_range_fixed = false;
		return;
	}

	__is_range_fixed = true;
	__v_range.first = min;
	__v_range.second = max;
}

void CPULoadGrapher::addData(qreal user, qreal kernel)
{
	__total.push_back(user + kernel);
	__kernel.push_back(kernel);

	emit dataChanged((user + kernel) * 100);

	if ( __is_range_fixed )
	{
	if ( user < __v_range.first )
		__v_range.first = user;

	if ( kernel < __v_range.first )
		__v_range.first = kernel;

	if ( user > __v_range.second )
		__v_range.second = user;

	if ( kernel > __v_range.second )
		__v_range.second = kernel;
		
	}

	update();
}

InfoConsumerInterface& CPULoadGrapher::InfoConsumerInterface_impl::operator <<(INFO *const &data)
{
	if ( __this->getMaxDataCount() > 0 )
	{
		if ( __this->count() > static_cast< uint32_t >(__this->getMaxDataCount()) )
		{
			__this->__total.pop_front();
			__this->__kernel.pop_front();
		}
	}

	__this->addData(data->cpuUsageUser / 100.0, data->cpuUsageKernel / 100.0);
	return *this;
}
