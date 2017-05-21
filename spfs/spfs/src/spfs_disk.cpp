#include <spfs/spfs.h>
#include <stdio.h>

void SetOffset(HANDLE disk, qword offset) {
	dword high = (offset >> 32) & 0xFFFFFFFF;
	SetFilePointer(disk, offset & 0xFFFFFFFF, (long*)&high, FILE_BEGIN);
}

HANDLE OpenDisk(const char* name) {
	HANDLE handle;

	handle = CreateFile(name, GENERIC_ALL, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (handle == 0) {
		printf("Failed to open disk \"%s\"\n", name);
		return handle;
	}

	return handle;
}

void DiskRead(HANDLE disk, dword offset, dword size, void* data) {
	SetOffset(disk, offset);
	ReadFile(disk, data, size, 0, 0);
}

void DiskWrite(HANDLE disk, dword offset, dword size, void* data) {
	SetOffset(disk, offset);
	WriteFile(disk, data, size, 0, 0);
}

void CloseDisk(HANDLE handle) {
	CloseHandle(handle);
}