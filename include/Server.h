#ifndef SERVER
    #define SERVER
    #include "HTTPUtils.h"
    class Server{
        public:
            char* ListenFor(int port);
    };
#endif