#include "file_reader.h"

/*

get_file_size(FILE *stream)

gets the size of the file by seeking EOF and then rewinding

*/
long get_file_size(FILE *stream) {
	(void) fseek(stream, 0L, SEEK_END);
	long size = ftell(stream);
	rewind(stream);
	return size;
}


/*

read_and_send_file(int client_socket, const char *file_path)

Reads the file from the path and sends it to the client

*/
void read_and_send_file(int client_socket, const char *file_path) {
	FILE *file = fopen(file_path, "rb");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}

	long file_size = get_file_size(file);

	char *file_buffer = (char *)malloc(file_size);
	if (file_buffer == NULL) {
		perror("Error allocating memory for file buffer");
		fclose(file);
		return;
	}

	fread(file_buffer, 1, file_size, file);

	const char *content_type = get_content_type(file_path);

	dprintf(client_socket, "HTTP/1.1 200 OK\r\n");
	dprintf(client_socket, "Content-Length: %ld\r\n", file_size);
	dprintf(client_socket, "Content-Type: %s\r\n", content_type);
	dprintf(client_socket, "\r\n");

	send(client_socket, file_buffer, file_size, 0);

	fclose(file);
	free(file_buffer);
}


/*

get_content_type(const char *file_path)

gets the file type for use in the Content-Type in the send

*/
const char *get_content_type(const char *file_path) {
	const char *extension = strchr(file_path, '.');
	if (extension == NULL) {
		return "text/html";
	}

	if (strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0) {
		return "text/html";
	} else if (strcmp(extension, ".css") == 0) {
		return "text/css";
	} else if (strcmp(extension, ".png") == 0) {
		return "image/png";
	} else if (strcmp(extension, ".ico") == 0) {
		return "image/x-icon";
	} else {
		return "text/html";
	}
}
