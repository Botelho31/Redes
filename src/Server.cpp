#include "../include/Server.h"

char* Server::ListenFor(int port){
	int server_fd, new_socket, new_socket2, valread;
	struct sockaddr_in address;
	struct timeval tv;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[64000] = { 0 };
	fd_set rfds; 

	tv.tv_sec = 0.1;
	tv.tv_usec = 0;
	

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Set the socket options
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// Forcefully attaching socket to the selected port 
	if (bind(server_fd, (struct sockaddr*) & address,
		sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
		(socklen_t*)& addrlen)) < 0) //Blocked until connect is called in the client
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	printf("Server accepted connection, reading message\n");


	
	size_t size = sizeof(buffer);
	size_t total = 0, n = 0;

	while((n = recv(new_socket, buffer+total, size-total-1, 0)) > 0) {
		total += n;
		std::cout << n << std::endl;
		int flag = select(1, &rfds, NULL, NULL, &tv);
		if (flag == 0 || flag == 1){
			std::cout << "Timeout" << std::endl;
			break;
		}
	}

	std::cout << "Finished Reading" << std::endl;
	std::cout << std::endl;
	buffer[total] = 0;
	char* test = buffer;

	HTTPUtils* http = new HTTPUtils(port,"127.0.0.1");
	std::stringstream request;
	request << test;
	std::cout << test << std::endl;
	std::cout << std::endl;
	char* response = http->MakeRequest("www.google.com",request.str());
	std::stringstream responseToClient;
	responseToClient << response;
	std::cout << response << std::endl;
	std::cout << std::endl;
	send(new_socket, responseToClient.str().c_str(), strlen(responseToClient.str().c_str()), 0);

	return test;
}