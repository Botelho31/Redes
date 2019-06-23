#ifndef SERVER
    #define SERVER
    #include "HTTPUtils.h"
    class Server{
        public:
            Server(int port);
            ~Server();
            void ListenFor();

            void KeepAlive(int new_socket);
        private:
            int server_fd, new_socket, new_socket2, valread,port;
            struct sockaddr_in address;
            struct timeval tv;
            int opt;
            int addrlen;
            fd_set rfds; 
    };
#endif