#include "../include/HTTPUtils.h"

int main(int argc, char const* argv[]){
	HTTPUtils *http = new HTTPUtils(8080,"127.0.0.1");
	http->MakeRequest("www.google.com","GET / HTTP/1.0\r\n\r\n");
	return 0;
}
