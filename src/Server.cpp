#include "../include/Server.h"


Server::Server(int port){
	this->port = port;
	this->opt = 1;
	this->addrlen = sizeof(address);

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

    int i = 0;

	bool keeprunning = true;
	while(keeprunning){

		if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
			(socklen_t*)& addrlen)) < 0) //Blocked until connect is called in the client
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		printf("Server accepted connection, reading message\n");
		
		HandleRequest(&new_socket);
		std::string input;
		std::cin >> input;
		if(input  ==  "QUIT"){
			keeprunning = false;
		}
	}	
}

void* Server::HandleRequest(void *arg){

	int new_socket = *((int *)arg);
	std::ostringstream bufferStream;
	char buffer;

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	while(recv(new_socket, &buffer, 1, 0) > 0){
		bufferStream << buffer;
	}

	std::cout << "Received Request" << " - Thread ID: " << std::this_thread::get_id() << std::endl;
	std::string request = bufferStream.str();
	std::cout << request << std::endl;

	HTTPUtils* http = new HTTPUtils(8080,"127.0.0.1");
	HTTPRequest HTTPresponse = http->ParseResponse(request);
	if(HTTPresponse.method == ""){
		std::cout << "Empty Request" << std::endl;
		close(new_socket);
		return &buffer;
	}else if(HTTPresponse.method == "CONNECT"){
		std::cout << "Discarding HTTPS Request" << std::endl;
	}else{
		std::cout << "Made Request" << " - Host: " << http->CleanURL(HTTPresponse.host) << std::endl;
		std::string response = http->MakeRequest(http->CleanURL(HTTPresponse.host),HTTPresponse.GetCleanedRequest());
		if(response == ""){
			std::cout << "Got No Response" << " - Host: " << HTTPresponse.host << std::endl;
		}else{
			std::cout << "Got Response" << " - Host: " << HTTPresponse.host << std::endl << response << std::endl;
			send(new_socket, response.c_str(), strlen(response.c_str()), 0);
		}
	}

	close(new_socket);
		
}

