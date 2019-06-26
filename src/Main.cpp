#include "../include/HTTPUtils.h"
#include "../include/Server.h"

int main(int argc, char const* argv[]){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	// char* response = http->MakeRequest("www.google.com",http->getRequest);
	// HTTPRequest request = http->ParseResponse(response);
	http->spider("http://www.ba.gov.br",0);
	// std::cout << http->CleanURL("#meatcontent") << std::endl;
	// std::cout << http->CleanURL("www.google.com") << std::endl;
	// std::cout << http->GetRequest("www.google.com") << std::endl;

	// Server server(8228);
	// server.ListenFor();

	return 0;
}
