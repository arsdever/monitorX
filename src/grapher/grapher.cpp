#include "grapher.h"

#include <QPainter>

Grapher::Grapher(QWidget *parent)
	: QWidget(parent)
	, __v_range(qInf(), -qInf())
	, __range_fixed(false)
{
}

void Grapher::popFront()
{
	__points.pop_front();
}

quint32 Grapher::count()
{
	return __points.size();
}

void Grapher::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.fillRect(rect(), Qt::white);
	QPainterPath path;
	path.moveTo(-1, height());
	for (int i = 0; i < __points.size(); ++i)
	{
		path.lineTo(map(i, __points[i]));
	}
	path.lineTo(width(), height());
	path.closeSubpath();
	QPainterPath grid;
	for (int i = 1; i < 10; ++i)
	{
		grid.moveTo(0, (qreal)height() / 10 * i);
		grid.lineTo(width(), (qreal)height() / 10 * i);
		grid.moveTo((qreal)width() / 10 * i, 0);
		grid.lineTo((qreal)width() / 10 * i, height());
	}
	painter.setPen(QPen(Qt::gray, 1));
	painter.setFont(QFont("Ubuntu", 72));
	if(!__points.empty())
		painter.drawText(rect(), Qt::AlignCenter, tr("%1").arg(qRound(__points.back() * 100)));
	painter.setBrush(Qt::NoBrush);
	painter.drawPath(grid);
	painter.setBrush(QColor(255, 0, 0, 128));
	painter.setPen(QPen(Qt::red, 2));
	painter.drawPath(path);
}

QPoint Grapher::map(quint32 index, qreal point)
{
	QSize s = size();
	QPoint result;
	point -= __v_range.first;

	if (__v_range.second - __v_range.first != 0)
		result.setY(s.height() - s.height() * point / (__v_range.second - __v_range.first));
	else
		result.setY(s.height() / 2);

	if (index != __points.size())
		result.setX(s.width() * (qreal)index / (qreal)(__points.size() - 1));
	else
		result.setX(s.width() / 2);

	return result;
}

void Grapher::setFixedRange(qreal min, qreal max)
{
	__v_range.first = min;
	__v_range.second = max;
}

Grapher& Grapher::operator << (qreal const& point)
{
	__points << point;
	if (__range_fixed)
		return *this;

	if (point < __v_range.first)
		__v_range.first = point;

	if (point > __v_range.second)
		__v_range.second = point;

	return *this;
}
