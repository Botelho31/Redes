#include "../include/HTTPUtils.h"
#include "../include/Server.h"

void spider(std::string sitename){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	HTTPUtils::Site *site = new HTTPUtils::Site(sitename);
	http->Spider(site,0);
	http->MakeGraph("spidergraph");
}

void dump(std::string sitename){
	HTTPUtils *http = new HTTPUtils(8228,"127.0.0.1");
	HTTPUtils::Site *site = new HTTPUtils::Site(sitename);
	http->Dump(site,0);
	http->MakeGraph("dumpgraph");
}

void proxyserver(int port){
	Server server(port);
	server.ListenFor();
}

int main(int argc, char const* argv[]){
	// spider("http://www.ba.gov.br/");

	// dump("http://www.ba.gov.br/");

	proxyserver(8228);

	return 0;
}
