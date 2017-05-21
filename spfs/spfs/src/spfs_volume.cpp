#include <spfs/spfs.h>

#include "disk.h"

void CalculateAllocationTable(qword sectors, qword size, qword* usable, qword* tracking) {
	*tracking = sectors / size;

	qword notNeeded = *tracking / size;

	if (notNeeded) notNeeded--;

	*tracking -= notNeeded;
	*usable = sectors - *tracking;
}

dword OpenVolume(HANDLE disk, byte partition, SPFS_VOLUME* vol) {
	if (!vol) return SPFS_ERROR_INVALID_PARAM;
	if (disk == INVALID_HANDLE_VALUE) return SPFS_ERROR_NO_MEDIA;

	vol->handle = disk;
	vol->offset = 0;

	DiskRead(disk, 0, sizeof(SPFS_HEADER), &vol->hdr);

	if (vol->hdr.Version != SPFS_VERSION) return SPFS_ERROR_NO_MEDIA;

	vol->offsetToAllocationTable = vol->hdr.FileTables + vol->hdr.ReservedSectors + vol->offset + 1;

	return SPFS_SUCCESS;
}

dword FormatVolume(HANDLE disk, byte partition, SPFS_VOLUME* vol, SPFS_FORMAT* format) {
	if (!vol) return SPFS_ERROR_INVALID_PARAM;
	if (disk == INVALID_HANDLE_VALUE) return SPFS_ERROR_NO_MEDIA;

	vol->handle = disk;

	vol->hdr.Version = SPFS_VERSION;
	vol->hdr.ReservedSectors = format->reservedSectors;
	vol->hdr.FileTables = format->fileTables;

	size_t len = strlen(format->name);

	if (len > 16) return SPFS_ERROR_INVALID_PARAM;

	memset(vol->hdr.VolumeLabel, 0, 16);
	memcpy(vol->hdr.VolumeLabel, format->name, len);

	GetDiskInfo(disk, &vol->hdr.BytesPerSecond, &vol->hdr.NumSectors);
	

}