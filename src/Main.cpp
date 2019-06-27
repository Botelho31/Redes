#include "../include/HTTPUtils.h"
#include "../include/Server.h"

void WaitForInput(Server *server){
	std::string input;
	while(input != "quit"){
		std::cin >> input;
	}
	std::cout << "Closing Server on next connection" << std::endl;
	server->Close();
}

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
	std::thread t1(WaitForInput,&server);
	server.ListenFor();
	t1.join();
}

int main(int argc, char const* argv[]){
	// spider("http://www.ba.gov.br/");

	// dump("http://www.ba.gov.br/");

	proxyserver(8228);

	// Server server(8228);
	// std::cout << server.WaitForEdit("content","Receiving-Sending/test.txt") << std::endl;

	return 0;
}
