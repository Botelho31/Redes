#ifndef SERVER
    #define SERVER
    #include "HTTPUtils.h"
    class Server{
        public:
            Server(int port);
            ~Server();
            void ListenFor();

            void* HandleRequest(void *args);
            std::string WaitForEdit(std::string content,std::string filename);
            void Close();
        private:
            int server_fd, new_socket, new_socket2, valread,port;
            struct sockaddr_in address;
            int opt;
            int addrlen;

            bool keeprunning;
    };
#endif