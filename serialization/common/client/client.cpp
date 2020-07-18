#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include "../message.h"
#include "deserializer.h"

using namespace std;


std::ostream& operator << (std::ostream& os, Message* mes) {
    os << "Message id: " << mes->messageId << std::endl;
    os << "Priority: " << mes->priority << std::endl;
    os << "Message: " << mes->message << std::endl;
}

int main() {
    Message* response = new Message();
    char* data;
    int size;

    const char* msgSize = new char[128];

    int clientSock = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(6666);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(clientSock, (const sockaddr*)(&sockAddr), sizeof(sockAddr));

    recv(clientSock, (void*)msgSize, 128, MSG_NOSIGNAL);

    send(clientSock, msgSize, sizeof(msgSize), MSG_NOSIGNAL);


    size = atoi(msgSize);
    data = new char[size];

    recv(clientSock, data, size, MSG_NOSIGNAL);

    shutdown(clientSock, SHUT_RDWR);
    close(clientSock);

    std::string buffer;
    buffer = data;

    Client::deserialize(buffer, response);

    std::cout << response;

    return 0;

}

