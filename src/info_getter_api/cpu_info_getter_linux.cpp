#include "api.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <cmath>
#include <stdint.h>
#include <cstring>

#include <ctime>

#include <sys/sysinfo.h> // for getting ram information
#include <sys/statvfs.h> // for getting hdd information

//static uint64_t FileTimeToInt64(const FILETIME &ft) { return (((uint64_t)(ft.dwHighDateTime)) << 32) | ((uint64_t)ft.dwLowDateTime); }

void WriteSize(int8_t &prescaler, int16_t &size, uint64_t value)
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
	data->signature[3] = '2';
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

	std::time_t time = std::time(0);
	std::tm *now = std::localtime(&time);
	data->year = now->tm_year + 1900;
	data->month = now->tm_mon;
	data->day = now->tm_wday;
}

extern "C" API_EXPORT void GetTime(INFO *data)
{
	if (data == nullptr)
		return;

	std::time_t time = std::time(0);
	std::tm *now = std::localtime(&time);
	data->hour = now->tm_hour;
	data->minu = now->tm_min;
	data->sec = now->tm_sec;
}

extern "C" API_EXPORT void GetHardwareInfo(INFO *data)
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

	std::fstream cpu_info_file_stream;
	cpu_info_file_stream.open("/proc/stat", std::fstream::in);

	if (!cpu_info_file_stream.is_open())
		return;

	std::string line;
	std::getline(cpu_info_file_stream, line);
	data->cores = sysconf(_SC_NPROCESSORS_ONLN);
	data->perCoreUsage = new int8_t[data->cores * 2];

	while (line != "")
	{
		std::stringstream line_stream(line);
		std::string word;
		line_stream >> word;
		if (word.substr(0, 3) == "cpu")
		{
			static std::vector<uint64_t> _previousKernelTicks(1, 0);
			static std::vector<uint64_t> _previousUserTicks(1, 0);
			static std::vector<uint64_t> _previousTotalTicks(1, 0);

			uint64_t user;
			uint64_t nice;
			uint64_t sys;
			uint64_t idle;
			uint64_t iowait;
			uint64_t irq;
			uint64_t softirq;

			line_stream >> user >> nice >> sys >> idle >> iowait >> irq >> softirq;
			uint64_t total = user + nice + sys + idle + iowait + irq + softirq;

			int32_t index = -1;
			if (word != "cpu")
			{
				std::stringstream cpu_id(word.substr(3));
				cpu_id >> index;
				if ((int32_t)_previousKernelTicks.size() <= index + 1)
				{
					_previousKernelTicks.resize(index + 2);
					_previousTotalTicks.resize(index + 2);
					_previousUserTicks.resize(index + 2);
				}
			}

			uint64_t totalTickCount = total - _previousTotalTicks[index + 1];
			uint64_t kernelTickCount = sys - _previousKernelTicks[index + 1];
			uint64_t userTickCount = user - _previousUserTicks[index + 1];
			double kernelUsage = (double)kernelTickCount / (double)totalTickCount;
			double userUsage = (double)userTickCount / (double)totalTickCount;

			if (!index)
			{
				data->cpuUsageKernel = kernelUsage * 100;
				data->cpuUsageUser = userUsage * 100;
			}
			// else
			// {
			// 	data->perCoreUsage[index * 2] = kernelUsage * 100;
			// 	data->perCoreUsage[index * 2 + 1] = userUsage * 100;
			// }

			_previousTotalTicks[index + 1] = total;
			_previousKernelTicks[index + 1] = sys;
			_previousUserTicks[index + 1] = user;
		}
		std::getline(cpu_info_file_stream, line);
	}
}

extern "C" API_EXPORT void GetRAMInfo(INFO *data)
{
	if (data == nullptr)
		return;

	struct sysinfo info;
	memset(&info, 0, sizeof(struct sysinfo));
	if (sysinfo(&info))
		return;

	int8_t prescaler;
	int16_t size;

	uint64_t memTotal = info.totalram;
	uint64_t memFree = info.freeram;
	WriteSize(prescaler, size, memTotal);
	data->memoryTotalPrescaler = prescaler;
	data->memoryTotal = size;
	WriteSize(prescaler, size, memFree);
	data->memoryFreePrescaler = prescaler;
	data->memoryFree = size;
	data->processes = info.procs;
}

extern "C" API_EXPORT void GetHDDInfo(INFO *data)
{
	if (data == nullptr)
		return;

	struct statvfs fs_info;
	memset(&fs_info, 0, sizeof(struct statvfs));
	if (statvfs("/", &fs_info))
		return;

	int8_t prescaler;
	int16_t size;

	uint64_t mem_free = fs_info.f_bavail * fs_info.f_bsize;
	uint64_t mem_total = fs_info.f_blocks * fs_info.f_bsize;
	WriteSize(prescaler, size, mem_total);
	data->hddTotalPrescaler = prescaler;
	data->hddTotal = size;
	WriteSize(prescaler, size, mem_free);
	data->hddFreePrescaler = prescaler;
	data->hddFree = size;
}