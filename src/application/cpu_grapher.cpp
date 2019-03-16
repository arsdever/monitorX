#include "cpu_grapher.h"

#include <QPainter>

CPULoadGrapher::CPULoadGrapher(QWidget *parent)
	: Grapher(parent)
	, __is_user(false)
{
	setFixedRange(0, 1);
}

void CPULoadGrapher::paintEvent(QPaintEvent *event)
{
	Grapher::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	QPainterPath path;
	path.moveTo(0, height());
	for (int i = 0; i < __kernel.size(); ++i)
	{
		path.lineTo(map(i, __kernel[i]));
	}
	path.lineTo(width(), height());
	//painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(Qt::red, 1));
	painter.drawPath(path);
}

Grapher& CPULoadGrapher::operator <<(qreal const& point)
{
	if (count() > 150)
	{
		popFront();
		__kernel.pop_front();
	}

	__is_user = !__is_user;

	if (__is_user)
		return Grapher::operator << (point);
	
	__kernel << point;
	update();
	return *this;
}