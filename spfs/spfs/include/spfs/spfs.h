#pragma once

#include <Windows.h>

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned int		dword;
typedef unsigned long long	qword;

typedef struct _SPFS_HEADER {
	byte  Reserved0[3];
	word  Version;
	word  Reserved1;
	byte  VolumeLabel[16];
	word  BytesPerSecond;
	byte  ReservedSectors;
	qword NumSectors;
	dword FileTables;
} SPFS_HEADER;

typedef struct _SPFS_DATE_TIME {
	byte minutes : 6;
	byte hours : 5;
	byte dayOfMonth : 5;
	word year : 12;
	byte month : 4;
} SPFS_DATE_TIME;

typedef struct _SPFS_FILE_ENTRY {
	byte			Type;
	byte			Reserved0;
	word			Reserved1;
	byte			Name[40];
	SPFS_DATE_TIME	CreationDate;
	qword			Size;
	qword			LBA;
} SPFS_FILE_ENTRY;

typedef struct _SPFS_VOLUME {
	SPFS_HEADER hdr;

	qword offset = 0;
	qword offsetToAllocationTable = 0;

	HANDLE handle;
} SPFS_VOLUME;

typedef struct _SPFS_FORMAT {
	const char* name;

	dword fileTables;

	byte reservedSectors;
} SPFS_FORMAT;

#define SPFS_VERSION 0x0010

#define SPFS_FILE_TYPE_UNUSED	0x00
#define SPFS_FILE_TYPE_FILE		0x01
#define SPFS_FILE_TYPE_FOLDER	0x02

#define SPFS_SECTOR_FREE		0x00
#define SPFS_SECTOR_USED		0x01
#define SPFS_SECTOR_SYSTEM		(0x02 | SPFS_SECTOR_USED)
#define SPFS_SECTOR_RESERVED	(0x04 | SPFS_SECTOR_USED)

#define SPFS_SUCCESS 0x00

#define SPFS_ERROR_NONE				SPFS_SUCCESS
#define SPFS_ERROR_NO_MEDIA			0x01
#define SPFS_ERROR_INVALID_PARAM	0x02
#define SPFS_ERROR_NOT_A_FILE		0x03
#define SPFS_ERROR_ALREADY_EXIST	0x04

#define SPFS_ATTRIBUTE_CREATE		0x01
#define SPFS_ATTRIBUTE_FILE			0x02
#define SPFS_ATTRIBUTE_FOLDER		0x04
#define SPFS_ATTRIBUTE_OVERWRITE	0x08

#define SPFS_ATTRIBUTE_FILE_OVERWRITE (SPFS_ATTRIBUTE_FILE | SPFS_ATTRIBUTE_OVERWRITE)

HANDLE		OpenDisk(const char* name);
dword		OpenVolume(HANDLE disk, byte partition, SPFS_VOLUME* vol);
dword		FormatVolume(HANDLE disk, byte partiion, SPFS_VOLUME* vol, SPFS_FORMAT* format);
void		CloseVolume(SPFS_VOLUME* vol);
void		CloseDisk(HANDLE handle);

word		VolumeGetFileEntry(SPFS_VOLUME* vol, const char* path, SPFS_FILE_ENTRY* entry);
word		VolumeReadFile(SPFS_VOLUME* vol, const char* path, void** file, qword* size);
word		VolumeReadFolder(SPFS_VOLUME* vol, const char* path, SPFS_FILE_ENTRY** file, qword* num_entries);

word		VolumeWrite(SPFS_VOLUME* vol, const char* path, byte* data, qword size, byte attributes);


void		DiskRead(HANDLE disk, dword offset, dword size, void* data);
void		DiskWrite(HANDLE disk, dword offset, dword size, void* data);