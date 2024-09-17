#include "project05.h"
#include "server.h"

/*

read_port_from_file()

in the case that the port is provided by the command line arguments, it reads it directly from port.txt

*/
char *read_port_from_file() {
	int fd = open(PORT_FILE, O_RDONLY);
	if (fd == -1) {
		printf("Error opening file from %s. Exiting...\n", PORT_FILE);
		exit(EXIT_FAILURE);
	}

	char *port = malloc(5);
	if (port == NULL) {
		printf("Reading from port file memory wasn't allocated\n");
		exit(EXIT_FAILURE);
	}
	ssize_t bytes_read = read(fd, port, 5);

	if (bytes_read == -1) {
		printf("Error opening file from %s. Exiting...\n", PORT_FILE);
		exit(EXIT_FAILURE);
	}

	port[4] = '\0';

	close(fd);

	return port;
}

/*

main(int argc, char **argv)

gets the port and starts the server

*/
int main(int argc, char **argv) {

	char *port_id = "-1";

	for (int i = 1; i < argc - 1; i++) {
		if (strncmp(argv[i], "-p", 2) == 0) {
			port_id = argv[++i];
		}
	}

	if (strncmp(port_id, "-1", 2) == 0) {
		printf("Port ID not supplied. Reading from %s\n.", PORT_FILE);
		port_id = read_port_from_file();
	}

	start_server(atoi(port_id));
	return 0;
	
}
