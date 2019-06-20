#include "../include/Server.h"

// int main(int argc, char const* argv[])
// {
// 	int server_fd, new_socket, new_socket2, valread;
// 	struct sockaddr_in address;
// 	int opt = 1;
// 	int addrlen = sizeof(address);
// 	char buffer[1024] = { 0 };
// 	char* hello = "Hello from server";

// 	// Creating socket file descriptor 
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("socket failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Set the socket options
// 	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
// 		&opt, sizeof(opt)))
// 	{
// 		perror("setsockopt");
// 		exit(EXIT_FAILURE);
// 	}
// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(PORT);

// 	// Forcefully attaching socket to the selected port 
// 	if (bind(server_fd, (struct sockaddr*) & address,
// 		sizeof(address)) < 0)
// 	{
// 		perror("bind failed");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (listen(server_fd, 3) < 0)
// 	{
// 		perror("listen");
// 		exit(EXIT_FAILURE);
// 	}
// 	if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
// 		(socklen_t*)& addrlen)) < 0) //Blocked until connect is called in the client
// 	{
// 		perror("accept");
// 		exit(EXIT_FAILURE);
// 	}

// 	printf("Server accepted connection, reading message\n");
// 	valread = read(new_socket, buffer, 1024); //Blocked until message is sent from the client
// 	printf("%s\n", buffer);
// 	printf("Server: Sending Hello\n");
// 	send(new_socket, hello, strlen(hello), 0);
// 	return 0;
// }