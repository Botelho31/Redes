#include "../include/HTTPUtils.h"

HTTPUtils::HTTPUtils(int port,std::string ip){
    this->port = port;
    this->ip = ip;
	this->sock = 0;
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

std::string HTTPUtils::MakeRequest(std::string address,std::string request){
	struct addrinfo hints , *results;
	int rv;
	int sockfd;
	char inputVal[address.size() + 1];
	strcpy(inputVal, address.c_str());
	// char buffer[64000];
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if( (rv = getaddrinfo(inputVal,"80",&hints,&results)) != 0){
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(rv));
		char *responseempty = "";
		return responseempty;
	}

    sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    connect(sockfd, results->ai_addr, results->ai_addrlen);
    send(sockfd, request.c_str(), strlen(request.c_str()), 0);
	
	std::ostringstream bufferStream;
	char buffer;
	while(recv(sockfd, &buffer, 1, 0) > 0){
		bufferStream << buffer;
	}
	freeaddrinfo(results);
	close(sockfd);

	return bufferStream.str();

}

void HTTPUtils::spider(std::string sitename){
	std::string response = MakeRequest(CleanURL(sitename),GetRequest(sitename));
	// std::cout << response << std::endl;
	HTTPRequest request = ParseResponse(response);
	std::stringstream html;
	std::string checkline;
	html << request.html;

	Site site;
	site.url = sitename;

	while(!html.eof()){
		getline(html,checkline,'<');
		getline(html,checkline,'>');
		std::stringstream tag;
		tag << checkline;
		if(checkline.c_str()[0] == '/'){
			continue;
		}else{
			tag >> checkline;
			if(checkline == "a"){
				bool foundhref = false;
				while(!foundhref){
					tag >> checkline;
					std::stringstream href;
					href << checkline;
					getline(href,checkline,'=');
					if(checkline == "href"){
						getline(href,checkline,'"');
						getline(href,checkline,'"');
						if(checkline.c_str()[0] == '/'){
							std::stringstream newhref;
							newhref << sitename << checkline;
							checkline = newhref.str();
						}
						site.conexoes.push_back(checkline);
						bool alreadyvisited = false;
						if(checkline == sitename){
							alreadyvisited = true;
						}
						for(int i  = 0;i < sites.size();i++){
							if(sites[i].url == checkline){
								alreadyvisited = true;
							}
						}
						if(!alreadyvisited){
							if(isUrl(checkline)){
								spider(checkline);
							}
						}
						std::cout << checkline << std::endl;
						foundhref = true;
					}
					if(href.eof()){
						foundhref = true;
					}
					if(tag.eof()){
						foundhref = true;
					}
				}
			}
		}
	}
	sites.push_back(site);
}

void HTTPUtils::saveFile(std::string filename,std::string content){
	std::ofstream savefile;
    savefile.open (filename);
    savefile << content;
    savefile.close();
}

HTTPRequest HTTPUtils::ParseResponse(std::string response,bool printHeader,bool printBody){
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

	HTTPRequest request = HTTPRequest(httpParams);
	return request;
}

std::string HTTPUtils::CleanURL(std::string url){
	std::stringstream endereco;
	endereco << url;
	getline(endereco,url,'/');
	if((url == "http:") || (url == "https:")){
		getline(endereco,url,'/');
	}
	endereco >> url;
	std::stringstream newendereco;
	newendereco << url;
	getline(newendereco,url,'/');
	return url;
}

bool HTTPUtils::isUrl(std::string url){
	url = CleanURL(url);
	std::stringstream urlStream;
	urlStream << url;
	if((url.c_str()[0] == 'w') && (url.c_str()[1] == 'w') && (url.c_str()[2] == 'w') && (url.c_str()[3] == '.')){
		return true;
	}
	if((url.c_str()[url.size() - 1] == 'm') && (url.c_str()[url.size() - 2] == 'o') && (url.c_str()[url.size() - 3] == 'c') && (url.c_str()[url.size() - 4] == '.')){
		return true;
	}
	if((url.c_str()[url.size() - 1] == 'r') && (url.c_str()[url.size() - 2] == 'b') && (url.c_str()[url.size() - 3] == '.')){
		return true;
	}
	return false;
}

std::string HTTPUtils::GetRequest(std::string sitename){

	std::string getRequest =  "GET "  + sitename + "/ HTTP/1.0\r\n" + "Host: " + CleanURL(sitename) + "\r\n" + "Connection: close" + "\r\n\r\n";
	std::cout << getRequest << std::endl;
	return getRequest;
}