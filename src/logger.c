#include "logger.h"
#include <string.h>
#include <stdlib.h>

int init_logger(Logger* logger, const char* file_path) {
	FILE* file;
	file = fopen(file_path,"a");
	if(file == NULL) {
		return 1;
	}
	logger->file = file;
	return 0;
}

int close_logger(Logger* logger) {
	if(logger->file == NULL) return 0;
	return fclose(logger->file);
}

int log_message (Logger* logger, const char* message) {

	if(logger->file == NULL) return 1;
	
	char* _message = malloc(sizeof(char)*(strlen(message)*2));
	sprintf(_message,"MESSAGE: %s\n", message);
	fwrite(_message,sizeof(char),strlen(_message),logger->file);
	free(_message);
	return 0;
}

int log_error   (Logger* logger, const char* message) {
	return 0;
}

