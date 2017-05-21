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
	vol->hdr.NumFileTables = format->fileTables;

	size_t len = strlen(format->name);

	if (len > 16) return SPFS_ERROR_INVALID_PARAM;

	memset(vol->hdr.VolumeLabel, 0, 16);
	memcpy(vol->hdr.VolumeLabel, format->name, len);

	qword totalSectors = 0;

	GetDiskInfo(disk, &vol->hdr.BytesPerSector, &totalSectors);
	
	if (vol->hdr.NumFileTables == 0) {
		//TODO:
		return SPFS_ERROR_INVALID_PARAM;
	}

	DiskWrite(disk, offset * vol->hdr.BytesPerSector, sizeof(SPFS_HEADER), &vol->hdr);

	
	vol->hdr.FileTable = vol->hdr.ReservedSectors + offset;
	vol->hdr.AllocationTable = vol->hdr.FileTable + vol->hdr.NumFileTables;

	CalculateAllocationTable(totalSectors - vol->hdr.NumFileTables - vol->hdr.ReservedSectors, vol->hdr.BytesPerSector, &vol->hdr.NumDataSectors, &vol->hdr.NumAllocationTables);

	vol->hdr.DataSector = vol->hdr.AllocationTable + vol->hdr.NumAllocationTables;

	byte* data = new byte[vol->hdr.BytesPerSector];
	memset(data, 0, vol->hdr.BytesPerSector);
	
	for (qword i = 0; i < vol->hdr.NumFileTables + vol->hdr.NumAllocationTables; i++) {
		qword sector = vol->hdr.FileTable + i;
		DiskWrite(disk, sector * vol->hdr.BytesPerSector, vol->hdr.BytesPerSector, data);
	}


}

void CloseVolume(SPFS_VOLUME* vol) {
	CloseHandle(vol->handle);
	vol->handle = INVALID_HANDLE_VALUE;
}

word VolumeGetFileEntry(SPFS_VOLUME* vol, const char* path, SPFS_FILE_ENTRY* entry) {

	byte* entries = new byte[vol->hdr.BytesPerSector];

	qword tablesPerSector = vol->hdr.BytesPerSector / sizeof(SPFS_FILE_ENTRY);

	for (qword sector = 0; sector < vol->hdr.NumFileTables; sector++) {
		DiskRead(vol->handle, (sector + vol->hdr.FileTable) * vol->hdr.BytesPerSector, vol->hdr.BytesPerSector, entries);

		for (qword i = 0; i < tablesPerSector; i++) {
			SPFS_FILE_ENTRY* e = ((SPFS_FILE_ENTRY*)entries) + i;


		}
	}
}