#ifndef _LOGGER_
#define _LOGGER_

#include <stdio.h>

typedef struct {
	FILE* file;
} Logger;

/***
		Expected user to manage memory
 */
int init_logger(Logger* logger, const char* filepath);
int close_logger(Logger* logger);

int log_message (Logger* logger, const char* message);
int log_error   (Logger* logger, const char* message);

#endif
