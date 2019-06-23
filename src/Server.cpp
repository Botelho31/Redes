#include "../include/Server.h"


Server::Server(int port){
	this->port = port;
	this->opt = 1;
	this->addrlen = sizeof(address);

	this->tv.tv_sec = 1;
	this->tv.tv_usec = 0;
	

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// Creating socket file descriptor 
	if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Set the socket options
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
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
}

Server::~Server(){
	close(server_fd);
}

void Server::ListenFor(){

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while(1){
		char buffer[64000] = { 0 };
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

		HTTPUtils* http = new HTTPUtils(8080,"127.0.0.1");
		std::stringstream request;
		request << test;
		std::cout << test << std::endl;
		std::cout << std::endl;
		// std::cout << http->RemovePort(http->ParseResponse(test)["Host"][0]) << std::endl;
		if(http->ParseResponse(test)["ResponseHeader"][0] == "CONNECT"){
			std::cout << "Discarding HTTPS request" << std::endl;
		}else{
			char* response = http->MakeRequest(http->RemovePort(http->ParseResponse(test)["Host"][0]),request.str());
			std::stringstream responseToClient;
			// responseToClient << "HTTP/1.1 200 OK\r\n\r\n <html><body>OI MEU NOME E GOKU<body></html>";
			responseToClient << response;
			std::cout << response << std::endl;
			std::cout << std::endl;
			std::cout << "OI " << std::endl;
			send(new_socket, responseToClient.str().c_str(), strlen(responseToClient.str().c_str()), 0);
			if(http->ParseResponse(test)["Connection"][0] == "keep-alive"){
				std::cout << "KEEPALIVE" << std::endl;
				KeepAlive(new_socket);
			}
		}

		close(new_socket);
	}
	
}

void Server::KeepAlive(int new_socket){

	while(1){
		char buffer[64000] = { 0 };
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

		HTTPUtils* http = new HTTPUtils(8080,"127.0.0.1");
		std::stringstream request;
		request << test;
		std::cout << test << std::endl;
		std::cout << std::endl;
		// std::cout << http->RemovePort(http->ParseResponse(test)["Host"][0]) << std::endl;
		if(http->ParseResponse(test)["ResponseHeader"][0] == "CONNECT"){
			std::cout << "Discarding HTTPS request" << std::endl;
		}else{
			// if(http->ParseResponse(test)["Error"][0] == "False"){
				for(auto it = http->ParseResponse(test).cbegin(); it != http->ParseResponse(test).cend(); ++it){
					std::cout << it->first << ": ";
					for(int i = 0;i < it->second.size();i++){
						std::cout << it->second[i] << " ";
					}
					std::cout << std::endl;
				}
				char* response = http->MakeRequest(http->RemovePort(http->ParseResponse(test)["Host"][0]),request.str());
				std::stringstream responseToClient;
				// responseToClient << "HTTP/1.1 200 OK\r\n\r\n <html><body>OI MEU NOME E GOKU<body></html>";
				responseToClient << response;
				// std::cout << response << std::endl;
				// std::cout << std::endl;
				// std::cout << "OI " << std::endl;
				send(new_socket, responseToClient.str().c_str(), strlen(responseToClient.str().c_str()), 0);
			// }else{
			// 	std::cout << "Error Parsing Response" << std::endl;
			// }
		}
	}
		
}