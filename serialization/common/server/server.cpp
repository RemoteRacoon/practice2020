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

int main(int argc, char** argv) {
    srand(time(NULL));

    std::vector<std::string> data;

    data.resize(2048);

    if (argc == 3) {
        int rows = atoi(argv[1]);
        int cols = atoi(argv[1]);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::string str = Server::generate_random_sequence(SEQ_LEN);
                data.push_back(str);
            }
        }
    }
    
    Message *msg = new Message;
    msg->priority = 1;
    msg->messageId = rand() % 100;

    for (int i = 0; i < SEQ_LEN; i++) {
        strcpy(msg->message[i], data[i].c_str());
    }

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

    // while (1) {
    //     int acceptor = accept(serverSock, 0 ,0);

    //     shutdown(acceptor, SHUT_RDWR);
    //     close(acceptor);
    // }

}

