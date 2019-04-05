#include "line_grapher.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>
#include <QMetaProperty>

LineGrapher::LineGrapher(QWidget *parent)
	: CPULoadGrapher(parent)
	  , __value(0) { connect(this, SIGNAL(dataChanged(qreal)), this, SLOT(animate(qreal))); }

void LineGrapher::paintEvent(QPaintEvent *event)
{
    if (__value == 0)
        return;

    QPainter painter(this);
	QColor clr;
	painter.fillRect(0, 0, width(), 2, Qt::black);
	clr.setHslF((1 - __value) / 3.0, 1, .5);
    painter.fillRect(width() / 2 * (1 - __value), 0, width() * __value, 2, clr);
}

void LineGrapher::animate(qreal)
{
    QPropertyAnimation *value_animation = new QPropertyAnimation(this, "value");
    value_animation->setDuration(1000);
    value_animation->setEasingCurve(QEasingCurve::SineCurve);
    value_animation->setStartValue(__value);
    value_animation->setEndValue(getValueToShow());
    connect(value_animation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
    value_animation->start(QAbstractAnimation::DeleteWhenStopped);
}