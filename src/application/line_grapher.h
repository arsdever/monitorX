#pragma once

#include "../grapher/grapher_types.h"

class LineGrapher : public CPULoadGrapher
{

    Q_OBJECT
    Q_PROPERTY(qreal value READ getValue WRITE setValue)

  public:
    LineGrapher(QWidget *parent = nullptr);

    void setValue(qreal value)
    {
        __value = value;
        update();
    }
    qreal getValue() const { return __value; }

  protected:
    void paintEvent(QPaintEvent *event) override;

  protected slots:
    void animate(qreal);

  private:
    qreal __value;
};