#pragma once

#include "../grapher.h"

class GRAPHER_EXPORT RAMUsageGrapher : public Grapher
{

	Q_OBJECT

	IMPL_BEGIN(RAMUsageGrapher, InfoConsumerInterface)
		InfoConsumerInterface & operator << (INFO * const &data) override;
	IMPL_END(InfoConsumerInterface)

public:
	RAMUsageGrapher(QWidget *parent = nullptr);

	void addData(qreal user);
	virtual QVector<qreal> const& getData() const override;
	virtual qreal getValueToShow() const override;
	virtual QPair<qreal, qreal> const& getVRange() const override;
	virtual void setFixedRange(qreal min, qreal max) override;

protected:
	void paintEvent(QPaintEvent *event) override;

signals:
	void dataChanged(qreal);

private:
	QVector<qreal> __used;
	QPair<qreal, qreal> __v_range;
	bool __is_range_fixed;
	bool __is_user;
};

