#include "../include/HTTPUtils.h"

int main(int argc, char const* argv[]){
	HTTPUtils *http = new HTTPUtils(8080,"127.0.0.1");
	char* response = http->MakeRequest("www.google.com","GET / HTTP/1.0\r\n\r\n");
	std::map<std::string,std::vector<std::string>> responseMap = http->ParseResponse(response);
	http->infoDump("dump/dump.html",responseMap["HTML"][0]);
	return 0;
}
