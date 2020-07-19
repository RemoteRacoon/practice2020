#include <iostream>
#include <fstream>
#include <sstream>
#include <csignal>
#include <streambuf>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include "../message.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "random.h"

void signalHandler(int signum)
{
    google::protobuf::ShutdownProtobufLibrary();
    exit(signum);
}

int main(int argc, char **argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    signal(SIGINT, signalHandler);
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int i, j;

    int serverSock = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(7777);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(serverSock, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));
    listen(serverSock, SOMAXCONN);

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number> <number>" << std::endl;
        return -1;
    }
    else if (argc == 2)
    {
        j = 1;
    }
    else
    {
        i = atoi(argv[1]);
        j = atoi(argv[2]);
    }

    std::string sequence;

    if (j == 1)
    {
        sequence = Server::generate_random_sequence(i);
    }
    else
    {
        for (int it = 0; it < i; it++)
        {
            std::string tmp = Server::generate_random_sequence(j);
            sequence.append(tmp);
        }
    }

    int acceptor = accept(serverSock, (struct sockaddr *)NULL, NULL);

    setsockopt(acceptor, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    while (1)
    {
        practice::Message message;

        std::uint32_t mesId = rand() % 100;
        std::uint32_t priority = rand() % 100;

        message.set_messageid(mesId);
        message.set_priority(priority);
        message.set_message(sequence.c_str());

        size_t size = message.ByteSizeLong();

        const char *sSize = std::to_string(size).c_str();

        send(acceptor, sSize, std::to_string(size).length(), MSG_NOSIGNAL);

        char successBuffer[2];
        if (recv(acceptor, successBuffer, 2, 0) != -1)
        {

            std::unique_ptr<char[]> serialized(new char[size]);

            message.SerializeToArray(&serialized[0], static_cast<int>(size));

            send(acceptor, serialized.get(), size, MSG_NOSIGNAL);
        }
    }

    shutdown(acceptor, SHUT_RDWR);
    close(acceptor);
    close(serverSock);

    google::protobuf::ShutdownProtobufLibrary();
}
