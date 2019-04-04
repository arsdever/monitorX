#pragma once

#include "../grapher.h"

class GRAPHER_EXPORT CPULoadGrapher : public Grapher
{

	Q_OBJECT

	IMPL_BEGIN(CPULoadGrapher, InfoConsumerInterface)
		InfoConsumerInterface & operator << (INFO * const &data) override;
	IMPL_END(InfoConsumerInterface)

public:
	CPULoadGrapher(QWidget *parent = nullptr);

	void addData(qreal user, qreal kernel);
	virtual QVector<qreal> const& getData() const override;
	virtual qreal getValueToShow() const override;
	virtual QPair<qreal, qreal> const& getVRange() const override;
	virtual void setFixedRange(qreal min, qreal max) override;

protected:
	void paintEvent(QPaintEvent *event) override;

signals:
	void dataChanged(qreal);

private:
	QVector<qreal> __total;
	QVector<qreal> __kernel;
	QPair<qreal, qreal> __v_range;
	bool __is_range_fixed;
	bool __is_user;
};

