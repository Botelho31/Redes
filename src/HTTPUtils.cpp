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

	if (connect(this->sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
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
	return buffer;
}

void HTTPUtils::MakeRequest(std::string address,std::string request){
	struct addrinfo hints , *results;
	int rv;
	int sockfd;
	char inputVal[address.size() + 1];
	strcpy(inputVal, address.c_str());
	char buffer[64000];
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if( (rv = getaddrinfo(inputVal,"80",&hints,&results)) != 0){
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(rv));
	}

    sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    connect(sockfd, results->ai_addr, results->ai_addrlen);
    send(sockfd, request.c_str(), strlen(request.c_str()), 0);

	size_t size = sizeof(buffer);
	size_t total = 0, n = 0;
    while((n = recv(sockfd, buffer+total, size-total-1, 0)) > 0) {
        total += n;
    }
    buffer[total] = 0;
    std::cout << buffer << std::endl;
	infoDump("dump/dump.txt",buffer);

    freeaddrinfo(results);

    printf("\n");
}

void HTTPUtils::infoDump(std::string filename,char* buffer){
	std::ofstream savefile;
    savefile.open (filename);
    savefile << buffer;
    savefile.close();
	std::cout << "Dumped" << std::endl;
}