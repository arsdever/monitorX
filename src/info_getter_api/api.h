#pragma once

#include "api_global.h"
#include "../shared/data.h"

extern "C" API_EXPORT void GetAllInfo(INFO *data);

extern "C" API_EXPORT void GetSignature(INFO *data);

extern "C" API_EXPORT void GetDateAndTime(INFO *data);
extern "C" API_EXPORT void GetDate(INFO *data);
extern "C" API_EXPORT void GetTime(INFO *data);

extern "C" API_EXPORT void GetHardwareInfo(INFO* data);
extern "C" API_EXPORT void GetCPUInfo(INFO *data);
extern "C" API_EXPORT void GetRAMInfo(INFO *data);
extern "C" API_EXPORT void GetHDDInfo(INFO *data);
