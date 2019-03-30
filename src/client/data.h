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
};