#include "../include/Server.h"


Server::Server(int port){
	this->port = port;
	this->opt = 1;
	this->addrlen = sizeof(address);

	keeprunning = true;

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

	while(keeprunning){

		if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
			(socklen_t*)& addrlen)) < 0) //Blocked until connect is called in the client
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		if(keeprunning){
			printf("Server accepted connection, reading message\n");
			
			HandleRequest(&new_socket);
		}
	}	
}

void* Server::HandleRequest(void *arg){

	int new_socket = *((int *)arg);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	std::ostringstream bufferStream;
	char buffer;
	while(recv(new_socket, &buffer, 1, 0) > 0){
		bufferStream << buffer;
	}

	std::cout << "Received Request" << std::endl << std::endl;
	std::string request = bufferStream.str();
	std::cout << request << std::endl;
	std::string edited = WaitForEdit(request,"Receiving-Sending/Receiving-Request.txt");
	std::cout << edited << std::endl;

	HTTPUtils* http = new HTTPUtils(8080,"127.0.0.1");
	HTTPRequest HTTPresponse = http->ParseResponse(request);
	if(HTTPresponse.method == ""){
		std::cout << "discarding Empty Request" << std::endl;
		
	}else if(HTTPresponse.method == "CONNECT"){
		std::cout << "Discarding HTTPS Request" << std::endl;
	}else{

		std::cout << "Made Request" << " - Host: " << http->CleanURL(HTTPresponse.host) << std::endl;
		std::string response = http->MakeRequest(http->CleanURL(HTTPresponse.host),HTTPresponse.GetCleanedRequest());

		if(response == ""){
			std::cout << "Got No Response" << " - Host: " << HTTPresponse.host << std::endl;
		}else{
			std::cout << "Got Response" << " - Host: " << HTTPresponse.host << std::endl << std::endl << response << std::endl;
			std::string edited = WaitForEdit(response,"Receiving-Sending/Receiving-Response.txt");
			send(new_socket, response.c_str(), strlen(response.c_str()), 0);
			std::cout << "Send Response" << " - Host: " << HTTPresponse.host << std::endl;
		}
	}

	close(new_socket);
		
}


std::string Server::WaitForEdit(std::string content,std::string filename){
	std::ofstream savefile;
    savefile.open (filename);
    savefile << content;
    savefile.close();

	std::cout << "Please Edit file: " << filename << " and then type anything on terminal"<< std::endl;
	std::string input;
	std::cin >> input;

	std::fstream editedfile;
    editedfile.open(filename);
	std::stringstream editedcontent;
	std::string checkline;
	if(editedfile.is_open()){
		while(!editedfile.eof()){
			getline(editedfile,checkline,'\n');
			editedcontent << checkline << "\r\n";
		}
	}
    editedfile.close();
	std::cout << "File edited" << std::endl;
	return editedcontent.str();
}

void Server::Close(){
	keeprunning = false;
}
