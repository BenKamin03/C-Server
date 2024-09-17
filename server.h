#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void start_server(int port);
void handle_connection(int client_socket);
char *get_requested_url(const char *request);
void handle_get_request(int client_socket, const char *url);

#endif
