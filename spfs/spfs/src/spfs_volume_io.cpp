#include <spfs/spfs.h>
#include "util.h"

qword GetFirstFreeSector(SPFS_VOLUME* vol) {
	qword table = vol->hdr.AllocationTable;
	qword num = vol->hdr.NumAllocationTables;
	qword size = vol->hdr.BytesPerSector;

	byte* sector = new byte[size];

	for (qword i = 0; i < num; i++) {
		qword offset = (table + i) * size;
		DiskRead(vol->handle, offset, size, sector);

		for (word j = 0; j < size; j++) {
			if (sector[j] & SPFS_SECTOR_USED == 0) {
				return VolumeGetFirstDataSector(vol) + (i * size) + j;
			}
		}
	}

	return (qword)-1;
}

qword GetFirstFileSector(SPFS_VOLUME* vol) {
	qword table = vol->hdr.AllocationTable;
	qword num = vol->hdr.NumAllocationTables;

	qword size = vol->hdr.BytesPerSector;

	byte* sector = new byte[size];

	for (qword i = 0; i < num; i++) {
		qword offset = (table + i) * size;
		DiskRead(vol->handle, offset, size, sector);

		for (word j = 0; j < size; j++) {
			if (sector[j] & SPFS_SECTOR_SYSTEM_FULL == SPFS_SECTOR_SYSTEM) {
				return VolumeGetFirstDataSector(vol) + (i * size) + j;
			}

			if (sector[j] & SPFS_SECTOR_USED == 0) {
				return VolumeGetFirstDataSector(vol) + (i * size) + j;
			}
		}
	}

}

word VolumeGetFileEntry(SPFS_VOLUME* vol, const char* path, SPFS_FILE_ENTRY* entry) {
	if (!vol || !path || !entry) return SPFS_ERROR_INVALID_PARAM;

	byte* sector = new byte[vol->hdr.BytesPerSector];

	qword tablesPerSector = vol->hdr.BytesPerSector / sizeof(SPFS_FILE_ENTRY);

	char** paths;
	dword numPaths = 0;

	SplitPath(path, &paths, &numPaths);

}

word VolumeReadFolder(SPFS_VOLUME* vol, const char* path, SPFS_FILE_ENTRY** entries, qword* num_entries) {

	dword numPaths = 0;
	char** paths;

	SplitPath(path, &paths, &numPaths);


}

word VolumeWrite(SPFS_VOLUME* vol, const char* path, const byte* data, qword size, byte attributes) {
	if (!vol) return SPFS_ERROR_INVALID_PARAM;
	if (!path) return SPFS_ERROR_INVALID_PARAM;

	qword firstDataSector = VolumeGetFirstDataSector(vol);
	qword allocationTable = vol->hdr.AllocationTable;
	word  bytesPerSector = vol->hdr.BytesPerSector;



}