#ifndef HTTPREQUEST
    #define HTTPREQUEST
    #include "HTTPUtils.h"
    class HTTPRequest{
        public:
            HTTPRequest(std::map<std::string,std::vector<std::string>> httpParams);

            std::string host;
            std::string method;
            std::string connection;
    };
#endif