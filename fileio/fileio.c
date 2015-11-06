/**
* Different methods for getting file size
* Uses C99 dialect, so compile with -std=gnu99 or -std=c99
* or via IDE-s Visual Studio 2013 (and higher) / DevC++5.7 (and higher)
*/
#include <stdlib.h>    // malloc / free
#include <stdio.h>     // printf / fopen / fread / ...
#include <sys/stat.h>  // fstat
#include <stdint.h>    // uint64_t



// if you don't want to open the file, but need the size
size_t filesize_by_stat(const char* fileName)
{
	struct stat s;
	stat(fileName, &s);
	return s.st_size;
}


// most reliable from personal experience, also the shortest
size_t filesize_by_fstat(FILE* f)
{
	struct stat s;
	fstat(fileno(f), &s);
	return s.st_size;
}


// if you open the file and also want to know the size
size_t filesize_by_ftell(FILE* f)
{
	size_t offset, size;
	offset = ftell(f);
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, offset, SEEK_SET);
	return size;
}



#if _WIN32
#define WIN32_LEAN_AND_MEAN // only include the basic stuff
#include <Windows.h>

// if you only target windows and don't want to open the file
// this is the fastest method to get filesize
size_t filesize_by_win32(const char* fileName)
{
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(fileName, GetFileExInfoStandard, &fad))
		return -1;
	return fad.nFileSizeLow;
}


// if you use windows API to open the file, this will give the filesize extremely fast
size_t filesize_by_win32handle(HANDLE hFile)
{
	return GetFileSize(hFile, NULL);
}

#endif



// something interesting to do with data (FNV64 hash, excellent spread properties)
uint64_t fnv64(void* data, size_t numBytes)
{
	uint64_t hash = 0xcbf29ce484222325ULL;
	for (size_t i = 0; i < numBytes; ++i) {
		hash ^= ((uint8_t*)data)[i];
		hash *= 0x100000001b3;
	}
	return hash;
}


int main(int argc, char** argv)
{
	const char srcFile[] = __FILE__;
	printf("Filesize of %s:\n", srcFile);
	printf("fsize stat  = %d\n", filesize_by_stat(srcFile));
	printf("fsize win32 = %d\n", filesize_by_win32(srcFile));

	FILE* f = fopen(srcFile, "rb");
	if (f)
	{
		printf("fsize ftell = %d\n", filesize_by_ftell(f));
		printf("fsize fstat = %d\n", filesize_by_fstat(f));

		int size = filesize_by_fstat(f);    // get file size right after file open

		char* buffer = malloc(size);        // allocate buffer
		fread(buffer, size, 1, f);          // read all data & update size
		printf("fnv64(\"%s\") = 0x%016llx\n", srcFile, fnv64(buffer, size)); // work with data

		free(buffer);       // free the allocated buffer
		fclose(f);          // close the file
	}


#if _WIN32
	HANDLE hFile = CreateFileA(srcFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD size = GetFileSize(hFile, 0);   // get file size right after open
		printf("fsize hFile = %d\n", size);

		char* buffer = malloc(size);
		ReadFile(hFile, buffer, size, &size, 0); // read via winapi (much faster than fread on win32)
		printf("fnv64(\"%s\") = 0x%016llx\n", srcFile, fnv64(buffer, size)); // work with data

		free(buffer);       // free the allocated buffer
		CloseHandle(hFile); // close win32 file handle
	}
#endif

	system("pause");
	return 0;
}