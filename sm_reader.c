#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/mman.h>

#define osErrorFatal(userMsg) osErrorFatalImpl((userMsg), __FILE__, __func__, __LINE__)
#define osAssert(expr, userMsg) \
	do {\
		if (!(expr)) \
			osErrorFatal(userMsg); \
	} while (0)

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber);

static char* osUsage = "./shared memory reader filePath";

void* osGetMemoryBlock(const char* filePath, unsigned* size);

int main(int argc, char** argv) {

	osAssert(argc == 2, osUsage);

	unsigned size = 0;
	int* niz = osGetMemoryBlock(argv[1], &size);
	int brojEl = size / sizeof(int);

	int i = 0;
	for (i = 0; i < brojEl; i++)
		printf("%d ", niz[i]);

	printf("\n");

	osAssert(munmap(niz, size) != -1, "munmap failed");

	osAssert(shm_unlink(argv[1]) != -1, "shm_unlink failed");

	exit(EXIT_SUCCESS);
}

void* osGetMemoryBlock(const char* filePath, unsigned* size) {

	int memFd = shm_open(filePath, O_RDONLY, 0);
	osAssert(memFd != -1, "shm_open failed");

	struct stat fInfo;
	osAssert(fstat(memFd, &fInfo) != -1, "fstat failed");
	*size = fInfo.st_size;

	void* addr;

	osAssert((addr = mmap(0, *size, PROT_READ, MAP_SHARED, memFd, 0)) != MAP_FAILED, "mmap failed");

	close(memFd);

	return addr;
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}