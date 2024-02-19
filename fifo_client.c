#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <time.h>
#include <strings.h>

#define osErrorFatal(userMsg) osErrorFatalImpl((userMsg), __FILE__, __func__, __LINE__)
#define osAssert(expr, userMsg) \
	do {\
		if (!(expr)) \
			osErrorFatal(userMsg); \
	} while (0)

#define MAX_SIZE (20)

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber);

static char* osUsage = "./fifo_client pathToFifo";

int main(int argc, char** argv) {

	osAssert(argc == 2, osUsage);

	int fd = open(argv[1], O_WRONLY);
	osAssert(fd != -1, "open failed");

	srand(time(NULL));
	char localBuf[MAX_SIZE];

	do {
		int broj = rand();
		sprintf(localBuf, "%d", broj);
		printf("Sent number: %d\n", broj);

		osAssert(write(fd, localBuf, MAX_SIZE) != -1, "write failed");
		printf("Continue sending: \n");
		scanf("%s", localBuf);
	} while (!strcasecmp("yes", localBuf));

	close(fd);

	exit(EXIT_SUCCESS);
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}