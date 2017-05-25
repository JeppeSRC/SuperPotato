#include <spfs/spfs.h>

#include "disk.h"
#include "util.h"

dword OpenVolume(HANDLE disk, byte partition, SPFS_VOLUME* vol) {
	if (!vol) return SPFS_ERROR_INVALID_PARAM;
	if (disk == INVALID_HANDLE_VALUE) return SPFS_ERROR_NO_MEDIA;

	vol->handle = disk;

	DiskRead(disk, 0, sizeof(SPFS_HEADER), &vol->hdr);

	if (vol->hdr.Version != SPFS_VERSION) return SPFS_ERROR_NO_MEDIA;

	

	return SPFS_SUCCESS;
}

dword FormatVolume(HANDLE disk, byte partition, SPFS_VOLUME* vol, SPFS_FORMAT* format) {
	if (!vol) return SPFS_ERROR_INVALID_PARAM;
	if (disk == INVALID_HANDLE_VALUE) return SPFS_ERROR_NO_MEDIA;

	qword offset = 0;//Unused for the moment

	vol->handle = disk;

	vol->hdr.Version = SPFS_VERSION;
	vol->hdr.ReservedSectors = format->reservedSectors;

	size_t len = strlen(format->name);

	if (len > 16) return SPFS_ERROR_INVALID_PARAM;

	memset(vol->hdr.VolumeLabel, 0, 16);
	memcpy(vol->hdr.VolumeLabel, format->name, len);

	qword totalSectors = 0;

	GetDiskInfo(disk, &vol->hdr.BytesPerSector, &totalSectors);

	CalculateAllocationTable(totalSectors, vol->hdr.BytesPerSector, &vol->hdr.NumDataSectors, &vol->hdr.NumAllocationTables);

	vol->hdr.AllocationTable = vol->hdr.ReservedSectors + offset;

	DiskWrite(disk, offset * vol->hdr.BytesPerSector, sizeof(SPFS_HEADER), &vol->hdr);

	qword size = vol->hdr.BytesPerSector;

	byte* tmpData = new byte[size];
	memset(tmpData, 0, size);

	for (qword i = 0; i < vol->hdr.NumAllocationTables; i++) {
		qword offset = (vol->hdr.AllocationTable + i) * size;
		DiskWrite(disk, offset, size, tmpData);
	}
}

void CloseVolume(SPFS_VOLUME* vol) {
	CloseHandle(vol->handle);
	vol->handle = INVALID_HANDLE_VALUE;
}


qword VolumeGetTotalSectors(SPFS_VOLUME* vol) {
	return vol->hdr.NumAllocationTables + vol->hdr.NumDataSectors + vol->hdr.ReservedSectors;
}

qword VolumeGetFirstDataSector(SPFS_VOLUME* vol) {
	return vol->hdr.AllocationTable + vol->hdr.NumAllocationTables;
}