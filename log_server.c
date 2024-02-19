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
#define PROC_SEM (!LOCAL_SEM)	

typedef struct {

	char buf[MSG_LEN];
	sem_t safeToRead;
	sem_t safeToWrite;
} osMemoryBlock;

static char* osUsage = "./log_server pathFile";

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber);

void* osCreateMemoryBlock(const char* filePath, unsigned size);

int main(int argc, char** argv) {

	osAssert(argc == 2, osUsage);

	osMemoryBlock* pMsgBuf = osCreateMemoryBlock(argv[1], sizeof(osMemoryBlock));

	osAssert(sem_init(&pMsgBuf->safeToRead, PROC_SEM, 0) != -1, "read sem init failed");
	osAssert(sem_init(&pMsgBuf->safeToWrite, PROC_SEM, 1) != -1, "write sem init failed");

	char localBuf[MSG_LEN];

	do {
	
		scanf("%s", localBuf);

		osAssert(sem_wait(&pMsgBuf->safeToWrite) != -1, "sem wait failed");

		strcpy(pMsgBuf->buf, localBuf);

		osAssert(sem_post(&pMsgBuf->safeToRead) != -1, "sem post failed");

	} while (strcasecmp(localBuf, "quit"));

	osAssert(munmap(pMsgBuf, sizeof(osMemoryBlock)) != -1, "munmap failed");

	osAssert(shm_unlink(argv[1]) != -1, "shm unlink failed");

	exit(EXIT_SUCCESS);
}

void* osCreateMemoryBlock(const char* filePath, unsigned size) {

	int memFd = shm_open(filePath, O_RDWR | O_CREAT, 0600);
	osAssert(memFd != -1, "shm_open failed");

	osAssert(ftruncate(memFd, size) != -1, "ftruncate failed");

	void* addr;
	osAssert((addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0)) != MAP_FAILED, "mmap failed");

	close(memFd);

	return addr;
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}