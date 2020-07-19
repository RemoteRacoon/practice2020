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
    std::cout << "Message id: " << message.priority() << std::endl;
    std::cout << "Message id: " << message.message() << std::endl;
}

int main(int argc, char **argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::ofstream file("benchmark.txt", std::ios_base::app);
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

    for (int i = 0; i < 10; i++)
    {
        practice::Message message;
        connect(clientSock, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));

        start = clock();

        if (!message.ParseFromFileDescriptor(clientSock))
        {
            std::cerr << "Failed to parse message." << std::endl;
            return -1;
        }

        end = clock() - start;

        file << ((float)end) / CLOCKS_PER_SEC << std::endl;

        ShowMessage(message);

        shutdown(clientSock, SHUT_RDWR);
        close(clientSock);
    }

    file.close();
    google::protobuf::ShutdownProtobufLibrary();
}
