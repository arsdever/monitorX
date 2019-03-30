#include "device_informator.h"

#include "../shared/data.h"
#include <QLibrary>

DeviceInformator::DeviceInformator()
{
}

void *DeviceInformator::DataRequestInterface_impl::requestData()
{
	typedef void (*CPUInformationGetterFunction)(INFO * data);
	CPUInformationGetterFunction getCPUInformation = (CPUInformationGetterFunction)QLibrary::resolve("info_getter_api", "GetAllInfo");
	if (getCPUInformation == nullptr)
		return nullptr;

	INFO *device_information = new INFO{0};
	getCPUInformation(device_information);

	return device_information;
}