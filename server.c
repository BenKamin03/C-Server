#include "server.h"
#include "file_reader.h"
#include "project05.h"
#include <stdlib.h>
#include <sys/ioctl.h>

#define MAX_CLIENTS 10

/*

make_socket(struct addrinfo *results)

Creates and returns a client socket, tests the socket to ensure:
it binds, doesn't flag (ioctl) and listens

*/
int make_socket(struct addrinfo *results) {
	struct addrinfo *r;
	int fd;

	printf("Creating socket...\n");

	for (r = results; r != NULL; r = r->ai_next) {
		fd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

		printf("Trying socket fd: %d\n", fd);

		if (fd == -1) {
			continue;
		}

		int opt_val = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) {
			perror("Error setting sock opt");
			close(fd);
			continue;
		}

		int flags = 1;
		if (ioctl(fd, FIONBIO, &flags) == -1) {
			perror("Error setting non-blocking option");
			close(fd);
			continue;
		}

		if (bind(fd, r->ai_addr, r->ai_addrlen) == -1) {
			perror("Error binding socket");
			close(fd);
			continue;
		}

		if (listen(fd, SOMAXCONN) == -1) {
			perror("Error listening on socket");
			close(fd);
			continue;
		}

		printf("Successfully bound and listening: %d\n", fd);
		return fd;
	}

	printf("Cound not bind and listen. Exiting...\n");
	exit(EXIT_FAILURE);
}

/*

start_server(int port)

starts the server with the given port

*/
void start_server(int port) {
	struct addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	struct addrinfo *results;

	printf("Creating server with Port ID: %d\n", port);

	char port_id[4];
	(void) sprintf(port_id, "%d", port);

	int e = getaddrinfo("vlab07", port_id, &hints, &results);
	if (e != 0) {
		printf("getaddrinfo: %s\n", gai_strerror(e));
		exit(EXIT_FAILURE);
	}

	int server_fd = make_socket(results);

	freeaddrinfo(results);

	handle_connection(server_fd);
}


/*

handle_connection(int server_fd)

handles the connections using poll

*/
void handle_connection(int server_fd) {
	struct pollfd fds[MAX_CLIENTS + 1];
	int num_clients = 0;

	fds[0].fd = server_fd;
	fds[0].events = POLLIN;

	printf("Waiting for connections...\n");

	while (1) {
		int result = poll(fds, num_clients + 1, -1);

		if (result == -1) {
			perror("Error in poll");
			exit(EXIT_FAILURE);
		}

		if (fds[0].revents & POLLIN) {
			int client_socket = accept(server_fd, NULL, NULL);
			if (client_socket == -1) {
				perror("Error accepting connection");
				continue;
			} 

			fds[num_clients + 1].fd = client_socket;
			fds[num_clients + 1].events = POLLIN;
			num_clients++;

			printf("New connection accepted. Total clients: %d\n", num_clients);
	
		}

		for (int i = 1; i <= num_clients; i++) {
			if (fds[i].revents & POLLIN) {
				char buffer[MAX_BUFFER_SIZE];
				
				ssize_t bytes_recieved = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytes_recieved <= 0) {
					close(fds[i].fd);

					fds[i] = fds[num_clients];
					num_clients--;

					printf("Connection closed. Total clients: %d\n", num_clients);
				} else {
					buffer[bytes_recieved] = '\0';

					char *url = get_requested_url(buffer);

					if (strncmp(buffer, "GET", 3) == 0) {
						handle_get_request(fds[i].fd, url);
					} else {
						fprintf(stderr, "Unsupported UTTP Method\n");
						close(fds[i].fd);
					}

					free(url);
				}
				
			}
		}
	}
}

/*

get_requested_url(const char *request)

gets the url for the requested webpage

*/
char *get_requested_url(const char *request) {
	char *token = strtok((char *)request, " ");
	token = strtok(NULL, " ");
	char *url = strdup(token);

	return url;
}
