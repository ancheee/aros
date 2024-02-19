#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <strings.h>

#define osErrorFatal(userMsg) osErrorFatalImpl((userMsg), __FILE__, __func__, __LINE__)
#define osAssert(expr, userMsg) \
	do {\
		if (!(expr)) \
			osErrorFatal(userMsg); \
	} while (0)


#define MSG_LEN (256)
#define LOCAL_SEM (0)
#define PROC_SEM (!(LOCAL_SEM))

typedef struct {

	char buf[MSG_LEN];
	sem_t safeToRead;
	sem_t safeToWrite;

} osMemoryBlock;

static char* osUsage = "./log_client pathFile";

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber);

void* osGetMemoryBlock(const char* filePath, unsigned* size);

int main(int argc, char** argv) {

	osAssert(argc == 2, osUsage);

	unsigned size;
	osMemoryBlock* pMsgBuf = osGetMemoryBlock(argv[1], &size);

	char localBuf[MSG_LEN];
	do {

		osAssert(sem_wait(&pMsgBuf->safeToRead) != -1, "sem wait failed");

		strcpy(localBuf, pMsgBuf->buf);

		osAssert(sem_post(&pMsgBuf->safeToWrite) != -1, "sem post failed");

		printf("Received message: '%s'\n", localBuf);

	} while (strcasecmp(localBuf, "quit"));

	osAssert(munmap(pMsgBuf, size) != -1, "munmap failed");

	exit(EXIT_SUCCESS);
}

void* osGetMemoryBlock(const char* filePath, unsigned* size) {

	int memFd = shm_open(filePath, O_RDWR, 0);
	osAssert(memFd != -1, "shm open failed");

	struct stat fInfo;
	osAssert(fstat(memFd, &fInfo) != -1, "stat failed");
	*size = fInfo.st_size;

	void* addr;
	osAssert((addr = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0)) != MAP_FAILED, "mmap failed");

	close(memFd);

	return addr;
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}
