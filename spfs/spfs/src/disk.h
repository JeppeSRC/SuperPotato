#pragma once
#include <Windows.h>

unsigned int GetBytesPerSector(HANDLE disk) {

	DISK_GEOMETRY geo;
	DWORD ret = 0;

	DeviceIoControl(disk, IOCTL_DISK_GET_DRIVE_GEOMETRY, 0, 0, &geo, sizeof(DISK_GEOMETRY), &ret, 0);

	if (ret != sizeof(DISK_GEOMETRY)) return 512;

	return geo.BytesPerSector;
}
