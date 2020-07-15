#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include "message.h"

void serialize(Message* msg, std::string& data);

int main() {
    srand(time(NULL));
    const char* mes = "Hello from the tcp server!";

    Message *msg = new Message;
    msg->priority = 1;
    msg->messageId = rand() % 100;
    strcpy(msg->message, mes);

    std::string data;
    serialize(msg, data);

    int serverSock = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    struct sockaddr_in sockAddr {
        AF_INET,
        htons(45678),
        htonl(INADDR_ANY)
    };

    bind(serverSock, (struct sockaddr*)(&sockAddr), sizeof(sockAddr));

    listen(serverSock, SOMAXCONN);

    while (1) {
        int acceptor = accept(serverSock, 0 ,0);

        send(acceptor, data.c_str(), data.size(), MSG_NOSIGNAL);
        shutdown(acceptor, SHUT_RDWR);
        close(acceptor);
    }

}




void serialize(Message* msg, std::string& data)
{
    data.append(std::to_string(msg->messageId));
    data += SEP;
    data.append(std::to_string(msg->priority));
    data += SEP;
    data.append(msg->message);
    data += '\0';
}