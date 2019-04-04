#include "informator.h"

#include <QDate>
#include <QTime>

Informator::Informator(INFO const *source, QWidget *parent)
	: QLabel(parent)
{
	setDataSource(source);
}

Informator::Informator(QWidget *parent)
	: QLabel(parent)
	, __source(nullptr)
{}

void Informator::setDataSource(INFO const *source)
{
	__source = source;
	dataChanged();
}

void Informator::dataChanged()
{
	if (__source == nullptr)
		return;

	QString const temp = QString("<font color=\"%1\">%3</font> : <font color=\"%2\">%4</font><br>").arg("#de4512").arg("#080808");
	QString info;
	info += temp.arg("Date").arg(QDate(__source->year, __source->month, __source->day).toString("dd.MM.yyyy"));
	info += temp.arg("Time").arg(QTime(__source->hour, __source->minu, __source->sec).toString("HH:mm:ss"));
	info += temp.arg("CPU cores").arg(__source->cores);
	info += temp.arg("CPU usage in user mode").arg(QString("%1%").arg(__source->cpuUsageUser));
	info += temp.arg("CPU usage in kernel mode").arg(QString("%1%").arg(__source->cpuUsageKernel));
	info += temp.arg("RAM memory total").arg(QString("%1 %2").arg(__source->memoryTotal).arg(prescalerToString(__source->memoryTotalPrescaler)));
	info += temp.arg("RAM memory free").arg(QString("%1 %2").arg(__source->memoryFree).arg(prescalerToString(__source->memoryFreePrescaler)));
	info += temp.arg("HDD memory total").arg(QString("%1 %2").arg(__source->hddTotal).arg(prescalerToString(__source->hddTotalPrescaler)));
	info += temp.arg("HDD memory free").arg(QString("%1 %2").arg(__source->hddFree).arg(prescalerToString(__source->hddFreePrescaler)));
	setText(info);
}
