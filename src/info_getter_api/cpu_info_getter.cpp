#include "api.h"

#include <windows.h>
#include <cmath>
#include <stdint.h>

static uint64_t FileTimeToInt64(const FILETIME & ft) { return (((uint64_t)(ft.dwHighDateTime)) << 32) | ((uint64_t)ft.dwLowDateTime); }

static void CalculateCPULoad(uint64_t idleTicks, uint64_t kernelTicks, uint64_t userTicks, float *idleLoad, float *kernelLoad, float *userLoad)
{
	static uint64_t _previousKernelTicks = 0;
	static uint64_t _previousUserTicks = 0;
	static uint64_t _previousIdleTicks = 0;

	uint64_t kernelTicksSinceLastTime = kernelTicks - _previousKernelTicks;
	uint64_t userTicksSinceLastTime = userTicks - _previousUserTicks;
	uint64_t idleTicksSinceLastTime = idleTicks - _previousIdleTicks;
	uint64_t totalTicksSinceLastTime = kernelTicksSinceLastTime + userTicksSinceLastTime;

	*kernelLoad = (kernelTicksSinceLastTime > 0 ? ((float)(kernelTicksSinceLastTime - idleTicksSinceLastTime) / (float)totalTicksSinceLastTime) : 0.0f);
	*userLoad = (userTicksSinceLastTime > 0 ? ((float)userTicksSinceLastTime / (float)totalTicksSinceLastTime) : 0.0f);
	*idleLoad = (idleTicksSinceLastTime > 0 ? ((float)idleTicksSinceLastTime / (float)totalTicksSinceLastTime) : 0.0f);

	_previousKernelTicks = kernelTicks;
	_previousUserTicks = userTicks;
	_previousIdleTicks = idleTicks;
}

extern "C" API_EXPORT void GetAllInfo(INFO *data)
{
	if (data == nullptr)
		return;

	GetSignature(data);
	GetDateAndTime(data);
	GetHardwareInfo(data);
}

extern "C" API_EXPORT void GetSignature(INFO *data)
{
	if (data == nullptr)
		return;

	data->signature[0] = 'I';
	data->signature[1] = 'N';
	data->signature[2] = 'F';
	data->signature[3] = 'O';
}

extern "C" API_EXPORT void GetDateAndTime(INFO *data)
{
	if (data == nullptr)
		return;

	GetDate(data);
	GetTime(data);
}

extern "C" API_EXPORT void GetDate(INFO *data)
{
	if (data == nullptr)
		return;

	SYSTEMTIME date;
	GetLocalTime(&date);

	data->year = date.wYear;
	data->month = date.wMonth;
	data->day = date.wDay;
}

extern "C" API_EXPORT void GetTime(INFO *data)
{
	if (data == nullptr)
		return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	data->hour = time.wHour;
	data->minu = time.wMinute;
	data->sec = time.wSecond;
}

extern "C" API_EXPORT void GetHardwareInfo(INFO* data)
{
	if (data == nullptr)
		return;

	GetCPUInfo(data);
	GetRAMInfo(data);
	GetHDDInfo(data);
}

extern "C" API_EXPORT void GetCPUInfo(INFO *data)
{
	if (data == nullptr)
		return;

	FILETIME idleTime, kernelTime, userTime;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	float userLoad = -1;	
	float kernelLoad = -1;
	float idleLoad = -1;
	if (GetSystemTimes(&idleTime, &kernelTime, &userTime))
	{
		CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime), FileTimeToInt64(userTime), &idleLoad, &kernelLoad, &userLoad);
	}
	data->cpuUsageUser = round(userLoad * 100);
	data->cpuUsageKernel = round(kernelLoad * 100);
	data->cores = sysInfo.dwNumberOfProcessors;
}

extern "C" API_EXPORT void GetRAMInfo(INFO *data)
{
	if (data == nullptr)
		return;

	MEMORYSTATUSEX memInfoEx = { sizeof MEMORYSTATUSEX };
	GlobalMemoryStatusEx(&memInfoEx);

	data->memoryTotal = memScaleDown(memInfoEx.ullTotalPhys, data->memoryTotalPrescaler);
	data->memoryFree = memScaleDown(memInfoEx.ullAvailPhys, data->memoryFreePrescaler);
}

extern "C" API_EXPORT void GetHDDInfo(INFO *data)
{
	if (data == nullptr)
		return;

	DWORD sectors, bytes, clusters, total;
	GetDiskFreeSpaceA("/", &sectors, &bytes, &clusters, &total);
	uint64_t totalSize = total;
	uint64_t freeSize = clusters;
	totalSize *= bytes;
	totalSize *= sectors;
	freeSize *= bytes;
	freeSize *= sectors;

	data->hddTotal = memScaleDown(totalSize , data->hddTotalPrescaler);
	data->hddFree = memScaleDown(freeSize, data->hddFreePrescaler);

}