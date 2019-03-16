#pragma once

#include "../grapher/grapher.h"

class CPULoadGrapher : public Grapher
{
public:
	CPULoadGrapher(QWidget *parent = nullptr);

	Grapher& operator << (qreal const& point) override;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QVector<qreal> __kernel;
	bool __is_user;
};

