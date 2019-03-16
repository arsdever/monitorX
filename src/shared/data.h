#pragma once

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
	__int16 year;
	__int16 memoryTotal;
	__int16 memoryFree;
	__int16 hddTotal;
	__int16 hddFree;
	__int8 month;
	__int8 day;
	__int8 hour;
	__int8 minu;
	__int8 sec;
	__int8 cores;
	__int8 memoryTotalPrescaler;
	__int8 memoryFreePrescaler;
	__int8 hddTotalPrescaler;
	__int8 hddFreePrescaler;
	__int8 cpuUsageUser;
	__int8 cpuUsageKernel;
};