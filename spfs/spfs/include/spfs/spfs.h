#pragma once

#include <Windows.h>

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned int	dword;

typedef struct _SPFS_HEADER {
	byte  Reserved0[3];
	word  Version;
	word  Reserved1;
	byte  VolumeLabel[16];
	word  BytesPerSecond;
	byte  ReservedSectors;
	dword NumSectorsLow;
	dword NumSectorsHigh;
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
	dword			SizeLow;
	dword			SizeHigh;
	dword			LBALow;
	dword			LBAHigh;
} SPFS_FILE_ENTRY;

HANDLE OpenVolume(const char* name, )