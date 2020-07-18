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
    std::string data;
    std::string msgSize;

    const void* ptrData = data.c_str();
    const void* ptrMsgSize = msgSize.c_str();

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

    recv(clientSock, (void*)ptrMsgSize, 512, MSG_NOSIGNAL);
    msgSize = static_cast<const char*>(ptrMsgSize);
    std::cout << msgSize << std::endl;

    recv(clientSock, (void*)ptrData, atoi(msgSize.c_str()) , MSG_NOSIGNAL);
    data = static_cast<const char*>(ptrData);
    std::cout << data.substr(0, 20) << std::endl;

    shutdown(clientSock, SHUT_RDWR);
    close(clientSock);

    Client::deserialize(data, response);

    std::cout << response;

    return 0;

}

