#include "api.h"

#include <windows.h>
#include <cmath>

static unsigned __int64 FileTimeToInt64(const FILETIME & ft) { return (((unsigned __int64)(ft.dwHighDateTime)) << 32) | ((unsigned __int64)ft.dwLowDateTime); }

static void CalculateCPULoad(unsigned __int64 idleTicks, unsigned __int64 kernelTicks, unsigned __int64 userTicks, float *idleLoad, float *kernelLoad, float *userLoad)
{
	static unsigned __int64 _previousKernelTicks = 0;
	static unsigned __int64 _previousUserTicks = 0;
	static unsigned __int64 _previousIdleTicks = 0;

	unsigned __int64 kernelTicksSinceLastTime = kernelTicks - _previousKernelTicks;
	unsigned __int64 userTicksSinceLastTime = userTicks - _previousUserTicks;
	unsigned __int64 idleTicksSinceLastTime = idleTicks - _previousIdleTicks;
	unsigned __int64 totalTicksSinceLastTime = kernelTicksSinceLastTime + userTicksSinceLastTime;

	*kernelLoad = (kernelTicksSinceLastTime > 0 ? ((double)(kernelTicksSinceLastTime - idleTicksSinceLastTime) / (double)totalTicksSinceLastTime) : 0.0);
	*userLoad = (userTicksSinceLastTime > 0 ? ((double)userTicksSinceLastTime / (double)totalTicksSinceLastTime) : 0.0);
	*idleLoad = (idleTicksSinceLastTime > 0 ? ((double)idleTicksSinceLastTime / (double)totalTicksSinceLastTime) : 0.0);
	//*kernelLoad = 0;
	//*userLoad = 1.0 - ((userTicksSinceLastTime > 0) ? ((double)idleTicksSinceLastTime/ (kernelTicksSinceLastTime + userTicksSinceLastTime)) : 0.0);
	//*idleLoad = 0;

	_previousKernelTicks = kernelTicks;
	_previousUserTicks = userTicks;
	_previousIdleTicks = idleTicks;
}

void WriteSize(__int8& prescaler, __int16& size, unsigned __int64 value)
{
	prescaler = 0;
	while (value > 65535)
	{
		++prescaler;
		value >>= 10;
	}

	size = value;
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

	WriteSize(data->memoryTotalPrescaler, data->memoryTotal, memInfoEx.ullTotalPhys);
	WriteSize(data->memoryFreePrescaler, data->memoryFree, memInfoEx.ullAvailPhys);
}

extern "C" API_EXPORT void GetHDDInfo(INFO *data)
{
	if (data == nullptr)
		return;

	DWORD sectors, bytes, clusters, total;
	GetDiskFreeSpaceA("/", &sectors, &bytes, &clusters, &total);
	unsigned __int64 totalSize = total;
	unsigned __int64 freeSize = clusters;
	totalSize *= bytes;
	totalSize *= sectors;
	freeSize *= bytes;
	freeSize *= sectors;

	WriteSize(data->hddTotalPrescaler, data->hddTotal, totalSize);
	WriteSize(data->hddFreePrescaler, data->hddFree, freeSize);

}