#pragma once
#include <spfs/spfs.h>

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
