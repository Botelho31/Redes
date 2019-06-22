#include "../include/HTTPUtils.h"
#include "../include/Server.h"

int main(int argc, char const* argv[]){
	// HTTPUtils *http = new HTTPUtils(8080,"127.0.0.1");
	// char* response = http->MakeRequest("www.google.com","GET / HTTP/1.0\r\n\r\n");
	// std::map<std::string,std::vector<std::string>> responseMap = http->ParseResponse(response);
	// http->infoDump("dump/dump.html",responseMap["HTML"][0]);

	Server server;
	char* serverresponse = server.ListenFor(8228);
	std::cout << serverresponse << std::endl;
	// std::stringstream request;
	// request << serverresponse;
	// char* response = http->MakeRequest("www.youtube.com",request.str());
	// std::cout << response << std::endl;




	//HOW TO PARSE MAPS
	// for(auto it = responseMap.cbegin(); it != responseMap.cend(); ++it){
	// 	std::cout << it->first << ": ";
	// 	for(int i = 0;i < it->second.size();i++){
	// 		std::cout << it->second[i] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	return 0;
}
