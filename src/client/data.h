#pragma once

#include <stdint.h>

enum PRESCALER
{
	B,
	KB,
	MB,
	GB,
	TB,
	PB
};

inline const char* prescalerToString(uint8_t sc)
{
	switch (sc)
	{
	case B:
		return "B";
	case KB:
		return "KB";
	case MB:
		return "MB";
	case GB:
		return "GB";
	case TB:
		return "TB";
	case PB:
		return "PB";
	}
	return "";
}

static uint64_t memScaleUp(uint16_t mem_size, uint8_t prescaler)
{
	uint64_t result = mem_size;
	while (prescaler--)
		result <<= 10;
	return result;
}

static uint16_t memScaleDown(uint64_t mem_size, uint8_t & prescaler)
{
	prescaler = 0;
	while (mem_size > 65535)
	{
		mem_size >>= 10;
		++prescaler;
	}
	return (uint16_t)mem_size;
}

struct INFO
{
	char signature[4];

	// v1.0 (windows)
	uint16_t year;
	uint16_t memoryTotal;
	uint16_t memoryFree;
	uint16_t hddTotal;
	uint16_t hddFree;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minu;
	uint8_t sec;
	uint8_t cores;
	uint8_t memoryTotalPrescaler;
	uint8_t memoryFreePrescaler;
	uint8_t hddTotalPrescaler;
	uint8_t hddFreePrescaler;
	uint8_t cpuUsageUser;
	uint8_t cpuUsageKernel;

	// v2.0 (windwos + linux)
	int8_t processes;
	//int8_t *perCoreUsage;
};