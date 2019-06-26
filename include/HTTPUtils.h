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
            class Site{
                public:
                    Site(std::string url){
                        this->url = url;
                        for(int i = 0;i < url.size();i++){
                            if((i%20) == 0){
                                url.insert(url.begin() + i,'\n');
                            }
                        }
                        currentID ++;
                        this->id = currentID;
                    }
                    int id;
                    std::string url;
                    std::vector<Site*> conexoes;
                    static int currentID;

                    std::string BreakUrl(){
                        std::stringstream newURL;
                        // newURL << url;
                        for(int i = 0;i < url.size();i++){
                            newURL << url[i];
                            if(((i%30) == 0) && (i != 0)){
                                newURL << "\\n";
                            }
                        }
                        return newURL.str();
                    }
            };

            HTTPUtils(int port,std::string ip);
            void SendMessage(std::string message);

            std::string MakeRequest(std::string address,std::string request);
            void spider(Site* currentsite,int depthcounter);
            void saveFile(std::string filename,std::string content);
            HTTPRequest ParseResponse(std::string response,bool printHeader = false,bool printBody = false);

            void MakeSpiderGraph();
            std::string CleanURL(std::string url,bool nopaths = false);
            bool isUrl(std::string url);
            std::string GetRequest(std::string link);
        private:
            int port;
            std::string ip;
            int sock;

            std::vector<Site*> sites;
    };
#endif