#include "requests.h"
#include "file_reader.h"

/*

handle_get_request(int client_socket, const char *url)

takes the url and appends "www" at the start and then calls to read and send the file
Basically a go between

*/
void handle_get_request(int client_socket, const char *url) {
	char file_path[256];

	snprintf(file_path, sizeof(file_path), "www%s", url);

	read_and_send_file(client_socket, file_path);
}
