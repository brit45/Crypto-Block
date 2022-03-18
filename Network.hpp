#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class Server {
    public: 
        Server();
        ~Server();
        int Bind(std::string IP, int Port);
        int Listen();
        int Lisenning = 0;
        int clienSocket = 0;
        
};


#endif // __NETWORK_H__