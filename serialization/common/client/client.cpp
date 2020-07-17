#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include "../message.h"

using namespace std;

void deserialize(std::string& data, Message* msg);

int main() {
    Message* response = new Message;
    char buffer[512];
    std::string data;

    int clientSock = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    struct sockaddr_in sockAddr {
        AF_INET,
        htons(45678),
        htonl(INADDR_ANY)
    };

    connect(clientSock, (const sockaddr*)(&sockAddr), sizeof(sockAddr));
    recv(clientSock, buffer, sizeof(buffer), MSG_NOSIGNAL);

    data = buffer;
    deserialize(data, response);

    shutdown(clientSock, SHUT_RDWR);
    close(clientSock);


    cout << response->messageId << endl;
    cout << response->priority << endl;
    cout << response->message << endl;

    return 0;

}



void deserialize(std::string& data, Message* msg)
{
    std::string buffer;

    bool messageIdSet = 0;
    bool prioritySet = 0;
    bool messageSet = 0;
    size_t dataSize = data.size();

    for (int i = 0; i < dataSize; i++) {

        if (data[i] == SEP && !messageIdSet) {
            msg->messageId = atoi(buffer.c_str());
            buffer.clear();
            messageIdSet = true;
            continue;
        }

        if (data[i] == SEP && !prioritySet) {
            msg->priority = atoi(buffer.c_str());
            buffer.clear();
            prioritySet = true;
            continue;
        }

        if ((data[i] == SEP || i == dataSize - 1) && !messageSet) {
            buffer += data[i];
            strcpy(msg->message, buffer.c_str());
            buffer.clear();
            messageSet = true;
        }

        buffer += data[i];
    }
}