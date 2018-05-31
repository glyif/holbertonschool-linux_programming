#ifndef INC_0X01_GETLINE_GETLINE_H
#define INC_0X01_GETLINE_GETLINE_H

#include <stdlib.h>
#include <unistd.h>

#define READ_SIZE 1024
#define BUFSIZE (READ_SIZE * 2)

char *_getline(const int fd);

#endif
