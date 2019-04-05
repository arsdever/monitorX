#include "line_grapher.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>
#include <QMetaProperty>

LineGrapher::LineGrapher()
    : __value(0)
{
    connect(this, SIGNAL(dataChanged(qreal)), this, SLOT(animate(qreal)));
}

void LineGrapher::paintEvent(QPaintEvent *event)
{
    if (__value == 0)
        return;

    QPainter painter(this);
    painter.setPen(QPen(getGraphColor(), 2));
    painter.drawLine(width() / 2 * (1 - __value), 1, width() / 2 * (__value + 1), 1);
}

void LineGrapher::animate(qreal)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "value");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::SineCurve);
    animation->setStartValue(__value);
    animation->setEndValue(getValueToShow());
    connect(animation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}