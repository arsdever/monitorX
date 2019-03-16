#pragma once

#include "../grapher/grapher.h"

class RAMUsageGrapher : public Grapher
{
public:
	RAMUsageGrapher(QWidget *parent = nullptr);

	Grapher& operator << (qreal const& point) override;
};

