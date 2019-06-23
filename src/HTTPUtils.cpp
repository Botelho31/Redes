#include "../include/HTTPUtils.h"

HTTPUtils::HTTPUtils(int port,std::string ip){
    this->port = port;
    this->ip = ip;
	this->sock = 0;
	// if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	// 	printf("\n Socket creation error \n");
	// }
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

	struct timeval tv;
	fd_set rfds; 
	tv.tv_sec = 0.01;
	tv.tv_usec = 0;
	

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

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
		// int flag = select(1, &rfds, NULL, NULL, &tv);
		// if (flag == 0 || flag == 1){
		// 	std::cout << "Timeout" << std::endl;
		// 	break;
		// }
    }
    buffer[total] = 0;
    freeaddrinfo(results);
	char* response = buffer; 
	close(sockfd);


	return response;

}

void HTTPUtils::infoDump(std::string filename,std::string content){
	std::ofstream savefile;
    savefile.open (filename);
    savefile << content;
    savefile.close();
}

std::map<std::string,std::vector<std::string>> HTTPUtils::ParseResponse(char* response,bool printHeader,bool printBody){
	std::map<std::string,std::vector<std::string>> httpParams;
	std::stringstream streamresponse;
	streamresponse << response;
    std::string checkline;

	//Getting the HTTP Header
	std::vector<std::string> responseTop;
	streamresponse >> checkline;
	responseTop.push_back(checkline);
	streamresponse >> checkline;
	responseTop.push_back(checkline);
	streamresponse >> checkline;
	responseTop.push_back(checkline);
	getline(streamresponse,checkline,'\n');
	httpParams.insert({"ResponseHeader",responseTop});
	if(printHeader){
		for(int i = 0;i < responseTop.size();i ++){
			std::cout << responseTop[i];
		}
		std::cout << checkline;
		std::cout << std::endl;
	}

	//Gets the params of HTTP
	while (!streamresponse.eof() ) {
		getline(streamresponse,checkline,'\n');
		if(printHeader){
			std::cout << checkline << std::endl;
		}
		if(checkline == "\r"){
			if(printHeader){
				std::cout << "--End Of Header--" << std::endl;
			}
			break;
		}
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
	while(!streamresponse.eof()){
		getline(streamresponse,checkline,'\n');
		responseContent << checkline;
	}
	if(printBody){
		std::cout << responseContent.str() << std::endl;
	}
	httpParams.insert({"HTML",{responseContent.str()}});
	if((httpParams.find("Date") == httpParams.end()) || (httpParams.find("Host") == httpParams.end())){
		httpParams.insert({"Error",{"True"}});
	}else{
		httpParams.insert({"Error",{"False"}});
	}
	return httpParams;
}

std::string HTTPUtils::RemovePort(std::string url){
	std::stringstream endereco;
	endereco << url;
	getline(endereco,url,'/');
	if(url == "http:"){
		getline(endereco,url,'/');
	}
	endereco >> url;
	std::cout << url << std::endl;
	getline(endereco,url,':');
	return url;
}
