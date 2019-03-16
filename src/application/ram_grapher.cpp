#include "ram_grapher.h"

RAMUsageGrapher::RAMUsageGrapher(QWidget *parent)
	: Grapher(parent)
{
	setFixedRange(0, 1);
}

Grapher& RAMUsageGrapher::operator <<(qreal const& point)
{
	if (count() > 150)
	{
		popFront();
	}

	return Grapher::operator << (point);
}
