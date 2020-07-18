#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include "../message.h"
#include "random.h"
#include "serializer.h"


typedef unsigned int message_size;

int main(int argc, char** argv) {
    srand(time(NULL));
    message_size i,j;

    if (argc == 3) {
        i = atoi(argv[1]);
        j = atoi(argv[2]);
    } else {
        i = SEQ_LEN;
        j = SEQ_LEN;
    }

    Message *msg = new Message();
    msg->messageId = rand() % 100;
    msg->priority = rand() % 10;

    for (int it = 0; it < i; it++) {
        std::string str = Server::generate_random_sequence(j);
        msg->append(str);
    }

    int serverSock = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (serverSock  < 0) {
        std::cerr << "Cannot create a server socket" << std::endl;
        exit(-1);
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(6666);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    std::string serializable;
    Server::serialize(msg, serializable);

    bind(serverSock, (struct sockaddr*)(&sockAddr), sizeof(sockAddr));

    listen(serverSock, SOMAXCONN);

    int mesSize = serializable.length();
    std::string size = std::to_string(mesSize);

    char buffer[512];

    while (1) {
        int acceptor = accept(serverSock, (struct sockaddr*)NULL, NULL);
        send(acceptor, size.c_str(), size.length(), MSG_NOSIGNAL);

        if (recv(acceptor, buffer, 512, MSG_NOSIGNAL) != -1) {
            send(acceptor, serializable.c_str(), serializable.length(), MSG_NOSIGNAL);
            memset(buffer, 0, 512);
        };

        shutdown(acceptor, SHUT_RDWR);
        close(acceptor);
    }

}

