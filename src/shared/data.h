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

struct INFO
{
	char signature[4];

	// v1.0 (windows)
	int16_t year;
	int16_t memoryTotal;
	int16_t memoryFree;
	int16_t hddTotal;
	int16_t hddFree;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minu;
	int8_t sec;
	int8_t cores;
	int8_t memoryTotalPrescaler;
	int8_t memoryFreePrescaler;
	int8_t hddTotalPrescaler;
	int8_t hddFreePrescaler;
	int8_t cpuUsageUser;
	int8_t cpuUsageKernel;

	// v2.0 (windwos + linux)
	int8_t processes;
	int8_t *perCoreUsage;
};