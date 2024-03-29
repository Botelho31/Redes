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


    std::string flag;
    int port;
    while (true){
        std::cout << "Digite 1 para rodar o algoritmo spider." << std::endl;
        std::cout << "Digite 2 para rodar o algoritmo de Dump." << std::endl;
        std::cout << "Digite 3 para rodar o algoritmo de Proxy Server." << std::endl;
        std::cout << "Digite 4 para encerrar o programa." << std::endl;
        std::cin >> flag;
        if (flag == "1"){
            std::cout << "Opcao de Spider selecionado" << std::endl;
            spider("http://www.ba.gov.br/");
            std::cout << "On graph directory --- Output file : spidergraph.svg (open on Browser)" << std::endl;
			std::cout << std::endl;
        }
        else{
            if (flag == "2"){
                std::cout << "Opcao de Dump selecionado" << std::endl;
                dump("http://www.ba.gov.br/");

            std::cout << "On dump directory --- Output file : (nameofwebsite)" << std::endl;
			std::cout << std::endl;
            }
            else{
                if (flag == "3"){
                    std::cout << "Opcao de Proxy Server selecionado" << std::endl;
                    std::cout << "Escolha uma porta: ";
                    std::cin >> port;
                    proxyserver(port);
                }
                else{
                    if (flag == "4"){
                        std::cout << "--QUIT--" << std::endl;
                        return 0;
                    }
                    else{
                        std::cout << "Digite um numero de 1 a 4" << std::endl;
                    }
                }
            }
        }
    }
}