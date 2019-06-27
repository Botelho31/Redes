#include "../include/Server.h"


//Cria e da Bind na porta do servidor
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

//Escuta as tentativas de connect na socket do servidor e as manda pra HandleRequest onde sao tratadas
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
		}else{
			std::cout << "Shutting down server" << std::endl;
			std::cout << std::endl;
		}
	}	
}

//Cuida das requests recebidas pelo servidos
void* Server::HandleRequest(void *arg){

	int new_socket = *((int *)arg);

	//Seta um timeout para a socket
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	//Pega a informacao byte a byte da socket
	std::ostringstream bufferStream;
	char buffer;
	while(recv(new_socket, &buffer, 1, 0) > 0){
		bufferStream << buffer;
	}

	//Envia a request para WaitForEdit onde sera editada pelo usuario
	std::cout << "Received Request" << std::endl << std::endl;
	std::string request = bufferStream.str();
	std::cout << request << std::endl;
	std::string edited1 = WaitForEdit(request,"Receiving-Sending/Receiving-Request.txt");
	std::cout << edited1 << std::endl;

	//Da o parse na request
	HTTPUtils* http = new HTTPUtils(8080,"127.0.0.1");
	HTTPRequest HTTPresponse = http->ParseResponse(edited1);
	if(HTTPresponse.method == ""){
		//Caso a request seja vazia ele descarta dela
		std::cout << "Discarding Empty Request" << std::endl;
	}else if(HTTPresponse.method == "CONNECT"){
		//Descarta de requests do tipo HTTPS
		std::cout << "Discarding HTTPS Request" << std::endl;
	}else{		

		//Realiza a request
		std::cout << "Made Request" << " - Host: " << http->CleanURL(HTTPresponse.host) << std::endl;
		std::string response = http->MakeRequest(http->CleanURL(HTTPresponse.host),HTTPresponse.GetCleanedRequest());

		if(response == ""){
			//Caso a resposta venha vazia volta pro listen do servidor e espera outra conexão
			std::cout << "Got No Response" << " - Host: " << HTTPresponse.host << std::endl;
		}else{
			//Recebe a request e espera a edição do usuario
			std::cout << "Got Response" << " - Host: " << HTTPresponse.host << std::endl << std::endl << response << std::endl;
			std::string edited2 = WaitForEdit(response,"Receiving-Sending/Receiving-Response.txt");
			//Manda de volta para o cliente;
			send(new_socket, edited2.c_str(), strlen(edited2.c_str()), 0);
			std::cout << "Send Response" << " - Host: " << HTTPresponse.host << std::endl;
		}
	}

	//Fecha a conexao do cliente
	close(new_socket);
		
}

//Espera a aprovação do usuario do arquivo especificado pra voltar pro programa
std::string Server::WaitForEdit(std::string content,std::string filename){
	//Salva oque esta a ser editado na file especificada
	std::ofstream savefile;
    savefile.open (filename);
    savefile << content;
    savefile.close();

	//Espera o OK do usuario
	std::cout << "Please Edit file: " << filename << " and then type anything on terminal and press ENTER."<< std::endl<< std::endl;
	std::string input;
	std::cin >> input;
	if(input == "quit"){
		std::cout << "Shutting Down Server On Next Request" << std::endl;
		std::cout << std::endl;
		keeprunning = false;
	}
	std::cout << "File edited" << std::endl;

	//Pega o conteudo editado/ou não da file
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
	return editedcontent.str();
}

//Fecha o servidor na proxima request
void Server::Close(){
	keeprunning = false;
}
