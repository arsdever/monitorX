#pragma once

#include <QList>
#include <QObject>

#include "../shared/interfaces.h"
#include "../shared/macros.h"

class Comport;
class Collector;
class DeviceInformator;
class InfoConsumerInterface;

class Application: public QObject
{
	Q_OBJECT

	IMPL_BEGIN(Application, InterfaceRegistratorInterface)
		void registerInterface(Base *) override;
	IMPL_END(InterfaceRegistratorInterface)

public:
	Application(QObject *parent = nullptr);

	void setComport(Comport *port) { __comport = port; }
	
public slots:
	void updateData();

private:
	DeviceInformator *__device_info;
	Collector *__collector;
	Comport *__comport;
	QList< InfoConsumerInterface* > __consumers;
};
