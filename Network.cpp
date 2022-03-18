#include "Network.hpp"

Server::Server() {
    
    this->Lisenning = socket(AF_INET,SOCK_STREAM,0);

    if(this->Lisenning == -1) {
        std::cerr << "Create Socket fail.";
        return;
    }
}

Server::~Server() {

    close(this->Lisenning);
}

int Server::Bind(std::string IP, int port) {
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET,IP.c_str(),(sockaddr *) &hint.sin_addr);

    if(bind(this->Lisenning, (sockaddr *)&hint, sizeof(hint)) == -1) {
        std::cerr << "Fail to bind socket IP/port.";
        return -2;
    }
}

int Server::Listen() {

    if(listen(this->Lisenning, SOMAXCONN) == -1) {
        std::cerr << "Listen socket fail.";
        return -3;
    }
    sockaddr_in *client;
    socklen_t clientSize = sizeof(client);

    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    this->clienSocket = accept(this->Lisenning,(sockaddr *)&client, &clientSize);

    if(this->clienSocket == -1) {
        std::cerr << "Error to accept Client connection.";
        return -4;
    }

    

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *)&client,sizeof(client),host,NI_MAXHOST,svc,NI_MAXSERV,0);

    if(result) {
        
        std::cout << host << "Connected on " << svc << std::endl;
    }
    else {
        
        inet_ntop(AF_INET,&client->sin_addr, host,NI_MAXHOST);
        std::cout << host << "Connected on " << ntohs(client->sin_port) << std::endl;
    }

    char buffer[4096];
    
    while (true) {
        
        // Clear Buffer
        memset(buffer, 0, 4096);

        // Wait Instructions
        int BytesRecv = recv(this->clienSocket,buffer,4096,0);

        if(BytesRecv == -1) {
            
            std::cerr << "There was connection issue" << std::endl;
            break;
        }
        else if(BytesRecv == 0) {
            std::cerr << "Client Disconnected." << std::endl;
            break;
        }

        std::cout << "Received : " << std::string(buffer, 0, BytesRecv) << std::endl;

        send(this->clienSocket, buffer, BytesRecv +1, 0);
    }
     
}