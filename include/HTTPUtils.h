#ifndef HTTPUTILS
    #define HTTPUTILS
    #include <stdio.h> 
    #include <bits/stdc++.h>
    #include <cstdlib>
    #include <stdlib.h> 
    #include <string.h> 
    #include <unistd.h>
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <netdb.h>
    #include <thread>

    class HTTPUtils{
        public:
            HTTPUtils(int port,std::string ip);
            void SendMessage(std::string message);
            char* ListenForResponse(int socket);


            char* MakeRequest(std::string address,std::string request);
            void infoDump(std::string filename,std::string content);
            std::map<std::string,std::vector<std::string>> ParseResponse(char* response,bool printHeader = false,bool printBody = false);

            std::string RemovePort(std::string);
        private:
            int port;
            std::string ip;
            int sock;
    };
#endif