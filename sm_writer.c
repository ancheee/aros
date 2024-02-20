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

static char* osUsage = "./shared_memory_writer pathToFile num0 num1 num2 ...";

void* osCreateMemoryBlock(const char* filePath, unsigned size);

int main(int argc, char** argv) {

	osAssert(argc >= 3, osUsage);

	int n = argc - 2;
	int size = (argc - 2) * sizeof(int);
	int* niz = osCreateMemoryBlock(argv[1], size);
	int i;
	for (i = 0; i < n; i++)
		niz[i] = atoi(argv[i + 2]);

	osAssert(munmap(niz, size) != -1, "unmap failed");

	exit(EXIT_SUCCESS);
}

void* osCreateMemoryBlock(const char* filePath, unsigned size) {

	int memFd = shm_open(filePath, O_RDWR | O_CREAT, 0600);
	osAssert(memFd != -1, "shm_open failed");

	osAssert(ftruncate(memFd, size) != -1, "ftruncate failed");

	void* addr;

	osAssert((addr = mmap(0, size, PROT_READ | PROT_WRITE, \
		MAP_SHARED, memFd, 0)) != MAP_FAILED, "mmap failed");

	close(memFd);

	return addr;
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}
