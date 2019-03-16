#pragma once

#include "grapher_global.h"
#include <QWidget>
#include <QVector>
#include <QPair>

class QPaintEvent;

class GRAPHER_EXPORT Grapher : public QWidget
{

	Q_OBJECT

public:
	Grapher(QWidget *parent = nullptr);

	virtual Grapher& operator <<(qreal const& point);
	void popFront();
	quint32 count();
	void setFixedRange(qreal min, qreal max);

protected:
	void paintEvent(QPaintEvent *event) override;

	QPoint map(quint32 index, qreal point);

private:
	QVector<qreal> __points;
	QPair<qreal, qreal> __v_range;
	bool __range_fixed;
};
