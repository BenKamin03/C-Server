#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

long get_file_size(FILE *stream);
void read_and_send_file(int client_socket, const char *file_path);
const char *get_content_type(const char *file_path);

#endif
