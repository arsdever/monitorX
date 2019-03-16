#include <iostream>
#include "comport.h"
#include "typedef.h"


std::ostream & operator << (std::ostream & strm, INFO const& info)
{
	strm << "{\n" << std::flush;
	strm << "\t\"version\" : \"" << (__int32)info.version << "\"\n" << std::flush;
	strm << "\t\"date\" :\n" << std::flush;
	strm << "\t{\n" << std::flush;
	strm << "\t\t\"year\" : \"" << (__int32)info.year << "\"\n" << std::flush;
	strm << "\t\t\"month\" : \"" << (__int32)info.month << "\"\n" << std::flush;
	strm << "\t\t\"day\" : \"" << (__int32)info.day << "\"\n" << std::flush;
	strm << "\t\t\"hour\" : \"" << (__int32)info.hour << "\"\n" << std::flush;
	strm << "\t\t\"minute\" : \"" << (__int32)info.minu << "\"\n" << std::flush;
	strm << "\t\t\"second\" : \"" << (__int32)info.sec << "\"\n" << std::flush;
	strm << "\t}\n" << std::flush;
	strm << "\t\"memoryTotal\" : \"" << (__int32)info.memoryTotal << "\"\n" << std::flush;
	strm << "\t\"memoryTotalPrescaler\" : \"" << (__int32)info.memoryTotalPrescaler << "\"\n" << std::flush;
	strm << "\t\"memoryFree\" : \"" << (__int32)info.memoryFree << "\"\n" << std::flush;
	strm << "\t\"memoryFreePrescaler\" : \"" << (__int32)info.memoryFreePrescaler << "\"\n" << std::flush;
	strm << "\t\"hddTotal\" : \"" << (__int32)info.hddTotal << "\"\n" << std::flush;
	strm << "\t\"hddTotalPrescaler\" : \"" << (__int32)info.hddTotalPrescaler << "\"\n" << std::flush;
	strm << "\t\"hddFree\" : \"" << (__int32)info.hddFree << "\"\n" << std::flush;
	strm << "\t\"hddFreePrescaler\" : \"" << (__int32)info.hddFreePrescaler << "\"\n" << std::flush;
	strm << "\t\"cores\" : \"" << (__int32)info.cores << "\"\n" << std::flush;
	strm << "\t\"cpuUsage\" : \"" << (__int32)info.cpuUsage << "\"\n" << std::flush;
	return strm << "}\n" << std::flush;
}
char buffer[255];

static i8 CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;

	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

	i8 ret = 100 - ((totalTicksSinceLastTime > 0) ? (idleTicksSinceLastTime * 100 / totalTicksSinceLastTime) : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME & ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
float GetCPULoad()
{
	static FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}

void WriteSize(i8& prescaler, i16& size, ui64 value)
{
	prescaler = 0;
	while (value > 64000)
	{
		++prescaler;
		value >>= 10;
	}

	size = value;
}

void CalculateHDD(INFO &buffer)
{
	static DWORD sectors, bytes, clusters, total;
	GetDiskFreeSpaceA("/", &sectors, &bytes, &clusters, &total);
	unsigned long long totalSize = total;
	unsigned long long freeSize = clusters;
	totalSize *= bytes;
	totalSize *= sectors;
	freeSize *= bytes;
	freeSize *= sectors;

	WriteSize(buffer.hddTotalPrescaler, buffer.hddTotal, totalSize);
	WriteSize(buffer.hddFreePrescaler, buffer.hddFree, freeSize);
}

void CalculateRAM(INFO &buffer)
{
	static MEMORYSTATUSEX memInfoEx = { sizeof MEMORYSTATUSEX };
	GlobalMemoryStatusEx(&memInfoEx);

	WriteSize(buffer.memoryTotalPrescaler, buffer.memoryTotal, memInfoEx.ullTotalPhys);
	WriteSize(buffer.memoryFreePrescaler, buffer.memoryFree, memInfoEx.ullAvailPhys);
}

void CollectCPUInfo(INFO& info)
{
	static SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	info.cpuUsage = GetCPULoad();
	info.cores = sysInfo.dwNumberOfProcessors;
}

void CollectTimeInfo(INFO& info)
{
	static SYSTEMTIME time;
	GetLocalTime(&time);
	info.year = time.wYear;
	info.month = time.wMonth;
	info.day = time.wDay;		
	info.hour = time.wHour;
	info.minu = time.wMinute;
	info.sec = time.wSecond;
}

void Connect(HANDLE & com, CString portName)
{
	std::cout << "connecting" << std::endl;
	com = CreateFile(portName.ToCString(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DCB m_sSettings = { 0 };
	m_sSettings.DCBlength = sizeof(m_sSettings);

	if (!GetCommState(com, &m_sSettings))
	{
		Sleep(500);
		Connect(com, portName);
	}

	m_sSettings.BaudRate = 19200;
	m_sSettings.ByteSize = 8;
	m_sSettings.StopBits = 1;
	m_sSettings.Parity = 0;
	m_sSettings.fDtrControl = DTR_CONTROL_ENABLE;
	SetCommState(com, &m_sSettings);
	Sleep(2000);
}

int main(int argc, char** argv)
{
	std::cout << "Running on core " << GetCurrentProcessorNumber() << std::endl;

	CString portName;
	std::cout << "Portname:" << std::flush;
	std::cin >> portName;

	HANDLE com;
	Connect(com, portName);
	DWORD written;

	while (1)
	{
		CalculateHDD(file);
		CalculateRAM(file);
		CollectCPUInfo(file);
		CollectTimeInfo(file);
		//std::cout << file << std::endl;
		WriteFile(com, &file, 25, &written, 0);
		if (written != 25)
			Connect(com, portName);

		Sleep(1000);
		ReadFile(com, &file, 1, &written, 0);
	}

	return 0;
}