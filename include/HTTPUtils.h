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

    class HTTPUtils{
        public:
            HTTPUtils(int port,std::string ip);
            void SendMessage(std::string message);
            std::string ReceiveMessage();

            void MakeRequest(std::string address,std::string request);
            void infoDump(std::string filename,char* buffer);
        private:
            int port;
            std::string ip;
            int sock;
    };
#endif