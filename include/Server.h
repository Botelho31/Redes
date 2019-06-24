#ifndef SERVER
    #define SERVER
    #include "HTTPUtils.h"
    class Server{
        public:
            Server(int port);
            ~Server();
            void ListenFor();

            static void* HandleRequest(void *args);
        private:
            int server_fd, new_socket, new_socket2, valread,port;
            struct sockaddr_in address;
            int opt;
            int addrlen;
    };
#endif