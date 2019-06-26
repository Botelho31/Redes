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
    #include "HTTPRequest.h"

    class HTTPRequest;

    class HTTPUtils{
        public:
            HTTPUtils(int port,std::string ip);
            void SendMessage(std::string message);

            std::string MakeRequest(std::string address,std::string request);
            void spider(std::string sitename,int depthcounter);
            void saveFile(std::string filename,std::string content);
            HTTPRequest ParseResponse(std::string response,bool printHeader = false,bool printBody = false);

            std::string CleanURL(std::string url,bool nopaths = false);
            bool isUrl(std::string url);
            std::string GetRequest(std::string link);
        private:
            class Site{
                public:
                    std::string url;
                    std::vector<std::string> conexoes;
            };
            int port;
            std::string ip;
            int sock;

            std::vector<Site> sites;
    };
#endif