#ifndef HTTPREQUEST
    #define HTTPREQUEST
    #include "HTTPUtils.h"
    class HTTPRequest{
        public:
            HTTPRequest(std::map<std::string,std::vector<std::string>> httpParams);

            std::string GetCleanedRequest();

            std::string host;
            std::string method;
            std::string connection;
            std::string accept;
            std::string html;
            std::map<std::string,std::vector<std::string>> params;
    };
#endif