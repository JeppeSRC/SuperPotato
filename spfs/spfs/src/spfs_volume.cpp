#include <spfs/spfs.h>

#include "disk.h"

inline void FreePaths(char** paths, dword num) {
	for (dword i = 0; i < num; i++) {
		delete[] paths[i];
	}

	delete[] paths;
}

inline size_t CountSlash(const char* path) {
	size_t len = strlen(path);
	size_t res = 0;
	for (size_t i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') res++;
	}

	return res;
}

inline void SplitPath(const char* path, char*** paths, dword* num_paths) {
	word index = 0;

	*num_paths = CountSlash(path)-1;

	*paths = new char*[*num_paths];


	char** dir = *paths;

	size_t len = strlen(path);
	size_t last = 1;

	for (size_t i = 1; i < len; i++) {
		const char c = path[i];
		if (c == '/' || c == '\\') {
			size_t strlen = i - last;
			dir[index] = new char[strlen];
			memcpy(dir[index], path + last, strlen);
			last = i + 1;
			index++;
		}
	}

	size_t strlen = len - last;
	dir[index] = new char[strlen];
	memcpy(dir[index], path + last, strlen);
}

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

	size_t len = strlen(format->name);

	if (len > 16) return SPFS_ERROR_INVALID_PARAM;

	memset(vol->hdr.VolumeLabel, 0, 16);
	memcpy(vol->hdr.VolumeLabel, format->name, len);

	qword totalSectors = 0;

	GetDiskInfo(disk, &vol->hdr.BytesPerSector, &totalSectors);

	CalculateAllocationTable(totalSectors, vol->hdr.BytesPerSector, &vol->hdr.NumDataSectors, &vol->hdr.NumAllocationTables);

	vol->hdr.AllocationTable = vol->hdr.ReservedSectors + offset;

	DiskWrite(disk, offset * vol->hdr.BytesPerSector, sizeof(SPFS_HEADER), &vol->hdr);

}

void CloseVolume(SPFS_VOLUME* vol) {
	CloseHandle(vol->handle);
	vol->handle = INVALID_HANDLE_VALUE;
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

