#include "../include/HTTPUtils.h"

int HTTPUtils::Site::currentID = 0;

HTTPUtils::HTTPUtils(int port,std::string ip){
    this->port = port;
    this->ip = ip;
	this->sock = 0;
}

//Faz uma request pro site especificado
std::string HTTPUtils::MakeRequest(std::string address,std::string request){
	struct addrinfo hints , *results;
	int rv;
	int sockfd;
	char inputVal[address.size() + 1];
	strcpy(inputVal, address.c_str());
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	//Resolve o DNS do nome do site
	if( (rv = getaddrinfo(inputVal,"80",&hints,&results)) != 0){
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(rv));
		char *responseempty = "";
		return responseempty;
	}

    sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

	//Seta um timeout pra socket recem criada
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	//Se conecta e manda pro servidor
    connect(sockfd, results->ai_addr, results->ai_addrlen);
    send(sockfd, request.c_str(), strlen(request.c_str()), 0);
	
	//Recebe a resposta byte a byte
	std::ostringstream bufferStream;
	char buffer;
	while(recv(sockfd, &buffer, 1, 0) > 0){
		bufferStream << buffer;
	}
	freeaddrinfo(results);
	close(sockfd);

	return bufferStream.str();

}

//Realiza o algoritmo Spider requisitado no trabalho
void HTTPUtils::Spider(HTTPUtils::Site* site,int depthcounter){
	sites.push_back(site);
	if(depthcounter >= 3){
		return;
	}
	std::string sitename = site->url;
	depthcounter ++;
	std::cout << "Depth: " << depthcounter << std::endl;
	std::string response = MakeRequest(CleanURL(sitename),GetRequest(sitename));
	HTTPRequest request = ParseResponse(response);
	std::stringstream html;
	std::string checkline;
	html << request.html;

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
						bool alreadyvisited = false;
						if(checkline.c_str()[0] == '/'){
							std::stringstream newhref;
							newhref << CleanURL(sitename,true) << checkline;
							checkline = newhref.str();
						}
						if(isUrl(checkline)){
							if(checkline.c_str()[checkline.size() - 1] != '/'){
								checkline.push_back('/');
							}
							checkline = AddHTTPToUrl(checkline);
							for(int i  = 0;i < sites.size();i++){
								if(sites[i]->url == checkline){
									alreadyvisited = true;
								}
								for(int j = 0;j < sites[i]->conexoes.size();j++){
									if(sites[i]->conexoes[j]->url == checkline){
										alreadyvisited = true;
									}
								}
							}
							if(!alreadyvisited){
								Site *newsite;
								newsite = new Site(checkline);
								site->conexoes.push_back(newsite);
								Spider(newsite,depthcounter);
							}
							foundhref = true;
						}
						
					}
					if(href.eof() || tag.eof()){
						foundhref = true;
					}
				}
			}
		}
	}
}

//Realiza o algoritmo dump requisitado no trabalho
void HTTPUtils::Dump(HTTPUtils::Site* site,int depthcounter){

	sites.push_back(site);
	std::string sitename = site->url;
	depthcounter ++;

	std::cout << "Depth: " << depthcounter << std::endl;
	std::string response = MakeRequest(CleanURL(sitename),GetRequest(AddHTTPToUrl(sitename)));
	HTTPRequest request = ParseResponse(response);

	std::stringstream html;
	std::string checkline;
	html << request.html;

	std::stringstream newhtml;

	while(!html.eof()){
		getline(html,checkline,'<');
		if(!html.eof()){
			newhtml << checkline << "<";
			getline(html,checkline,'>');
			std::stringstream tag;
			tag << checkline;
			if(checkline.c_str()[0] == '/'){
				newhtml << tag.str() << ">";
				continue;
			}else{
				tag >> checkline;
				if(checkline == "a"){
					newhtml << "a";
					bool foundhref = false;
					while(!foundhref){
						tag >> checkline;
						std::stringstream href;
						href << checkline;
						getline(href,checkline,'=');
						if(checkline == "href"){
							getline(href,checkline,'"');
							getline(href,checkline,'"');
							bool alreadyvisited = false;
							int alreadyvisitedIt = 0;

							if(checkline.c_str()[0] == '/'){
								std::stringstream newhref;
								newhref << CleanURL(sitename,true) << checkline;
								checkline = newhref.str();
							}

							if(isUrl(checkline)){
								if(checkline.c_str()[checkline.size() - 1] != '/'){
									checkline.push_back('/');
								}
								checkline = AddHTTPToUrl(checkline);
								for(int i  = 0;i < sites.size();i++){
									if(sites[i]->url == checkline){
										alreadyvisited = true;
										alreadyvisitedIt = sites[i]->id;
									}
									for(int j = 0;j < sites[i]->conexoes.size();j++){
										if(sites[i]->conexoes[j]->url == checkline){
											alreadyvisited = true;
											alreadyvisitedIt = sites[i]->conexoes[j]->id;
										}
									}
								}
								if((CleanURL(checkline)) == CleanURL(sitename)){
									if(!alreadyvisited){
										// if(depthcounter < 2){
											std::cout << checkline << std::endl;
											Site *newsite;
											newsite = new Site(checkline);
											newhtml << " " << "href=\"" << "dump" << newsite->id << ".html\"";
											Dump(newsite,depthcounter);
											site->conexoes.push_back(newsite);
										// } 
									}else{
										newhtml << " " << "href=\"" << "dump" << alreadyvisitedIt << ".html\"";
									}
								}
								foundhref = true;
							}
						}
						else{
							newhtml << " " << href.str();
						}
						if(href.eof() || tag.eof()){
							foundhref = true;
						}
					}

				}else{
					newhtml << tag.str();
				}
				newhtml << ">";
			}
		}
	}

	std::stringstream filesave;
	filesave << "dump/" << "dump" << site->id << ".html";
	saveFile(filesave.str(),newhtml.str());
}

