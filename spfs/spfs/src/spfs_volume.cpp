#include <spfs/spfs.h>

#include "disk.h"

void CalculateAllocationTable(qword sectors, qword size, qword* usable, qword* tracking) {
	*tracking = sectors / size;

	qword notNeeded = *tracking / size;

	if (notNeeded) notNeeded--;

	*tracking -= notNeeded;
	*usable = sectors - *tracking;
}

qword GetDiskSize(HANDLE disk) {
	dword high = 0;
	dword low = GetFileSize(disk, (LPDWORD)&high);
	return low | (high << 32);
}

dword OpenVolume(HANDLE disk, byte partition, SPFS_VOLUME* vol) {
	if (!vol) return SPFS_ERROR_INVALID_PARAM;

	vol->handle = disk;
	vol->offset = 0;

	DiskRead(disk, 0, sizeof(SPFS_HEADER), &vol->hdr);

	if (vol->hdr.Version != SPFS_VERSION) return SPFS_ERROR_NO_MEDIA;

	vol->offsetToAllocationTable = vol->hdr.FileTables + vol->offset + 1;

	return SPFS_SUCCESS;
}