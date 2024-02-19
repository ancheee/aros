#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <errno.h>

#define osErrorFatal(userMsg) osErrorFatalImpl((userMsg), __FILE__, __func__, __LINE__)
#define osAssert(expr, userMsg) \
	do {\
		if (!(expr)) \
			osErrorFatal(userMsg); \
	} while (0)

#define MAX_SIZE (20)	

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber);

static char* osUsage = "./fifo_server pathToFifo";

int main(int argc, char** argv) {

	osAssert(argc == 2, osUsage);

	int result = mkfifo(argv[1], 0600);
	if (result == -1) {

		osAssert(errno == EEXIST, "mkfifo failed");

		struct stat fInfo;
		osAssert(stat(argv[1], &fInfo) != -1, "stat failed");
		osAssert(S_ISFIFO(fInfo.st_mode), "Not a fifo.");
	}

	int fd = open(argv[1], O_RDONLY);
	osAssert(fd != -1, "open failed");

	char buf[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(fd, buf, MAX_SIZE)) > 0) {

		int broj;
		sscanf(buf, "%d", &broj);

		printf("Received number: '%d'\n", broj);
	}

	osAssert(readBytes != -1, "read failed");

	close(fd);

	exit(EXIT_SUCCESS);
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}