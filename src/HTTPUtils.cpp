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

	send(sock, message, strlen(message), 0);
	return;
}

char* HTTPUtils::ListenForResponse(int socket){
	char buffer[6400] = { 0 };
	size_t size = sizeof(buffer);
	size_t total = 0, n = 0;
    while((n = recv(socket, buffer+total, size-total-1, 0)) > 0) {
        total += n;
    }
    buffer[total] = 0;
	char* test = buffer;
	return test;
}

char* HTTPUtils::MakeRequest(std::string address,std::string request){
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
    freeaddrinfo(results);
	char* response = buffer; 

	return response;

}

void HTTPUtils::infoDump(std::string filename,std::string content){
	std::ofstream savefile;
    savefile.open (filename);
    savefile << content;
    savefile.close();
}

std::map<std::string,std::vector<std::string>> HTTPUtils::ParseResponse(char* response){
	std::map<std::string,std::vector<std::string>> httpParams;
	std::stringstream streamresponse;
	streamresponse << response;
    std::string checkline;

	//Getting the HTTP Header
	std::vector<std::string> responseTop;
	getline(streamresponse,checkline,'<');
	std::stringstream header;
	header << checkline;
	header >> checkline;
	responseTop.push_back(checkline);
	header >> checkline;
	responseTop.push_back(checkline);
	header >> checkline;
	responseTop.push_back(checkline);
	getline(header,checkline,'\n');
	httpParams.insert({"ResponseTop",responseTop});

	//Gets the params of HTTP
	while (!header.eof() ) {
		getline(header,checkline,'\n');

		std::string paramName;
		std::vector<std::string> paramContent;
		std::stringstream param;
		param << checkline;
		getline(param,checkline,':');
		paramName = checkline;
		getline(param,checkline,'\r');
		std::stringstream paramContentStream;
		paramContentStream << checkline;
		while(!paramContentStream.eof()){
			std::string checkparam;
			paramContentStream >> checkparam;
			paramContent.push_back(checkparam);
		}

		httpParams.insert({paramName,paramContent});
	}

	//Getting the content of a response
	std::stringstream responseContent;
	responseContent << '<';
	while(!streamresponse.eof()){
		getline(streamresponse,checkline,'\n');
		responseContent << checkline;
	}
	httpParams.insert({"HTML",{responseContent.str()}});
	return httpParams;
}

