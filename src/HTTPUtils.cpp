#include "../include/HTTPUtils.h"

HTTPUtils::HTTPUtils(int port,std::string ip){
    this->port = port;
    this->ip = ip;
	this->sock = 0;
	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
	}

}

void HTTPUtils::SendMessage(std::string Message){
    struct sockaddr_in address;
	struct sockaddr_in serv_addr;
	const char* message = Message.c_str();
	char buffer[1024] = { 0 };
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form 
	if (inet_pton(AF_INET, this->ip.c_str(), &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return;
	}

	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return;
	}

	printf("Client: Sending Hello\n");
	send(sock, message, strlen(message), 0);
	return;
}

std::string HTTPUtils::ReceiveMessage(){
	char buffer[1024] = { 0 };
	int valread;
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
}