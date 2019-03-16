#pragma once

#include "collector_global.h"
#include "../shared/interfaces.h"
#include "../shared/macros.h"

class COLLECTOR_EXPORT DeviceInformator
{
	IMPL_BEGIN(DeviceInformator, DataRequestInterface)	
		void* requestData() override;
	IMPL_END(DataRequestInterface)

public:
	DeviceInformator();
};
