#pragma once
#include <Windows.h>
#include <spfs/spfs.h>


inline qword GetDiskSize(HANDLE disk) {
	dword high = 0;
	dword low = GetFileSize(disk, (LPDWORD)&high);
	return low | (high << 32);
}

inline void GetDiskInfo(HANDLE disk, word* bytesPerSector, qword* sectors) {

	DISK_GEOMETRY geo;
	DWORD ret = 0;

	DeviceIoControl(disk, IOCTL_DISK_GET_DRIVE_GEOMETRY, 0, 0, &geo, sizeof(DISK_GEOMETRY), &ret, 0);

	if (ret != sizeof(DISK_GEOMETRY)) {
		if (bytesPerSector) *bytesPerSector = 512;
		if (sectors) *sectors = GetDiskSize(disk) / 512;
		return;
	}

	
	if (bytesPerSector) *bytesPerSector = geo.BytesPerSector;
	if (sectors) *sectors = geo.Cylinders.QuadPart * geo.TracksPerCylinder * geo.SectorsPerTrack;
}