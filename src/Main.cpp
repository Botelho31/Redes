#include "../include/HTTPUtils.h"
#include "../include/Server.h"

void spider(std::string sitename){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	HTTPUtils::Site *site = new HTTPUtils::Site(sitename);
	http->Spider(site,0);
	http->MakeSpiderGraph();
}

void dump(std::string sitename){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	HTTPUtils::Site *site = new HTTPUtils::Site(sitename);
	http->Dump(site,0);
	http->MakeSpiderGraph();
}

int main(int argc, char const* argv[]){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	// char* response = http->MakeRequest("www.google.com",http->getRequest);
	spider("http://www.ba.gov.br/");

	// dump("http://www.ba.gov.br/");

	// std::cout << http->MakeRequest("www.ba.gov.br",http->GetRequest("http://www.ba.gov.br/node/232/")) << std::endl;

	// http->CleanURL("www.ba.gov.br/node/")
	// Server server(8228);
	// server.ListenFor();

	return 0;
}
