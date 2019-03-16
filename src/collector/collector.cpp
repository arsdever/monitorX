#include <collector.h>

#include "../shared/interfaces.h"

#include <QVector>

Collector::Collector(DataRequestInterface *requestor, int interval)
	: __max_buffer_size(300)
	, __interval(interval)
	, __request_interface(requestor)
{
	__buffer.reserve(__max_buffer_size);
	connect(&__timer, SIGNAL(timeout()), this, SLOT(collectDataOnTimeout()));
	__timer.start(__interval);
}

Collector::~Collector()
{
	for (void* data : __buffer)
		delete data;
}

void Collector::collectDataOnTimeout()
{
	__timer.start(__interval);
	collectData();
}

void Collector::collectData()
{
	if (__buffer.size() == __max_buffer_size)
		__buffer.dequeue();

	__buffer.enqueue(__request_interface->requestData());
	emit dataChanged();
}

int Collector::size() const
{
	return __buffer.size();
}

void* Collector::getLast() const
{
	return __buffer.back();
}

QVector<void*> Collector::getAll() const
{
	QVector<void*> result;
	result.resize(__buffer.size());
	for (void* i : __buffer)
		result.push_back(i);

	return result;
}