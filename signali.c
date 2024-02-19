#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <signal.h>

#define osErrorFatal(userMsg) osErrorFatalImpl((userMsg), __FILE__, __func__, __LINE__)
#define osAssert(expr, userMsg) \
	do {\
		if (!(expr)) \
			osErrorFatal(userMsg); \
	} while (0)

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber);

bool osShouldTerminate = false;

void osHandleSIGINT(int signum);
void osHandleSIGTERM(int signum);
void HandleSig(int signum);

int main(int argc, char** argv) {
	osAssert(signal(SIGINT, HandleSig) != SIG_ERR, "SIGINT handler setup failed");
	osAssert(signal(SIGTERM, HandleSig) != SIG_ERR, "SIGTERM handler setup failed");

	printf("PID is: %jd\n", (intmax_t)getpid());

	do {

		printf("waiting fo signal\n");

		printf("signal caught\n");

	} while (!osShouldTerminate);

	exit(EXIT_SUCCESS);
}

void HandleSig(int signum) {

	switch (signum) {

	case SIGINT:
		printf("SIGINT caught\n");
		break;
	case SIGTERM:
		printf("SIGTERM caught\n");
		osShouldTerminate = true;
		break;
	default:
		break;
	}
}

void osHandleSIGINT(int signum) {

	printf("SIGINT caught\n");
}

void osHandleSIGTERM(int signum) {

	printf("SIGTERM caught\n");
	osShouldTerminate = true;
}

void osErrorFatalImpl(const char* userMsg, const char* fileName, const char* functionName, const int lineNumber) {

	perror(userMsg);

	fprintf(stderr, "File name: %s\nFunction name: %s\nLine number: %d\n", fileName, functionName, lineNumber);

	exit(EXIT_FAILURE);
}