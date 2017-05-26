#include <spfs/spfs.h>
#include "util.h"

qword GetFirstFreeSector(SPFS_VOLUME* vol) {
	qword table = VolumeGetFirstAllocationTable(vol);
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

	return SPFS_ERROR;
}

byte GetFirstFreeFileEntry(SPFS_VOLUME* vol, SPFS_FILE_ENTRY* parent, byte* data) {
	qword sector = VolumeGetFirstDataSector(vol);
	qword size = vol->hdr.BytesPerSector;
	byte  entriesPerSector = (size - 8) / sizeof(SPFS_FILE_ENTRY);

	if (parent) {

	} else {
		while(true) {
			DiskRead(vol->handle, sector * size, size, data);

			for (byte i = 0; i < entriesPerSector; i++) {
				SPFS_FILE_ENTRY* e = (SPFS_FILE_ENTRY*)data + i;

				if (e->Type == SPFS_FILE_TYPE_UNUSED) return 0;
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
	if (!attributes) return SPFS_ERROR_INVALID_PARAM;

	qword firstDataSector = VolumeGetFirstDataSector(vol);
	qword allocationTable = VolumeGetFirstAllocationTable(vol);
	word  bytesPerSector = vol->hdr.BytesPerSector;



}