//Salva o conteudo no arquivo especificado
void HTTPUtils::saveFile(std::string filename,std::string content){
	std::ofstream savefile;
    savefile.open (filename);
    savefile << content;
    savefile.close();
}

//Realiza o parse nos parametros HTTP
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

//Limpa o url deixando somente o www.site.com por exemplo
std::string HTTPUtils::CleanURL(std::string url,bool withhttp){
	std::stringstream endereco;
	endereco << url;
	getline(endereco,url,'/');
	if((url == "http:") || (url == "https:")){
		getline(endereco,url,'/');
		endereco >> url;
	}
	std::stringstream newendereco;
	newendereco << url;
	getline(newendereco,url,'/');

	if(withhttp){
		std::stringstream withhttp;
		withhttp << "http://" << url;
		return url;
	}
	return url;
}

//Gera um grafo usando a biblioteca graphviz e a extensão dot
void HTTPUtils::MakeGraph(std::string graphname){
	std::cout << "Generating Spider Graph" << std::endl;
    std::ofstream myfile;
	std::stringstream filename;
	filename << "graph/" << graphname << ".dot";
    myfile.open (filename.str().c_str());
    myfile << "digraph {\n";

	myfile << "\t" << "compound=true;" << std::endl;
	myfile << "\t" << "overlap=scalexy;" << std::endl;
	myfile << "\t" << "splines=true;" << std::endl;
	myfile << "\t" << "layout=\"neato\";" << std::endl;
	myfile << "\t" << "sep=-0.4;" << std::endl;

	for(int i = 0;i < sites.size();i++){
		if(i == 0){
			myfile << "\t" << sites[i]->id << " [label=\"" << sites[i]->BreakUrl() << "\"; shape=record; color=red; height=.1; fontsize=9];" << std::endl;
		}else{
			myfile << "\t" << sites[i]->id << " [label=\"" << sites[i]->BreakUrl() << "\"; shape=record; height=.1; fontsize=9];" << std::endl;
		}
    }
    for(int i = 0;i < sites.size();i++){
        for(int j = 0;j < sites[i]->conexoes.size();j ++){
            myfile << "\t" << sites[i]->id << " -> " << sites[i]->conexoes[j]->id << ";\n";
        }
    }
    myfile << "}\n";
    myfile.close();

	std::stringstream command;
	command << "dot -Tsvg graph/" << graphname << ".dot" << " -o graph/" << graphname << ".svg";
    system(command.str().c_str());
}

//Checa se é um URL
bool HTTPUtils::isUrl(std::string url){
	url = CleanURL(url);
	std::stringstream urlStream;
	urlStream << url;

	if((url.c_str()[0] == '#') || (url.c_str()[0] == '.')){
		return false;
	}
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


//Addiciona o http:// ao começo de um site
std::string HTTPUtils::AddHTTPToUrl(std::string sitename){
	if(sitename.c_str()[0] != 'h'){
		std::stringstream refwithhttp;
		refwithhttp << "http://" << sitename;
		sitename = refwithhttp.str();
	}
	return sitename;						
}

//Retorna um HTTP GET padrão
std::string HTTPUtils::GetRequest(std::string sitename){
	if(sitename.c_str()[sitename.size() - 1] == '/'){
		sitename.pop_back();
	}
	std::string getRequest =  "GET "  + sitename + "/ HTTP/1.0\r\n" + "Host: " + CleanURL(sitename) + "\r\n" + "Connection: close" + "\r\n\r\n";
	std::cout << getRequest << std::endl;
	return getRequest;
}