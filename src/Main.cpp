#include "../include/HTTPUtils.h"
#include "../include/Server.h"

int main(int argc, char const* argv[]){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	// char* response = http->MakeRequest("www.google.com",http->getRequest);
	// HTTPRequest request = http->ParseResponse(response);
	HTTPUtils::Site *site = new HTTPUtils::Site("http://www.ba.gov.br");
	HTTPUtils::Site::currentID = 0;
	http->spider(site,0);
	http->MakeSpiderGraph();

	// Server server(8228);
	// server.ListenFor();

	return 0;
}
