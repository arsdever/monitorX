#pragma once

#include "collector_global.h"

#include <QObject>
#include <QString>
#include <QQueue>
#include <QTimer>

class DataRequestInterface;

template <typename __type>
class QVector;

class COLLECTOR_EXPORT Collector : public QObject
{

	Q_OBJECT

  public:
	Collector(DataRequestInterface *requestInterface, int interval);
	~Collector();

	int size() const;
	void* getLast() const;
	QVector<void*> getAll() const;

public slots:
	void collectData();
	void collectDataOnTimeout();

signals:
	void dataChanged();

  private:
	const int __max_buffer_size;
	const int __interval;
	DataRequestInterface *__request_interface;
	QQueue<void *> __buffer;
	QTimer __timer;
};
