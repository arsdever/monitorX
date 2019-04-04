#include "application.h"

#include "../client/data.h"
#include "../collector/device_informator.h"
#include "../collector/collector.h"
#include "../comport/comport.h"

Application::Application(QObject *parent)
	: QObject(parent)
	  , __device_info(new DeviceInformator)
	  , __collector(new Collector(__device_info->GetInterface_DataRequestInterface(), 500))
	  , __comport(nullptr) { connect(__collector, SIGNAL(dataChanged()), this, SLOT(updateData())); }

void Application::InterfaceRegistratorInterface_impl::registerInterface(Base *consumer)
{
	InfoConsumerInterface *ptr = dynamic_cast< InfoConsumerInterface* >(consumer);
	if ( ptr != nullptr )
		__this->__consumers.push_back(ptr);
}

void Application::updateData()
{
	INFO lastInfo = *(INFO *) __collector->getLast();

	for ( InfoConsumerInterface *itf : __consumers ) { *itf << &lastInfo; }

	if ( __comport == nullptr || !__comport->is_opened() )
		return;

	__comport->write_data((const char *) &lastInfo, sizeof(INFO));
}
