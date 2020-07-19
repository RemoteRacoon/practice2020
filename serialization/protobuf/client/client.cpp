#include <iostream>
#include <tuple>
#include <list>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include "../message.pb.h"

void ShowMessage(practice::Message &message)
{
    std::cout << "Message id: " << message.messageid() << std::endl;
    std::cout << "Message priority " << message.priority() << std::endl;
    std::cout << "Message content: " << message.message() << std::endl;
}

int main(int argc, char **argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::ofstream file("benchmark.txt", std::ios_base::out);
    clock_t start;
    clock_t end;

    int clientSock = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(7777);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    file << "Time elapsed (sec)" << std::endl;

    connect(clientSock, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));

    for (int i = 0; i < 10; i++)
    {
        practice::Message message;
        std::unique_ptr<char[]> msgSize(new char[32]);

        if (recv(clientSock, msgSize.get(), 32, MSG_NOSIGNAL) != -1)
        {
            send(clientSock, "OK", 2, MSG_NOSIGNAL);
        }
        else
        {
            std::cerr << "Cannot receive packet size" << std::endl;
            return -1;
        }

        int size = atoi(msgSize.get());
        std::unique_ptr<char[]> mesBuffer(new char[size]);

        if (recv(clientSock, mesBuffer.get(), size, MSG_NOSIGNAL) != -1)
        {
            start = clock();

            message.ParseFromArray(mesBuffer.get(), size);

            end = clock() - start;
            file << ((float)end) / CLOCKS_PER_SEC << std::endl;

            ShowMessage(message);
        }
        else
        {
            std::cerr << "Cannot receive message" << std::endl;
            return -1;
        }
    }

    shutdown(clientSock, SHUT_RDWR);
    close(clientSock);
    file.close();
    google::protobuf::ShutdownProtobufLibrary();
}
